/*============================================================================*
 * O     O          __                   ______  __                           *
 *  \   /      /\  / /_      _    __    / /___/ / /_     _                    *
 *   [+]      /  \/ / \\    //__ / /__ / /____ / / \\   //                    *
 *  /   \    / /\  /   \\__// --/ /---/ /----// /   \\_//                     *
 * O     O  /_/  \/     \__/    \_\/ /_/     /_/ ____/_/                      *
 *                                                                            *
 *                                                                            *
 * Multi-Rotor controller firmware for Nuvoton Cortex M4 series               *
 *                                                                            *
 * Written by by T.L. Shen for Nuvoton Technology.                            *
 * tlshen@nuvoton.com/tzulan611126@gmail.com                                  *
 *                                                                            *
 *============================================================================*
 */
#include <stdio.h>
#ifdef M451
#include "M451Series.h"
#else
#include "Driver\DrvGPIO.h"
#include "Driver\DrvI2C.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvUART.h"
#endif
#include "NVT_I2C.h"
#include "Def.h"
#define BusError    1
#define ArbitLoss   2
#define TimeOut     4
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t Device_W_Addr;
uint8_t Device_R_Addr;
uint8_t ReadFlag;

uint8_t Tx_Data0[MAX_I2C_DATA_LEN];
uint8_t Rx_Data0[MAX_I2C_DATA_LEN];
uint8_t DataLen0;
uint8_t RxLen0;
uint8_t ContinueLen;
volatile uint8_t EndFlag0 = 0;
volatile uint8_t ErrorFlag = 0;
uint8_t  Addr1[3] = {0};
uint8_t  DataLen1;
uint8_t  Slave_Buff1[32] = {0};
uint16_t Slave_Buff_Addr1;
extern void DelayUsec(unsigned int usec);
extern void DelayMsec(unsigned int usec);
#ifdef M451
typedef void (*I2C_FUNC)(uint32_t u32Status);
static volatile I2C_FUNC s_I2C0HandlerFn = NULL;
static volatile I2C_FUNC s_I2C1HandlerFn = NULL;
void I2CX_IRQHandler(uint8_t id)
{
	uint32_t u32Status;

	u32Status = I2C_GET_STATUS(I2C_PORT);

	if(I2C_GET_TIMEOUT_FLAG(I2C_PORT)) {
	ErrorFlag = 1;
	/* Clear I2C0 Timeout Flag */
	I2C_ClearTimeoutFlag(I2C_PORT);
	}
	else {
		switch (u32Status) {	
			case 0x38:{/* Arbitration loss */
				I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI); 
				I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA);
				//printf("I2C Arbitration loss\n");
				break;
			}
			case 0x00:	/* Bus error */
			case 0x30:
			case 0xF8:
			case 0x48: {
				I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI);
				//I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
				I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA);
				//printf("I2C bus error\n");
				break;
			}
			default: {
				if(id==0) {
				if(s_I2C0HandlerFn != NULL)
					s_I2C0HandlerFn(u32Status);
			}	
				else if(id==1) {
					if(s_I2C1HandlerFn != NULL)
						s_I2C1HandlerFn(u32Status);
		}
	}
}
	}
}
void I2C0_IRQHandler(void)
{
	I2CX_IRQHandler(0);
}
void I2C1_IRQHandler(void)
{
	I2CX_IRQHandler(1);
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C0 (Master) Rx Callback Function                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_Callback_Rx(uint32_t status)
{
    if (status == 0x08)                     /* START has been transmitted and prepare SLA+W */
    {
#ifdef M451
        I2C_SET_DATA(I2C_PORT, Device_W_Addr << 1);    /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
        DrvI2C_WriteData(I2C_PORT, Device_W_Addr<<1);
        DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }   
    else if (status == 0x18)                /* SLA+W has been transmitted and ACK has been received */
    {
#ifdef M451
			I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x20)                /* SLA+W has been transmitted and NACK has been received */
    {
#ifdef M451
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA_STO_SI);
#else
      DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);
#endif
    }
    else if (status == 0x28)                /* DATA has been transmitted and ACK has been received */
    {
        if (DataLen0 != 2)
        {
#ifdef M451
          I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
          I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
          DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
          DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
        }
        else
        {
#ifdef M451
          I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA_SI);
#else
          DrvI2C_Ctrl(I2C_PORT, 1, 0, 1, 0);
          EndFlag0 = 1;
#endif
        }
    }
    else if (status == 0x10)                /* Repeat START has been transmitted and prepare SLA+R */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, ((Device_W_Addr << 1) | 0x01));   /* Write SLA+R to Register I2CDAT */
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Device_W_Addr<<1 | 0x01);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x40)                /* SLA+R has been transmitted and ACK has been received */
    {
#ifdef M451
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x58)                /* DATA has been received and NACK has been returned */
    {
#ifdef M451
      Rx_Data0[RxLen0++] = (unsigned char) I2C_GET_DATA(I2C_PORT);
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI);
#else
      Rx_Data0[0] = DrvI2C_ReadData(I2C_PORT);
      DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);
      EndFlag0 = 1;
#endif 
    }
    else
    {
        DEBUG_PRINT("Status 0x%x is NOT processed\n", status);
    }           
}
void I2C_Callback_Rx_Continue(uint32_t status)
{
    if (status == 0x08)                     /* START has been transmitted and prepare SLA+W */
    {
#ifdef M451
        I2C_SET_DATA(I2C_PORT, Device_W_Addr << 1|ReadFlag);    /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
        DrvI2C_WriteData(I2C_PORT, Device_W_Addr<<1|ReadFlag);
        DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }   
    else if (status == 0x18)                /* SLA+W has been transmitted and ACK has been received */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x20)                /* SLA+W has been transmitted and NACK has been received */
    {
#ifdef M451
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI);
#else
      DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);
#endif
    }
    else if (status == 0x28)                /* DATA has been transmitted and ACK has been received */
    {
#ifdef M451
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA_SI);
#else
      DrvI2C_Ctrl(I2C_PORT, 1, 0, 1, 0);
#endif
    }
    else if (status == 0x10)                /* Repeat START has been transmitted and prepare SLA+R */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, ((Device_W_Addr << 1) | 0x01));   /* Write SLA+R to Register I2CDAT */
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Device_W_Addr<<1 | 0x01);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x40)                /* SLA+R has been transmitted and ACK has been received */
    {
#ifdef M451
      if(ContinueLen>1)
        I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI_AA);
      else 
        I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      if(ContinueLen>1)
        DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 1);
      else 
        DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }       
    else if (status == 0x58)                /* DATA has been received and NACK has been returned */
    {
#ifdef M451
      Rx_Data0[RxLen0++] = (unsigned char) I2C_GET_DATA(I2C_PORT);  
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI);
      EndFlag0 = 1;
#else
      Rx_Data0[RxLen0++] = DrvI2C_ReadData(I2C_PORT);
      DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);
      EndFlag0 = 1;
#endif
    }
    else if (status == 0x50)                /* DATA has been received and ACK has been returned */
    {
#ifdef M451
      Rx_Data0[RxLen0++] = (unsigned char) I2C_GET_DATA(I2C_PORT);  
      if(RxLen0<(ContinueLen-1))
        I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI_AA);
      else {
        I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
      }
#else
      Rx_Data0[RxLen0++] = DrvI2C_ReadData(I2C_PORT);
      if(RxLen0<(ContinueLen-1))
        DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 1);
      else
        DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else
    {
        DEBUG_PRINT("Status 0x%x is NOT processed\n", status);
    }           
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C0 (Master) Tx Callback Function                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_Callback_Tx(uint32_t status)
{
    if (status == 0x08)                     /* START has been transmitted */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, Device_W_Addr << 1);    /* Write SLA+W to Register I2CDAT */
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Device_W_Addr<<1);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }   
    else if (status == 0x18)                /* SLA+W has been transmitted and ACK has been received */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x20)                /* SLA+W has been transmitted and NACK has been received */
    {
#ifdef M451
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA_STO_SI);
#else
      DrvI2C_Ctrl(I2C_PORT, 1, 1, 1, 0);
#endif
    }   
    else if (status == 0x28)                /* DATA has been transmitted and ACK has been received */
    {
        if (DataLen0 != 3)
        {
#ifdef M451
          I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
          I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
          DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
          DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
        }
        else
        {
#ifdef M451
          I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI);
#else
          DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);   
#endif 
          EndFlag0 = 1;
        }       
    }
    else
    {
        DEBUG_PRINT("Status 0x%x is NOT processed\n", status);
    }       
}
void I2C_Callback_Tx_Continue(uint32_t status)
{
    if (status == 0x08)                     /* START has been transmitted */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, Device_W_Addr << 1);    /* Write SLA+W to Register I2CDAT */
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Device_W_Addr<<1);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }   
    else if (status == 0x18)                /* SLA+W has been transmitted and ACK has been received */
    {
#ifdef M451
      I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
      DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
      DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
    }
    else if (status == 0x20)                /* SLA+W has been transmitted and NACK has been received */
    {
#ifdef M451
      I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA_STO_SI);
#else    
      DrvI2C_Ctrl(I2C_PORT, 1, 1, 1, 0);
#endif
    }   
    else if (status == 0x28)                /* DATA has been transmitted and ACK has been received */
    {
        if (DataLen0 != ContinueLen)
        {
#ifdef M451
          I2C_SET_DATA(I2C_PORT, Tx_Data0[DataLen0++]);
          I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_SI);
#else
          DrvI2C_WriteData(I2C_PORT, Tx_Data0[DataLen0++]);
          DrvI2C_Ctrl(I2C_PORT, 0, 0, 1, 0);
#endif
        }
        else
        {
#ifdef M451
          I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STO_SI);
#else
          DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);        
#endif
          EndFlag0 = 1;
        }       
    }
    else
    {
        DEBUG_PRINT("Status 0x%x is NOT processed\n", status);
    }       
}

#ifndef M451
/*---------------------------------------------------------------------------------------------------------*/
/*  I2C1 (Slave) Callback Function                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void I2C1_Callback_Slave(uint32_t status)
{
    
    if ((status == 0x60) || (status == 0x68))       /* SLA+W has been received and ACK has been returned */
    {
        DataLen1 = 0; 
        DrvI2C_Ctrl(I2C_PORT1, 0, 0, 1, 1);
    }   
    else if (status == 0x80)                        /* DATA has been received and ACK has been returned */
    {
        Addr1[DataLen1++] = DrvI2C_ReadData(I2C_PORT1);
        
        if (DataLen1 == 2)
        {
            Slave_Buff_Addr1 = (Addr1[0] << 8) + Addr1[1];
        }

        if ((DataLen1 == 3) && (Slave_Buff_Addr1 < 32))
        {
            Slave_Buff1[Slave_Buff_Addr1] = Addr1[2];
            DataLen1 = 0;       
        }

        DrvI2C_Ctrl(I2C_PORT1, 0, 0, 1, 1);
    }   
    else if ((status == 0xB0) || (status == 0xA8))  /* SLA+R has been received and ACK has been returned */
    {
        DrvI2C_WriteData(I2C_PORT1, Slave_Buff1[Slave_Buff_Addr1++]);
        if (Slave_Buff_Addr1 >= 32)
            Slave_Buff_Addr1 = 0;
        DrvI2C_Ctrl(I2C_PORT1, 0, 0, 1, 1);
    }
    else if (status == 0xC0)                        /* DATA has been transmitted and NACK has been received */
    {
        DrvI2C_Ctrl(I2C_PORT1, 0, 0, 1, 1);
    }
    else if (status == 0xA0)                        /* STOP or Repeat START has been received */
    {
        DataLen1 = 0;
        DrvI2C_Ctrl(I2C_PORT1, 0, 0, 1, 1);
    }
    else
    {
        DEBUG_PRINT("Status 0x%x is NOT processed\n", status);
    }
}

void ResetPort(E_I2C_PORT port) 
{
	DrvI2C_Open(port, 400000);
	DrvI2C_SetTimeoutCounter(port, true, 1);
	DrvI2C_EnableInt(port);
	DelayMsec(4);
	ErrorFlag = 0;
	DrvI2C_ClearIntFlag(port);
}

void WaitEndFlag0(uint16_t timeout)
{
	DrvI2C_SetTimeoutCounter(I2C_PORT, true, 1);
	DrvI2C_ClearIntFlag(I2C_PORT);
	while (EndFlag0 == 0)
	{
		if(timeout&&ErrorFlag)
			break;
		if(ErrorFlag) {
			ResetPort(I2C_PORT);
			DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Rx_Continue);
			DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);
			DelayMsec(3);
		}
	};
	DrvI2C_SetTimeoutCounter(I2C_PORT, false, 1);
}

void NVT_WriteByte(uint16_t address,uint8_t data)
{
	Tx_Data0[0]=address>>8;
	Tx_Data0[1]=(address&0xff);
	Tx_Data0[2] = data;
	DataLen0 = 0;
	EndFlag0 = 0;
	DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Tx);
	DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);
	WaitEndFlag0(0);
}

void NVT_WriteByteContinue(uint16_t address,uint8_t* data, uint8_t len)
{
	uint8_t i;
	uint8_t check_sum=0;
	Tx_Data0[0]=address>>8;
	Tx_Data0[1]=(address&0xff);
	for(i=0;i<len;i++)
		Tx_Data0[2+i]=data[i];

	DataLen0 = 0;
	EndFlag0 = 0;
	ContinueLen=len+2;

	DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Tx_Continue);
	DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);

	WaitEndFlag0(0);
	for(i=0;i<len;i++) {
		if((address+i)<0x80ff)
			check_sum+=Tx_Data0[2+i];
	}
	check_sum=(check_sum^0xff)+1;
}

void NVT_WriteMultiBytes(uint16_t address,uint8_t* data, uint8_t len)
{
	uint16_t i=0;
	uint8_t check_sum=0;
	for (i = 0; i < len; i++) {
		NVT_WriteByte(address+i,data[i])	;
		check_sum+=data[i];
	}
	check_sum=(check_sum^0xff)+1;
}

void NVT_ReadByte(uint16_t address, uint8_t *data)
{
	Tx_Data0[0]=address>>8;
	Tx_Data0[1]=(address&0xff);
	DataLen0 = 0;
	EndFlag0 = 0;
	DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Rx);
	DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);

	while (EndFlag0 == 0);
	*data=Rx_Data0[0];
}

void NVT_ReadMultiBytes(uint16_t address,uint8_t* data, uint8_t len)
{
	uint16_t i=0;
	uint8_t check_sum=0;
	for (i = 0; i < len; i++) {
		NVT_ReadByte(address+i,&data[i]);
		check_sum+=data[i];
	}
	check_sum=(check_sum^0xff)+1;
}

void NVT_ReadByteContinue(uint16_t address,uint8_t* data, uint8_t len)
{
	uint8_t i;

	for(i=0;i<len;i++) {
		RxLen0 = 0;
		DataLen0 = 0;
		EndFlag0 = 0;
		ReadFlag=0;
		ContinueLen=2;
		
		Tx_Data0[0]=address>>8;
		Tx_Data0[1]=(address&0xff);
	
		DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Rx_Continue);
		DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);
		
		WaitEndFlag0(0);
		ContinueLen=1;
		ReadFlag=1;
		EndFlag0 = 0;
		DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);
		
		WaitEndFlag0(0);
		data[i]=Rx_Data0[0];
		address++;
	}
}
#else
void WaitEndFlag0(uint16_t timeout)
{
	if(timeout)
		I2C_EnableTimeout(I2C_PORT, (uint8_t) timeout);
	
	while (EndFlag0 == 0) {
		if(ErrorFlag) {
			break;
		}
	};
	I2C_DisableTimeout(I2C_PORT);
}
#endif

uint8_t NVT_WriteByteContinue_addr8(uint8_t address,uint8_t* data, uint8_t len)
{
	uint8_t i;
	
	Tx_Data0[0]=address;
	
	for(i=0;i<len;i++)
		Tx_Data0[1+i]=data[i];
	
	DataLen0 = 0;
	EndFlag0 = 0;
	ErrorFlag = 0;
	ContinueLen=len+1;
#ifdef M451
	s_I2C0HandlerFn = (I2C_FUNC)I2C_Callback_Tx_Continue;
	s_I2C1HandlerFn = (I2C_FUNC)I2C_Callback_Tx_Continue;
	while(I2C_PORT->CTL & I2C_CTL_STO_Msk);
	I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA);
	WaitEndFlag0(1);
#else
	DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Tx_Continue);
	DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);

	WaitEndFlag0(0);
#endif	
	return ErrorFlag;
}

uint8_t NVT_ReadByteContinue_addr8(uint8_t address,uint8_t* data, uint8_t len, uint16_t timeout)
{
	uint8_t i;
	
	RxLen0 = 0;
	DataLen0 = 0;
	EndFlag0 = 0;
	ReadFlag = 0;
	ErrorFlag = 0;
	ContinueLen = len;
	Tx_Data0[0] = address;
#ifdef M451
	s_I2C0HandlerFn = (I2C_FUNC)I2C_Callback_Rx_Continue;
	s_I2C1HandlerFn = (I2C_FUNC)I2C_Callback_Rx_Continue;
	while(I2C_PORT->CTL & I2C_CTL_STO_Msk);
	I2C_SET_CONTROL_REG(I2C_PORT, I2C_CTL_STA);

	WaitEndFlag0(timeout);
#else		
	DrvI2C_InstallCallback(I2C_PORT, I2CFUNC, I2C_Callback_Rx_Continue);
	DrvI2C_Ctrl(I2C_PORT, 1, 0, 0, 0);

	WaitEndFlag0(timeout);
#endif
	for(i = 0; i<len; i++)
		data[i]=Rx_Data0[i];
	
	return ErrorFlag;
}

#ifndef M451

void I2C_Callback_TimeOutError(uint32_t status)
{
	ErrorFlag|=TimeOut;
}

void I2C_Callback_BusError(uint32_t status)
{
	ErrorFlag|=BusError;
	DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);
}

void I2C_Callback_ArbitLoss(uint32_t status)
{
	ErrorFlag|=ArbitLoss;
	DrvI2C_Ctrl(I2C_PORT, 0, 1, 1, 0);
}

#endif
void NVT_I2C_Init()
{

#ifdef USE_I2C_PORT1
#ifdef M451
	GPIO_SetMode(PC, BIT4, GPIO_MODE_OUTPUT);
	PC4=0;
	DelayMsec(1);

	CLK_EnableModuleClock(I2C1_MODULE);
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC4MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC4MFP_I2C1_SCL;
	SYS->GPE_MFPL &= ~SYS_GPE_MFPL_PE0MFP_Msk;
	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE0MFP_I2C1_SDA;
#else
	DrvGPIO_InitFunction(E_FUNC_I2C1);
#endif
#ifdef M451
	/* Open I2C module and set bus clock */
	I2C_Open(I2C1, 400000);

	/* Get I2C1 Bus Clock */
  	printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C1));

	/* Enable I2C interrupt */
	I2C_EnableInt(I2C1);
	NVIC_EnableIRQ(I2C1_IRQn);
#else
	/* Open I2C0 and I2C1, and set clock = 400Kbps */
	DrvI2C_Open(I2C_PORT, 400000);
	u32data = DrvI2C_GetClockFreq(I2C_PORT);
	printf("I2C clock: %d Hz\n", u32data);
	/* Enable I2C interrupt and set corresponding NVIC bit */
	DrvI2C_EnableInt(I2C_PORT);
	DrvI2C_InstallCallback(I2C_PORT, BUSERROR, I2C_Callback_BusError);
	DrvI2C_InstallCallback(I2C_PORT, TIMEOUT, I2C_Callback_TimeOutError);
	DrvI2C_InstallCallback(I2C_PORT, ARBITLOSS, I2C_Callback_ArbitLoss);
#endif	
#else /* Port 0*/
#ifdef M451
	GPIO_SetMode(PE, BIT12, GPIO_MODE_OUTPUT);
	PE12=0;
	DelayMsec(1);
	CLK_EnableModuleClock(I2C0_MODULE);
	/* Set I2C PA multi-function pins */
	SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE12MFP_Msk | SYS_GPE_MFPH_PE13MFP_Msk);
	SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE13MFP_I2C0_SDA | SYS_GPE_MFPH_PE12MFP_I2C0_SCL);
#ifdef M451
	/* Open I2C module and set bus clock */
	I2C_Open(I2C0, 400000);

	/* Get I2C1 Bus Clock */
 printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C0));

	/* Enable I2C interrupt */
	I2C_EnableInt(I2C0);
	NVIC_EnableIRQ(I2C0_IRQn);
#else
	/* Open I2C0 and I2C1, and set clock = 400Kbps */
	DrvI2C_Open(I2C_PORT, 400000);
	u32data = DrvI2C_GetClockFreq(I2C_PORT);
	printf("I2C clock: %d Hz\n", u32data);
	/* Enable I2C interrupt and set corresponding NVIC bit */
	DrvI2C_EnableInt(I2C_PORT);
	DrvI2C_InstallCallback(I2C_PORT, BUSERROR, I2C_Callback_BusError);
	DrvI2C_InstallCallback(I2C_PORT, TIMEOUT, I2C_Callback_TimeOutError);
	DrvI2C_InstallCallback(I2C_PORT, ARBITLOSS, I2C_Callback_ArbitLoss);
#endif	
#else
	DrvGPIO_InitFunction(E_FUNC_I2C0);
#endif
#endif
}

void NVT_SetDeviceAddress(uint8_t devAddr)
{
	Device_W_Addr=devAddr;
	Device_R_Addr=devAddr+1;
}


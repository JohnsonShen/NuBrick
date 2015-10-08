/*=============================================================*
*  _  _             ___              _              _         *
* | \| |   _  _    | _ )     _ _    (_)     __     | |__      *
* | .` |  | +| |   | _ \    | '_|   | |    / _|    | / /      *
* |_|\_|   \_,_|   |___/   _|_|_   _|_|_   \__|_   |_\_\      *
*_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|     *
*"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'     *
*                                                             *
* NuBrick IoT applications firmware                           *
*                                                             *
* Written by MB40 for Nuvoton Technology.                     *
* tlshen@nuvoton.com/tzulan611126@gmail.com                   *
* HCHEISH@nuvoton.com                                         *
*=============================================================*
*/
 
#include "i2c_ms.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t  I2CMSData[SLEEP_REG+1] = {0};						//data information
uint8_t  I2CDataStartBit;							//Master data start bit
uint8_t  I2CDataEndBit;								//Master data end bit
uint8_t  I2CCurrentLen;								//M/S Data current length
// slave variables
uint8_t  I2CSlvDataLen;										//data length
uint8_t  I2CSlvBuffAddr;									//orgined address			//****
uint8_t  I2CSlvState;										//I2c Slave Receive state
// master variables
uint8_t  I2CMstAddr;											//Master send address to slave
uint8_t  I2CMstTxLenFlag;								//Master Transfer data length flag
uint8_t  I2CMstSleepCount;								//Master Sleep Mode counter
uint8_t  I2CMstWakeupEndFlag;							//Master Wake up Flag
uint8_t  I2CMstTRxEndFlag;								//Master Transfer/Receive End Flag
uint8_t  I2CMstSleepEndFlag;							//Master sleep End Flag
uint8_t  I2CMstWakeupEnStartFlag;					//Master Wake up start Flag
uint8_t  I2CMstTRxEnStartFlag;						//Master Transfer/Receive start Flag
uint8_t  I2CMstSleepEnStartFlag;					//Master sleep start Flag
uint8_t  I2CMasterRxState;								//Master Receive state ticker
uint8_t  I2CMstEndFlag;										//Master End Flag
uint8_t  I2CMstTimeOutCounter;						//Master End Flag
// slave variables

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C (Slave) Callback Function                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MS_Slave(uint32_t status)
{
    
	if ((status == 0x60) || (status == 0x68))       /* SLA+W has been received and ACK has been returned */
	{
		I2CMSData[SLEEP_REG] = 1;								//I2C is awake snd Sleep flag is unset
		I2CSlvState = 0;
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}   
	else if (status == 0x80)                        /* DATA has been received and ACK has been returned */
	{		
		if (I2CSlvState == 0)
		{
			I2CDataStartBit = (unsigned char) I2C_GET_DATA(I2C_MS_PORT);
			I2CCurrentLen = I2CDataStartBit;
			I2CSlvState = 1;
		}
		else if (I2CSlvState == 1)
		{
			I2CDataEndBit = (unsigned char) I2C_GET_DATA(I2C_MS_PORT);
			I2CSlvState = 2;
		}
		else if (I2CSlvState == 2)
		{
			if(I2CCurrentLen <= I2CDataEndBit)
			I2CMSData[I2CCurrentLen++] = (unsigned char) I2C_GET_DATA(I2C_MS_PORT);
		}

		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else if ((status == 0xB0) || (status == 0xA8))  /* SLA+R has been received and ACK has been returned */
	{
		I2CCurrentLen = I2CDataStartBit;								//I2C is awake snd Sleep flag is unset
		I2C_SET_DATA(I2C_MS_PORT, I2CMSData[I2CCurrentLen++]);
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else if (status == 0xB8)                        /* DATA has been transmitted and NACK has been received */
	{
		if(I2CCurrentLen <= I2CDataEndBit)
		{
			I2C_SET_DATA(I2C_MS_PORT, I2CMSData[I2CCurrentLen++]);
		}
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else if (status == 0xC0)                        /* DATA has been transmitted and NACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else if(status == 0x88)                 /* Previously addressed with own SLA address; NOT ACK has
                                                   been returned */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else if (status == 0xA0)                        /* STOP or Repeat START has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else
	{
		//DEBUG_PRINT("Status 0x%x is NOT processed\n", status);
	}
}

// **********************************************************************
//               I2C Slave Initialize called by I2CWakeUpInit
// **********************************************************************
void I2C_MS_Slave_Init()
{
	int testi;
	for(testi=0;testi<16;testi++)
	{	I2CMSData[testi]=testi;}
	// =============PIN SETTNG================== 
	#ifdef I2C_MS_PORT0
	CLK_EnableModuleClock(I2C0_MODULE);
	/* Set I2C PA multi-function pins */
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk | SYS_GPA_MFPL_PA3MFP_Msk);
	SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA2MFP_I2C0_SDA | SYS_GPA_MFPL_PA3MFP_I2C0_SCL);
	#else
	CLK_EnableModuleClock(I2C1_MODULE);
	/* Set I2C PA multi-function pins */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC4MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC4MFP_I2C1_SCL;
	SYS->GPE_MFPL &= ~SYS_GPE_MFPL_PE0MFP_Msk;
	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE0MFP_I2C1_SDA;
	#endif
	
	/* Open I2C module and set bus clock */
	I2C_Open(I2C_MS_PORT, 100000);

	/* Get I2C0 Bus Clock */
	printf("I2C MS Slave clock %d Hz\n", I2C_GetBusClockFreq(I2C_MS_PORT));

	/* Set I2C 4 Slave Addresses */
	I2C_SetSlaveAddr(I2C_MS_PORT, 0, 0x15, 0);   /* Slave Address : 0x15 */
	I2C_SetSlaveAddr(I2C_MS_PORT, 1, 0x35, 0);   /* Slave Address : 0x35 */
	I2C_SetSlaveAddr(I2C_MS_PORT, 2, 0x55, 0);   /* Slave Address : 0x55 */
	I2C_SetSlaveAddr(I2C_MS_PORT, 3, 0x75, 0);   /* Slave Address : 0x75 */

	/* Enable I2C interrupt */
	I2C_EnableInt(I2C_MS_PORT);
	#ifdef I2C_MS_PORT0 
	NVIC_EnableIRQ(I2C0_IRQn);
	#else
	NVIC_EnableIRQ(I2C1_IRQn);
	#endif
	
#ifdef I2C_MS_PORT0
	s_I2C0HandlerFn = (I2C_FUNC)I2C_MS_Slave;
#else
	s_I2C1HandlerFn = (I2C_FUNC)I2C_MS_Slave;
#endif

	/* Enable I2C wake-up */
	I2C_EnableWakeup(I2C_MS_PORT);
	
	/* Set I2C0 enter Not Address SLAVE mode */
	I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
}

// **********************************************************************
//                          I2C Master Initialize 
// **********************************************************************
void I2C_MS_Master_Init()
{
	int testi;
	for(testi=15;testi<31;testi++)
	{	I2CMSData[testi]=testi;}
	// =============PIN SETTNG================== 
	#ifdef I2C_MS_PORT0
	CLK_EnableModuleClock(I2C0_MODULE);
	/* Set I2C PA multi-function pins */
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk | SYS_GPA_MFPL_PA3MFP_Msk);
	SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA2MFP_I2C0_SDA | SYS_GPA_MFPL_PA3MFP_I2C0_SCL);
	#else
	CLK_EnableModuleClock(I2C1_MODULE);
	/* Set I2C PA multi-function pins */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC4MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC4MFP_I2C1_SCL;
	SYS->GPE_MFPL &= ~SYS_GPE_MFPL_PE0MFP_Msk;
	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE0MFP_I2C1_SDA;
	#endif
	
	/* Open I2C module and set bus clock */
	I2C_Open(I2C_MS_PORT, 100000);

	/* Get I2C0 Bus Clock */
	printf("I2C MS Master clock %d Hz\n", I2C_GetBusClockFreq(I2C_MS_PORT));

	/* Enable I2C interrupt */
	I2C_EnableInt(I2C_MS_PORT);
	#ifdef I2C_MS_PORT0 
	NVIC_EnableIRQ(I2C0_IRQn);
	#else
	NVIC_EnableIRQ(I2C1_IRQn);
	#endif
	
	I2CMstWakeupEnStartFlag=1;
	I2CMstTRxEnStartFlag=1;
	I2CMstSleepEnStartFlag=1;
	I2CMstEndFlag=1;
}

// =========================================================
//        I2C Master Tx Wake Up Callback Function   
// =========================================================
void I2C_MasterTxWakeup(uint32_t u32Status)
{
	if(u32Status == 0x08)                       /* START has been transmitted */
	{
		I2C_SetData(I2C_MS_PORT, (I2CMstAddr << 1));    /* Write SLA+W to Register I2CDAT */
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
	}
	else if(u32Status == 0x18)                  /* SLA+W has been transmitted and ACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STO_SI);
		I2CMstWakeupEndFlag = 1;
		//I2C_DisableTimeout(I2C_MS_PORT);
	}
	else if(u32Status == 0x20)                  /* SLA+W has been transmitted and NOT ACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STA_STO_SI);
	}
	else
	{
		/* TO DO */
		printf("Status 0x%x is NOT processed\n", u32Status);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Tx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterTx(uint32_t u32Status)
{
	if(u32Status == 0x08)                       /* START has been transmitted */
	{
		I2C_SetData(I2C_MS_PORT, I2CMstAddr << 1);    /* Write SLA+W to Register I2CDAT */
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
	}
	else if(u32Status == 0x18)                  /* SLA+W has been transmitted and ACK has been received */
	{
		I2C_SetData(I2C_MS_PORT, I2CDataStartBit);
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
		I2CMstTxLenFlag=0;
	}
	else if(u32Status == 0x20)                  /* SLA+W has been transmitted and NACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STA_STO_SI);
	}
	else if(u32Status == 0x28)                  /* DATA has been transmitted and ACK has been received */
	{
		if(I2CMstTxLenFlag==0)
		{
			I2C_SetData(I2C_MS_PORT, I2CDataEndBit);
			I2CMstTxLenFlag=1;
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
		}
		else if(I2CCurrentLen <= I2CDataEndBit)
		{
			I2C_SetData(I2C_MS_PORT, I2CMSData[I2CCurrentLen++]);
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
		}
		else if(I2CCurrentLen > I2CDataEndBit)
		{
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STO_SI);
			I2CMstTRxEndFlag = 1;
			//I2C_DisableTimeout(I2C_MS_PORT);
		}
	}
	else
	{
		/* TO DO */
	}
}

// =========================================================
//        I2C Master Transfer Callback Function   
// =========================================================
void I2C_MasterRx(uint32_t u32Status)
{
	if(u32Status == 0x08)                       /* START has been transmitted and prepare SLA+W */
	{
		I2C_SET_DATA(I2C_MS_PORT, (I2CMstAddr << 1));    /* Write SLA+W to Register I2CDAT */
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
	}
	else if(u32Status == 0x18)                  /* SLA+W has been transmitted and ACK has been received */
	{
		I2C_SET_DATA(I2C_MS_PORT, I2CDataStartBit);
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
		I2CMasterRxState = 0;
	}
	else if(u32Status == 0x20)                  /* SLA+W has been transmitted and NACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STA_STO_SI);
	}
	else if(u32Status == 0x28)                  /* DATA has been transmitted and ACK has been received */
	{
		if(I2CMasterRxState == 0)
		{
			I2C_SET_DATA(I2C_MS_PORT, I2CDataEndBit);
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
			I2CMasterRxState = 1;
		}
		else
		{
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STA_SI);
			I2CMasterRxState = 0;
		}
	}
	else if(u32Status == 0x10)                  /* Repeat START has been transmitted and prepare SLA+R */
	{
		I2C_SET_DATA(I2C_MS_PORT, ((I2CMstAddr << 1) | 0x01));   /* Write SLA+R to Register I2CDAT */
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
	}
	else if(u32Status == 0x40)                  /* SLA+R has been transmitted and ACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
	}
	else if(u32Status == 0x50)                  /* DATA has been received and ACK has been returned */
	{ 
		if(I2CCurrentLen <= I2CDataEndBit)
		{
			I2CMSData[I2CCurrentLen++] = (unsigned char) I2C_GET_DATA(I2C_MS_PORT);
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);
		}
		else if((I2CCurrentLen > I2CDataEndBit))
		{
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STO_SI);
			I2CMstTRxEndFlag = 1;
			//I2C_DisableTimeout(I2C_MS_PORT);
		}
	}
	else if (u32Status == 0x58)                /* DATA has been received and NACK has been returned */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STO_SI);
		I2CMstTRxEndFlag = 1;
		//I2C_DisableTimeout(I2C_MS_PORT);
	}
	else
	{
		/* TO DO */
	}
}
// =========================================================
//        I2C Master Transfer SLEEP FLAG Function   
// =========================================================
void I2C_MasterTxSleepFlag(uint32_t u32Status)
{
	if(u32Status == 0x08)                       /* START has been transmitted */
	{
		I2C_SET_DATA(I2C_MS_PORT, I2CMstAddr << 1);    /* Write SLA+W to Register I2CDAT */
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
	}
	else if(u32Status == 0x18)                  /* SLA+W has been transmitted and ACK has been received */
	{
		I2C_SET_DATA(I2C_MS_PORT, SLEEP_REG);
		I2CMstSleepCount = 0;
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
	}
	else if(u32Status == 0x20)                  /* SLA+W has been transmitted and NACK has been received */
	{
		I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STA_STO_SI);
	}
	else if(u32Status == 0x28)                  /* DATA has been transmitted and ACK has been received */
	{
		if(I2CMstSleepCount==0)
		{
			I2C_SET_DATA(I2C_MS_PORT, SLEEP_REG);
			I2CMstSleepCount=1;
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
		}
		else if(I2CMstSleepCount == 1)
		{
			I2C_SET_DATA(I2C_MS_PORT, SLEEP_TICKER);
			I2CMstSleepCount = 2;
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI);
		}
		else if(I2CMstSleepCount == 2)
		{
			I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_STO_SI);
			I2CMstSleepEndFlag = 1;
			//I2C_DisableTimeout(I2C_MS_PORT);
		}
	}
	else
	{
		/* TO DO */
	}
}

void I2C_MASTER_Read_Write_Start(uint8_t slvaddr, uint8_t WriteRead)
{
	// -------------Wake up Slave------------------
	if(I2CMstWakeupEnStartFlag == 1)
	{
		I2CMstWakeupEnStartFlag = 0;
		
		I2CMstAddr = slvaddr;
		I2CMstTRxEnStartFlag = 1;
		I2CMstEndFlag=0;
		
		I2CMstTimeOutCounter = getTickCount();
		/* I2C function to write data to slave */
		#ifdef I2C_MS_PORT0
		s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterTxWakeup;
		#else
		s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterTxWakeup;
		#endif

		/* I2C as master sends START signal */
		I2C_START(I2C_MS_PORT);
		/* I2C TimeOut Start operation */
		//I2C_EnableTimeout(I2C_MS_PORT, 1);
	}
}

void I2C_MASTER_Read_Write_Con(uint8_t slvaddr, uint8_t slvreg, uint8_t DatLen, uint8_t WriteRead)
{
	// -------------Transfer/Receive Data------------------
	if (I2CMstWakeupEndFlag == 1 && I2CMstTRxEnStartFlag == 1)
	{
		I2CMstTRxEnStartFlag = 0;
		I2CMstWakeupEndFlag = 0;
		I2CCurrentLen++;
	
		I2CMstAddr = slvaddr;
		I2CDataStartBit = slvreg;
		I2CDataEndBit = slvreg + DatLen - 1;
		I2CCurrentLen = slvreg;
		//renew state
		I2CMstTRxEndFlag = 0;
		I2CMstSleepEnStartFlag = 1;

		if(WriteRead==I2C_WRITE)
		{
			/* I2C function to read data from slave */
			#ifdef I2C_MS_PORT0
			s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterTx;
			#else
			s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterTx;
			#endif

			I2C_START(I2C_MS_PORT);
			/* I2C TimeOut Start operation */
			//I2C_EnableTimeout(I2C_MS_PORT, 1);
		}
		else if(WriteRead==I2C_READ)
		{
			/* I2C function to read data from slave */
			#ifdef I2C_MS_PORT0
			s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterRx;
			#else
			s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterRx;
			#endif

			I2C_START(I2C_MS_PORT);
			/* I2C TimeOut Start operation */
			//I2C_EnableTimeout(I2C_MS_PORT, 1);
		}
	}
	// -------------Require Slave Sleep------------------
	else if (I2CMstTRxEndFlag == 1 && I2CMstSleepEnStartFlag == 1)
	{
		I2CMstTRxEndFlag = 0;
		I2CMstSleepEnStartFlag = 0;
		/* I2C function to read data from slave */
		#ifdef I2C_MS_PORT0
		s_I2C0HandlerFn = (I2C_FUNC)I2C_MasterTxSleepFlag;
		#else
		s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterTxSleepFlag;
		#endif

		I2C_START(I2C_MS_PORT);
		/* I2C TimeOut Start operation */
		//I2C_EnableTimeout(I2C_MS_PORT, 1);
	}
	//-------Excute End Recover Master state-----------
	if(I2CMstSleepEndFlag == 1)
	{
		I2CMstSleepEndFlag = 0;
		/* renew state */ 				
		I2CMstWakeupEnStartFlag=1;
		I2CMstEndFlag=1;
		//I2C_EnableTimeout(I2C_MS_PORT, 1);
	}
	//------TimeOut Recover Master state-----------
	if(I2CMstEndFlag==0 && (getTickCount() - I2CMstTimeOutCounter) > 10)
	{
		I2CMstWakeupEnStartFlag=1;
		I2CMstTRxEnStartFlag=1;
		I2CMstSleepEnStartFlag=1;
		I2CMstEndFlag=1;
	}
}


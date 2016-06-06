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

#include "i2c_HTU21D.h"

uint8_t I2CTempEndFlag;
uint8_t I2CTempTRxEndFlag;
uint8_t I2CTempTimeOutFlag = 0;
uint8_t  I2CTemp_TxData[4] = {0};				//Store Received data in Master
uint8_t  I2CTemp_RxData[4] = {0};				//Store Received data in Master
uint8_t  I2CTempCurrentLen = 0;
uint8_t  I2CTempEndBit = 0;
uint8_t  I2CTempAddr;							//Master send address to slave
uint8_t  I2CTempReg;							//Master send command to slave
uint32_t  I2CTempTimeOutCounter;				//Master End Flag
uint32_t  test1;								//Master End Flag
uint32_t  test2;								//Master End Flag

// **********************************************************************
//                          I2C Master Initialize
// **********************************************************************
void I2C_Temp_Init()
{
	CLK_EnableModuleClock(I2C1_MODULE);

	/* Open I2C module and set bus clock */
	I2C_Open(I2C1, 100000);

	/* Set I2C PA multi-function pins */
	GPIO_SetMode(PC, BIT4, GPIO_MODE_OUTPUT);
	PC4=0;
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC4MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC4MFP_I2C1_SCL;
	SYS->GPE_MFPL &= ~SYS_GPE_MFPL_PE0MFP_Msk;
	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE0MFP_I2C1_SDA;
}

void I2CTemp_readBytes(I2C_T* i2c, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	uint8_t i, tmp;
	I2C_START(i2c);                         	//Start
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, devAddr);             	//send slave address+W
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, regAddr);             	//send index
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	I2C_SET_CONTROL_REG(i2c, I2C_CTL_STA_SI);	//Start
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, (devAddr+1));    			//send slave address+R
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	for (i=0; i<length; i++)
	{
		I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
		I2C_WAIT_READY(i2c);
		tmp = I2C_GET_DATA(i2c);           		//read data
		data[i]=tmp;
	}
	I2C_STOP(i2c);								//Stop
}

void I2CTemp_writeBytes(I2C_T *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	uint8_t i;
	uint32_t tmp;
	I2C_START(i2c);                    			//Start
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, devAddr);        			//send slave address
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	I2C_SET_DATA(i2c, regAddr);        			//send index
	I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
	I2C_WAIT_READY(i2c);

	for (i=0; i<length; i++)
	{
		tmp = data[i];
		I2C_SET_DATA(i2c, tmp);            		//send Data
		I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
		I2C_WAIT_READY(i2c);
	}

	I2C_STOP(i2c);								//Stop
}

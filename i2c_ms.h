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
#ifndef I2C_MS_H_
#define I2C_MS_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "def.h"
#include "Config.h"
#include "timerctrl.h"
#include "nvti2c.h"
#include "devCheck.h"

#define I2C_WRITE						0		//I2C write bit
#define I2C_READ						1		//I2C read bit
#define SONAR_I2CDATA					1
#define SONAR_I2CDATALEN				1
#define GAS_I2CDATA						2
#define GAS_I2CDATALEN					1
#define BATTERY_I2CDATA					3
#define BATTERY_I2CDATALEN				1
#define BUZZER_I2CDATA					4
#define BUZZER_I2CDATALEN				1
#define TEMPERATURE_I2CDATA				5		//5,6,   2 bytes
#define TEMPERATURE_I2CDATALEN			2		//2 bytes
#define AHRS_I2CDATA					7		//7~12,  6 bytes
#define AHRS_I2CDATALEN					6		//6 bytes
#define IRTx_I2CDATA					13		//13~20, 8 bytes
#define IRTx_I2CDATALEN					8		//8 bytes
#define IRRx_I2CDATA					21		//21~28, 8 bytes
#define IRRx_I2CDATALEN					8		//8 bytes
#define SLEEP_REG						31		//SLEEP FLAG:1:SLEEP
#define SLEEP_TICKER					0		//SLEEP Ticker:1:SLEEP
#define DEV_NOT_INIT					2		//Device initialize not complete

void I2C_MS_Slave_Init(void);
void I2C_MS_Master_Restart(void);
void I2C_MS_Master_Init(void);
void I2C_MASTER_Read_Write_Start(uint8_t, uint8_t);
void I2C_MASTER_Read_Write_Con(uint8_t, uint8_t, uint8_t);

extern uint8_t I2CMstEndFlag;										//Master End Flag
extern uint8_t I2CMstTimeOutFlag;								//Master Time Out Flag
extern uint8_t  I2CMS_MstRxData[64];								//Store Received data in Master
extern uint8_t  I2CMS_SlvRxData[64];								//Store Received data in Master
extern uint8_t  I2CMS_SlvTxDevDesData[32];					//Device Descript Data be transfered from slave
extern uint8_t  I2CMS_SlvTxRPDesData[80];						//Report Descript Data be transfered from slave
extern uint8_t  I2CMS_SlvTxInRPData[32];						//Input Report Data be transfered from slave
extern uint8_t  I2CMS_SlvTxGetFTData[32];						//Get Feature Data be transfered from slave
extern uint8_t  I2CSlvCmd;																	//Master send command to slave
extern uint8_t  I2CSlvDev;																	//Master send command to slave
extern uint8_t  I2CSlv_InitFlag;														//I2C Slave device initialized falg
extern uint8_t	I2CMS_SlvRxFin;
extern uint8_t  I2CMS_MstTxOutRPData[32];						//Output Data be transfered from Master
extern uint8_t  I2CMS_MstTxSetFTData[32];						//Set Feature Data be transfered from slave
extern uint8_t  I2CMstCmd;																	//Master send command to slave
extern uint8_t  I2CMstDev;																//Master send address to slave

#endif //I2C_MS

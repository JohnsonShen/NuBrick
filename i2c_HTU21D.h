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
#ifndef I2C_HTU21D_H_
#define I2C_HTU21D_H_

#define I2C_TEMP_WRITE 1
#define I2C_TEMP_READ 0

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "def.h"
#include "Config.h"
#include "timerctrl.h"
#include "nvti2c.h"
#include "devCheck.h"

void I2C_Temp_Init(void);
void I2CTemp_readBytes(I2C_T*, uint8_t, uint8_t, uint8_t, uint8_t*);
void I2CTemp_writeBytes(I2C_T*, uint8_t, uint8_t, uint8_t, uint8_t*);
void I2C_TEMP_Read_Write(uint8_t, uint8_t, uint32_t, uint8_t);


extern uint8_t	I2CTempTRxEndFlag;
extern uint8_t  I2CTemp_TxData[4];								//Store Received data in Master
extern uint8_t  I2CTemp_RxData[4];								//Store Received data in Master

#endif //I2C_HTU21D

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
#ifndef TEMP_HTU21D_H_
#define TEMP_HTU21D_H_
//
// HTU21D : Digital Relative Humidity Sensor with Temperature Output
//
#define HTU21D_I2C_PORT      I2C1
#define HTU21D_I2C_SLA       0x80

#define HTU21D_TriggerTempHold    0xE3 // Hold I2C master
#define HTU21D_TriggerHumidHold   0xE5 // Hold I2C mater
#define HTU21D_TriggerTempnoHold  0xF3 // no Hold I2C master
#define HTU21D_TriggerHumidnoHold 0xF5 // no Hold I2C master
#define HTU21D_Userregister       0xF6
#define HTU21D_SoftReset          0xFE

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "timerctrl.h"
#include "tid.h"
#include "i2c_HTU21D.h"

void HTU21D_Init(void);
void GetHTU21DTemp(void);
void WaitHTU21D(void);

#endif /* TEMP_HTU21D_H_ */

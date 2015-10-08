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
 *                                                             *
 *=============================================================*
 */
#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "temperature.h"
#include "i2c_ms.h"
#include "ir.h"
#include "buzzer.h"
#include "gas.h"

void Battery_Init(void);
void GetBattery(void);
void PowerControl(void);

extern uint8_t  BatteryData;
extern volatile uint32_t MainTimeMSCounter;

#endif /* BATTERY_H_ */

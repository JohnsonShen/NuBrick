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

#ifndef AHRS_H_
#define AHRS_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "AHRSLib.h"
#include "MPU6050.h"
#include "def.h"
#include "flash.h"
#include "timerctrl.h"
#include "tid.h"

#define ACC_PRESCALE		2
#define GYRO_PRESCALE		50

void AhrsParaInit(void);
void AhrsRead(uint8_t, uint16_t, uint8_t);

extern float Gyro[3];
extern int8_t VibrationOverFlag;
extern float GyroValue;

void AHRS_Init(void);
void AHRS_Control(void);

#endif /* AHRS_H_ */

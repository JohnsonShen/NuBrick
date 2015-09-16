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

#define ACC_PRESCALE		2
#define GYRO_PRESCALE		360

uint8_t AhrsRead(uint8_t, uint8_t);

#endif /* AHRS_H_ */

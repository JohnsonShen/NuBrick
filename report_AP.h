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
#ifndef _REPORT_AP_H
#define _REPORT_AP_H

#include <stdio.h>
#include "retarget.h"
#include "tid.h"
#include "tidmst.h"
#include "flash.h"

#define INDEX_REPORT_LENGTH 					8
#define DEVICE_NUMBER			 				9
#define INDEX_DATA_LENGTH						34
#define DEVICELINK_REPORT_LENGTH 				8
#define DEVICELINK_DATA_LENGTH					6

void report_index_binary(void);
void report_devLink_binary(void);
void report_binary(TID_Device);
void report_buzzer_binary(void);
void report_led_binary(void);
void report_ahrs_binary(void);
void SetPara_Binary(void);

extern int8_t reportNewTabFlag;
extern int8_t reportAPFlag;

#endif

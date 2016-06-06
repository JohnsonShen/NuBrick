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
#ifndef _REPORT_H
#define _REPORT_H

#include <stdio.h>
#include "AHRSLib.h"
#include "retarget.h"
#include "Sensors.h"
#include "tid.h"
#include "tidmst.h"
#include "timer1IRQ.h"
#include "report_AP.h"

#define REPORT_BATTERY				0
#define REPORT_BUZZER				1
#define REPORT_LED    				2
#define REPORT_AHRS					3
#define REPORT_SONAR				4
#define REPORT_TEMP					5
#define REPORT_GAS					6
#define REPORT_IR					7
#define REPORT_KEY					8
#define REPORT_RESDEV9				9
#define REPORT_RESDEV10				10
#define REPORT_RESDEV11				11
#define REPORT_RESDEV12				12
#define REPORT_RESDEV13				13
#define REPORT_RESDEV14				14
#define REPORT_INDEX				15
#define REPORT_DEVLINK				16

#define REPORT_FEATURE 			      1
#define REPORT_INPUT  						2
#define REPORT_OUTPUT							3

#define REPORT_FORMAT_TEXT        0
#define REPORT_FORMAT_BINARY      1

#define STREAM_START              1
#define STREAM_PAUSE              0
extern char report_mode;
extern char report_format;
extern char stream_mode;
void report_sensors(void);
void report_status(void);
void CommandProcess(void);

void report_battery(void);
void report_buzzer(void);
void report_led(void);
void report_ahrs(void);
void report_sonar(void);
void report_temp(void);
void report_gas(void);
void report_ir(void);
void report_key(void);
void report_resDev9(void);
void report_resDev10(void);
void report_resDev11(void);
void report_resDev12(void);
void report_resDev13(void);
void report_resDev14(void);
#endif

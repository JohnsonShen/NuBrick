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

#define REPORT_INDEX				0
#define REPORT_BATTERY				1
#define REPORT_BUZZER				2
#define REPORT_LED    				3
#define REPORT_AHRS					4
#define REPORT_SONAR				5
#define REPORT_TEMP					6
#define REPORT_GAS					7
#define REPORT_IR					8
#define REPORT_KEY					9
#define REPORT_DEVLINK				10

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
#endif

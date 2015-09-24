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

#define REPORT_AHRS_EULER         1 
#define REPORT_AHRS_QUATERNION    2 
#define REPORT_SENSORS_CALIBRATED 3 
#define REPORT_SENSORS_RAW        4 
#define REPORT_MOTOR_POWER        5 
#define REPORT_PID                6
#define REPORT_RATE_PID           7
#define REPORT_VELOCITY           8
#define REPORT_RC_STATUS          9
#define REPORT_ALTHOLD_PID       10
#define REPORT_ALTHOLD_STATUS    11
#define REPORT_MODE_STATUS       12

#define REPORT_FORMAT_TEXT        0 
#define REPORT_FORMAT_BINARY      1 

#define STREAM_START              1
#define STREAM_PAUSE              0
extern char report_mode;
extern char report_format;
extern char stream_mode;
void report_sensors(void);
void report_status(void);
#endif

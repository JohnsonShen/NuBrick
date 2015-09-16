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

#define REPORT_FORMAT_TEXT        0 
#define REPORT_FORMAT_BINARY      1 

#define STREAM_START              1
#define STREAM_PAUSE              0

extern char report_mode;
extern char report_format;
extern char stream_mode;
void report_sensors(void);

#endif

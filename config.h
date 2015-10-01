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
#ifndef CONFIG_H_
#define CONFIG_H_
#include "AHRSLib.h"
#define HW__VERSION_CODE 1001
#define VERSION_CODE 100
#define BOARD_CODE   100
#define USE_I2C_PORT1
#define UART_BAUD_RATE 115200
#define OUTPUT_DATA_INTERVAL 20  //milliseconds
#define DEBUG_PRINT printf
#define DISPLAY_LOOP_TIME 0
/************************/
/*      ACC/GYRO        */
/************************/
#define AHRS
#define MPU6050
/**********************/
/*       sonar        */
/**********************/
//#define SONAR
/**********************/
/*       buzzer       */
/**********************/
#define BUZZER
/**********************/
/*       battery      */
/**********************/
#define BATTERY
/**********************/
/*       Gas          */
/**********************/
#define GAS
/**********************/
/*       IR          */
/**********************/
#define IR
/**********************/
/*   temperature      */
/**********************/
#define TEMPERATURE
#endif //CONFIG_H_


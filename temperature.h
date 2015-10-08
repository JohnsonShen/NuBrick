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
#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "timerctrl.h"

void Init_DHT11_PWM1(void);
void Get_DHT11(void);
void DHT11GetDATA(void);

extern volatile uint8_t DHT11_temperature;
extern volatile uint8_t DHT11_Humidity;
extern volatile uint16_t DHT11_DATA[42];
extern uint16_t capture_count;
extern uint8_t DHT11ExecuteFlag;

#endif /* TEMPERATURE_H_ */

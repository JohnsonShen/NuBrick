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
 * HCHEISH@nuvoton.com                                          *
 *=============================================================*
 */
#ifndef LED_H_
#define LED_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "pwm.h"
#include "timerctrl.h"
#include "tid.h"
#include "music.h"

void Led_Init(void);
void Led_Alerm(uint16_t, uint16_t, uint8_t, uint8_t, uint8_t);
void Led_Stop(void);
void Led_Blink_Start(void);
void Led_Blink_Check(void);
void Led_Control(void);

extern uint8_t LedExecuteFlag;
extern uint8_t LedMusicExecuteFlag;

#endif /* LED_H_ */

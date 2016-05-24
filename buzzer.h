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
#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "pwm.h"
#include "timerctrl.h"
#include "tid.h"
#include "music.h"

void Buzzer_Init(void);
void Buzzer_Alerm(uint16_t, uint16_t, uint8_t);
void Buzzer_Stop(void);
void Buzzer_Song_Start(void);
void Buzzer_Song_Check(void);
void Buzzer_Control(void);

extern uint8_t BuzzerExecuteFlag;

#endif /* BUZZER_H_ */

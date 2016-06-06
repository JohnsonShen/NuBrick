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
#ifndef SONAR_H_
#define SONAR_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "config.h"
#include "timerctrl.h"
#include "tid.h"

#define	SONARDISTANCE_SCALE 11.7546;						//distance(cm) = clock(5us)/11.7546

void SonarInit(void);
void SonarDetect(void);
void SonarTimeOutCheck(void);

extern float SonarDistance_Scale;					//Distance between sonar and object
extern float Sonar_Distance;							//Distance between sonar and object
extern float Sonar_Distance_OUT;					//Distance between sonar and object
extern uint32_t Sonar_caprure_timer;			//Sonar PWM capture time between rising and falling
extern uint8_t SonarExecuteFLAG;					//Sonar start to detect distance
extern uint8_t SonarERRORCounter;					//Sonar time out flag
extern int32_t SonarOverTimeCounter;

#endif /* SONAR_H_ */

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
 * HCHSIEH@nuvoton.com                                         *
 *=============================================================*
 */

#ifndef TIDDEV_H_
#define TIDDEV_H_

#include <stdio.h>
#include "M451Series.h"
#include "devCheck.h"
#include "tid.h"
#include "i2c_ms.h"

void SlvDataInit(void);
void SlvDataStore(void);

void SlvBuzDataInit(void);
void SlvBuzDataStore(void);

extern uint8_t TIDDevEnFlag[8];
extern uint8_t TIDMstStage;
extern uint8_t TIDMstInitFIN;
extern uint8_t TIDMstFirstInitFIN;
extern uint16_t TIDMstInitDevState;

#endif //TIDDEV

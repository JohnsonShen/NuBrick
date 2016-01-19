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
#ifndef TIDMST_H_
#define TIDMST_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "def.h"
#include "i2c_ms.h"
#include "tid.h"
#include "tiddev.h"

void TIDMst_GetDev(void);
void TIDMstUpdateDevState(void);

void TIDMstDevTRx(void);
void MstDataInit(void);

extern uint16_t TIDMstInitDevState;
extern uint8_t TIDMstDevState[16];


#endif /* TIDMST_H_ */

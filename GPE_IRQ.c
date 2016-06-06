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

#include "GPE_IRQ.h"

void GPE_IRQHandler(void)
{
	/* To check if PE.0 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PE, BIT0))
	{
		GPIO_CLR_INT_FLAG(PE, BIT0);
		if(PE0 == 0)
			keyState |= 1 << 7;
		else if(PE0 == 1)
			keyState &= ~(1 << 7);
	}
	KeyDev.Input.data1.value = keyState;
	if(KeyDev.Input.data1.value > 0)
	{
		PA2 = 0;
	}
	else
	{
		PA2 = 1;
	}
}

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

#include "GPC_IRQ.h"

void GPC_IRQHandler(void)
{
	/* To check if PC.0 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT0))
	{
		GPIO_CLR_INT_FLAG(PC, BIT0);
		if(PC0 == 0)
			keyState |= 1;
		else if(PC0 == 1)
			keyState &= ~(1);
	}
	/* To check if PC.1 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT1))
	{
		GPIO_CLR_INT_FLAG(PC, BIT1);
		if(PC1 == 0)
			keyState |= 1 << 1;
		else if(PC1 == 1)
			keyState &= ~(1 << 1);
	}
	/* To check if PC.2 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT2))
	{
		GPIO_CLR_INT_FLAG(PC, BIT2);
		if(PC2 == 0)
			keyState |= 1 << 2;
		else if(PC2 == 1)
			keyState &= ~(1 << 2);
	}
	/* To check if PC.3 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT3))
	{
		GPIO_CLR_INT_FLAG(PC, BIT3);
		if(PC3 == 0)
			keyState |= 1 << 3;
		else if(PC3 == 1)
			keyState &= ~(1 << 3);
	}
	/* To check if PC.4 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT4))
	{
		GPIO_CLR_INT_FLAG(PC, BIT4);
		if(PC4 == 0)
			keyState |= 1 << 4;
		else if(PC4 == 1)
			keyState &= ~(1 << 4);
	}
	/* To check if PC.5 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT5))
	{
		GPIO_CLR_INT_FLAG(PC, BIT5);
		if(PC5 == 0)
			keyState |= 1 << 5;
		else if(PC5 == 1)
			keyState &= ~(1 << 5);
	}
	/* To check if PC.6 interrupt occurred */
	if(GPIO_GET_INT_FLAG(PC, BIT6))
	{
		GPIO_CLR_INT_FLAG(PC, BIT6);
		if(PC6 == 0)
			keyState |= 1 << 6;
		else if(PC6 == 1)
			keyState &= ~(1 << 6);
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

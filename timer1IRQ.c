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

#include "timer1IRQ.h"

volatile uint32_t TMR1INTCount;				// 1s timer counter
volatile uint32_t TMR1TimerCounter;			// 1s timer counter

// *************************************************************************
//  Timer1 initialize setting
//	Sleep every 1s and wake up
// *************************************************************************

void Timer1Init(uint16_t freq)
{
	/* Enable peripheral clock */
	CLK_EnableModuleClock(TMR1_MODULE);

	/* Peripheral clock source */
	CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_LIRC, 0);

	/* Enable Timer0 NVIC */
	NVIC_EnableIRQ(TMR1_IRQn);

	/* Open Timer0 time-out frequency to (freq) Hz in periodic mode */
	TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, freq);

	/* Enable Timer0 time-out interrupt and wake-up function */
	TIMER_EnableInt(TIMER1);
	TIMER_EnableWakeup(TIMER1);

	/* Start Timer0 counting */
	TIMER_Start(TIMER1);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  TMR1 IRQ handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void)
{
	if(TIMER_GetIntFlag(TIMER1) == 1)
	{
		/* Clear Timer0 time-out interrupt flag */
		TIMER_ClearIntFlag(TIMER1);
		TMR1INTCount++;
	}

	if(TIMER_GetWakeupFlag(TIMER1) == 1)
	{
		/* Clear Timer0 wake-up flag */
		TIMER_ClearWakeupFlag(TIMER1);
		//printf("I'm awake by TMR.\n");
	}
}

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
#include <stdio.h>
#include "math.h"
#include "def.h"

#include "M451Series.h"
#include "timerctrl.h"
#include "AHRSLib.h"

volatile uint32_t tick_counter = 0;
uint32_t frame_counter = 0;
uint32_t GPABCallback_Counter=0;

volatile uint32_t u32Timer0Cnt=0, u32Timer1Cnt=0, u32Timer2Cnt=0, u32Timer3Cnt=0;

void Delay(uint32_t delayCnt)
{
	while(delayCnt--)
	{
		__NOP();
		__NOP();
	}
}
void DelayLoop(unsigned short delay)
{
	while(--delay);//About 200 psec per run
}
void DelayUsec(unsigned int usec)
{
	while(usec--)
		DelayLoop(5);
}
void DelayMsec(unsigned short msec)
{
	int tick_count= getTickCount();
	while((getTickCount()-tick_count)<msec);
}
void setup_system_tick(uint32_t sampleRate)
{
	uint32_t tickPeriod = SystemCoreClock/sampleRate;

	SysTick_Config(tickPeriod);
	printf("SystemCoreClock:%d\n",SystemCoreClock);
	printf("Tick Time: %d us\n",1000000/sampleRate);
}
void SysTick_Handler(void)
{
	tick_counter++;
	nvtMillisecondTick();
}
int getTickCount()
{
	return tick_counter;
}

uint32_t micros()
{
	return u32Timer2Cnt;
}

uint32_t millis()
{
	return (u32Timer2Cnt/1000);
}


void TMR2_IRQHandler(void)
{
	if(TIMER_GetIntFlag(TIMER2) == 1)
	{
		/* Clear Timer2 time-out interrupt flag */
		TIMER_ClearIntFlag(TIMER2);
		u32Timer2Cnt+=10;
	}
}

void TIMER_Enable(char enable)
{
	if(enable)
		TIMER_EnableInt(TIMER2);
	else
		TIMER_DisableInt(TIMER2);
}


void TIMER_Init()
{
	/* Enable peripheral clock */
	CLK_EnableModuleClock(TMR2_MODULE);
	CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR2SEL_HIRC, 0);

	TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 100000);
	TIMER_EnableInt(TIMER2);
	NVIC_SetPriority (TMR2_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	/* Enable Timer2 NVIC */
	NVIC_EnableIRQ(TMR2_IRQn);
	/* Start Time2 counting */
	TIMER_Start(TIMER2);
}

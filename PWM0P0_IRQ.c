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

#include "PWM0P0_IRQ.h"

uint8_t DHT11_CalPeriodTime(PWM_T *PWM, uint32_t u32Ch)
{
    uint16_t u16RisingTime, u16FallingTime, u16Period;

		u16RisingTime = PWM_GET_CAPTURE_RISING_DATA(PWM, u32Ch);
	
		u16FallingTime = PWM_GET_CAPTURE_FALLING_DATA(PWM, u32Ch);
	
		u16Period = u16RisingTime - u16FallingTime;
	
		//printf("\nCapture Result: Rising Time = %d, Falling Time = %d \nHigh Period = %d",
           //u16RisingTime, u16FallingTime, u16Period);
	
		return u16Period;
		
}

void PWM0P0_IRQHandler(void)
{
	if(capture_count<41){
		DHT11_DATA[capture_count] = DHT11_CalPeriodTime(PWM0, 0);
		capture_count++;
	}
		PWM_ClearCaptureIntFlag(PWM0, 0, PWM_CAPTURE_INT_FALLING_LATCH);
}

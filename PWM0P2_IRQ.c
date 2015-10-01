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

#include "PWM0P2_IRQ.h"

// ============================================================
//		caculate time during pwm0_ch4 falling and rising edge
// ============================================================
uint16_t IrSonar_CalPeriodTime(PWM_T *PWM, uint32_t u32Ch)
{
    uint16_t u16RisingTime, u16FallingTime, u16Period;

		u16RisingTime = PWM_GET_CAPTURE_RISING_DATA(PWM, u32Ch);
	
		u16FallingTime = PWM_GET_CAPTURE_FALLING_DATA(PWM, u32Ch);
	
		u16Period = u16RisingTime - u16FallingTime;
	
		//printf("\nCapture Result: Rising Time = %d, Falling Time = %d \nHigh Period = %d",
           //u16RisingTime, u16FallingTime, u16Period);
	
		return u16Period;
		
}

// **************************************************************
//		PWM0P0 interrupt is for IR Reciver
// **************************************************************
void PWM0P2_IRQHandler(void)
{
#ifdef SONAR
	// ============================================================
	//			Caculate distance between object and sonar
	//			distance(cm) = time(us)/58.773
	//			distance(cm) = clock(10us)/5.8773
	// ============================================================
	if(PWM_GetCaptureIntFlag(PWM0, 4))
	{
		//Clear interrupt
		PWM_ClearCaptureIntFlag(PWM0, 4, PWM_CAPTURE_INT_FALLING_LATCH);
	
		Sonar_caprure_timer = IrSonar_CalPeriodTime(PWM0, 4);
		// Caculate distance between Sonar and object
		Sonar_Distance = (float)Sonar_caprure_timer/(float)SONARDISTANCE_SCALE;
		Sonar_Distance_OUT = Sonar_Distance;
		//Enable next time distence detect
		SonarExecuteFLAG=0;
	}
#endif
#ifdef IR
	// ============================================================
	//			Get IR data
	// ============================================================
	if(PWM_GetCaptureIntFlag(PWM0, 5))
	{
		//Clear interrupt
		PWM_ClearCaptureIntFlag(PWM0, 5, PWM_CAPTURE_INT_FALLING_LATCH);
	
		PWM_caprure_timer = IrSonar_CalPeriodTime(PWM0, 5);
	
		//start receive IR raw data
		if(IR_RxExecute_Flag==0)
		{
			if(PWM_caprure_timer < IRRX_FIRST_BIT_MAX && PWM_caprure_timer > IRRX_FIRST_BIT_MIN)  // start if > 3ms 
			{
				IR_RxExecute_Flag=1;
				IR_init_counter=getTickCount();
			}
		}
		//store IR raw data
		else if(IR_capture_count<32 && IR_RxExecute_Flag==1)
		{
			IR_rawDATA[IR_capture_count] = PWM_caprure_timer;
			IR_capture_count++;
			if(IR_capture_count==32)
			{
				IR_RxComplete_Flag=1;
			}
		}
		// Detect stop bit
		if(PWM_caprure_timer>1500)
		{
			IR_RxExecute_Flag = 0;
			IR_capture_count = 0;
		}
	}
#endif
}

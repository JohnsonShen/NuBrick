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

#include "temperature.h"

volatile uint16_t DHT11_temperature;
volatile uint16_t DHT11_Humidity;
uint16_t DHT11_Enable_Flag;
uint16_t DHT11_init_counter;
extern uint16_t capture_count;
extern uint16_t DHT11_DATA[40];

// ---------------------------------------------------------------------------------------
//  Timer0 initialize setting
//	Set PD.11 as output
//  Select PCLK as PWM module clock source  
// ---------------------------------------------------------------------------------------
void Init_DHT11(void)
{	
		TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 1000000);
		TIMER_SET_PRESCALE_VALUE(TIMER3, 72);
	  // Set compare value as large as possible, so don't need to worry about counter overrun too frequently.
    TIMER_SET_CMP_VALUE(TIMER3, 0xFFFFFF);
		TIMER_EnableCapture(TIMER3, TIMER_CAPTURE_RISING_EDGE, TIMER_COUNTER_FALLING_EDGE);
		//TIMER_EnableCapture(TIMER3, TIMER_CAPTURE_TRIGGER_COUNTING_MODE, TIMER_CAPTURE_RISING_THEN_FALLING_EDGE);
    NVIC_EnableIRQ(TMR3_IRQn);
		GPIO_SetMode(PD,BIT11,GPIO_MODE_OUTPUT);
}
// ---------------------------------------------------------------------------------------
//  Get tempture and Humidity data from DHT11 
//	Output: DHT11_temperature  
// ---------------------------------------------------------------------------------------
void Get_DHT11()
{
	int i;																		//for data update
	uint16_t DHT11_Humidity,DHT11_temp;				//for tempturature and humidity data tramsfer
	
	if(DHT11_init_counter == 0)
	{
		if(DHT11_Enable_Flag==1)
		{
			DHT11_init_counter = getTickCount();
			capture_count  = 0;
			DHT11_Humidity = 0; 
			DHT11_temp     = 0;
			PD11 = 0;
			DHT11_Enable_Flag=0;
		}
		else
		{
			if((getTickCount()-DHT11_init_counter)>18)
			{
				PD11 = 0;
				SYS->GPC_MFPL = (SYS->GPC_MFPL & ~SYS_GPD_MFPH_PD11MFP_Msk) | SYS_GPD_MFPH_PD11MFP_T3;
				// Start Timer 0
				TIMER_Start(TIMER3);
				TIMER_EnableCaptureInt(TIMER3);
			}
		}
	}
	
	else if(DHT11_init_counter > 40)
	{
		// Stop Timer0 INT
		TIMER_DisableCaptureInt(TIMER3);
		TIMER_Stop(TIMER3);
		
		//Update Data
		for (i=1;i<17;i++)
		{
			if (DHT11_DATA[i] >45 )
				DHT11_Humidity = ((DHT11_Humidity << 1) | 1);
			else
				DHT11_Humidity = DHT11_Humidity << 1 ;
		}	
		
		for (i=17;i<33;i++)
		{
			if (DHT11_DATA[i] >30 )
				DHT11_temp = ((DHT11_temp << 1)  | 1);
			else
				DHT11_temp = DHT11_temp << 1 ;
		}	
		DHT11_Humidity = DHT11_Humidity>>8;
		DHT11_temp = DHT11_temp>>8;
		DHT11_init_counter=0;
	}
}

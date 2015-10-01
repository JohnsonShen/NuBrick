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

#include "buzzer.h"

uint8_t BuzzerExecuteFlag;

/*
uint16_t music[72] = {
	E6 ,D6,	E6 ,D6,	E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0 ,
	C5 ,E5 ,A5 ,B5 ,B5 , 0 ,C5 ,A5 ,B5 ,C6 ,C6 , 0 ,
	E6 ,D6,	E6 ,D6,	E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0 ,
	C5 ,E5 ,A5 ,B5 ,B5 , 0 ,E5 ,C6 ,B5 ,A5 ,A5 , 0 ,
	B5 ,C6 ,D6 ,E6 ,E6 , 0 ,G5 ,F6 ,E6 ,D6 ,D6 , 0 ,
	F5 ,E6 ,D6 ,C6 ,C6 , 0 ,E5 ,D6 ,C6 ,B5 ,B5 , 0 };
	
uint32_t pitch[72] = {
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms
	};
*/

// ---------------------------------------------------------------------------------------
//  PWM initialize setting
//	Set PC9~PC11 as PWM output
//  Select PCLK as PWM module clock source  
// ---------------------------------------------------------------------------------------
void Buzzer_Init()
{
		 /* Enable PWM module clock */
    CLK_EnableModuleClock(PWM0_MODULE);
	
	  /* Select PWM module clock source */
    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, 0);
	
	  /* Reset PWM1 channel 0~5 */
    SYS_ResetModule(PWM0_RST);
	
	  /* Set PC9~PC11 multi-function pins for PWM1 Channel0~2  */
    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC0MFP_Msk);
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC0MFP_PWM0_CH0;
	
}

// ----------------------------------------------------------------------------------------
//  Start PWM Output
// ----------------------------------------------------------------------------------------
void Buzzer_Alerm()
{
		BuzzerExecuteFlag=1;
		/* set PWMB channel 0 output configuration */
		PWM_ConfigOutputChannel(PWM0, 0, 1200, 20);
		/* Enable PWM Output path for PWMB channel 0 */
		PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
	  // Start
    PWM_Start(PWM0, PWM_CH_0_MASK);
}

// ----------------------------------------------------------------------------------------
//  Stop PWM Output
// ----------------------------------------------------------------------------------------
void Buzzer_Stop()
{
		/* Diable PWM Output path for PWMB channel 0 */
		PWM_DisableOutput(PWM0, BIT0); // disable PWM0 Channel 3
		BuzzerExecuteFlag=0;
}

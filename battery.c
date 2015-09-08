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
 
#include "battery.h"
 
// ---------------------------------------------------------------------------------------
//  ADC initialize setting
//	Set PB2 as ADC converter
//  Select APB0/8 as ADC module clock source  
// --------------------------------------------------------------------------------------- 
void Battery_Init()
{
	SYS_UnlockReg();
	/* Enable EADC module clock */
	CLK_EnableModuleClock(EADC_MODULE);	
	/* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
	CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));
	SYS_LockReg();	
	/* Configure the GPB0 - GPB3 ADC analog input pins.  */
	SYS->GPB_MFPL &= ~SYS_GPB_MFPL_PB2MFP_Msk;
	SYS->GPB_MFPL |= SYS_GPB_MFPL_PB2MFP_EADC_CH2;
	
	GPIO_DISABLE_DIGITAL_PATH(PB, 0x4);
	
	/* Set the ADC internal sampling time, input mode as single-end and enable the A/D converter */
	EADC_Open(EADC, EADC_CTL_DIFFEN_SINGLE_END);
	EADC_SetInternalSampleTime(EADC, 6);

	/* Configure the sample module 0 for analog input channel 2 and software trigger source.*/
	EADC_ConfigSampleModule(EADC, 0, EADC_SOFTWARE_TRIGGER, 2);
	
	/* Clear the A/D ADINT0 interrupt flag for safe */
	EADC_CLR_INT_FLAG(EADC, 0x1);

	/* Enable the sample module 0 interrupt.  */
	EADC_ENABLE_INT(EADC, 0x1);//Enable sample module A/D ADINT0 interrupt.
	EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 0, 0x1);//Enable sample module 0 interrupt.
}

// ----------------------------------------------------------------------------------------
//  Start ADC conversion
// ----------------------------------------------------------------------------------------
uint8_t GetBattery()
{
	EADC_START_CONV(EADC, 0x1);
	while(EADC_GET_INT_FLAG(EADC, 0x1) == 0);
	return ((EADC_GET_CONV_DATA(EADC, 0))*100/4096);
}

void PowerControl()
{
	
}


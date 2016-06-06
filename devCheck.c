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
 * Written by MU20 for Nuvoton Technology.                     *
 * tlshen@nuvoton.com/tzulan611126@gmail.com                   *
 * HCHSIEH@nuvoton.com                                         *
 *=============================================================*
 */

#include "devCheck.h"

/*----------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                  */
/*----------------------------------------------------------------------------------------*/
volatile uint32_t devNum;

// ---------------------------------------------------------------------------------------
//  Gas ADC initialize setting
//	Set PB2 as ADC converter
//  Select APB0/8 as ADC module clock source
// ---------------------------------------------------------------------------------------
void ID_Init()
{
	SYS_UnlockReg();
	/* Enable EADC module clock */
	CLK_EnableModuleClock(EADC_MODULE);
	/* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
	CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));
	SYS_LockReg();
	/* Configure the GPB0 - GPB3 ADC analog input pins.  */
	SYS->GPB_MFPL &= ~SYS_GPB_MFPL_PB0MFP_Msk;
	SYS->GPB_MFPL |= SYS_GPB_MFPL_PB0MFP_EADC_CH0;

	GPIO_DISABLE_DIGITAL_PATH(PB, BIT0);

	/* Set the ADC internal sampling time, input mode as single-end and enable the A/D converter */
	EADC_Open(EADC, EADC_CTL_DIFFEN_SINGLE_END);
	EADC_SetInternalSampleTime(EADC, 6);

	/* Configure the sample module 0 for analog input channel 1 and software trigger source.*/
	EADC_ConfigSampleModule(EADC, 0, EADC_SOFTWARE_TRIGGER, 0);

	/* Clear the A/D ADINT0 interrupt flag for safe */
	EADC_CLR_INT_FLAG(EADC, 0x1);

	/* Enable the sample module 0 interrupt.  */
	EADC_ENABLE_INT(EADC, 0x1);//Enable sample module A/D ADINT0 interrupt.
	EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 0, 0x1);//Enable sample module 0 interrupt.
}

void GetID()
{
	static uint16_t u16Range[][2] =
	{
		{ 125,  310},
		{ 373,  558},
		{ 621,  806},
		{ 869, 1055},
		{1118, 1303},
		{1366, 1551},
		{1616, 1799},
		{1861, 2048},
		{2111, 2296},
		{2359, 2544},
		{2607, 2792},
		{2855, 3040},
		{3103, 3289},
		{3351, 3537},
		{3600, 3785}
	};
	uint16_t adcData;
	uint32_t i;

	// Clear the ADC INT0 interrupt flag
	EADC_CLR_INT_FLAG(EADC, 0x1);
	// Clear the ADC INT0 interrupt flag
	EADC_START_CONV(EADC, 0x1);
	//Trigger sample module 0 to start A/D conversion
	while(EADC_GET_INT_FLAG(EADC, 0x1) == 0);
	//Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)
	adcData = EADC_GET_CONV_DATA(EADC, 0);

	//check module ID

	for(i=0; i<(sizeof(u16Range)/sizeof(uint16_t)/2); i++)
	{
		if (adcData > u16Range[i][0] && adcData < u16Range[i][1])
		{
			devNum = i;
			break;
		}
	}

	if (i >= (sizeof(u16Range)/sizeof(uint16_t)/2))
		devNum = 100;

}

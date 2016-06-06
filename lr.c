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

#include "lr.h"

/*----------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                  */
/*----------------------------------------------------------------------------------------*/
uint16_t  	LrData;
uint16_t  	LrOverTimeCounter;

/* ---------------------------------------------------------------------------------------*/
/*  ADC initialize setting																																*/
/*	Set PB2 as ADC converter																															*/
/*  Select APB0/8 as ADC module clock source  																						*/
/* ---------------------------------------------------------------------------------------*/
void Lr_Init()
{
	SYS_UnlockReg();
	/* Enable EADC module clock */
	CLK_EnableModuleClock(EADC_MODULE);
	/* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
	CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));
	SYS_LockReg();
	/* Configure the GPB0 - GPB3 ADC analog input pins.  */
	SYS->GPB_MFPL &= ~SYS_GPB_MFPL_PB1MFP_Msk;
	SYS->GPB_MFPL |= SYS_GPB_MFPL_PB1MFP_EADC_CH1;

	GPIO_DISABLE_DIGITAL_PATH(PB, BIT1);

	/* Set the ADC internal sampling time, input mode as single-end and enable the A/D converter */
	EADC_Open(EADC, EADC_CTL_DIFFEN_SINGLE_END);
	EADC_SetInternalSampleTime(EADC, 6);

	/* Configure the sample module 0 for analog input channel 1 and software trigger source.*/
	EADC_ConfigSampleModule(EADC, 1, EADC_SOFTWARE_TRIGGER, 1);

	/* Clear the A/D ADINT0 interrupt flag for safe */
	EADC_CLR_INT_FLAG(EADC, 0x2);

	/* Enable the sample module 0 interrupt.  */
	EADC_ENABLE_INT(EADC, 0x2);//Enable sample module A/D ADINT0 interrupt.
	EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 1, 0x2);//Enable sample module 0 interrupt.

	ResDev9.DevDesc.DevDesc_leng = 26;						//Report descriptor
	ResDev9.DevDesc.RptDesc_leng = 36;						//Report descriptor
	ResDev9.DevDesc.InRptLeng = 5;								//Input report
	ResDev9.DevDesc.OutRptLeng = 0;							//Output report
	ResDev9.DevDesc.GetFeatLeng = 6;							//Get feature
	ResDev9.DevDesc.SetFeatLeng = 6;							//Set feature
	ResDev9.DevDesc.CID = 0;											//manufacturers ID
	ResDev9.DevDesc.DID = 0;											//Product ID
	ResDev9.DevDesc.PID = 0;											//Device firmware revision
	ResDev9.DevDesc.UID = 0;											//Device Class type
	ResDev9.DevDesc.UCID = 0;										//reserve
	/* Feature */
	ResDev9.Feature.data1.minimum = 0;						//Sleep period
	ResDev9.Feature.data1.maximum = 1024;
	ResDev9.Feature.data1.value = 100;
	ResDev9.Feature.data2.minimum = 0;						//Light resistant alerm value
	ResDev9.Feature.data2.maximum = 100;
	ResDev9.Feature.data2.value = 20;
	ResDev9.Feature.arg[0] = 1;
	ResDev9.Feature.arg[1] = 2;
	ResDev9.Feature.datalen[0] = 2;
	ResDev9.Feature.datalen[1] = 2;
	ResDev9.Feature.dataNum = 2;
	/* Input */
	ResDev9.Input.data1.minimum = 0;							//light resistance value
	ResDev9.Input.data1.maximum = 100;
	ResDev9.Input.data1.value = 100;
	ResDev9.Input.data2.minimum = 0;							//Over flag
	ResDev9.Input.data2.maximum = 1;
	ResDev9.Input.data2.value = 0;
	ResDev9.Input.arg[0] = 1;
	ResDev9.Input.arg[1] = 2;
	ResDev9.Input.datalen[0] = 2;
	ResDev9.Input.datalen[1] = 1;
	ResDev9.Input.dataNum = 2;
	/* Output */
	ResDev9.Output.dataNum = 0;
}

// ----------------------------------------------------------------------------------------
//  Start ADC conversion
// ----------------------------------------------------------------------------------------
void GetLr(void)
{
	// Clear the ADC INT0 interrupt flag
	EADC_CLR_INT_FLAG(EADC, 0x2);
	//Trigger sample module 0 to start A/D conversion
	EADC_START_CONV(EADC, 0x2);
	//Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)
	while(EADC_GET_INT_FLAG(EADC, 0x2) == 0);
	//Trigger sample module 0 to start A/D conversion
	LrData = ((EADC_GET_CONV_DATA(EADC, 1))*100/4096);

	/* Update TID value */
	ResDev9.Input.data1.value = LrData;
	if(ResDev9.Input.data1.value > ResDev9.Feature.data2.value)
	{
		ResDev9.Input.data2.value = 1;
		LrOverTimeCounter = getTickCount()+ ResDev9.Feature.data3.value*1000;
	}
	else
	{
		ResDev9.Input.data2.value = 0;
	}
	/* reset alerm flag after 10s */
	if(ResDev9.Output.data1.value == 1)
		ResDev9.Input.data2.value = 0;
}

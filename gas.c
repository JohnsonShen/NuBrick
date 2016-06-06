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

#include "gas.h"

/*----------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                  */
/*----------------------------------------------------------------------------------------*/
uint16_t	GasData;
int32_t		GasOverTimeCounter;			//Sonar alerm time

// ---------------------------------------------------------------------------------------
//  Gas ADC initialize setting
//	Set PB2 as ADC converter
//  Select APB0/8 as ADC module clock source
// ---------------------------------------------------------------------------------------
void Gas_Init()
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
	/* Configure the PC.5 output pin.  */
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC5MFP_Msk));
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC5MFP_GPIO;
	GPIO_SetMode(PC,BIT5,GPIO_MODE_OUTPUT);
	PC5=1;
	//LED
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk);
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_GPIO;
	GPIO_SetMode(PA,BIT2,GPIO_MODE_OUTPUT);
	PA2 = 1;

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

	GasDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	GasDev.DevDesc.RptDesc_leng = 36;						//Report descriptor
	GasDev.DevDesc.InRptLeng = 5;							//Input report
	GasDev.DevDesc.OutRptLeng = 0;							//Output report
	GasDev.DevDesc.GetFeatLeng = 6;							//Get feature
	GasDev.DevDesc.SetFeatLeng = 6;							//Set feature
	GasDev.DevDesc.CID = 0;									//manufacturers ID
	GasDev.DevDesc.DID = 0;									//Product ID
	GasDev.DevDesc.PID = 0;									//Device firmware revision
	GasDev.DevDesc.UID = 0;									//Device Class type
	GasDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	GasDev.Feature.data1.minimum = 0;						//Sleep period
	GasDev.Feature.data1.maximum = 1024;
	GasDev.Feature.data1.value = 100;
	GasDev.Feature.data2.minimum = 0;						//Gas alerm value
	GasDev.Feature.data2.maximum = 100;
	GasDev.Feature.data2.value = 50;
	GasDev.Feature.arg[0] = 1;
	GasDev.Feature.arg[1] = 2;
	GasDev.Feature.datalen[0] = 2;
	GasDev.Feature.datalen[1] = 2;
	GasDev.Feature.dataNum = 2;
	/* Input */
	GasDev.Input.data1.minimum = 0;							//sensored disance
	GasDev.Input.data1.maximum = 100;
	GasDev.Input.data1.value = 50;
	GasDev.Input.data2.minimum = 0;							//Over flag
	GasDev.Input.data2.maximum = 1;
	GasDev.Input.data2.value = 0;
	GasDev.Input.arg[0] = 1;
	GasDev.Input.arg[1] = 2;
	GasDev.Input.datalen[0] = 2;
	GasDev.Input.datalen[1] = 1;
	GasDev.Input.dataNum = 2;
	/* Output */
	GasDev.Output.dataNum = 0;
}

// ----------------------------------------------------------------------------------------
//  Start Gas ADC conversion
// ----------------------------------------------------------------------------------------
void GetGas()
{
	// Clear the ADC INT0 interrupt flag
	EADC_CLR_INT_FLAG(EADC, 0x2);
	// Clear the ADC INT0 interrupt flag
	EADC_START_CONV(EADC, 0x2);
	//Trigger sample module 0 to start A/D conversion
	while(EADC_GET_INT_FLAG(EADC, 0x2) == 0);
	//Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)
	GasData = ((EADC_GET_CONV_DATA(EADC, 1))*100/4096);
	//printf("GasData = %d\n",GasData);

	/* Update TID value */
	GasDev.Input.data1.value = GasData;
	if(GasDev.Input.data1.value < GasDev.Feature.data2.value)
	{
		PA2 = 0;
		GasDev.Input.data2.value = 1;
		//GasOverTimeCounter = getTickCount()+ GasDev.Feature.data3.value*1000;
	}
	else
	{
		PA2 = 1;
		GasDev.Input.data2.value = 0;
	}
	
	/* reset alerm flag after 10s */
//	if(GasDev.Input.data2.value == 1)
//	{
//		if(getTickCount() > GasOverTimeCounter)
//			GasDev.Input.data2.value = 0;
//		if(GasDev.Output.data1.value == 1)
//			GasDev.Input.data2.value = 0;
//	}
	
}

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
#include "timer1IRQ.h"
#include "tidmst.h"

/*----------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                  */
/*----------------------------------------------------------------------------------------*/
uint16_t	BatteryData;
int32_t 	BatOverTimeCounter;			//Sonar alerm time

/* ---------------------------------------------------------------------------------------*/
/*  ADC initialize setting																																*/
/*	Set PB2 as ADC converter																															*/
/*  Select APB0/8 as ADC module clock source  																						*/
/* ---------------------------------------------------------------------------------------*/
void Battery_Init()
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

	BatDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	BatDev.DevDesc.RptDesc_leng = 36;						//Report descriptor
	BatDev.DevDesc.InRptLeng = 5;							//Input report
	BatDev.DevDesc.OutRptLeng = 0;							//Output report
	BatDev.DevDesc.GetFeatLeng = 6;							//Get feature
	BatDev.DevDesc.SetFeatLeng = 6;							//Set feature
	BatDev.DevDesc.CID = 0;									//manufacturers ID
	BatDev.DevDesc.DID = 0;									//Product ID
	BatDev.DevDesc.PID = 0;									//Device firmware revision
	BatDev.DevDesc.UID = 0;									//Device Class type
	BatDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	BatDev.Feature.data1.minimum = 0;						//Sleep period
	BatDev.Feature.data1.maximum = 1024;
	BatDev.Feature.data1.value = 100;
	BatDev.Feature.data2.minimum = 0;						//Battery alerm value
	BatDev.Feature.data2.maximum = 100;
	BatDev.Feature.data2.value = 50;
	BatDev.Feature.arg[0] = 1;
	BatDev.Feature.arg[1] = 2;
	BatDev.Feature.datalen[0] = 2;
	BatDev.Feature.datalen[1] = 2;
	BatDev.Feature.dataNum = 2;
	/* Input */
	BatDev.Input.data1.minimum = 0;							//Battery value
	BatDev.Input.data1.maximum = 100;
	BatDev.Input.data1.value = 100;
	BatDev.Input.data2.minimum = 0;							//Over flag
	BatDev.Input.data2.maximum = 1;
	BatDev.Input.data2.value = 0;
	BatDev.Input.arg[0] = 1;
	BatDev.Input.arg[1] = 2;
	BatDev.Input.datalen[0] = 2;
	BatDev.Input.datalen[1] = 1;
	BatDev.Input.dataNum = 2;
	/* Output */
	BatDev.Output.dataNum = 0;
}

// ----------------------------------------------------------------------------------------
//  Start ADC conversion
// ----------------------------------------------------------------------------------------
void GetBattery(void)
{
	// Clear the ADC INT0 interrupt flag
	EADC_CLR_INT_FLAG(EADC, 0x2);
	//Trigger sample module 0 to start A/D conversion
	EADC_START_CONV(EADC, 0x2);
	//Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)
	while(EADC_GET_INT_FLAG(EADC, 0x2) == 0);
	//Trigger sample module 0 to start A/D conversion
	BatteryData = ((EADC_GET_CONV_DATA(EADC, 1))*100/4096);

	/* Update TID value */
	BatDev.Input.data1.value = BatteryData;
	if(BatDev.Input.data1.value < BatDev.Feature.data2.value)
	{
		BatDev.Input.data2.value = 1;
		BatOverTimeCounter = getTickCount()+ BatDev.Feature.data3.value*1000;
	}
	/* reset alerm flag after 10s */
	if(BatDev.Input.data2.value == 1)
	{
		if(getTickCount() > BatOverTimeCounter)
			BatDev.Input.data2.value = 0;
		if(BatDev.Output.data1.value == 1)
			BatDev.Input.data2.value = 0;
	}
}

void PowerControl()
{

}

void MasterControl(void)
{
	/* Master Store data */
	TIDMstUpdateDevState();
	/* Master recheck device */
	if(TIDMstFirstInitFIN==1)
	{
		if(TMR1TimerCounter > 10)
		{
			if(I2CMstEndFlag==1)
			{
				TIDMstInitFIN=0;
				TMR1TimerCounter=0;
				I2C_Close(I2C_MS_PORT);
				I2C_MS_Master_Restart();
			}
		}
		else
		{
			TIDMstInitFIN = 1;
			TMR1TimerCounter++;
		}
	}
	GetBattery();
}


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
 
/*----------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                  */
/*----------------------------------------------------------------------------------------*/
uint8_t  BatteryData;

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
}

void PowerControl()
{
	int i;
	// =======================================================
	//                Print data & Sleep
	// =======================================================
	if( 1
#ifdef SONAR	
		&& (SonarExecuteFLAG == 0)
#endif
#ifdef TEMPERATURE
		&& (DHT11ExecuteFlag == 0)
#endif
#ifdef IR
		&& (IR_TxExecuteFLAG == 0)
		&& (IR_RxExecute_Flag == 0)
#endif
#ifdef BUZZER
		&& (BuzzerExecuteFlag == 0)
#endif
#ifdef I2C_MS_MASTER
		&& (I2CMstEndFlag == 1)
#else
		&& (I2CMSData[SLEEP_REG]==0)
#endif
	)
	{
#ifdef SONAR	
		printf("Sonar distance = %f\n",Sonar_Distance);
#endif
#ifdef IR
		printf("IR DATA[0]= %X, IR DATA[1]= %X, IR DATA[2]=%X, IR DATA[3]=%X\n",IR_DATA_OUT[0],IR_DATA_OUT[1],IR_DATA_OUT[2],IR_DATA_OUT[3]);
#endif
#ifdef BATTERY
		printf("BatteryData = %d\n",BatteryData);
#endif
#ifdef GAS
		printf("GasData = %d\n",GasData);
#endif
#ifdef TEMPERATURE
		printf("DHT11_Humidity=%d,DHT11_temperature=%d\n", DHT11_Humidity,DHT11_temperature);
#endif
		for(i = 0;i < 31;i++)
		printf("Data[%d]=%d\n",i,I2CMSData[i]);
		printf("Whole time:%d\n",(getTickCount()-MainTimeMSCounter));
		printf("I'm sleep.\n");
		while(((UART0->FIFOSTS) & UART_FIFOSTS_TXEMPTY_Msk) == 0);
		SYS_UnlockReg();
		CLK_PowerDown();
		SYS_LockReg();
	}
}


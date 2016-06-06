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

#include "led.h"

uint8_t LedStartFlag;							//single buzzer cycle execute flag
uint8_t LedSingleExecuteFlag;					//single buzzer cycle execute flag
uint8_t LedSingleCounter;						//buzzer single buzzer cycle counter
uint8_t LedBlinkSize;							//buzzer single buzzer cycle counter
uint16_t LedBri_G;								//buzzer Brightness
uint16_t LedBri_R;								//buzzer Brightness
uint16_t LedBri_B;								//buzzer Brightness
uint16_t LedTone;								//buzzer Tone
uint16_t LedBlink;								//buzzer song
uint16_t LedToneStartTime;						//buzzer period
uint16_t LedToneStopTime;						//buzzer duty
uint32_t LedTimeCnt;							//buzzer total time counter
uint32_t LedSingleTimeCnt;						//single buzzer cycle Time out flag
uint32_t LedLatency;							//single buzzer cycle Time out flag

// ---------------------------------------------------------------------------------------
//  PWM initialize setting
//	Set PC9~PC11 as PWM output
//  Select PCLK as PWM module clock source
// ---------------------------------------------------------------------------------------
void Led_Init()
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
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC1MFP_Msk);
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC1MFP_PWM0_CH1;
	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC2MFP_Msk);
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC2MFP_PWM0_CH2;
	//LED
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk);
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_GPIO;
	GPIO_SetMode(PA,BIT2,GPIO_MODE_OUTPUT);
	PA2 = 1;

	/* Initialize parameter */
	LedSingleExecuteFlag=0;
	LedStartFlag=0;

	LedDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	LedDev.DevDesc.RptDesc_leng = 74;						//Report descriptor
	LedDev.DevDesc.InRptLeng = 3;							//Input report
	LedDev.DevDesc.OutRptLeng = 4;							//Output report
	LedDev.DevDesc.GetFeatLeng = 12;						//Get feature
	LedDev.DevDesc.SetFeatLeng = 12;						//Set feature
	LedDev.DevDesc.CID = 0;									//manufacturers ID
	LedDev.DevDesc.DID = 0;									//Product ID
	LedDev.DevDesc.PID = 0;									//Device firmware revision
	LedDev.DevDesc.UID = 0;									//Device Class type
	LedDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	LedDev.Feature.data1.minimum = 0;						//SleepPrd
	LedDev.Feature.data1.maximum = 2048;
	LedDev.Feature.data1.value = 100;
	LedDev.Feature.data2.minimum = 0;						//Brightness
	LedDev.Feature.data2.maximum = 100;
	LedDev.Feature.data2.value = 30;
	LedDev.Feature.data3.minimum = 0;						//Color
	LedDev.Feature.data3.maximum = 4095;
	LedDev.Feature.data3.value = 4095;
	LedDev.Feature.data4.minimum = 0;						//Blink
	LedDev.Feature.data4.maximum = 2;
	LedDev.Feature.data4.value = 0;
	LedDev.Feature.data5.minimum = 0;						//Period
	LedDev.Feature.data5.maximum = 2048;
	LedDev.Feature.data5.value = 500;
	LedDev.Feature.data6.minimum = 0;						//Duty
	LedDev.Feature.data6.maximum = 100;
	LedDev.Feature.data6.value = 30;
	LedDev.Feature.data7.minimum = 0;						//Latency
	LedDev.Feature.data7.maximum = 60;
	LedDev.Feature.data7.value = 1;
	LedDev.Feature.arg[0] = 1;
	LedDev.Feature.arg[1] = 2;
	LedDev.Feature.arg[2] = 3;
	LedDev.Feature.arg[3] = 4;
	LedDev.Feature.arg[4] = 5;
	LedDev.Feature.arg[5] = 6;
	LedDev.Feature.arg[6] = 7;
	LedDev.Feature.datalen[0] = 2;
	LedDev.Feature.datalen[1] = 1;
	LedDev.Feature.datalen[2] = 2;
	LedDev.Feature.datalen[3] = 1;
	LedDev.Feature.datalen[4] = 2;
	LedDev.Feature.datalen[5] = 1;
	LedDev.Feature.datalen[6] = 1;
	LedDev.Feature.dataNum = 7;
	/* Input */
	LedDev.Input.data1.minimum = 0;							//Execute Flag
	LedDev.Input.data1.maximum = 1;
	LedDev.Input.data1.value = 0;
	LedDev.Input.arg[0] = 1;
	LedDev.Input.datalen[0] = 1;
	LedDev.Input.dataNum = 1;
	/* Output */
	LedDev.Output.data1.minimum = 0;						//Start flag
	LedDev.Output.data1.maximum = 1;
	LedDev.Output.data1.value = 1;
	LedDev.Output.data2.minimum = 0;						//Stop flag
	LedDev.Output.data2.maximum = 1;
	LedDev.Output.data2.value = 0;
	LedDev.Output.arg[0] = 1;
	LedDev.Output.arg[1] = 2;
	LedDev.Output.datalen[0] = 1;
	LedDev.Output.datalen[1] = 1;
	LedDev.Output.dataNum = 2;

}

// ----------------------------------------------------------------------------------------
//  Single PWM Output
// ----------------------------------------------------------------------------------------
//
void Led_Alerm(uint16_t freq, uint16_t duration, uint8_t G_level, uint8_t R_level, uint8_t B_level)
{
	LedSingleExecuteFlag=1;
	/* set output time(ms) */
	LedSingleTimeCnt = getTickCount() + duration;
	/* set PWMB channel 0 output configuration */
	PWM_ConfigOutputChannel(PWM0, 0, freq, G_level);
	PWM_ConfigOutputChannel(PWM0, 1, freq, R_level);
	PWM_ConfigOutputChannel(PWM0, 2, freq, B_level);
	/* Enable PWM Output path for PWMB channel 0 */
	PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
	PWM_EnableOutput(PWM0, PWM_CH_1_MASK);
	PWM_EnableOutput(PWM0, PWM_CH_2_MASK);
	// Start
	PWM_Start(PWM0, PWM_CH_0_MASK);
	PWM_Start(PWM0, PWM_CH_1_MASK);
	PWM_Start(PWM0, PWM_CH_2_MASK);
}

void Led_SingleCheck(uint16_t freq, uint16_t duration, uint8_t G_level, uint8_t R_level, uint8_t B_level)
{
	if(LedSingleExecuteFlag==0)
	{
		/* check if Led time out */
		if(getTickCount() > LedSingleTimeCnt)
		{
			/* Enable PWM Output path for PWMB channel 0 */
			Led_Alerm(freq, duration, G_level, R_level, B_level);
			//PWM_DisableOutput(PWM1, PWM_CH_0_MASK); // disable PWM1 Channel 0

			/* check if buzzer single counter reach limit */
			if(LedSingleCounter==(LedBlinkSize-1))
			{
				LedSingleCounter=0;
				LedSingleExecuteFlag=0;
			}
		}
	}
	else if (LedSingleExecuteFlag==1)
	{
		LedSingleExecuteFlag=0;
		LedSingleCounter++;
	}
}

// ----------------------------------------------------------------------------------------
//  check PWM state
// ----------------------------------------------------------------------------------------
void Led_Stop()
{
	PWM_DisableOutput(PWM0, PWM_CH_0_MASK); // disable PWM0 Channel 0
	PWM_DisableOutput(PWM0, PWM_CH_1_MASK); // disable PWM0 Channel 1
	PWM_DisableOutput(PWM0, PWM_CH_2_MASK); // disable PWM0 Channel 2
	LedStartFlag=0;
	LedSingleCounter = 0;
	LedSingleExecuteFlag=0;
	LedDev.Input.data1.value = 0;
}
void Led_Check()
{
	uint32_t currentTimeCnt;

	/* check if buzzer time out */
	currentTimeCnt = getTickCount()/1000;				//current time(s)

	if(currentTimeCnt > LedLatency)
	{
		/* Diable PWM Output path for PWMB channel 0 */
		Led_Stop();
	}
}

// ----------------------------------------------------------------------------------------
//  Led start Blink
// ----------------------------------------------------------------------------------------
void Led_Blink_Start()
{
	LedLatency = getTickCount()/1000+LedDev.Feature.data7.value;	//Led operation time(s)
	LedBlink = LedDev.Feature.data4.value;
	LedStartFlag=1;
	LedDev.Input.data1.value = 1;
	switch (LedBlink)
	{
	case 0:
	{
		LedBri_B = LedDev.Feature.data3.value%16;
		LedBri_B = LedBri_B*(LedDev.Feature.data2.value)/15;
		LedBri_B = 100-LedBri_B;
		LedBri_G = (LedDev.Feature.data3.value>>4)%16;
		LedBri_G = LedBri_G*(LedDev.Feature.data2.value)/15;
		LedBri_G = 100-LedBri_G;
		LedBri_R = (LedDev.Feature.data3.value>>8)%16;
		LedBri_R = LedBri_R*(LedDev.Feature.data2.value)/15;
		LedBri_R = 100-LedBri_R;
		LedToneStartTime = (uint16_t)(((float)LedDev.Feature.data5.value/(float)100)*(float)LedDev.Feature.data6.value);
		LedToneStopTime = LedDev.Feature.data5.value-(uint16_t)(((float)LedDev.Feature.data5.value/(float)100)*(float)LedDev.Feature.data6.value);;
		LedBlinkSize = 2;
		LedSingleCounter = 0;
		Led_Alerm(G7, LedToneStartTime, LedBri_G, LedBri_R, LedBri_B);
		break;
	}
	case 1:
	{
		LedBlinkSize = sizeof(MusicScale)/2;
		LedSingleCounter = 0;
		Led_Alerm(MusicScale[LedSingleCounter], MusicTime[LedSingleCounter], MusicLevel[LedSingleCounter], MusicLevel[LedSingleCounter], MusicLevel[LedSingleCounter]);
		break;
	}
	}
}
// ----------------------------------------------------------------------------------------
//  Led singing
// ----------------------------------------------------------------------------------------
void Led_Blink_Check()
{
	if(LedStartFlag==1)
	{
		PA2 = 0;
		switch (LedBlink)
		{
		case 0:
		{
			if(LedSingleCounter == 0)	Led_SingleCheck(G7, LedToneStartTime, LedBri_G, LedBri_R, LedBri_B);
			else if(LedSingleCounter == 1)	Led_SingleCheck(G7, LedToneStopTime, 100, 100, 100);				//close LED
			break;
		}
		case 1:
		{
			Led_SingleCheck(MusicScale[LedSingleCounter], MusicTime[LedSingleCounter], MusicLevel[LedSingleCounter], MusicLevel[LedSingleCounter], MusicLevel[LedSingleCounter]);
			break;
		}
		case 2:
		{
			break;
		}
		}

		/* Check Time Out */
		Led_Check();
	}
	else
	{
		PA2 = 1;
		CLK_PowerDown();
	}
}

void Led_Control(void)
{
	if(LedDev.Output.data2.value==0)
	{
		if(LedDev.Output.data1.value == 1)																			//Period, Duty
		{
			Led_Blink_Start();
			LedDev.Output.data1.value=0;
		}
	}
	else
	{
		Led_Stop();
	}
	Led_Blink_Check();
}


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

uint8_t BuzzerStartFlag;						//single buzzer cycle execute flag
uint8_t BuzzerSingleExecuteFlag;				//single buzzer cycle execute flag
uint8_t BuzzerSingleCounter;					//buzzer single buzzer cycle counter
uint8_t BuzzerSongSize;							//buzzer single buzzer cycle counter
uint16_t BuzzerVol;								//buzzer volume
uint16_t BuzzerTone;							//buzzer Tone
uint16_t BuzzerSong;							//buzzer song
uint16_t BuzzerToneStartTime;					//buzzer period
uint16_t BuzzerToneStopTime;					//buzzer duty
uint32_t BuzzerTimeCnt;							//buzzer total time counter
uint32_t BuzzerSingleTimeCnt;					//single buzzer cycle Time out flag
uint32_t BuzzerLatency;							//single buzzer cycle Time out flag

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
	//LED
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk);
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_GPIO;
	GPIO_SetMode(PA,BIT2,GPIO_MODE_OUTPUT);
	PA2 = 1;

	/* Initialize parameter */
	BuzzerSingleExecuteFlag=0;
	BuzzerStartFlag=0;

	BuzDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	BuzDev.DevDesc.RptDesc_leng = 74;						//Report descriptor
	BuzDev.DevDesc.InRptLeng = 3;							//Input report
	BuzDev.DevDesc.OutRptLeng = 4;							//Output report
	BuzDev.DevDesc.GetFeatLeng = 12;						//Get feature
	BuzDev.DevDesc.SetFeatLeng = 12;						//Set feature
	BuzDev.DevDesc.CID = 0;									//manufacturers ID
	BuzDev.DevDesc.DID = 0;									//Product ID
	BuzDev.DevDesc.PID = 0;									//Device firmware revision
	BuzDev.DevDesc.UID = 0;									//Device Class type
	BuzDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	BuzDev.Feature.data1.minimum = 0;						//Sleep period
	BuzDev.Feature.data1.maximum = 2048;
	BuzDev.Feature.data1.value = 100;
	BuzDev.Feature.data2.minimum = 0;						//Volume
	BuzDev.Feature.data2.maximum = 100;
	BuzDev.Feature.data2.value = 20;
	BuzDev.Feature.data3.minimum = 0;						//Tone
	BuzDev.Feature.data3.maximum = 5000;
	BuzDev.Feature.data3.value = G3;
	BuzDev.Feature.data4.minimum = 0;						//Song
	BuzDev.Feature.data4.maximum = 2;
	BuzDev.Feature.data4.value = 0;
	BuzDev.Feature.data5.minimum = 0;						//Period
	BuzDev.Feature.data5.maximum = 2048;
	BuzDev.Feature.data5.value = 200;
	BuzDev.Feature.data6.minimum = 0;						//Duty
	BuzDev.Feature.data6.maximum = 100;
	BuzDev.Feature.data6.value = 20;
	BuzDev.Feature.data7.minimum = 0;						//Latency
	BuzDev.Feature.data7.maximum = 60;
	BuzDev.Feature.data7.value = 1;
	BuzDev.Feature.arg[0] = 1;
	BuzDev.Feature.arg[1] = 2;
	BuzDev.Feature.arg[2] = 3;
	BuzDev.Feature.arg[3] = 4;
	BuzDev.Feature.arg[4] = 5;
	BuzDev.Feature.arg[5] = 6;
	BuzDev.Feature.arg[6] = 7;
	BuzDev.Feature.datalen[0] = 2;
	BuzDev.Feature.datalen[1] = 1;
	BuzDev.Feature.datalen[2] = 2;
	BuzDev.Feature.datalen[3] = 1;
	BuzDev.Feature.datalen[4] = 2;
	BuzDev.Feature.datalen[5] = 1;
	BuzDev.Feature.datalen[6] = 1;
	BuzDev.Feature.dataNum = 7;
	/* Input */
	BuzDev.Input.data1.minimum = 0;							//Execute Flag
	BuzDev.Input.data1.maximum = 1;
	BuzDev.Input.data1.value = 0;
	BuzDev.Input.arg[0] = 1;
	BuzDev.Input.datalen[0] = 1;
	BuzDev.Input.dataNum = 1;
	/* Output */
	BuzDev.Output.data1.minimum = 0;						//Start Flag
	BuzDev.Output.data1.maximum = 1;
	BuzDev.Output.data1.value = 1;
	BuzDev.Output.data2.minimum = 0;						//Stop Flag
	BuzDev.Output.data2.maximum = 1;
	BuzDev.Output.data2.value = 0;
	BuzDev.Output.arg[0] = 1;
	BuzDev.Output.arg[1] = 2;
	BuzDev.Output.datalen[0] = 1;
	BuzDev.Output.datalen[1] = 1;
	BuzDev.Output.dataNum = 2;

}

// ----------------------------------------------------------------------------------------
//  Single PWM Output
// ----------------------------------------------------------------------------------------
//
void Buzzer_Alerm(uint16_t freq, uint16_t duration, uint8_t level)
{
	BuzzerSingleExecuteFlag=1;
	/* set output time(ms) */
	BuzzerSingleTimeCnt = getTickCount() + duration;
	/* set PWMB channel 0 output configuration */
	PWM_ConfigOutputChannel(PWM0, 0, freq, level);
	/* Enable PWM Output path for PWMB channel 0 */
	PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
	// Start
	PWM_Start(PWM0, PWM_CH_0_MASK);
}

void Buzzer_SingleCheck(uint16_t freq, uint16_t duration, uint8_t level)
{
	if(BuzzerSingleExecuteFlag==0)
	{
		/* check if buzzer time out */
		if(getTickCount() > BuzzerSingleTimeCnt)
		{
			/* Enable PWM Output path for PWMB channel 0 */
			Buzzer_Alerm(freq, duration, level);
			//PWM_DisableOutput(PWM0, PWM_CH_0_MASK); // disable PWM0 Channel 3

			/* check if buzzer single counter reach limit */
			if(BuzzerSingleCounter==(BuzzerSongSize-1))
			{
				BuzzerSingleCounter=0;
				BuzzerSingleExecuteFlag=0;
			}
		}
	}
	else if (BuzzerSingleExecuteFlag==1)
	{
		BuzzerSingleExecuteFlag=0;
		BuzzerSingleCounter++;
	}
}

// ----------------------------------------------------------------------------------------
//  check PWM state
// ----------------------------------------------------------------------------------------
void Buzzer_Stop()
{
	PWM_DisableOutput(PWM0, PWM_CH_0_MASK); // disable PWM0 Channel 3
	BuzzerStartFlag=0;
	BuzzerSingleCounter = 0;
	BuzzerSingleExecuteFlag=0;
	BuzDev.Input.data1.value = 0;
}
void Buzzer_Check()
{
	uint32_t currentTimeCnt;

	/* check if buzzer time out */
	currentTimeCnt = getTickCount()/1000;	//current time(s)

	if(currentTimeCnt > BuzzerLatency)
	{
		/* Diable PWM Output path for PWMB channel 0 */
		Buzzer_Stop();
	}
}

// ----------------------------------------------------------------------------------------
//  Buzzer start sing
// ----------------------------------------------------------------------------------------
void Buzzer_Song_Start()
{
	BuzzerLatency = getTickCount()/1000+BuzDev.Feature.data7.value;		//buzzer operation time(s)
	BuzzerSong = BuzDev.Feature.data4.value;
	BuzzerStartFlag=1;
	BuzDev.Input.data1.value = 1;
	switch (BuzzerSong)
	{
	case 0:
	{
		BuzzerVol = BuzDev.Feature.data2.value;
		BuzzerToneStartTime = (uint16_t)(((float)BuzDev.Feature.data5.value/(float)100)*(float)BuzDev.Feature.data6.value);
		BuzzerToneStopTime = BuzDev.Feature.data5.value-(uint16_t)(((float)BuzDev.Feature.data5.value/(float)100)*(float)BuzDev.Feature.data6.value);
		BuzzerSongSize = 2;
		BuzzerSingleCounter = 0;
		BuzzerTone = BuzDev.Feature.data3.value;
		Buzzer_Alerm(BuzzerTone, BuzzerToneStartTime, BuzzerVol);
		break;
	}
	case 1:
	{
		BuzzerSongSize = sizeof(MusicScale)/2;
		BuzzerSingleCounter = 0;
		Buzzer_Alerm(MusicScale[BuzzerSingleCounter], MusicTime[BuzzerSingleCounter], MusicLevel[BuzzerSingleCounter]);
		break;
	}
	}
}
// ----------------------------------------------------------------------------------------
//  Buzzer singing
// ----------------------------------------------------------------------------------------
void Buzzer_Song_Check()
{
	if(BuzzerStartFlag==1)
	{
		PA2 = 0;
		switch (BuzzerSong)
		{
		case 0:
		{
			if(BuzzerSingleCounter == 0)
				Buzzer_SingleCheck(BuzzerTone, BuzzerToneStartTime, BuzzerVol);
			else if(BuzzerSingleCounter == 1)
				Buzzer_SingleCheck(BuzzerTone, BuzzerToneStopTime, 0);
			break;
		}
		case 1:
		{
			Buzzer_SingleCheck(MusicScale[BuzzerSingleCounter], MusicTime[BuzzerSingleCounter], MusicLevel[BuzzerSingleCounter]);
			break;
		}
		case 2:
		{
			break;
		}
		}

		/* Check Time Out */
		Buzzer_Check();
	}
	else
	{
		PA2 = 1;
		CLK_PowerDown();
	}
}

void Buzzer_Control(void)
{
	if(BuzDev.Output.data2.value==0)
	{
		if(BuzDev.Output.data1.value == 1)																			//Period, Duty
		{
			Buzzer_Song_Start();
			BuzDev.Output.data1.value = 0;																																		//Song
		}
	}
	else
	{
		Buzzer_Stop();
	}
	Buzzer_Song_Check();
}

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

#include "ir.h"
#include "timer1IRQ.h"

uint8_t TxIR_CODE[4];
uint8_t RxIR_CODE[4];
uint8_t IR_Tx_stage;
uint8_t	IR_TX_Bitn;
uint8_t	IR_TX_Byten;
uint8_t IR_bit_state;
uint8_t IR_byte_state;
uint8_t IR_Tx_LearnedDataByten;
uint8_t IR_TxExecuteFLAG;
uint8_t IRTx_StartFlag;
uint8_t IRTx_StopFlag;
//PWM0P0 IR receive DATA
volatile uint16_t IR_rawDATA[68];	//IR receive raw date
volatile uint32_t IR_DATA;			//IR receive data
volatile uint32_t IR_DATA_OUT;		//IR receive data output
uint8_t IR_RxExecute_Flag;			//IR start receive data flag
uint8_t IR_RxComplete_Flag;			//IR complete receiving data
uint8_t IR_RxReveice_Bit;			//IR complete receiving data
uint8_t IR_LearnedDataLeng[6];		//IR Rx learning mode data base
uint16_t IR_capture_count;			//IR receiver Data counter
uint16_t IR_caprure_timer_falling;	//PWM capture time between rising and falling
uint16_t IR_caprure_timer_rising;	//PWM capture time between rising and falling
uint32_t IR_init_counter;			//IR start receive time
//IR learning mode
uint8_t IR_LearnMode;				//IR learning mode ON/OFF
uint8_t IR_LearnedFlag;				//IR learned Flag
uint8_t IR_LearnedByte;				//IR learning current Byte
uint8_t IR_RxStartLearn_Flag;		//IR start receive data flag
uint16_t IR_JudgeBase_falling;		//IR Rx Judge 1/0 Base
uint16_t IR_JudgeBase_rising;		//IR Rx Judge 1/0 Base
uint16_t IR_FallingMaxTime;			//IR Rx Falling Max Time
uint16_t IR_FallingMinTime;			//IR Rx Falling Min Time
uint16_t IR_RisingMaxTime;			//IR Rx Rising Max Time
uint16_t IR_RisingMinTime;			//IR Rx Rising Min Time
uint16_t IR_FallingMaxFreq;			//IR Rx Falling Max Time
uint16_t IR_FallingMinFreq;			//IR Rx Falling Min Time
uint16_t IR_RisingMaxFreq;			//IR Rx Rising Max Time
uint16_t IR_RisingMinFreq;			//IR Rx Rising Min Time
uint32_t IR_LearnedData[12];		//IR Rx learning mode data base
uint32_t IRLearningModeTimer;		//IR Rx learning mode Timer for 5s timeout

// *************************************************************************
//  Timer3, PWM0 initialize setting
//	Set PC2 as output
//  Select PCLK as PWM module clock source
// *************************************************************************
void IR_Init(void)
{

	IRDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	IRDev.DevDesc.RptDesc_leng = 58;						//Report descriptor
	IRDev.DevDesc.InRptLeng = 3;							//Input report
	IRDev.DevDesc.OutRptLeng = 4;							//Output report
	IRDev.DevDesc.GetFeatLeng = 8;							//Get feature
	IRDev.DevDesc.SetFeatLeng = 8;							//Set feature
	IRDev.DevDesc.CID = 0;									//manufacturers ID
	IRDev.DevDesc.DID = 0;									//Product ID
	IRDev.DevDesc.PID = 0;									//Device firmware revision
	IRDev.DevDesc.UID = 0;									//Device Class type
	IRDev.DevDesc.UCID = 0;									//reserve
	/* Feature */
	IRDev.Feature.data1.minimum = 0;						//Sleep period
	IRDev.Feature.data1.maximum = 2048;
	IRDev.Feature.data1.value = 100;
	IRDev.Feature.data2.minimum = 0;						//learned data
	IRDev.Feature.data2.maximum = 6;
	IRDev.Feature.data2.value = 0;
	IRDev.Feature.data3.minimum = 0;						//using data type
	IRDev.Feature.data3.maximum = 1;
	IRDev.Feature.data3.value = 0;
	IRDev.Feature.data4.minimum = 0;						//number of original data
	IRDev.Feature.data4.maximum = 1;
	IRDev.Feature.data4.value = 1;
	IRDev.Feature.data5.minimum = 1;						//number of learned data
	IRDev.Feature.data5.maximum = 6;
	IRDev.Feature.data5.value = 0;
	IRDev.Feature.arg[0] = 1;
	IRDev.Feature.arg[1] = 2;
	IRDev.Feature.arg[2] = 3;
	IRDev.Feature.arg[3] = 4;
	IRDev.Feature.arg[4] = 5;
	IRDev.Feature.datalen[0] = 2;
	IRDev.Feature.datalen[1] = 1;
	IRDev.Feature.datalen[2] = 1;
	IRDev.Feature.datalen[3] = 1;
	IRDev.Feature.datalen[4] = 1;
	IRDev.Feature.dataNum = 5;
	/* Input */
	IRDev.Input.data1.minimum = 0;							//Execute Flag
	IRDev.Input.data1.maximum = 6;
	IRDev.Input.data1.value = 0;
	IRDev.Input.arg[0] = 1;
	IRDev.Input.datalen[0] = 1;
	IRDev.Input.dataNum = 1;
	/* Output */
	IRDev.Output.data1.minimum = 0;							//Start Flag
	IRDev.Output.data1.maximum = 1;
	IRDev.Output.data1.value = 1;
	IRDev.Output.data2.minimum = 0;							//Stop Flag
	IRDev.Output.data2.maximum = 1;
	IRDev.Output.data2.value = 0;
	IRDev.Output.arg[0] = 1;
	IRDev.Output.arg[1] = 2;
	IRDev.Output.datalen[0] = 1;
	IRDev.Output.datalen[1] = 1;
	IRDev.Output.dataNum = 2;

	/* PIN initialize*/
	/* IR Tx:PWM output(PD7,PWM0_CH5),quasi mode for no pull up R */
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC5MFP_Msk));
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC5MFP_PWM0_CH5;
	GPIO_SetMode(PC,BIT5,GPIO_MODE_QUASI);
	//IR Rx:PWM capture(PE3,PWM0_CH3)
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC3MFP_Msk));
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC3MFP_PWM0_CH3;
	//LED
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk);
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_GPIO;
	GPIO_SetMode(PA,BIT2,GPIO_MODE_OUTPUT);
	PA2 = 1;

	/* Unlock protected registers */
	SYS_UnlockReg();

	/* Enable PWM1 module clock */
	CLK_EnableModuleClock(PWM0_MODULE);

	/*---------------------------------------------------------------------------------------------------------*/
	/* PWM clock frequency configuration                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	/* Select HCLK clock source as PLL and and HCLK clock divider as 2 */
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(2));

	/* PWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
	/* case 1.PWM clock frequency is set equal to HCLK: select PWM module clock source as PCLK */
	CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, NULL);

	/* Reset PWM1 module */
	//SYS_ResetModule(PWM0_RST);

	/* Update System Core Clock */
	SystemCoreClockUpdate();

	/* Lock protected registers */
	SYS_LockReg();

	// ----------Set PWM0 CH2 as IR Tx---------------
	/* PWM0 channel 3 frequency is 38000Hz, duty 30% */
	PWM_ConfigOutputChannel(PWM0, 3, 38000, 30);

	// ----------Set PWM0 CH4 as IR Rx---------------
	/* set PWM1 channel 5 capture configuration, 5us*/
	PWM_ConfigCaptureChannel(PWM0, 5, 5000, 0);

	/* Enable capture falling edge interrupt for PWM0 channel 3 */
	PWM_EnableCaptureInt(PWM0, 5, PWM_CAPTURE_INT_FALLING_LATCH);

	/* Enable PWM0 NVIC interrupt */
	NVIC_EnableIRQ(PWM0P2_IRQn);

	/* Enable Timer for PWM0 channel 3 */
	PWM_Start(PWM0, PWM_CH_5_MASK);

	/*Enable Input Schmitt Trigger*/
	//PC->SMTEN |= GPIO_SMTEN_SMTEN4_Msk;

	/* Enable Capture Function for PWM0 channel 3 */
	PWM_EnableCapture(PWM0, PWM_CH_5_MASK);

	/* Enable falling capture reload */
	PWM0->CAPCTL |= PWM_CAPCTL_FCRLDEN5_Msk;

	// Initialize parameter
	IR_JudgeBase_rising = IRRX_DATA_BIT_JUDGE;
	IR_RxReveice_Bit = Receive_DATA_BIT;

	/* Wait until PWM0 channel 5 Timer start to count */
	while((PWM0->CNT[5]) == 0);

	/*---------------------------------------------------------------------------------------------------------*/
	/* Timer3 interrupt configuration                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	//Enable Timer0 clock and select Timer0 clock source
	CLK_EnableModuleClock(TMR3_MODULE);
	CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_PCLK1, 0);

	//Initial Timer3 to periodic mode with 111Hz(9ms)
	//TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, START_TRAN_FIRST);//TIMER_PERIODIC_MODE
	//Enable Timer3 interrupt
	NVIC_EnableIRQ(TMR3_IRQn);

	/*---------------------------------------------------------------------------------------------------------*/
	/* parameter initialize                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	IR_TxExecuteFLAG=0;
	TxIR_CODE[0]=0xA5;
	TxIR_CODE[1]=0x5A;
	TxIR_CODE[2]=0xF0;
	TxIR_CODE[3]=0x0F;

}

//*******************************
// Send  IR Format
//
// PC2 (PWM1_CH2)
//*******************************
void SendIR()
{
	if((IRTx_StartFlag == 1) && (IR_TxExecuteFLAG==0))
	{
		IRTx_StartFlag = 0;
		IR_TxExecuteFLAG = 1;
		IR_bit_state=1;
		//Initial Timer0 to periodic mode with 111Hz(9ms)
		TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, START_TRAN_FIRST);//TIMER_PERIODIC_MODE
		TIMER_EnableInt(TIMER3);
		TIMER_Start(TIMER3);
		// Enable PWM0 CH2 output
		/* Enable Counter for PWM0 CH2 */
		PWM_Start(PWM0, PWM_CH_3_MASK);
		PWM_EnableOutput(PWM0, PWM_CH_3_MASK);
	}
}

// =====================================================
// Compute IR Format
// CALL BY: ReceiveIR()
//
// Input:PC4 (PWM1_CH4)
// =====================================================
void UpdateIRDATA()
{
	int i;		//for data update
	int count;

	IR_DATA=0;

	for (i=0; i<32; i++)
	{
		count = i*2+1;
		if (IR_rawDATA[count] > IRRX_DATA_BIT_JUDGE )
			IR_DATA = ((IR_DATA << 1) | 1);
		else
			IR_DATA = IR_DATA << 1 ;
	}

	IR_DATA_OUT=IR_DATA;

}

void RxLearnData()
{
	int i;
	int count;

	/* dafult Time setting */
	IR_JudgeBase_falling = IR_rawDATA[8];
	IR_FallingMaxTime = IR_rawDATA[8];
	IR_FallingMinTime = IR_rawDATA[8];
	IR_JudgeBase_rising = IR_rawDATA[9];
	IR_RisingMaxTime = IR_rawDATA[9];
	IR_RisingMinTime = IR_rawDATA[9];

	for(i=9; i<(IR_RxReveice_Bit/2); i++)
	{
		count = i*2;
		/* Learn falling data format */
		if((IR_rawDATA[count] - 20) > IR_rawDATA[8])
		{
			IR_JudgeBase_falling = ((IR_rawDATA[count] + IR_rawDATA[8])/2);
			IR_FallingMaxTime = IR_rawDATA[count];
			IR_FallingMinTime = IR_rawDATA[8];
			break;
		}
		else if((IR_rawDATA[8]- 20) > IR_rawDATA[count])
		{
			IR_JudgeBase_falling = ((IR_rawDATA[count] + IR_rawDATA[8])/2);
			IR_FallingMaxTime = IR_rawDATA[8];
			IR_FallingMinTime = IR_rawDATA[count];
			break;
		}
	}
	for(i=9; i<(IR_RxReveice_Bit/2); i++)
	{
		count=i*2+1;
		/* Learn rising data format */
		if((IR_rawDATA[count] - 20) > IR_rawDATA[9])
		{
			IR_JudgeBase_rising = ((IR_rawDATA[count] + IR_rawDATA[9])/2);
			IR_RisingMaxTime = IR_rawDATA[count];
			IR_RisingMinTime = IR_rawDATA[9];
			break;
		}
		else if((IR_rawDATA[9] - 20) > IR_rawDATA[count])
		{
			IR_JudgeBase_rising = ((IR_rawDATA[count] + IR_rawDATA[9])/2);
			IR_RisingMaxTime = IR_rawDATA[9];
			IR_RisingMinTime = IR_rawDATA[count];
			break;
		}
	}
	for(i=0; i<32; i++)
	{
		if(IR_rawDATA[i]%2==1)
		{
			if (IR_rawDATA[i] > (IR_JudgeBase_rising+15) )
				IR_LearnedData[IR_LearnedByte] = ((IR_LearnedData[IR_LearnedByte] << 1) | 1);
			else
				IR_LearnedData[IR_LearnedByte] = IR_LearnedData[IR_LearnedByte] << 1 ;
		}
		if(IR_rawDATA[i]%2==0)
		{
			if (IR_rawDATA[i] > (IR_JudgeBase_falling+15) )
				IR_LearnedData[IR_LearnedByte] = ((IR_LearnedData[IR_LearnedByte] << 1) | 1);
			else
				IR_LearnedData[IR_LearnedByte] = IR_LearnedData[IR_LearnedByte] << 1 ;
		}
	}
	for(i=32; i<64; i++)
	{
		if(IR_rawDATA[i]%2==1)
		{
			if (IR_rawDATA[i] > (IR_JudgeBase_rising+15) )
				IR_LearnedData[IR_LearnedByte+1] = ((IR_LearnedData[IR_LearnedByte+1] << 1) | 1);
			else
				IR_LearnedData[IR_LearnedByte+1] = IR_LearnedData[IR_LearnedByte+1] << 1 ;
		}
		if(IR_rawDATA[i]%2==0)
		{
			if (IR_rawDATA[i] > (IR_JudgeBase_falling+15) )
				IR_LearnedData[IR_LearnedByte+1] = ((IR_LearnedData[IR_LearnedByte+1] << 1) | 1);
			else
				IR_LearnedData[IR_LearnedByte+1] = IR_LearnedData[IR_LearnedByte+1] << 1 ;
		}
	}
	IR_FallingMaxFreq = 200000/IR_FallingMaxTime;		//For Timer3IRQ to output IR Freq:1M/(5us*Time)
	IR_FallingMinFreq = 200000/IR_FallingMinTime;		//For Timer3IRQ to output IR Freq:1M/(5us*Time)
	IR_RisingMaxFreq = 200000/IR_RisingMaxTime;			//For Timer3IRQ to output IR Freq:1M/(5us*Time)
	IR_RisingMinFreq = 200000/IR_RisingMinTime;			//For Timer3IRQ to output IR Freq:1M/(5us*Time)
}

// =====================================================
// Receive IR Format
//
// INPUT: PC4 (PWM1_CH4)
// =====================================================
void ReceiveIR()
{
	if(IR_LearnMode==0)
	{
		// compute IR data
		if(IR_RxComplete_Flag == 1)	//32 bit data
		{
			PA2 = 0;
			IRDev.Input.data1.value = 1;
			//Update Data
			UpdateIRDATA();
			IR_RxExecute_Flag = 0;
			IR_RxComplete_Flag = 0;
		}

		/* --Time out-- */
		if((IR_RxExecute_Flag==1) && (getTickCount()-IR_init_counter)>80 ) //Time out:80ms
		{
			IR_RxExecute_Flag = 0;
			IR_capture_count = 0;
		}
	}
	else if(IR_LearnMode==1)
	{
		/* --One bit time out-- */
		if((IR_RxExecute_Flag==1) && (IR_capture_count>63) ) //Time out:120ms
		{
			PA2 = 0;
			IRDev.Input.data1.value = 1;
			//Update learned Data
			IR_RxReveice_Bit = IR_capture_count;		//update received data bits
			RxLearnData();
			IR_LearnedDataLeng[IR_LearnedByte/2] = IR_capture_count;
			IR_LearnedByte+=2;
			IRDev.Feature.data2.value = IR_LearnedByte/2;
			IRDev.Feature.data3.value = 1;
			IR_RxExecute_Flag = 0;
			IR_RxComplete_Flag = 0;
			IR_LearnedFlag=1;
			IR_LearnMode = 0;
		}
		else if((IR_RxExecute_Flag==1) && (getTickCount()-IR_init_counter)>120 ) //Time out:120ms
		{
			//Update learned Data
			IR_RxReveice_Bit = IR_capture_count;		//update received data bits
			RxLearnData();
			IR_LearnedDataLeng[IR_LearnedByte/2] = IR_capture_count;
			IR_LearnedByte+=2;
			IRDev.Feature.data2.value = IR_LearnedByte/2;
			IRDev.Feature.data3.value = 1;
			IR_RxExecute_Flag = 0;
			IR_RxComplete_Flag = 0;
			IR_LearnedFlag=1;
			IR_LearnMode = 0;
		}

		/* --Time out-- */
//		if((getTickCount()-IRLearningModeTimer)>10000 ) //Time out:10s
//		{
//			IR_LearnMode = 0;
//			IR_RxExecute_Flag = 0;
//			IR_capture_count = 0;
//			IR_RxComplete_Flag = 0;
//		}
	}
}

void IR_Control(void)
{
	/* Send IR */
	if(IRDev.Output.data1.value == 1)																			//Period, Duty
	{
		IRTx_StartFlag = 1;
		IR_LearnedFlag = IRDev.Feature.data3.value;
		IR_Tx_LearnedDataByten = IRDev.Feature.data5.value-1;
		IRDev.Output.data1.value=0;
	}
	SendIR();

	/* Reflash Input */
	if(IRDev.Input.data1.value == 1)
	{
		TMR1TimerCounter++;
	}
	if(TMR1TimerCounter > 2)
	{
		PA2 = 1;
		IRDev.Input.data1.value = 0;
		TMR1TimerCounter=0;
	}
}

void IR_Check(void)
{
	if(IRDev.Output.data2.value == 1)
	{
		IR_LearnMode = 1;
		IRDev.Output.data2.value=0;
	}
	ReceiveIR();
}

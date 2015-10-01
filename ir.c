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

uint8_t TxIR_CODE[4];
uint8_t RxIR_CODE[4];
uint8_t IR_Tx_stage;
uint8_t	IR_TX_Bitn;
uint8_t	IR_TX_Byten;
uint8_t IR_bit_state;
uint8_t IR_bits_counter;
uint8_t IR_bytes_counter;
uint8_t IR_TxExecuteFLAG;
uint8_t IRTx_StopFlag;
//PWM0P0 IR receive DATA
volatile uint16_t IR_rawDATA[34];	//IR receive raw date
volatile uint16_t IR_DATA[4];			//IR receive data
volatile uint16_t IR_DATA_OUT[4];	//IR receive data output
uint16_t IR_capture_count;			//IR receiver Data counter
uint8_t IR_RxExecute_Flag;				//IR start receive data flag
uint8_t IR_RxComplete_Flag;		//IR complete receiving data
uint32_t IR_init_counter;				//IR start receive time
uint32_t PWM_caprure_timer;			//PWM capture time between rising and falling

// *************************************************************************
//  Timer3, PWM0 initialize setting
//	Set PC2 as output
//  Select PCLK as PWM module clock source  
// *************************************************************************
void IR_Init(void)
{	
	/* PIN initialize*/
	/* IR Tx:PWM output(PD7,PWM0_CH5),quasi mode for no pull up R */
	SYS->GPD_MFPL = (SYS->GPD_MFPL & (~SYS_GPD_MFPL_PD7MFP_Msk));
	SYS->GPD_MFPL |= SYS_GPD_MFPL_PD7MFP_PWM0_CH5;
	GPIO_SetMode(PD,BIT7,GPIO_MODE_QUASI);
	//IR Rx:PWM capture(PE3,PWM0_CH3)
	SYS->GPE_MFPL = (SYS->GPE_MFPL & (~SYS_GPE_MFPL_PE3MFP_Msk));
	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE3MFP_PWM0_CH3;
	
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
	RxIR_CODE[0]=0xA5;
	RxIR_CODE[1]=0x5A;
	RxIR_CODE[2]=0x5A;
	RxIR_CODE[3]=0xA5;

}

//*******************************
// Send  IR Format
//
// PC2 (PWM1_CH2)
//*******************************
void SendIR()
{
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

// =====================================================
// Receive IR Format
//
// PC4 (PWM1_CH4)
// =====================================================
void UpdateIRDATA()
{
	int i;								//for data update
	
	IR_DATA[0]=0;
	IR_DATA[1]=0;
	IR_DATA[2]=0;
	IR_DATA[3]=0;
	
	for (i=0;i<8;i++)
	{
		if (IR_rawDATA[i] > IRRX_DATA_BIT_JUDGE )
			IR_DATA[0] = ((IR_DATA[0] << 1) | 1);
		else
			IR_DATA[0] = IR_DATA[0] << 1 ;
	}
	for (i=8;i<16;i++)
	{
		if (IR_rawDATA[i] > IRRX_DATA_BIT_JUDGE )
			IR_DATA[1] = ((IR_DATA[1] << 1) | 1);
		else
			IR_DATA[1] = IR_DATA[1] << 1 ;
	}		
	
	for (i=16;i<24;i++)
	{
		if (IR_rawDATA[i] > IRRX_DATA_BIT_JUDGE )
			IR_DATA[2] = ((IR_DATA[2] << 1)  | 1);
		else
			IR_DATA[2] = IR_DATA[2] << 1 ;
	}
	for (i=24;i<32;i++)
	{
		if (IR_rawDATA[i] > IRRX_DATA_BIT_JUDGE )
			IR_DATA[3] = ((IR_DATA[3] << 1)  | 1);
		else
			IR_DATA[3] = IR_DATA[3] << 1 ;
	}
	
	IR_DATA_OUT[0]=IR_DATA[0];
	IR_DATA_OUT[1]=IR_DATA[1];
	IR_DATA_OUT[2]=IR_DATA[2];
	IR_DATA_OUT[3]=IR_DATA[3];

}

void ReceiveIR()
{
		// compute IR data
	if(IR_RxComplete_Flag == 1)	//32 bit data
	{
		//Update Data
		UpdateIRDATA();
		//printf("IR DATA[0]= %X, IR DATA[1]= %X, IR DATA[2]=%X, IR DATA[3]=%X\n",IR_DATA_OUT[0],IR_DATA_OUT[1],IR_DATA_OUT[2],IR_DATA_OUT[3]);
		IR_RxComplete_Flag = 0;
	}
		
	/* --Time out-- */
	if((IR_RxExecute_Flag==1) && (getTickCount()-IR_init_counter)>80 ) //Time out:80ms	
	{
		IR_RxExecute_Flag = 0;
		IR_capture_count = 0;
		IR_RxComplete_Flag = 0;
	}
}

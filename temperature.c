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

#include "temperature.h"

volatile uint16_t DHT11_temperature;
volatile uint16_t DHT11_Humidity;
volatile uint16_t DHT11_DATA[42];
uint32_t DHT11_init_counter;							//Record start execution time 
uint16_t Capture_EnableFlag;							//whether capture start
uint16_t capture_count;
uint8_t DHT11ExecuteFlag;									//whether start temperature execution

// ---------------------------------------------------------------------------------------
//  Timer0 initialize setting
//	Set PD.11 as output
//  Select PCLK as PWM module clock source  
// ---------------------------------------------------------------------------------------
void Init_DHT11_PWM1(void)
{	
	/* Unlock protected registers */
	SYS_UnlockReg();
	
	/* Enable PWM1 module clock */
  CLK_EnableModuleClock(PWM1_MODULE);

  /*---------------------------------------------------------------------------------------------------------*/
  /* PWM clock frequency configuration                                                                       */
  /*---------------------------------------------------------------------------------------------------------*/
  /* Select HCLK clock source as PLL and and HCLK clock divider as 1 */
  CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(2));

  /* PWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
  /* case 1.PWM clock frequency is set equal to HCLK: select PWM module clock source as PCLK */
  CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, NULL);
	
  /* Reset PWM1 module */
  SYS_ResetModule(PWM1_RST);

  /* Update System Core Clock */
  SystemCoreClockUpdate();
	
	/* Lock protected registers */
  SYS_LockReg();
	
	/* PIN initialize */
	SYS->GPC_MFPH = (SYS->GPC_MFPH & (~SYS_GPC_MFPH_PC11MFP_Msk));
	SYS->GPC_MFPH |= SYS_GPC_MFPH_PC11MFP_GPIO;
	GPIO_SetMode(PC,BIT11,GPIO_MODE_OUTPUT);
	PC11=1;

}

void Set_PWM_Pin()
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
	/* Set PC multi-function pins for PWM1 Channel 0 and 2 */
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC6MFP_Msk));
  SYS->GPC_MFPL |= SYS_GPC_MFPL_PC6MFP_PWM1_CH0;
	SYS->GPC_MFPH = (SYS->GPC_MFPH & (~SYS_GPC_MFPH_PC11MFP_Msk));
  SYS->GPC_MFPH |= SYS_GPC_MFPH_PC11MFP_PWM1_CH2;
}

void Start_PWMCapture()
{
  /*--------------------------------------------------------------------------------------*/
  /* Set the PWM1 channel 2 for capture function                                          */
  /*--------------------------------------------------------------------------------------*/

  /* If input minimum frequency is 250Hz, user can calculate capture settings by follows.
     Capture clock source frequency = PLL = 72000000 in the sample code.
     (CNR+1) = Capture clock source frequency/prescaler/minimum input frequency
             = 72000000/5/250 = 57600
     (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
     CNR = 0xFFFF
     (Note: In capture mode, user should set CNR to 0xFFFF to increase capture frequency range.)

     Capture unit time = 1/Capture clock source frequency/prescaler
     69.4ns = 1/72000000/5
  */

  /* set PWM1 channel 2 capture configuration */
  PWM_ConfigCaptureChannel(PWM1, 2, 1000, 0);

  /* Enable capture falling edge interrupt for PWM1 channel 2 */
  PWM_EnableCaptureInt(PWM1, 2, PWM_CAPTURE_INT_FALLING_LATCH);

  /* Enable PWM1 NVIC interrupt */
  NVIC_EnableIRQ(PWM1P1_IRQn);

  /* Enable Timer for PWM1 channel 2 */
  PWM_Start(PWM1, PWM_CH_2_MASK);

  /* Enable Capture Function for PWM1 channel 2 */
  PWM_EnableCapture(PWM1, PWM_CH_2_MASK);

  /* Enable falling capture reload */
  PWM1->CAPCTL |= PWM_CAPCTL_FCRLDEN2_Msk;

  /* Wait until PWM1 channel 2 Timer start to count */
  while((PWM1->CNT[2]) == 0);
}

void Stop_PWMCapture()
{
  /*---------------------------------------------------------------------------------------------------------*/
  /* Stop PWM1 channel 2 (Recommended procedure method 1)                                                    */
  /* Set PWM Timer loaded value(Period) as 0. When PWM internal counter(CNT) reaches to 0, disable PWM Timer */
  /*---------------------------------------------------------------------------------------------------------*/

  /* Disable PWM1 NVIC */
  NVIC_DisableIRQ(PWM1P1_IRQn);

  /* Set loaded value as 0 for PWM1 channel 2 */
  PWM_Stop(PWM1, PWM_CH_2_MASK);
	
	/* Disable Capture Function and Capture Input path for  PWM1 channel 2*/
  PWM_DisableCapture(PWM1, PWM_CH_2_MASK);

  /* Wait until PWM1 channel 2 current counter reach to 0 */
  while((PWM1->CNT[2] & PWM_CNT_CNT_Msk) != 0);

  /* Disable Timer for PWM1 channel 2 */
  PWM_ForceStop(PWM1, PWM_CH_2_MASK);

  /* Clear Capture Interrupt flag for PWM1 channel 2 */
  PWM_ClearCaptureIntFlag(PWM1, 2, PWM_CAPTURE_INT_FALLING_LATCH);
}

void UpdateTempDATA()
{
	int i;								//for data update
	volatile uint16_t DHT11_Humidity_integ,DHT11_Humidity_decim;
	volatile uint16_t DHT11_temp_integ,DHT11_temp_decim;
	uint16_t DHT11_CRC;
	uint16_t CRC_check;
	
	DHT11_Humidity_decim = 0;
	DHT11_Humidity_integ = 0;
	DHT11_temp_decim     = 0;
	DHT11_temp_integ     = 0;
	DHT11_CRC			 = 0;
	for (i=1;i<9;i++)
	{
		if (DHT11_DATA[i] > 45 )
			DHT11_Humidity_integ = ((DHT11_Humidity_integ << 1) | 1);
		else
			DHT11_Humidity_integ = DHT11_Humidity_integ << 1 ;
	}
	for (i=9;i<17;i++)
	{
		if (DHT11_DATA[i] > 45 )
			DHT11_Humidity_decim = ((DHT11_Humidity_decim << 1) | 1);
		else
			DHT11_Humidity_decim = DHT11_Humidity_decim << 1 ;
	}		
	
	for (i=17;i<25;i++)
	{
		if (DHT11_DATA[i] > 45 )
			DHT11_temp_integ = ((DHT11_temp_integ << 1)  | 1);
		else
			DHT11_temp_integ = DHT11_temp_integ << 1 ;
	}
	for (i=25;i<33;i++)
	{
		if (DHT11_DATA[i] > 45 )
			DHT11_temp_decim = ((DHT11_temp_decim << 1)  | 1);
		else
			DHT11_temp_decim = DHT11_temp_decim << 1 ;
	}
	
	for (i=33;i<41;i++)
	{
		if (DHT11_DATA[i] > 45 )
			DHT11_CRC = ((DHT11_CRC << 1)  | 1);
		else
			DHT11_CRC = DHT11_CRC << 1 ;
	}
	//printf("Humidity.integer=%d,Humidity.decimal=%d\n", DHT11_Humidity_integ,DHT11_Humidity_decim);
	//printf("temp.integer=%d,temp.decimal=%d\n", DHT11_temp_integ,DHT11_temp_decim);
	//printf("CRC=%d\n",DHT11_CRC);
		
	/* --Check CRC-- */
	CRC_check = DHT11_Humidity_integ+DHT11_Humidity_decim+DHT11_temp_integ+DHT11_temp_decim;
	if(CRC_check==DHT11_CRC && DHT11_CRC>0)	{
		DHT11_Humidity = DHT11_Humidity_integ;
		DHT11_temperature = DHT11_temp_integ;
		//printf("DHT11_Humidity=%d,DHT11_temperature=%d,CRC=%d\n", DHT11_Humidity,DHT11_temperature,DHT11_CRC);
	}
}

// ---------------------------------------------------------------------------------------
//  Get tempture and Humidity data from DHT11 
//	Output: DHT11_temperature  
// ---------------------------------------------------------------------------------------
void Get_DHT11()
{
	if(capture_count == 0)
	{
		/* --Send start bit to DHT11-- */
		if(DHT11ExecuteFlag == 0)
		{
			DHT11_init_counter = getTickCount();
			SYS->GPC_MFPH = (SYS->GPC_MFPH & (~SYS_GPC_MFPH_PC11MFP_Msk));
			SYS->GPC_MFPH |= SYS_GPC_MFPH_PC11MFP_GPIO;
			GPIO_SetMode(PC,BIT11,GPIO_MODE_OUTPUT);
			PC11 = 0;
			DHT11ExecuteFlag++;
		}
	}
}

void DHT11GetDATA()
{
			/* --Change PC11 to PWM capture mode-- */
		if ((DHT11ExecuteFlag==1) && ((getTickCount()-DHT11_init_counter)>25))
		{
				DHT11ExecuteFlag++;
				Set_PWM_Pin();
				Start_PWMCapture();
				Capture_EnableFlag=1;
		}
	/* --confirm data currenctness -- */
	if(capture_count > 40 && Capture_EnableFlag==1)	
	{
		//Stop PWM1 capture, Change PC11 to GPIO
		Stop_PWMCapture();
		PC11 = 1;		
		SYS->GPC_MFPH = (SYS->GPC_MFPH & (~SYS_GPC_MFPH_PC11MFP_Msk));
		SYS->GPC_MFPH |= SYS_GPC_MFPH_PC11MFP_GPIO;
		GPIO_SetMode(PC,BIT11,GPIO_MODE_OUTPUT);
		//Update Data
		UpdateTempDATA();
		DHT11ExecuteFlag = 0;
		capture_count = 0;
		Capture_EnableFlag=0;
	}
	
	// 100ms TIME OUT
	if((DHT11ExecuteFlag > 0) && (getTickCount()-DHT11_init_counter)>50)	
	{
		Stop_PWMCapture();
		PC11 = 1;		
		SYS->GPC_MFPH = (SYS->GPC_MFPH & (~SYS_GPC_MFPH_PC11MFP_Msk));
		SYS->GPC_MFPH |= SYS_GPC_MFPH_PC11MFP_GPIO;
		GPIO_SetMode(PC,BIT11,GPIO_MODE_OUTPUT);
		//printf("capture_count=%d,DHT11ExecuteFlag=%d\n", capture_count, DHT11ExecuteFlag);
		DHT11ExecuteFlag = 0;
		capture_count = 0;
		Capture_EnableFlag=0;
	}
}

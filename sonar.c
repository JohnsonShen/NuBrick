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

// ***********************************************************
//                        SONAR MODULE
// Description: Detect distance
// Suggest:Fastest excute ratio: 100ms
// 
// OUTPUT PIN: 			PC2(PWM_CH2)
// INPUT PIN: 			PC4(PWM0_CH4),quasi mode
// Interrupt:				PWM0P2
// Main parameter:	Sonar_Distance:distance between sonar and object
// ***********************************************************

#include "sonar.h"
#include "timerctrl.h"

float SonarDistance_Scale;				//Distance between sonar and object
float Sonar_Distance;							//Distance between sonar and object		
float Sonar_Distance_OUT;					//Distance between sonar and object		
uint32_t Sonar_caprure_timer;			//Sonar PWM capture time between rising and falling
uint32_t SonarTimeOutFlag;				//Sonar time out flag	
uint8_t SonarExecuteFLAG;					//Sonar start to detect distance	
uint8_t SonarErrorCounter;						//Sonar start to detect distance	

void SonarInit()
{
	/* PIN initialize*/
	/* Sonar Tx:PWM output(PC2,PWM0_CH2) */
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC2MFP_Msk)) | SYS_GPC_MFPL_PC2MFP_PWM0_CH2;
	/* IR Rx:PWM capture(PC4,PWM0_CH4),quasi mode for no pull up R */
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC4MFP_Msk)) | SYS_GPC_MFPL_PC4MFP_PWM0_CH4;
	GPIO_SetMode(PC,BIT4,GPIO_MODE_QUASI);
	
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
	
	// ----------Set PWM0 CH2 as SONAR Tx---------------
	/* PWM0 channel 2 frequency is 38000Hz, duty 38% for output 10us */
  PWM_ConfigOutputChannel(PWM0, 2, 38000, 38);
	/* Set PWM0 CH2 at one shot mode */
	PWM0->CTL1 |= (PWM_CTL1_CNTMODE0_Msk << 2);
	/* start pwm0 ch2 output */
	PWM_Start(PWM0, PWM_CH_2_MASK);
	PWM_EnableOutput(PWM0, PWM_CH_2_MASK);
	
	// ----------Set PWM0 CH4 as SONAR Rx---------------
	/* set PWM1 channel 5 capture configuration, 5us*/
	PWM_ConfigCaptureChannel(PWM0, 4, 5000, 0);

	/* Enable capture falling edge interrupt for PWM0 channel 3 */
	PWM_EnableCaptureInt(PWM0, 4, PWM_CAPTURE_INT_FALLING_LATCH);

	/* Enable PWM0 NVIC interrupt */
	NVIC_EnableIRQ(PWM0P2_IRQn);

	/* Enable Timer for PWM0 channel 4 */
	PWM_Start(PWM0, PWM_CH_4_MASK);

	/*Enable Input Schmitt Trigger*/
	//PC->SMTEN |= GPIO_SMTEN_SMTEN4_Msk;

	/* Enable Capture Function for PWM0 channel 3 */
	PWM_EnableCapture(PWM0, PWM_CH_4_MASK);

	/* Enable falling capture reload */
	PWM0->CAPCTL |= PWM_CAPCTL_FCRLDEN4_Msk;
	
	/* Wait until PWM0 channel 5 Timer start to count */
  while((PWM0->CNT[4]) == 0);	
	
}

void SonarDetect()
{
	if(SonarExecuteFLAG==0)
	{
		SonarExecuteFLAG=1;
		SonarTimeOutFlag = getTickCount();
		PWM_SET_CMR(PWM0, 2, 800);								//720=72M(Hz)*10us
	}
}

void SonarTimeOut()
{
	/* Time out */
	if( (SonarExecuteFLAG == 1) && (getTickCount()-SonarTimeOutFlag>100))
	{
		SonarExecuteFLAG=0;
		SonarErrorCounter++;
	}
}

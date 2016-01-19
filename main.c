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
#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "Def.h"
#include "devCheck.h"
#include "nvti2c.h"
#include "timerctrl.h"
#include "i2cdev.h"
#include "flash.h"
#include "scheduler.h"
#include "battery.h"
#include "buzzer.h"
#include "led.h"
#include "Gas.h"
#include "ir.h"
#include "temperature.h"
#include "retarget.h"
#include "AHRSLib.h"
#include "Sensors.h"
#include "Report.h"
#include "Calibrate.h"
#include "ahrs.h"
#include "sonar.h"
#include "key.h"
#include "timer1IRQ.h"
#include "i2c_ms.h"
#include "tiddev.h"
#include "tidmst.h"

volatile uint32_t RecentTimeCounter;
volatile uint32_t MainTimeMSCounter;

#define SYSTEM_CLOCK 72000000

void setupSystemClock()
{
	SYS_UnlockReg();
  /*---------------------------------------------------------------------------------------------------------*/
  /* Init System Clock                                                                                       */
  /*---------------------------------------------------------------------------------------------------------*/
	/* Enable HIRC clock */
	CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

	/* Waiting for HIRC clock ready */
	CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

	/* Switch HCLK clock source to HIRC */
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

	/* Enable external XTAL 12MHz clock */
	//CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

	/* Waiting for external XTAL clock ready */
	//CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

	/* Set core clock as PLL_CLOCK from PLL and SysTick source to HCLK/2*/
	CLK_SetCoreClock(SYSTEM_CLOCK);
	CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLKSEL_HCLK_DIV2);

	/* Waiting for PLL clock ready */
	CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

	SYS_LockReg();
}

void setupUART()
{
	/* Enable peripheral clock */
	CLK_EnableModuleClock(UART0_MODULE);
	/* Peripheral clock source */
	CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HIRC, CLK_CLKDIV0_UART(1));
	/* Set PD multi-function pins for UART0 RXD, TXD */
	SYS->GPD_MFPL = SYS_GPD_MFPL_PD0MFP_UART0_RXD | SYS_GPD_MFPL_PD1MFP_UART0_TXD;
	/* Reset UART module */
	SYS_ResetModule(UART0_RST);
	/* Configure UART0 and set UART0 Baudrate */
	UART_Open(UART0, 115200);
	printf("Version:%d\n", VERSION_CODE);
}

void setup()
{
	setupSystemClock();
	setupUART();
	setup_system_tick(SYSTEM_TICK_FREQ);
	ID_Init();
	GetID();
	/* Init AHRS I2C */
	/* ----Initialize I2C slave mode--- */
	if(devNum == 0)
		I2C_MS_Master_Init();
	else
		I2C_MS_Slave_Init();
	//I2CWakeUpInit();
	TIDMstFirstInitFIN=0;
	TIDMstInitFIN = 0;
	TIDMstStage = 0;
	TIDMstInitDevState = 1;
	FlashInit();
	/*load TID feature from flash*/


	if(devNum == 0)
	{
		Battery_Init();
		GetFlashTID(&BatDev.Feature, devNum);
	}
	else if(devNum == 1)
	{
		Buzzer_Init();
		GetFlashTID(&BuzDev.Feature, devNum);
	}
	else if(devNum == 2)
	{
		Led_Init();
		GetFlashTID(&LedDev.Feature, devNum);
	}
	else if(devNum == 3)
	{
		I2C_Init();
		nvtAHRSInit();
		SensorsInit();
		AhrsParaInit();
		GetFlashTID(&AHRSDev.Feature, devNum);
	}
	else if(devNum == 4)
	{
		SonarInit();
		GetFlashTID(&SonDev.Feature, devNum);
	}
	else if(devNum == 5)
	{
		Init_DHT11_PWM0();
		GetFlashTID(&TempDev.Feature, devNum);
	}
	else if(devNum == 6)
	{
		Gas_Init();
		GetFlashTID(&GasDev.Feature, devNum);
	}
	else if(devNum == 7)
	{
		IR_Init();
		GetFlashTID(&IRDev.Feature, devNum);
	}
	else if(devNum == 8)
	{
		key_init();
		GetFlashTID(&KeyDev.Feature, devNum);
	}




  /* TID initialize */
	SlvDataInit();
	MstDataInit();
	
	//==================TEST PIN==================
//	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC5MFP_Msk));
//	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC5MFP_GPIO;
//	GPIO_SetMode(PC,BIT5,GPIO_MODE_OUTPUT);
//	PC5=1;
//	SYS->GPE_MFPH &= ~SYS_GPE_MFPH_PE9MFP_Msk;
//	GPIO_SetMode(PE,BIT9,GPIO_MODE_OUTPUT);
//	PE9=1;
	//==================TEST PIN==================
	// Sleep mode setting, put this at last setup line
	Timer1Init();
}

// Main Control loop
void loop()
{
	CommandProcess();
	TaskScheduler();
	PowerControl();
	// ******************************************************
	//                Get data for once erery 0.1s
	// ******************************************************
	if(TMR1INTCount != RecentTimeCounter)
	{
		MainTimeMSCounter = getTickCount();

		// ========================================================
		//									TID Master
		// ========================================================
		if(devNum == 0)
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
					TMR1TimerCounter++;
				}
			}
			GetBattery();
		}
		// ========================================================
		//									TID Slave
		// ========================================================
		// -------------AHRS----------------
		else if(devNum == 3)
		{
			SensorsRead(SENSOR_ACC|SENSOR_GYRO,1);
			nvtUpdateAHRS(SENSOR_ACC|SENSOR_GYRO);
			AhrsRead(AHRSDev.Feature.data2.value, AHRSDev.Feature.data3.value, AHRSDev.Output.data1.value);				//(Vibration Level, alerm time, clear flag)
		}
		// ------------SONAR----------------
		else if(devNum == 4)
		SonarDetect();
		// ---------TEMPERATURE-------------
		else if(devNum == 5)
		{
			if(TMR1TimerCounter == 3)
			{
				Get_DHT11();
				TMR1TimerCounter = 0;
			}
			else
			{
				TMR1TimerCounter++;
			}
		}			
		// -------------GAS-----------------
		else if(devNum == 6)
			GetGas();
		// --------------IR-----------------
		else if(devNum == 7)
		{
			if(IRDev.Output.data1.value == 1)																			//Period, Duty
			{
				IRTx_StartFlag = 1;
				IR_LearnedFlag = IRDev.Feature.data3.value;
				IR_Tx_LearnedDataByten = IRDev.Feature.data5.value;
				IRDev.Output.data1.value=0;
			}
			SendIR();
		}

		RecentTimeCounter = TMR1INTCount;
	}
	
	// =============TID Master handle received data===============
	// -----------TID Master handle received data------------
	if(devNum == 0)
	{
		if(TIDMstInitFIN==0)
		{
			TIDMst_GetDev();
		}
		else if(TIDMstInitFIN==1)
		{
			TIDMstDevTRx();
		}
	}
	// ====================  SLAVE PART  ======================
	// -----------TID slave handle received data--------------
	else
	{
		if(I2CMS_SlvRxFin==1)
		TID_SlvRxUpdate();
	}
	// -----------Slave part excute subfunction--------------
	/*  BUZZER  */
	if(devNum == 1)
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
	/*  LED  */
	else if(devNum == 2)
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
	/*   SONAR   */
	else if(devNum == 4)
	SonarTimeOutCheck();
	/*   Temprature TIMEOUT   */
	else if(devNum == 5)
	{
		DHT11GetDATA();				
	}
	/*   IR TIMEOUT   */
	else if(devNum == 7)
	{
		if(IRDev.Output.data2.value == 1)																			
		{
			IR_LearnMode = 1;
			IRDev.Output.data2.value=0;
		}
		ReceiveIR();
	}

	// --------------TID Slave Store data-----------------
	if(devNum != 0)
	SlvDataStore();
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
	setup();
	while(true) loop();
}

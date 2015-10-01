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
#include "nvti2c.h"
#include "timerctrl.h"
#include "i2cdev.h"
#include "flash.h"
#include "scheduler.h"
#include "battery.h"
#include "buzzer.h"
#include "ir.h"
#include "timer3IRQ.h"
#include "Gas.h"
#include "temperature.h"
#include "retarget.h"
#include "AHRSLib.h"
#include "Sensors.h"
#include "Report.h"
#include "Calibrate.h"
#include "ahrs.h"
#ifdef SONAR
#include "sonar.h"
#endif
#include "timer1IRQ.h"

extern int IsDebugFifoEmpty(void);

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
	I2C_Init();
	FlashInit();
#ifdef BUZZER
	Buzzer_Init();
#endif
#ifdef GAS
	Gas_Init();
#endif
#ifdef BATTERY
	Battery_Init();
#endif
#ifdef TEMPERATURE
	Init_DHT11_PWM1();	
#endif
#ifdef AHRS
	nvtAHRSInit();
	SensorsInit();
#endif
#ifdef IR
	IR_Init();
#endif
#ifdef SONAR
	SonarInit();
#endif
	//==================TEST PIN==================
	SYS->GPC_MFPL = (SYS->GPC_MFPL & (~SYS_GPC_MFPL_PC5MFP_Msk));
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC5MFP_GPIO;
	GPIO_SetMode(PC,BIT5,GPIO_MODE_OUTPUT);
	PC5=1;
	SYS->GPE_MFPH &= ~SYS_GPE_MFPH_PE9MFP_Msk;
	GPIO_SetMode(PE,BIT9,GPIO_MODE_OUTPUT);
	PE9=1;
	//==================TEST PIN==================
	// Sleep mode setting, put this at last setup line
	Timer1Init();

}
void CommandProcess()
{
	// Read incoming control messages
	if (Serial_available() >= 2)
	{
		char start=Serial_read();
		if (start == '@') {// Start of new control message
			int command = Serial_read(); // Commands
			if (command == 'h') {//Hook AHRS Stack Device
				// Read ID
				char id[2];
				id[0] = GetChar();
				id[1] = GetChar();
				// Reply with synch message
				printf("@HOOK");
				Serial_write(id, 2);
			}
			else if (command == 'c') {// A 'c'calibration command
				SensorCalibration();
			}
			else if (command == 'm') {// Set report 'm'ode
				char mode = GetChar();
			}
			else if (command == 'f') {// Set report 'f'ormat
				char format = GetChar();
				if (format == 'b') {// Report 'b'inary format
					report_format = REPORT_FORMAT_BINARY;
				}
				else if (format == 't') {// Report 't'ext format
					report_format = REPORT_FORMAT_TEXT;
				}
			}
			else if (command == 's') {// 's'tream output control
				char mode = GetChar();
				if (mode == 's') {// 's'tart stream
					stream_mode = STREAM_START;
				}
				else if (mode == 'p') {// 'p'ause stream
					stream_mode = STREAM_PAUSE;
				}
				else if (mode == 't') {// 't'oggle stream
					if(stream_mode==STREAM_START)
						stream_mode = STREAM_PAUSE;
					else
						stream_mode = STREAM_START;
				}
			}
		}
		else { 
			printf("Unknown command.\n");
		} // Skip character
	}
}
// Main Control loop
void loop()
{
	CommandProcess();
	TaskScheduler();
	PowerControl();
	// =======================================================
	//                Get data for once
	// =======================================================
	if(TMR1INTCount != RecentTimeCounter)
	{
		MainTimeMSCounter = getTickCount();
		// ------------SONAR----------------
		#ifdef SONAR
		SonarDetect();
		#endif
		// ---------TEMPERATURE-------------
		#ifdef TEMPERATURE
		Get_DHT11();				
		#endif	
		// -----------BUZZER----------------
		#ifdef BUZZER
		Buzzer_Alerm();
		Buzzer_Stop();
		#endif
		// -----------BATTERY---------------
		#ifdef BATTERY
		GetBattery();
		#endif
		// -------------GAS-----------------
		#ifdef GAS
		GetGas();
		#endif
		// -------------AHRS----------------
		#ifdef AHRS
		SensorsRead(SENSOR_ACC|SENSOR_GYRO,1);
		nvtUpdateAHRS(SENSOR_ACC|SENSOR_GYRO);
		AhrsRead(1,1);
		#endif
		// --------------IR-----------------
		#ifdef IR
		SendIR();
		#endif
		
		RecentTimeCounter = TMR1INTCount;
	}
	// =======================================================
	//          Get data until execution done
	// =======================================================
	// ------------SONAR TIMEOUT----------------
	#ifdef SONAR
	SonarTimeOut();
	#endif
	// --------------IR TIMEOUT-----------------
	#ifdef IR
	ReceiveIR();
	#endif
	// ---------Temprature TIMEOUT--------------
	#ifdef TEMPERATURE
	DHT11GetDATA();				
	#endif	
	
	// =======================================================
	//                Print data & Sleep
	// =======================================================
	while( (IsDebugFifoEmpty() == 0) );
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
	)
	{
#ifdef SONAR	
		printf("Sonar distance = %f\n",Sonar_Distance);
#endif
#ifdef IR
		printf("IR DATA[0]= %X, IR DATA[1]= %X, IR DATA[2]=%X, IR DATA[3]=%X\n",IR_DATA_OUT[0],IR_DATA_OUT[1],IR_DATA_OUT[2],IR_DATA_OUT[3]);
#endif
#ifdef BATTERY
		printf("BatteryData = %d, GasData = %d\n",BatteryData,GasData);
#endif
#ifdef TEMPERATURE
		printf("DHT11_Humidity=%d,DHT11_temperature=%d\n", DHT11_Humidity,DHT11_temperature);
#endif
		printf("Whole time:%d\n",(getTickCount()-MainTimeMSCounter));
		printf("I'm sleep.\n");
		DelayMsec(10);
		SYS_UnlockReg();
		CLK_PowerDown();
	}
}

/*-----------------------------------------------------------------------------------*/
/*  Fly Controller Main Function                                                     */
/*-----------------------------------------------------------------------------------*/
int32_t main (void)
{
	setup();
	while(true) loop();
}





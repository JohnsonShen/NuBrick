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
#include "lr.h"
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
#include "temp_HTU21D.h"

#define MAX_TID_DEV         (15)
#define EMBEDDED_TID_DEV    (9)

/***********************************************
 Customized session to add another sensor boards
 ***********************************************/

/*
 To include your board header file which declares PTID_Device_T and control functions.
 for example:

 #include "lightsensor.h"

 */
//#include "lightsensor.h"


PTID_Device_T pTidList[MAX_TID_DEV] =
{
	/* for embedded device boards */

	&BatDev,
	&BuzDev,
	&LedDev,
	&AHRSDev,
	&SonDev,
	&TempDev,
	&GasDev,
	&IRDev,
	&KeyDev,

	/* for custom device boards
	   fill your PTID_Device_T structure here to specified ID. */

	&ResDev9,		// ID 9
	&ResDev10,		// ID 10
	&ResDev11,		// ID 11
	&ResDev12,		// ID 12
	&ResDev13,		// ID 13
	&ResDev14		// ID 14
};


APFN_FUNC_T pfnDevFunc[MAX_TID_DEV] =
{
	/*
	   Functions for each device:

	    {Initial, Period, Pulling, Report}

	    Initial : Init and configure device
	    Period  : Process device sensor data or set sensor once per 0.1 seconds.
	    Pulling : Process device sensor data or set sensor frequently.
	    Report  : Report process data to master
	*/

	/* for embedded device boards */

	{Battery_Init,      MasterControl,  NULL,               report_battery},
	{Buzzer_Init,       NULL,           Buzzer_Control,     report_buzzer},
	{Led_Init,          NULL,           Led_Control,        report_led},
	{AHRS_Init,         AHRS_Control,   NULL,               report_ahrs},
	{SonarInit,         SonarDetect,    SonarTimeOutCheck,  report_sonar},
	{HTU21D_Init,   	WaitHTU21D,		GetHTU21DTemp,      report_temp},
	{Gas_Init,          GetGas,         NULL,               report_gas},
	{IR_Init,           IR_Control,     IR_Check,           report_ir},
	{key_init,          NULL,           NULL,               report_key},

	/* for custom device boards
	   fill your four board functions here to specified ID. */

	{Lr_Init, GetLr, NULL, report_resDev9},   // For device ID 9
	{NULL, NULL, NULL, report_resDev10},   // For device ID 10
	{NULL, NULL, NULL, report_resDev11},   // For device ID 11
	{NULL, NULL, NULL, report_resDev12},   // For device ID 12
	{NULL, NULL, NULL, report_resDev13},   // For device ID 13
	{NULL, NULL, NULL, report_resDev14}    // For device ID 14
};

/***********************************************
 End of customized session
 ***********************************************/

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
	int i;

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

	TIDMstFirstInitFIN = 0;
	TIDMstInitFIN = 0;
	TIDMstStage = 0;
	TIDMstInitDevState = 1;
	FlashInit();

	// Setup function pointers for default sensor boards.
	for(i=0; i<MAX_TID_DEV; i++)
	{
		if (pTidList[i])
			SetDeviceFunction(pTidList[i], &(pfnDevFunc[i]));
	}


	// Call board init function
	if (pTidList[devNum]->func.pfnSetup)
		pTidList[devNum]->func.pfnSetup();

	// Load TID feature from flash
	GetFlashTID(&(pTidList[devNum]->Feature), devNum);

	// Set Timer1 to (1000/value) in (Hz)
	Timer1Init(1000 / (pTidList[devNum]->Feature.data1.value));

	/* TID initialize */
	SlvDataInit();
	MstDataInit();
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

		if (pTidList[devNum]->func.pfnPeriod)
			pTidList[devNum]->func.pfnPeriod();

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

	if (pTidList[devNum]->func.pfnPulling)
		pTidList[devNum]->func.pfnPulling();

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

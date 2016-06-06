/*================================================================================*
 * O     O          __             ______  __   __  ____     __  ___          __  *
 *  \   /      /\  / /_      _    / /___/ / /  / / / __ \   / / /   \    /\  / /  *
 *   [+]      /  \/ / \\    //   / /____ / /  / /  \ \_    / / | | | |  /  \/ /   *
 *  /   \    / /\  /   \\__//   / /----// /__/ /  \ \__ \ / /  | | | | / /\  /    *
 * O     O  /_/  \/     \__/   /_/      \_ ___/    \___ //_/    \___/ /_/  \/     *
 *                                                                                *
 *                                                                                *
 * Nuvoton Sensor Fusion Application Firmware for Cortex M4 Series                *
 *                                                                                *
 * Written by by T.L. Shen for Nuvoton Technology.                                *
 * tlshen@nuvoton.com/tzulan611126@gmail.com                                      *
 *                                                                                *
 *================================================================================*
 */
#include <stdio.h>
#include <stdlib.h>
#include "retarget.h"
#include "AHRSLib.h"
#include "timerctrl.h"
#include "flash.h"
#include "Sensors.h"
#include "Report.h"
void CalibrationFail()
{
	const char fail = 'f';
	Serial_write((char*)&fail, 1);
}
void AccCalibration()
{
	const char direction = GetChar();
	const char calibration_done = 'd';
	char side = atoi(&direction);
	signed char status;

	if (direction == '0')
		nvtCalACCInit();
	do
	{
		DelayMsec(1);
		SensorsRead(SENSOR_ACC,1);
		status = nvtCalACCBufferFill(side);
	}
	while(status==STATUS_BUFFER_NOT_FILLED);

	if(status==STATUS_BUFFER_FILLED)
	{
		if (direction == '0')
			UpdateFlashCal(SENSOR_ACC, false);
		Serial_write((char*)&direction, 1);
	}
	else
	{
		Serial_write((char*)&calibration_done, 1);
		UpdateFlashCal(SENSOR_ACC, false);
	}
}
void GyroCalibration()
{
	const char axis_done = GetChar();
	const char axis = axis_done - 0x78;
	const char calibration_done = 'd';

	signed char status;

	nvtCalGyroInit(axis);

	do
	{
		SensorsRead(SENSOR_GYRO,1);
		DelayMsec(16);
		status=nvtGyroScaleCalibrate(axis);
	}
	while(status==STATUS_GYRO_CAL_RUNNING);

	if(status==STATUS_GYRO_AXIS_CAL_DONE)
		Serial_write((char*)&axis_done, 1);
	else
	{
		Serial_write((char*)&calibration_done, 1);
		UpdateFlashCal(SENSOR_GYRO, false);
	}
}
void MagCalibration()
{
	const char calibration_done = 'd';
	signed char status;
	uint8_t CalQFactor;
	int16_t RawMAG[3];

	nvtCalMAGInit();
	do
	{
		DelayMsec(320);
		SensorsRead(SENSOR_MAG,1);
		status = nvtCalMAGBufferFill();
		nvtGetSensorRawMAG(RawMAG);
		if (report_format == REPORT_FORMAT_BINARY)
		{
			Serial_write((char*)RawMAG, 6);
		}
		else if (report_format == REPORT_FORMAT_TEXT)
		{
			printf("@rM:%d,%d,%d\n",RawMAG[0],RawMAG[1],RawMAG[2]);
		}
	}
	while(status==STATUS_BUFFER_NOT_FILLED);

	if(status==STATUS_CAL_DONE)
	{
		CalQFactor = nvtGetMagCalQFactor();
		if (report_format == REPORT_FORMAT_BINARY)
		{
			Serial_write((char*)&calibration_done, 1);
			Serial_write((char*)&CalQFactor, 1);
		}
		else if (report_format == REPORT_FORMAT_TEXT)
		{
			printf("%c,%d\n",calibration_done,CalQFactor);
		}
	}
}
void SensorCalibration()
{
	char InitState = GetSensorInitState();
	char calibration_sensor = GetChar();
	TIMER_Enable(false);
	if((calibration_sensor=='a')&&(InitState&SENSOR_ACC))         // Do 'a'cc calibration
	{
		AccCalibration();
	}
	else if((calibration_sensor=='g')&&(InitState&SENSOR_GYRO))  // Do 'g'yro calibration
	{
		GyroCalibration();
	}
	else if((calibration_sensor=='m')&&(InitState&SENSOR_MAG))   // Do 'g'yro calibration
	{
		MagCalibration();
		UpdateFlashCal(SENSOR_MAG, false);
	}
	else                                                        	// Fail doing calibration
		CalibrationFail();
	
	TIMER_Enable(true);
}

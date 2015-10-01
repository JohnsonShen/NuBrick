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
#include "AHRSLib.h"
#include "Report.h"
#include "retarget.h"
#include "Sensors.h"

char report_mode = REPORT_AHRS_EULER;
char report_format = REPORT_FORMAT_TEXT;
char stream_mode = STREAM_PAUSE;

void report_ahrs_euler()
{
	float Euler[3];
	
	nvtGetEulerRPY(Euler);

	if (report_format == REPORT_FORMAT_BINARY) {
		float rpy[4];  
		rpy[0] = Euler[0];
		rpy[1] = Euler[1];
		rpy[2] = Euler[2];
		Serial_write((char*)rpy, 12);
	}
	else if (report_format == REPORT_FORMAT_TEXT)
		printf("@RPYA:%f,%f,%f\n",Euler[0],Euler[1],Euler[2]);
}
void report_ahrs_quaternion()
{
	float Quaternion[4];
	nvtGetQuaternion(Quaternion);
	
	if (report_format == REPORT_FORMAT_BINARY) {
		Serial_write((char*)&Quaternion[0], 4);
		Serial_write((char*)&Quaternion[1], 4);
		Serial_write((char*)&Quaternion[2], 4);
		Serial_write((char*)&Quaternion[3], 4);
	}
	else if (report_format == REPORT_FORMAT_TEXT) {
		printf("@Quaternion:%f,%f,%f,%f\n",Quaternion[0],Quaternion[1],Quaternion[2],Quaternion[3]);
	}
}
void report_sensor_raw()
{
	int16_t RawACC[3], RawGYRO[3], RawMAG[3];
	
	nvtGetSensorRawACC(RawACC);
	nvtGetSensorRawGYRO(RawGYRO);
	nvtGetSensorRawMAG(RawMAG);

	if (report_format == REPORT_FORMAT_BINARY) {
		Serial_write((char*)RawACC, 6);
		Serial_write((char*)RawGYRO, 6);
		Serial_write((char*)RawMAG, 6);
	}
	else if (report_format == REPORT_FORMAT_TEXT) {
		printf("@rA:%d,%d,%d  ",RawACC[0],RawACC[1],RawACC[2]);
		printf("@rG:%d,%d,%d  ",RawGYRO[0],RawGYRO[1],RawGYRO[2]);
		printf("@rM:%d,%d,%d  \n",RawMAG[0],RawMAG[1],RawMAG[2]);
	}
}

void report_sensor_calibrated()
{
	float CalACC[3], CalGYRO[3], CalMAG[3];
	nvtGetCalibratedACC(CalACC);
	nvtGetCalibratedGYRO(CalGYRO);
	nvtGetCalibratedMAG(CalMAG);

	if (report_format == REPORT_FORMAT_BINARY) {
		Serial_write((char*)CalACC, 12);
		Serial_write((char*)CalGYRO, 12);
		Serial_write((char*)CalMAG, 12);
	}
	else if (report_format == REPORT_FORMAT_TEXT) {
		printf("@cA:%f,%f,%f  ",CalACC[0],CalACC[1],CalACC[2]);
		printf("@cG:%f,%f,%f  ",CalGYRO[0],CalGYRO[1],CalGYRO[2]);
		printf("@cM:%f,%f,%f  \n",CalMAG[0],CalMAG[1],CalMAG[2]);
	}
}

void report_sensors()
{
	if(stream_mode==STREAM_PAUSE)
		return;
	
	if (report_mode == REPORT_AHRS_EULER) {
		report_ahrs_euler();
	}
	else if (report_mode == REPORT_AHRS_QUATERNION) {
		report_ahrs_quaternion();
	}
	else if (report_mode == REPORT_SENSORS_RAW) {
		report_sensor_raw();
	}
	else if (report_mode == REPORT_SENSORS_CALIBRATED) {
		report_sensor_calibrated();
	}
}

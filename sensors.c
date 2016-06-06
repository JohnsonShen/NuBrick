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
#include <math.h>
#include "MPU6050.h"
#include "sensors.h"
#include "flash.h"
#include "timerctrl.h"
#include "Report.h"
SensorInit_T SensorInitState = {false,false,false};
SensorInit_T SensorCalState  = {false,false,false};
CAL_FLASH_STATE_T CalFlashState =  {false,false,false,0xff};
Sensor_T Sensor;
float GyroScale[3];
float AccScale[3];
float GyroOffset[3];
float AccOffset[3];
float MagCalMatrix[10];
CAL_FLASH_STATE_T* GetFlashState()
{
	return &CalFlashState;
}
void SetFlashState(CAL_FLASH_STATE_T* State)
{
	CalFlashState.ACC_FLASH = State->ACC_FLASH;
	CalFlashState.GYRO_FLASH = State->GYRO_FLASH;
	CalFlashState.MAG_FLASH = State->MAG_FLASH;
	CalFlashState.MAG_QFACTOR = State->MAG_QFACTOR;
}
void temperatureRead(float *temperatureOut)
{
#if STACK_ACC
#ifdef MPU6050
	*temperatureOut = (MPU6050_getTemperature()-512)/340+34;
	*temperatureOut = *temperatureOut*25/20;
#endif
#endif
}

#if STACK_GYRO
#ifdef MPU6050
#endif
#endif
/* Sensors Init */
void SensorInitACC()
{
	float Cal[ACC_CAL_DATA_SIZE];
	bool FlashValid;

	if(!SensorInitState.ACC_Done)
	{
#ifdef MPU6050
		SensorInitState.ACC_Done = MPU6050_initialize();
		SensorInitState.GYRO_Done = SensorInitState.ACC_Done;
#endif
	}
	if(SensorInitState.ACC_Done)
	{
		printf("ACC connect      - [OK]\n");
		FlashValid = GetFlashCal(SENSOR_ACC, Cal);
		if(FlashValid)
		{
			CalFlashState.ACC_FLASH = true;
			AccOffset[0] = Cal[0];
			AccOffset[1] = Cal[1];
			AccOffset[2] = Cal[2];
			AccScale[0]  = Cal[3];
			AccScale[1]  = Cal[4];
			AccScale[2]  = Cal[5];
			printf("ACC calibration  - [FLASH]\n");

		}
		else
		{
			AccOffset[0] = ACC_OFFSET_X;//0;
			AccOffset[1] = ACC_OFFSET_Y;//0;
			AccOffset[2] = ACC_OFFSET_Z;//0;
			AccScale[0] = ACC_SCALE_X;//IMU_G_PER_LSB_CFG;
			AccScale[1] = ACC_SCALE_Y;//IMU_G_PER_LSB_CFG;
			AccScale[2] = ACC_SCALE_Z;//IMU_G_PER_LSB_CFG;
			printf("ACC calibration  - [DEF]\n");
		}
		//printf("Offset: %f  %f  %f\n", AccOffset[0], AccOffset[1], AccOffset[2]);
		//printf("Scale: %f  %f  %f\n", AccScale[0], AccScale[1], AccScale[2]);
		nvtSetAccScale(AccScale);
		nvtSetAccOffset(AccOffset);
		nvtSetAccG_PER_LSB(IMU_G_PER_LSB_CFG);
	}
	else
		printf("ACC connect      - [FAIL]\n");
}
void SensorInitGYRO()
{
	float Cal[GYRO_CAL_DATA_SIZE];
	bool FlashValid;
	if(!SensorInitState.GYRO_Done)
	{
#ifdef MPU6050
		SensorInitState.GYRO_Done = MPU6050_initialize();
		SensorInitState.ACC_Done = SensorInitState.GYRO_Done;
#endif
	}

	if(SensorInitState.GYRO_Done)
	{
		printf("GYRO connect     - [OK]\n");
		FlashValid = GetFlashCal(SENSOR_GYRO, Cal);

		if(FlashValid)
		{
			CalFlashState.GYRO_FLASH = true;
			GyroOffset[0] = Cal[0];
			GyroOffset[1] = Cal[1];
			GyroOffset[2] = Cal[2];
			GyroScale[0]  = Cal[3]*IMU_DEG_PER_LSB_CFG;
			GyroScale[1]  = Cal[4]*IMU_DEG_PER_LSB_CFG;
			GyroScale[2]  = Cal[5]*IMU_DEG_PER_LSB_CFG;
			printf("GYRO calibration - [FLASH]\n");

		}
		else
		{
			GyroOffset[0] = 0;
			GyroOffset[1] = 0;
			GyroOffset[2] = 0;
			GyroScale[0] = IMU_DEG_PER_LSB_CFG;
			GyroScale[1] = IMU_DEG_PER_LSB_CFG;
			GyroScale[2] = IMU_DEG_PER_LSB_CFG;
			printf("GYRO calibration - [DEF]\n");
		}
		//printf("Offset: %f  %f  %f\n", GyroOffset[0], GyroOffset[1], GyroOffset[2]);
		//printf("Scale: %f  %f  %f\n", GyroScale[0], GyroScale[1], GyroScale[2]);
		nvtSetGyroScale(GyroScale);
		nvtSetGyroOffset(GyroOffset);
		nvtSetGYRODegPLSB(IMU_DEG_PER_LSB_CFG);
	}
	else
		printf("GYRO connect     - [FAIL]\n");
}
void SensorInitMAG()
{
	float Cal[MAG_CAL_DATA_SIZE + QUALITY_FACTOR_SIZE],magCal[3];
	bool FlashValid;
	int i;

	if(!SensorInitState.MAG_Done)
	{
#ifdef HMC5883
		SensorInitState.MAG_Done = hmc5883lInit();
		hmc5883lSelfTest();
		hmc5883lGetRatioFactor(&magCal[0],&magCal[1],&magCal[2]);
#endif
#ifdef AK8975
		SensorInitState.MAG_Done = AK8975_initialize();
#endif
	}

	if(SensorInitState.MAG_Done)
	{
		if (report_format == REPORT_FORMAT_TEXT)
			printf("MAG connect      - [OK]\n");
		FlashValid = GetFlashCal(SENSOR_MAG, Cal);

		if(FlashValid)
		{
			CalFlashState.MAG_FLASH = true;
			for(i=0; i<MAG_CAL_DATA_SIZE; i++)
				MagCalMatrix[i] = Cal[i];
			CalFlashState.MAG_QFACTOR = Cal[i];
			if (report_format == REPORT_FORMAT_TEXT)
				printf("MAG calibration from - [FLASH], Q:%d\n",CalFlashState.MAG_QFACTOR);
		}
		else
		{
			for(i=0; i<MAG_CAL_DATA_SIZE; i++)
				MagCalMatrix[i] = 0;

			MagCalMatrix[3] = magCal[0];//MAG_GAUSS_PER_LSB;
			MagCalMatrix[4] = magCal[1];//MAG_GAUSS_PER_LSB;
			MagCalMatrix[5] = magCal[2];//MAG_GAUSS_PER_LSB;

			if (report_format == REPORT_FORMAT_TEXT)
				printf("MAG calibration from - [DEFAULT], Q:%d\n",CalFlashState.MAG_QFACTOR);
		}
		/*printf("M[0][1][2]: %f %f %f\n", MagCalMatrix[0], MagCalMatrix[1], MagCalMatrix[2]);
		printf("M[3][4][5]: %f %f %f\n", MagCalMatrix[3], MagCalMatrix[4], MagCalMatrix[5]);
		printf("M[6][7][8]: %f %f %f\n", MagCalMatrix[6], MagCalMatrix[7], MagCalMatrix[8]);
		printf("M[9]: %f\n", MagCalMatrix[9]);*/
		nvtSetMagCalMatrix(MagCalMatrix);
		nvtSetMagGaussPLSB(MAG_GAUSS_PER_LSB);
	}
	else
		printf("MAG connect      - [FAIL]\n");
}
void SensorsInit()
{
#if STACK_ACC
	SensorInitACC();
#endif
#if STACK_GYRO
	SensorInitGYRO();
#endif
#if STACK_MAG
	SensorInitMAG();
#endif
}

/* Sensors Read */
void SensorReadACC()
{
#if STACK_ACC
	int16_t rawACC[3];
#ifdef MPU6050
	MPU6050_getAcceleration(&rawACC[0],&rawACC[1], &rawACC[2]);
#endif
	ACC_ORIENTATION(rawACC[0],rawACC[1],rawACC[2]);
#endif
}

void SensorReadGYRO()
{
#if STACK_GYRO
	int16_t rawGYRO[3];
#ifdef MPU6050
	MPU6050_getRotation(&rawGYRO[0],&rawGYRO[1], &rawGYRO[2]);
#endif
	GYRO_ORIENTATION(rawGYRO[0],rawGYRO[1],rawGYRO[2]);
	//printf("Raw GYRO:%d %d %d\n",Sensor.rawGYRO[0], Sensor.rawGYRO[1], Sensor.rawGYRO[2]);
#endif
}

void SensorReadMAG()
{
	int16_t rawMAG[3];
#ifdef HMC5883
	hmc5883lGetHeading(&rawMAG[0],&rawMAG[1], &rawMAG[2]);
#endif
#ifdef AK8975
	AK8975_getHeading(&rawMAG[0],&rawMAG[1], &rawMAG[2]);
#endif
	MAG_ORIENTATION(rawMAG[0],rawMAG[1],rawMAG[2]);
	//printf("Raw Mag:%d %d %d\n",Sensor.rawMAG[0], Sensor.rawMAG[1], Sensor.rawMAG[2]);
}

void SensorsRead(char SensorType, char interval)
{
#if STACK_ACC
	if(SensorType&SENSOR_ACC&&SensorInitState.ACC_Done)
	{
		SensorReadACC();
		nvtInputSensorRawACC(&Sensor.rawACC[0]);
	}
#endif
#if STACK_MAG
	if(SensorType&SENSOR_MAG&&SensorInitState.MAG_Done)
	{
		if((GetFrameCount()%interval)==0)
		{
			SensorReadMAG();
			nvtInputSensorRawMAG(&Sensor.rawMAG[0]);
		}
	}
	else
	{
		Sensor.rawMAG[0] = 0;
		Sensor.rawMAG[1] = 0;
		Sensor.rawMAG[2] = 0;
		nvtInputSensorRawMAG(&Sensor.rawMAG[0]);
	}
#endif
#if STACK_GYRO
	if(SensorType&SENSOR_GYRO&&SensorInitState.GYRO_Done)
	{
		SensorReadGYRO();
		nvtInputSensorRawGYRO(&Sensor.rawGYRO[0]);
	}
#endif
}

void SensorsDynamicCalibrate(char SensorType)
{
#if STACK_ACC
	if(SensorType&SENSOR_ACC&&SensorInitState.ACC_Done)
	{
		/* TBD */
	}
#endif
#if STACK_GYRO
	if(SensorType&SENSOR_GYRO&&SensorInitState.GYRO_Done)
	{
		if(!SensorCalState.GYRO_Done)
		{
			if(nvtGyroCenterCalibrate()!=STATUS_GYRO_CAL_DONE)	{}
			//led_arm_state(LED_STATE_TOGGLE);
			else
			{
				float GyroMean[3];

				SensorCalState.GYRO_Done = true;
				//led_arm_state(LED_STATE_OFF);
				nvtGetGyroOffset(GyroMean);
			}
		}
	}
#endif
#if STACK_MAG
	if(SensorType&SENSOR_MAG&&SensorInitState.MAG_Done)
	{
		if(!SensorCalState.MAG_Done)
		{
			static float rpy[3],lastY,diff;
			nvtGetEulerRPY(rpy);
			diff = fabsf(rpy[2] - lastY);
			if((diff>0.01f)||(diff==0)) {}
			//led_mag_state(LED_STATE_TOGGLE);
			else
			{
				//led_arm_state(LED_STATE_OFF);
				SensorCalState.MAG_Done = true;
			}
			lastY = rpy[2];
		}
	}
#endif
}

char GetSensorInitState()
{
	char InitState = 0;

	InitState = (((SensorInitState.ACC_Done<<ACC))|((SensorInitState.GYRO_Done<<GYRO))|((SensorInitState.MAG_Done<<MAG)));
	return InitState;
}

char GetSensorCalState()
{
	char CalState = 0;
	CalState = (((SensorCalState.ACC_Done<<ACC))|((SensorCalState.GYRO_Done<<GYRO))|((SensorCalState.MAG_Done<<MAG)));
	return CalState;
}

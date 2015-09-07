/*============================================================================*
 * O     O          __                   ______  __                           *
 *  \   /      /\  / /_      _    __    / /___/ / /_     _                    *
 *   [+]      /  \/ / \\    //__ / /__ / /____ / / \\   //                    *
 *  /   \    / /\  /   \\__// --/ /---/ /----// /   \\_//                     *
 * O     O  /_/  \/     \__/    \_\/ /_/     /_/ ____/_/                      *
 *                                                                            *
 *                                                                            *
 * Multi-Rotor controller firmware for Nuvoton Cortex M4 series               *
 *                                                                            *
 * Written by by T.L. Shen for Nuvoton Technology.                            *
 * tlshen@nuvoton.com/tzulan611126@gmail.com                                  *
 *                                                                            *
 *============================================================================*
 */
#ifndef _SENSORS_H
#define _SENSORS_H
#include "Def.h"

#if STACK_BARO
typedef struct {
	float baroPressure;
	float baroTemperature;
	float baroPressureSum;
}BaroInfo_T;
#endif

typedef struct {
	int16_t rawACC[3];
	int16_t rawGYRO[3];
	int16_t rawMAG[3];
	int16_t rawBARO[2];
#if STACK_BARO
	BaroInfo_T BaroInfo;
#endif
	float Altitude;
}Sensor_T;

typedef struct {
	bool ACC_Done;
	bool GYRO_Done;
	bool MAG_Done;
	bool BARO_Done;
	uint8_t BARO_BRAND;
	int32_t BARO_BasePressure;
}SensorInit_T;
typedef struct {
	bool ACC_FLASH;
	bool GYRO_FLASH;
	bool MAG_FLASH;
	uint8_t MAG_QFACTOR;
}CAL_FLASH_STATE_T;
#if STACK_ACC
#define ACC_OFFSET_X          36
#define ACC_OFFSET_Y          -322
#define ACC_OFFSET_Z          467
#define ACC_SCALE_X           0.000061f
#define ACC_SCALE_Y           0.000061f
#define ACC_SCALE_Z           0.000061f
#ifdef MPU6050
#define IMU_GYRO_FS_CFG       MPU6050_GYRO_FS_2000
#define IMU_DEG_PER_LSB_CFG   MPU6050_DEG_PER_LSB_2000
#define IMU_ACCEL_FS_CFG      MPU6050_ACCEL_FS_8
#define IMU_G_PER_LSB_CFG     MPU6050_G_PER_LSB_8
#define IMU_1G_RAW           (int16_t)(1.0 / IMU_G_PER_LSB_CFG)
#endif
#else
#define IMU_GYRO_FS_CFG       0
#define IMU_DEG_PER_LSB_CFG   0
#define IMU_ACCEL_FS_CFG      0
#define IMU_G_PER_LSB_CFG     0
#define IMU_1G_RAW            1
#endif

#ifdef HMC5883
#define MAG_GAUSS_PER_LSB_CFG    HMC5883L_GAIN_660
#define MAG_GAUSS_PER_LSB        (1/660.0)
#define MAG_RATIO_FACTOR_CALIBRATION
#endif
#ifdef AK8975
#define MAG_GAUSS_PER_LSB        (1/660.0)
#define MAG_RATIO_FACTOR_CALIBRATION
#endif
#ifndef MAG_GAUSS_PER_LSB
#define MAG_GAUSS_PER_LSB        (1/660.0)
#endif
#define MAG_CAL0 18.594749f
#define MAG_CAL1 -67.389771f
#define MAG_CAL2 -12.142605f
#define MAG_CAL3 0.038077f
#define MAG_CAL4 0.036936f
#define MAG_CAL5 0.040339f
#define MAG_CAL6 0.000455f
#define MAG_CAL7 -0.000022f
#define MAG_CAL8 0.000917f
#define MAG_CAL9 11.632515f
#define MAG_QFactor 3
void SensorsRead(char,char);
void SensorsDynamicCalibrate(char SensorType);
void temperatureRead(float *temperatureOut);
void SensorsInit(void);
char GetSensorInitState(void);
char GetSensorCalState(void);
void SetFlashState(CAL_FLASH_STATE_T* State);
void SensorInitMAG(void);
CAL_FLASH_STATE_T* GetFlashState(void);
#if STACK_BARO
int32_t GetBaroBasePressure(void);
float GetBaroAltitude(void);
void SetBaroAltitude(float alt);
BaroInfo_T* GetBaroInfo(void);
#endif
#endif

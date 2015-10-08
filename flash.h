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
#ifndef _FLASH_CTRL_H
#define _FLASH_CTRL_H
#include "def.h"
//Calibration Section
#define CAL_BASE	16
#define FIELD_VALID_SIZE 1
#define QUALITY_FACTOR_SIZE 1
#define FIELD_VALID      0x77
#define FIELD_INVALID    0x88
#define CAL_BASE_GYRO	 CAL_BASE
#define CAL_BASE_ACC	(CAL_BASE_GYRO + GYRO_CAL_DATA_SIZE + FIELD_VALID_SIZE)
#define CAL_BASE_MAG	(CAL_BASE_ACC + ACC_CAL_DATA_SIZE + FIELD_VALID_SIZE)



void FlashInit(void);
void UpdateFlashCounter(void);
uint32_t GetFlashCounter(void);
void UpdateFlashCal(int8_t sensorType, bool erase);
bool GetFlashCal(int8_t sensorType, float* Cal);
void TestFloat(void);
float GetFloatCounter(void);
int32_t float2dw(float f);
float dw2float(int32_t dw);
void FlashControl(void);
#endif

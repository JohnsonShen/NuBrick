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
#ifdef M451
#define PAGE_SIZE 512 //2K Bytes
#else
#define PAGE_SIZE 128 //512 Bytes
#endif
#define CAL_BASE	16
#define FIELD_VALID_SIZE 1
#define QUALITY_FACTOR_SIZE 1
#define FIELD_VALID      0x77
#define FIELD_INVALID    0x88
#define CAL_BASE_GYRO	 CAL_BASE
#define CAL_BASE_ACC	(CAL_BASE_GYRO + GYRO_CAL_DATA_SIZE + FIELD_VALID_SIZE)
#define CAL_BASE_MAG	(CAL_BASE_ACC + ACC_CAL_DATA_SIZE + FIELD_VALID_SIZE)

//TID section
#define TID_SIZE 11
#define TID_BASE (CAL_BASE_MAG + MAG_CAL_DATA_SIZE + FIELD_VALID_SIZE + QUALITY_FACTOR_SIZE)

void FlashInit(void);
void UpdateFlashCounter(void);
void UpdateFlashCal(int8_t sensorType, bool erase);
void DATA_FLASH_Write(uint32_t ,uint32_t);
bool GetFlashCal(int8_t sensorType, float* Cal);
void TestFloat(void);
int16_t dw2i16(int32_t dw);
int32_t i162dw(int16_t i16);
int32_t float2dw(float f);
uint32_t DATA_FLASH_Read(uint32_t);
float dw2float(int32_t dw);

#ifdef M451
extern uint32_t data_buff[PAGE_SIZE];
#endif
extern uint32_t DATA_Flash_Start_ADD;

#endif

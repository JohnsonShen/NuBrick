/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg
Adapted to Cortex-M4 Fly Controller by Nuvoton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
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
#include <string.h>
#ifdef M451
#else
#include "Driver\DrvGPIO.h"
#include "Driver\DrvI2C.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvUART.h"
#endif
#include "I2CDev.h"
#include "MPU6050.h"
#include "timerctrl.h"

static uint8_t devAddr=MPU6050_DEFAULT_ADDRESS;
static uint8_t buffer[14];

uint8_t MPU6050_pgm_read_byte(const uint8_t* i)
{
	return 0;
}

uint8_t MPU6050_getAuxVDDIOLevel()
{
	I2C_readBit(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_PWR_MODE_BIT, buffer,0);
	return buffer[0];
}

void MPU6050_setAuxVDDIOLevel(uint8_t level)
{
	I2C_writeBit(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_PWR_MODE_BIT, level);
}

// SMPLRT_DIV register
uint8_t MPU6050_getRate()
{
	I2C_readByte(devAddr, MPU6050_RA_SMPLRT_DIV, buffer,0);
	return buffer[0];
}

void MPU6050_setRate(uint8_t rate)
{
	I2C_writeByte(devAddr, MPU6050_RA_SMPLRT_DIV, rate);
}

uint8_t MPU6050_getExternalFrameSync()
{
	I2C_readBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setExternalFrameSync(uint8_t sync)
{
	I2C_writeBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, sync);
}

uint8_t MPU6050_getDLPFMode()
{
	I2C_readBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setDLPFMode(uint8_t mode)
{
	I2C_writeBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

uint8_t MPU6050_getFullScaleGyroRange()
{
	I2C_readBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setFullScaleGyroRange(uint8_t range)
{
	I2C_writeBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

uint8_t MPU6050_getFullScaleAccelRange()
{
	I2C_readBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setFullScaleAccelRange(uint8_t range)
{
	I2C_writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

void MPU6050_setI2CBypassEnabled(bool enabled)
{
	I2C_writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

bool MPU6050_getIntI2CMasterStatus()
{
	I2C_readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_I2C_MST_INT_BIT, buffer,0);
	return buffer[0];
}

bool MPU6050_getIntDataReadyStatus()
{
	I2C_readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DATA_RDY_BIT, buffer,0);
	return buffer[0];
}

void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
	I2C_readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer,0);
	*ax = (((int16_t)buffer[0]) << 8) | buffer[1];
	*ay = (((int16_t)buffer[2]) << 8) | buffer[3];
	*az = (((int16_t)buffer[4]) << 8) | buffer[5];
	*gx = (((int16_t)buffer[8]) << 8) | buffer[9];
	*gy = (((int16_t)buffer[10]) << 8) | buffer[11];
	*gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}
void MPU6050_getMotion9(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz)
{
	MPU6050_getMotion6(ax, ay, az, gx, gy, gz);
}

void MPU6050_getAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
	I2C_readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 6, buffer,0);
	*x = (((int16_t)buffer[0]) << 8) | buffer[1];
	*y = (((int16_t)buffer[2]) << 8) | buffer[3];
	*z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t MPU6050_getAccelerationX()
{
	I2C_readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050_getAccelerationY()
{
	I2C_readBytes(devAddr, MPU6050_RA_ACCEL_YOUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050_getAccelerationZ()
{
	I2C_readBytes(devAddr, MPU6050_RA_ACCEL_ZOUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}


int16_t MPU6050_getTemperature()
{
	I2C_readBytes(devAddr, MPU6050_RA_TEMP_OUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}


void MPU6050_getRotation(int16_t* x, int16_t* y, int16_t* z)
{
	I2C_readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 6, buffer,0);
	*x = (((int16_t)buffer[0]) << 8) | buffer[1];
	*y = (((int16_t)buffer[2]) << 8) | buffer[3];
	*z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t MPU6050_getRotationX()
{
	I2C_readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050_getRotationY()
{
	I2C_readBytes(devAddr, MPU6050_RA_GYRO_YOUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050_getRotationZ()
{
	I2C_readBytes(devAddr, MPU6050_RA_GYRO_ZOUT_H, 2, buffer,0);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}

bool MPU6050_getYNegMotionDetected()
{
	I2C_readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_YNEG_BIT, buffer,0);
	return buffer[0];
}

void MPU6050_resetTemperaturePath()
{
	I2C_writeBit(devAddr, MPU6050_RA_SIGNAL_PATH_RESET, MPU6050_PATHRESET_TEMP_RESET_BIT, true);
}

uint8_t MPU6050_getFreefallDetectionCounterDecrement()
{
	I2C_readBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_FF_COUNT_BIT, MPU6050_DETECT_FF_COUNT_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setFreefallDetectionCounterDecrement(uint8_t decrement)
{
	I2C_writeBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_FF_COUNT_BIT, MPU6050_DETECT_FF_COUNT_LENGTH, decrement);
}

uint8_t MPU6050_getMotionDetectionCounterDecrement()
{
	I2C_readBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_MOT_COUNT_BIT, MPU6050_DETECT_MOT_COUNT_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setMotionDetectionCounterDecrement(uint8_t decrement)
{
	I2C_writeBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_MOT_COUNT_BIT, MPU6050_DETECT_MOT_COUNT_LENGTH, decrement);
}

void MPU6050_setI2CMasterModeEnabled(bool enabled)
{
	I2C_writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

void MPU6050_resetSensors()
{
	I2C_writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_SIG_COND_RESET_BIT, true);
}

void MPU6050_reset()
{
	I2C_writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, true);
}

bool MPU6050_getSleepEnabled()
{
	I2C_readBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, buffer,0);
	return buffer[0];
}
void MPU6050_setSleepEnabled(bool enabled)
{
	I2C_writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}


void MPU6050_setTempSensorEnabled(bool enabled)
{
	// 1 is actually disabled here
	I2C_writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT, !enabled);
}

uint8_t MPU6050_getClockSource()
{
	I2C_readBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, buffer,0);
	return buffer[0];
}

void MPU6050_setClockSource(uint8_t source)
{
	I2C_writeBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

uint8_t MPU6050_getDeviceID()
{
	I2C_readBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer,1);
	return buffer[0];
}

void MPU6050_setDeviceID(uint8_t id)
{
	I2C_writeBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, id);
}
void MPU6050_ADDR(uint8_t address)
{
	devAddr = address;
}

bool MPU6050_initialize()
{
	bool connect;
	MPU6050_ADDR(MPU6050_DEFAULT_ADDRESS);
	connect = MPU6050_testConnection();

	if(connect)
	{
		MPU6050_reset();
		DelayMsec(4);
		MPU6050_setSleepEnabled(false);
		MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO);
		MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
		MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
		MPU6050_setRate(1);
		MPU6050_setDLPFMode(MPU6050_DLPF_BW_188);
#if defined(MPU6050_I2C_AUX_MASTER)
		I2C_writeByte(devAddr, 0x37, 0x02);
		I2C_writeByte(devAddr, 0x6A, 0x20);             //USER_CTRL     -- DMP_EN=0 ; FIFO_EN=0 ; I2C_MST_EN=1 (I2C master mode) ; I2C_IF_DIS=0 ; FIFO_RESET=0 ; I2C_MST_RESET=0 ; SIG_COND_RESET=0
		I2C_writeByte(devAddr, 0x37, 0x00);             //INT_PIN_CFG   -- INT_LEVEL=0 ; INT_OPEN=0 ; LATCH_INT_EN=0 ; INT_RD_CLEAR=0 ; FSYNC_INT_LEVEL=0 ; FSYNC_INT_EN=0 ; I2C_BYPASS_EN=0 ; CLKOUT_EN=0
		I2C_writeByte(devAddr, 0x24, 0x0D);             //I2C_MST_CTRL  -- MULT_MST_EN=0 ; WAIT_FOR_ES=0 ; SLV_3_FIFO_EN=0 ; I2C_MST_P_NSR=0 ; I2C_MST_CLK=13 (I2C slave speed bus = 400kHz)
		I2C_writeByte(devAddr, 0x25, 0x80|MAG_ADDRESS); //I2C_SLV0_ADDR -- I2C_SLV4_RW=1 (read operation) ; I2C_SLV4_ADDR=MAG_ADDRESS
		I2C_writeByte(devAddr, 0x26, MAG_DATA_REGISTER);//I2C_SLV0_REG  -- 6 data bytes of MAG are stored in 6 registers. First register address is MAG_DATA_REGISTER
		I2C_writeByte(devAddr, 0x27, 0x86);             //I2C_SLV0_CTRL -- I2C_SLV0_EN=1 ; I2C_SLV0_BYTE_SW=0 ; I2C_SLV0_REG_DIS=0 ; I2C_SLV0_GRP=0 ; I2C_SLV0_LEN=3 (3x2 bytes)
#else
		MPU6050_setI2CBypassEnabled(true);
		MPU6050_setI2CMasterModeEnabled(false);
#endif
	}

	return connect;
}
bool MPU6050_testConnection()
{
	return ((MPU6050_getDeviceID() == 0x38) || (MPU6050_getDeviceID() == 0x3c));
}

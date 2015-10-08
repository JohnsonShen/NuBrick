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
#ifndef _DRVI2C_NVT_H
#define _DRVI2C_NVT_H
#include <stdint.h>
#define MAX_I2C_DATA_LEN 200
void NVT_I2C_Init(void);
#ifndef M451
void NVT_WriteByte(uint16_t address,uint8_t data);
void NVT_ReadByte(uint16_t address, uint8_t *data);
#endif
uint8_t NVT_WriteByteContinue_addr8(uint8_t address,uint8_t* data, uint8_t len);
uint8_t NVT_ReadByteContinue_addr8(uint8_t address,uint8_t* data, uint8_t len, uint16_t timeout);
void    NVT_SetDeviceAddress(uint8_t devAddr);

typedef void (*I2C_FUNC)(uint32_t u32Status);
extern volatile I2C_FUNC s_I2C0HandlerFn;
extern volatile I2C_FUNC s_I2C1HandlerFn;
extern volatile uint8_t MSErrorFlag;

#endif

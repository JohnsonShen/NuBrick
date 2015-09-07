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
#include <string.h>
#include "retarget.h"
#include "M451Series.h"
#include "flash.h"
#define PAGE_SIZE 512 //2K Bytes
uint32_t data_buff[PAGE_SIZE];
uint32_t DATA_Flash_Start_ADD;
uint32_t BoardVersion;
float tfloat;

//============================================================================
// DATA FLASH OPERATION
// u32addr : 0-1024 (For 4KBytes Data Flash)
// u32data : 0-0xFFFFFFFF (4Bytes)
//============================================================================
void DATA_FLASH_Write(uint32_t u32addr,uint32_t u32data)
{
	uint32_t i=0;
#ifdef M451
	SYS_UnlockReg();
	FMC_Open();
	
	for(i=0;i<PAGE_SIZE;i++)
		data_buff[i] = FMC_Read(DATA_Flash_Start_ADD+i*4+ u32addr/PAGE_SIZE*2048);
	
	FMC_Erase(DATA_Flash_Start_ADD+u32addr/PAGE_SIZE*2048);
	data_buff[u32addr%PAGE_SIZE]=u32data;
	
	for(i=0; i<PAGE_SIZE; i++) 
		FMC_Write(DATA_Flash_Start_ADD+i*4+ u32addr/PAGE_SIZE*2048, data_buff[i]);
	
	FMC_Close();
	SYS_LockReg();
#else
	uint32_t data_buff[PAGE_SIZE];
	__set_PRIMASK(1);//Avoid interrupt

	UNLOCKREG();
	DrvFMC_EnableISP();

	for(i=0;i<PAGE_SIZE;i++)
		DrvFMC_Read(DATA_Flash_Start_ADD+i*4+ u32addr/128*512, &data_buff[i]);

	DrvFMC_Erase(DATA_Flash_Start_ADD+u32addr/128*512);

	data_buff[u32addr%128]=u32data; 

	for(i=0; i<PAGE_SIZE; i++) 
		DrvFMC_Write(DATA_Flash_Start_ADD+i*4+ u32addr/128*512, data_buff[i]);

	DrvFMC_DisableISP();
	LOCKREG();
	__set_PRIMASK(0);
#endif

}
//============================================================================
// u32addr : 0-1024  
//============================================================================
uint32_t DATA_FLASH_Read(uint32_t u32add)
{
	uint32_t u32data;

	SYS_UnlockReg();
	FMC_Open();
	u32data = FMC_Read(u32add*4+DATA_Flash_Start_ADD);
	FMC_Close();
	SYS_LockReg();
	return u32data;
}

static int  SetDataFlashBase(uint32_t u32DFBA)
{
	uint32_t au32Config[2];

	/* Read current User Configuration */
	FMC_ReadConfig(au32Config, 1);

	/* Just return when Data Flash has been enabled */
	if(!(au32Config[0] & 0x1))
		return 0;

	/* Enable User Configuration Update */
	FMC_EnableConfigUpdate();

	/* Erase User Configuration */
	FMC_Erase(FMC_CONFIG_BASE);

	/* Write User Configuration to Enable Data Flash */
	au32Config[0] &= ~0x1;
	au32Config[1] = u32DFBA;

	if(FMC_WriteConfig(au32Config, 2))
		return -1;

	//printf("\nSet Data Flash base as 0x%x.\n", FMC_ReadDataFlashBaseAddr());

	/* Perform chip reset to make new User Config take effect */
	SYS->IPRST0 |= SYS_IPRST0_CHIPRST_Msk;

	return 0;
}

void FlashInit()
{
	SYS_UnlockReg();
	FMC_Open();
	SetDataFlashBase(DATA_FLASH_TEST_BASE);
	DATA_Flash_Start_ADD = FMC_ReadDataFlashBaseAddr();
	FMC_Close();
	SYS_LockReg();

	printf("Flash initilize - [OK]\n");
}
void UpdateBoardVersion(bool erase)
{
	/* Enable ISP function */
	SYS_UnlockReg();
	FMC_Open();

	if(erase) {
		DATA_FLASH_Write(BOARD_CODE_BASE,0xffffffff);
	}
	else {
		BoardVersion = DATA_FLASH_Read(BOARD_CODE_BASE);

		if(BoardVersion==0xFFFFFFFF) {
			DATA_FLASH_Write(BOARD_CODE_BASE,BOARD_CODE);
			printf("Update Board Code:%d\n", BOARD_CODE);
		}
		else
			printf("Board Code:%d\n", BoardVersion);
	}
	FMC_Close();
	SYS_LockReg();
}
int32_t float2dw(float f)
{
	int32_t* pdw;

	pdw = (int32_t*)&f;
	return *pdw;
}
float dw2float(int32_t dw)
{
	float* pf;

	pf = (float*)&dw;
	return *pf;
}
int32_t i162dw(int16_t i16)
{
	int32_t* pdw;
	int32_t i32;
	i32 = i16;

	pdw = (int32_t*)&i32;
	return *pdw;
}
int16_t dw2i16(int32_t dw)
{
	int16_t* pi16;
	int16_t i16;
	
	i16 = dw;
	pi16 = (int16_t*)&i16;
	return *pi16;
}
uint32_t GetBoardVersion(void)
{
	return BoardVersion;
}


void FlashControl()
{
	char Action = GetChar();
}

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
 * HCHSIEH@nuvoton.com                                         *
 *=============================================================*
 */

#include "tid.h"

TID_Device BatDev;
/*
 *	Battery:
 *
 *			Feature:	Sleep period, 						2 bits
 *						Battery alerm value,				2 bit
 *
 *			Input:		Battery value,				 		2 bits
 *						Over Flag,							1 bit
 *
 */
TID_Device BuzDev;
/*
 *	Buzzer:
 *
 *			Feature:	Sleep period, 						2 bits
 *						Volume,								1 bit
 *						Tone,								2 bit
 *						Song,								1 bit
 *						Period,								2 bit
 *						Duty,								1 bit
 *						Latency,							1 bit
 *
 *			Input:		Execute Flag,				 		2 bits
 *
 *			Output:		Start flag,							1 bit
 *						Stop flag,							1 bit
 *
 */
TID_Device LedDev;
/*
 *	Led:
 *
 *			Feature:	Sleep period, 						2 bits
 *						Brightness,								1 bit
 *						color,								2 bit
 *						Blink,								1 bit
 *						Period,								2 bit
 *						Duty,								1 bit
 *						Latency,							1 bit
 *
 *			Input:		Execute Flag,				 		2 bits
 *
 *			Output:		Start flag,							1 bit
 *						Stop flag,							1 bit
 *
 */
TID_Device AHRSDev;
/*
 *	AHRS:
 *
 *			Feature:	Sleep period, 						2 bits
 *						Vibration Level,					1 bit
 *
 *			Input:		Vibration Value, 					2 bits
 *						Over Flag,							1 bit
 *
 */
TID_Device SonDev;
/*
 *	Sonar:
 *
 *			Feature:	Sleep period, 						2 bits
 *						alerm distance,						2 bit
 *
 *			Input:		sensored disance, 					2 bits
 *						Over Flag,							1 bit
 *
 */
TID_Device TempDev;
/*
 *	Temperature:
 *
 *			Feature:	Sleep period, 						2 bits
 *						Temp. alerm value,					2 bit
 *						Humidity alerm value,				2 bit
 *
 *			Input:		Temperature value, 					2 bits
 *						Humidity value, 					2 bits
 *						Temp. Over Flag,					1 bit
 *						Humity Over Flag,					1 bit
 *
 */
TID_Device GasDev;
/*
 *	Gas:
 *
 *			Feature:	Sleep period, 						2 bits
 *						Gas alerm value,					2 bit
 *
 *			Input:		Gas value,				 			2 bits
 *						Over Flag,							1 bit
 *
 */
TID_Device IRDev;
/*
 *	IR:
 *
 *			Feature:	Sleep period, 						2 bits
 *						learned data,						1 bit
 *						using data type,					1 bit
 *						number of original data,			1 bit
 *						number of learned data,				1 bit
 *
 *			Input:		Receive data,					 	1 bits
 *
 *			Output:		Senr IR flag,						1 bit
 *						learn IR flag,						1 bit
 *
 */
TID_Device KeyDev;
/*
 *	Key:
 *
 *			Feature:	Sleep period, 						2 bits
 *
 *			Input:		Key state,				 			1 bits
 *
 */
TID_Device ResDev9;
TID_Device ResDev10;
TID_Device ResDev11;
TID_Device ResDev12;
TID_Device ResDev13;
TID_Device ResDev14;

uint16_t TID_loadFromFlash;							//Load from Flash flag
uint8_t TID_HandSingDataFlag;						//Handle TID single Data is finished
uint8_t TID_RpDescType;								//Handle TID single Data is finished
uint16_t TID_RpDescFeatBase;						//Handle TID single Data is finished
uint16_t TID_RpDescInBase;							//Handle TID single Data is finished
uint16_t TID_RpDescOutBase;							//Handle TID single Data is finished
uint16_t* TID_RpFeatReg;							//Handle TID single Data is finished
uint16_t* TID_RpInReg;								//Handle TID single Data is finished
uint16_t* TID_RpOutReg;								//Handle TID single Data is finished


void SetDeviceFunction(TID_Device *tid, APFN_FUNC_T *apfn_func)
{
	tid->func.pfnSetup = (*apfn_func)[0];
	tid->func.pfnPeriod = (*apfn_func)[1];
	tid->func.pfnPulling = (*apfn_func)[2];
	tid->func.pfnReport = (*apfn_func)[3];
}

// *********************************************************************************
//							TID get/store data from flash
// *********************************************************************************
void GetFlashTID(TID_FEATURE* devData, int8_t devNum)
{
	uint8_t TIDBase;
	int16_t Valid;

	TID_loadFromFlash = 0;
	TIDBase=TID_BASE;
	SYS_UnlockReg();
	Valid = dw2i16(DATA_FLASH_Read(TIDBase++));
	if(Valid == (devNum + 256))
	{
		TID_loadFromFlash = 1;
		devData->data1.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data2.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data3.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data4.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data5.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data6.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data7.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data8.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data9.value = dw2i16(DATA_FLASH_Read(TIDBase++));
		devData->data10.value = dw2i16(DATA_FLASH_Read(TIDBase++));
	}
	SYS_LockReg();
}

void UpdateFlashTID(TID_FEATURE devData, int8_t devNum, bool reset)
{
	uint32_t TIDBase, i=0;
	int32_t TID_FIELD[TID_SIZE];

	TIDBase=TID_BASE;
	TID_FIELD[0] = devNum + 256;
	TID_FIELD[1] = devData.data1.value;
	TID_FIELD[2] = devData.data2.value;
	TID_FIELD[3] = devData.data3.value;
	TID_FIELD[4] = devData.data4.value;
	TID_FIELD[5] = devData.data5.value;
	TID_FIELD[6] = devData.data6.value;
	TID_FIELD[7] = devData.data7.value;
	TID_FIELD[8] = devData.data8.value;
	TID_FIELD[9] = devData.data9.value;
	TID_FIELD[10] = devData.data10.value;

	if(reset)
	{
		DATA_FLASH_Write(TIDBase++, TID_FIELD[0]);
		return;
	}
	else
	{
		SYS_UnlockReg();
		//(uint32_t u32addr,uint32_t u32data)
		/* Read FMC */
		for(i=0; i < PAGE_SIZE; i++)
			data_buff[i] = FMC_Read(DATA_Flash_Start_ADD+i*4+ TIDBase/PAGE_SIZE*2048);
		/* Erase FMC */
		for(i=0; i<TID_SIZE; i++)
		{
			FMC_Erase(DATA_Flash_Start_ADD+i*4+TIDBase/PAGE_SIZE*2048);
			data_buff[(TIDBase+i)%PAGE_SIZE]=TID_FIELD[i];
		}
		/* Write FMC */
		for(i=0; i<PAGE_SIZE; i++)
			FMC_Write(DATA_Flash_Start_ADD+i*4+ TIDBase/PAGE_SIZE*2048, data_buff[i]);
		//for(i=0;i<TID_SIZE;i++) {
		//DATA_FLASH_Write(TIDBase++,TID_FIELD[i]);
		//printf("PID_FIELD[%d]:%f\n", i, PID_FIELD[i]);
		//}
		SYS_LockReg();
	}
}

// *********************************************************************************
//												Subfunction
// *********************************************************************************
void StoreTIDTxData(uint8_t* storeData, uint16_t RxData, uint8_t DataLen, uint8_t* position)
{
	switch(DataLen)
	{
	case 1:
	{
		*storeData = (uint8_t)RxData;
		(*position)+=1;
		break;
	}
	case 2:
	{
		*storeData = (uint8_t)RxData;
		*(storeData+1) = (uint8_t)(RxData>>8);
		(*position)+=2;
		break;
	}
	}
}

void HandTIDRxData(uint16_t* storeData, uint8_t RxData, uint8_t RxData_1, uint8_t DataLen, uint8_t* position)
{
	switch(DataLen)
	{
	case 1:
	{
		*storeData = RxData;
		(*position)+=1;
		break;
	}
	case 2:
	{
		*storeData = (RxData_1<<8) + RxData;
		(*position)+=2;
		break;
	}
	}
}

void HandTIDRxDesRpt_feature(TID_Device* devPointer, TID_DATA* devDataPointer, int8_t num, int8_t* DataCnt, int8_t* i, int8_t* j)
{
	int8_t Datalen = 0;							//how many number in a single data
	int8_t DataMinlen = 0;						//how many number in a single data
	int8_t DataMaxlen = 0;						//how many number in a single data

	Datalen = I2CMS_MstRxData[*i]%4;
	devPointer->Feature.arg[*DataCnt] = num;
	devPointer->Feature.datalen[*DataCnt] = I2CMS_MstRxData[(*i)+1];
	(*i)=(*i)+Datalen+1;							//next data
	/* Handle Minimun */
	DataMinlen = I2CMS_MstRxData[(*i)]%4;
	devDataPointer->minimum = 0;
	for((*j)=0; (*j)<DataMinlen; (*j)++)
		devDataPointer->minimum |= (I2CMS_MstRxData[(*i)+(*j)+1]<<(8*(*j)));
	(*i)=(*i)+DataMinlen+1;							//next data
	/* Handle Maximun */
	DataMaxlen = I2CMS_MstRxData[(*i)]%4;
	devDataPointer->maximum = 0;
	for((*j)=0; (*j)<DataMaxlen; (*j)++)
		devDataPointer->maximum |= (I2CMS_MstRxData[(*i)+(*j)+1]<<(8*(*j)));
	(*i)=(*i)+DataMaxlen;							//last byte in this data
	/* update state */
	(*DataCnt)++;
}

void HandTIDRxDesRpt_input(TID_Device* devPointer, TID_DATA* devDataPointer, int8_t num, int8_t* DataCnt, int8_t* i, int8_t* j)
{
	int8_t Datalen = 0;								//how many number in a single data
	int8_t DataMinlen = 0;							//how many number in a single data
	int8_t DataMaxlen = 0;							//how many number in a single data

	Datalen = I2CMS_MstRxData[*i]%4;
	devPointer->Input.arg[*DataCnt] = num;
	devPointer->Input.datalen[*DataCnt] = I2CMS_MstRxData[(*i)+1];
	(*i)=(*i)+Datalen+1;							//next data
	/* Handle Minimun */
	DataMinlen = I2CMS_MstRxData[(*i)]%4;
	devDataPointer->minimum = 0;
	for((*j)=0; (*j)<DataMinlen; (*j)++)
		devDataPointer->minimum |= (I2CMS_MstRxData[(*i)+(*j)+1]<<(8*(*j)));
	(*i)=(*i)+DataMinlen+1;							//next data
	/* Handle Maximun */
	DataMaxlen = I2CMS_MstRxData[(*i)]%4;
	devDataPointer->maximum = 0;
	for((*j)=0; (*j)<DataMaxlen; (*j)++)
		devDataPointer->maximum |= (I2CMS_MstRxData[(*i)+(*j)+1]<<(8*(*j)));
	(*i)=(*i)+DataMaxlen;							//last byte in this data
	/* update state */
	(*DataCnt)++;
}

void HandTIDRxDesRpt_output(TID_Device* devPointer, TID_DATA* devDataPointer, int8_t num, int8_t* DataCnt, int8_t* i, int8_t* j)
{
	int8_t Datalen = 0;								//how many number in a single data
	int8_t DataMinlen = 0;							//how many number in a single data
	int8_t DataMaxlen = 0;							//how many number in a single data

	Datalen = I2CMS_MstRxData[*i]%4;
	devPointer->Output.arg[*DataCnt] = num;
	devPointer->Output.datalen[*DataCnt] = I2CMS_MstRxData[(*i)+1];
	(*i)=(*i)+Datalen+1;							//next data
	/* Handle Minimun */
	DataMinlen = I2CMS_MstRxData[(*i)]%4;
	devDataPointer->minimum = 0;
	for((*j)=0; (*j)<DataMinlen; (*j)++)
		devDataPointer->minimum |= (I2CMS_MstRxData[(*i)+(*j)+1]<<(8*(*j)));
	(*i)=(*i)+DataMinlen+1;							//next data
	/* Handle Maximun */
	DataMaxlen = I2CMS_MstRxData[(*i)]%4;
	devDataPointer->maximum = 0;
	for((*j)=0; (*j)<DataMaxlen; (*j)++)
		devDataPointer->maximum |= (I2CMS_MstRxData[(*i)+(*j)+1]<<(8*(*j)));
	(*i)=(*i)+DataMaxlen;							//last byte in this data
	/* update state */
	(*DataCnt)++;
}

void slvTxRpDes(TID_DATA devData, uint8_t datalen, int* i, int16_t tidReg)
{
	I2CMS_SlvTxRPDesData[(*i)++] = tidReg;
	I2CMS_SlvTxRPDesData[(*i)++] = (uint8_t)datalen;
	if(datalen==1)
	{
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)(TID_REG_MIN + datalen);
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)devData.minimum;
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)(TID_REG_MAX + datalen);
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)devData.maximum;
	}
	else if(datalen==2)
	{
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)(TID_REG_MIN + datalen);
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)devData.minimum;
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)(devData.minimum>>8);
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)(TID_REG_MAX + datalen);
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)devData.maximum;
		I2CMS_SlvTxRPDesData[(*i)++] = (int8_t)(devData.maximum>>8);
	}
}
// =============================================================================================
// =============================================================================================
// =============================================================================================
//
//                  Master Handle Device Descriptor
//
// =============================================================================================
// =============================================================================================
// =============================================================================================

// =========================================================================
// =========================================================================
//                  Master Handle Device Descriptor
// =========================================================================
// =========================================================================
void HandDevDesc()
{
	int8_t i;
	int8_t datLen;
	datLen = (I2CMS_MstRxData[0]>>1)-1;
	if(I2CMS_MstRxData[0] != 0)
	{
		for(i=0; i<=datLen; i++)
		{
			uint16_t u16Data;

			u16Data = I2CMS_MstRxData[i*2] | (I2CMS_MstRxData[i*2+1]<<8);
			(&(pTidList[I2CMstDev]->DevDesc.DevDesc_leng))[i] = u16Data;

		}
	}
	for(i=0; i<64; i++)
	{
		I2CMS_MstRxData[i]=0;
	}
}

// =========================================================================
// =========================================================================
//                  Master Handle Report Descriptor
// =========================================================================
// =========================================================================
void HandRptDesc_sub(TID_Device* devPointer)
{
	int8_t i=2,j=1;
	int8_t type=0;									//1:Feature,2:Input,3:Output
	int8_t DataCnt=0;								//Count data number in every type

	while(i<I2CMS_MstRxData[0])
	{
		if(I2CMS_MstRxData[i] == 1)
		{
			i++;
			type = I2CMS_MstRxData[i];
			DataCnt = 0;
		}
		else
		{
			TID_DATA *p;
			int n;
			
			p = NULL;
			n = 0;
			switch(type)
			{
			case 1:									//Feature
			{
				switch(I2CMS_MstRxData[i])
				{
				case TID_REG_ONE:					//Feature,data1
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data1), 1, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data1);
					n = 1;
					break;
				}
				case TID_REG_TWO:					//Feature,data2
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data2), 2, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data2);
					n = 2;
					break;
				}
				case TID_REG_THR:					//Feature,data3
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data3), 3, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data3);
					n = 3;
					break;
				}
				case TID_REG_FOUR:					//Feature,data4
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data4), 4, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data4);
					n = 4;
					break;
				}
				case TID_REG_FIVE:					//Feature,data5
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data5), 5, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data5);
					n = 5;
					break;
				}
				case TID_REG_SIX:					//Feature,data6
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data6), 6, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data6);
					n = 6;
					break;
				}
				case TID_REG_SEV:					//Feature,data7
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data7), 7, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data7);
					n = 7;
					break;
				}
				case TID_REG_EIG:					//Feature,data8
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data8), 8, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data8);
					n = 8;
					break;
				}
				case TID_REG_NINE:					//Feature,data9
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data9), 9, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data9);
					n = 9;
					break;
				}
				case TID_REG_TEN:					//Feature,data10
				{
					//HandTIDRxDesRpt_feature(devPointer, &(devPointer->Feature.data10), 10, &DataCnt, &i, &j);
					p = &(devPointer->Feature.data10);
					n = 10;
					break;
				}
				}
				
				HandTIDRxDesRpt_feature(devPointer, p, n, &DataCnt, &i, &j);
				break;
			}
			case 2:									//Input
			{
				switch(I2CMS_MstRxData[i])
				{
				case TID_REG_ONE:					//Input,data1
				{
					//HandTIDRxDesRpt_input(devPointer, &(devPointer->Input.data1), 1, &DataCnt, &i, &j);
					p = &(devPointer->Input.data1);
					n = 1;
					break;
				}
				case TID_REG_TWO:					//Input,data2
				{
					//HandTIDRxDesRpt_input(devPointer, &(devPointer->Input.data2), 2, &DataCnt, &i, &j);
					p = &(devPointer->Input.data2);
					n = 2;
					break;
				}
				case TID_REG_THR:					//Input,data3
				{
					//HandTIDRxDesRpt_input(devPointer, &(devPointer->Input.data3), 3, &DataCnt, &i, &j);
					p = &(devPointer->Input.data3);
					n = 3;
					break;
				}
				case TID_REG_FOUR:					//Input,data4
				{
					//HandTIDRxDesRpt_input(devPointer, &(devPointer->Input.data4), 4, &DataCnt, &i, &j);
					p = &(devPointer->Input.data4);
					n = 4;
					break;
				}
				case TID_REG_FIVE:					//Input,data5
				{
					//HandTIDRxDesRpt_input(devPointer, &(devPointer->Input.data5), 5, &DataCnt, &i, &j);
					p = &(devPointer->Input.data5);
					n = 5;
					break;
				}
				}
				
				HandTIDRxDesRpt_input(devPointer, p, n, &DataCnt, &i, &j);
				break;
			}
			case 3:									//Output
			{
				switch(I2CMS_MstRxData[i])
				{
				case TID_REG_ONE:					//Output,data1
				{
					//HandTIDRxDesRpt_output(devPointer, &(devPointer->Output.data1), 1, &DataCnt, &i, &j);
					p = &(devPointer->Output.data1);
					n = 1;
					break;
				}
				case TID_REG_TWO:					//Output,data2
				{
					//HandTIDRxDesRpt_output(devPointer, &(devPointer->Output.data2), 2, &DataCnt, &i, &j);
					p = &(devPointer->Output.data2);
					n = 2;
					break;
				}
				case TID_REG_THR:					//Output,data3
				{
					//HandTIDRxDesRpt_output(devPointer, &(devPointer->Output.data3), 3, &DataCnt, &i, &j);
					p = &(devPointer->Output.data3);
					n = 3;
					break;
				}
				case TID_REG_FOUR:					//Output,data4
				{
					//HandTIDRxDesRpt_output(devPointer, &(devPointer->Output.data4), 4, &DataCnt, &i, &j);
					p = &(devPointer->Output.data4);
					n = 4;
					break;
				}
				case TID_REG_FIVE:					//Output,data5
				{
					//HandTIDRxDesRpt_output(devPointer, &(devPointer->Output.data5), 5, &DataCnt, &i, &j);
					p = &(devPointer->Output.data5);
					n = 5;
					break;
				}
				}
				
				HandTIDRxDesRpt_output(devPointer, p, n, &DataCnt, &i, &j);
				break;
			}
			}
		}
		i++;
	}
	
	for(i=0; i<64; i++)
	{
		I2CMS_MstRxData[i]=0;
	}
}

void HandRptDesc()
{
	HandRptDesc_sub(pTidList[I2CMstDev]);
}

// =========================================================================
// =========================================================================
//                  Handle Input Report
// =========================================================================
// =========================================================================
void HandInRpt_sub(TID_Device* devPointer)
{
	uint8_t i=0,k=2;

	for(i=0; i<5; i++)
	{
		switch(devPointer->Input.arg[i])
		{
		case 1:							//data1
		{
			HandTIDRxData(&devPointer->Input.data1.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Input.datalen[i],&k);
			break;
		}
		case 2:							//data2
		{
			HandTIDRxData(&devPointer->Input.data2.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Input.datalen[i],&k);
			break;
		}
		case 3:							//data3
		{
			HandTIDRxData(&devPointer->Input.data3.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Input.datalen[i],&k);
			break;
		}
		case 4:							//data4
		{
			HandTIDRxData(&devPointer->Input.data4.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Input.datalen[i],&k);
			break;
		}
		case 5:							//data5
		{
			HandTIDRxData(&devPointer->Input.data5.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Input.datalen[i],&k);
			break;
		}
		}
	}
	
	for(i=0; i<64; i++)
	{
		I2CMS_MstRxData[i]=0;
	}
}

void HandInRpt()
{
	HandInRpt_sub(pTidList[I2CMstDev]);
}

// =========================================================================
// =========================================================================
//                  Handle Get Feature
// =========================================================================
// =========================================================================
void HandGetFeat_sub(TID_Device* devPointer)
{
	uint8_t i=0,k=2;

	for(i=0; i<10; i++)
	{
		switch(devPointer->Feature.arg[i])
		{
		case 1:							//data1
		{
			HandTIDRxData(&devPointer->Feature.data1.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 2:							//data2
		{
			HandTIDRxData(&devPointer->Feature.data2.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 3:							//data3
		{
			HandTIDRxData(&devPointer->Feature.data3.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 4:							//data4
		{
			HandTIDRxData(&devPointer->Feature.data4.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 5:							//data5
		{
			HandTIDRxData(&devPointer->Feature.data5.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 6:							//data6
		{
			HandTIDRxData(&devPointer->Feature.data6.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 7:							//data7
		{
			HandTIDRxData(&devPointer->Feature.data7.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 8:							//data8
		{
			HandTIDRxData(&devPointer->Feature.data8.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 9:							//data9
		{
			HandTIDRxData(&devPointer->Feature.data9.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 10:						//data10
		{
			HandTIDRxData(&devPointer->Feature.data10.value,I2CMS_MstRxData[k],I2CMS_MstRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		}
	}
	for(i=0; i<64; i++)
	{
		I2CMS_MstRxData[i]=0;
	}
}

void HandGetFeat()
{
	HandGetFeat_sub(pTidList[I2CMstDev]);
}

// =========================================================================
//                  			Master deal with received Data
// =========================================================================
void TID_MstRxUpdate()
{
	switch(I2CMstCmd)
	{
	case 1:																			//handle device descriptor
		HandDevDesc();
		break;

	case 2:																			//handle report descriptor
		HandRptDesc();
		break;

	case 3:																			//handle input report
		HandInRpt();
		break;

	case 5:																			//handle Get Feature
		HandGetFeat();
		break;
	}
}

// =============================================================================================
// =============================================================================================
// =============================================================================================
//
//                  			Slave deal with received Data
//
// =============================================================================================
// =============================================================================================
// =============================================================================================

// =========================================================================
// =========================================================================
//                  Handle Set Feature
// =========================================================================
// =========================================================================
void HandSetFeat_sub(TID_Device* devPointer)
{
	uint8_t i=0,k=2;

	for(i=0; i<10; i++)
	{
		switch(devPointer->Feature.arg[i])
		{
		case 1:
		{
			HandTIDRxData(&devPointer->Feature.data1.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 2:
		{
			HandTIDRxData(&devPointer->Feature.data2.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 3:
		{
			HandTIDRxData(&devPointer->Feature.data3.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 4:
		{
			HandTIDRxData(&devPointer->Feature.data4.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 5:
		{
			HandTIDRxData(&devPointer->Feature.data5.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 6:
		{
			HandTIDRxData(&devPointer->Feature.data6.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 7:
		{
			HandTIDRxData(&devPointer->Feature.data7.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 8:
		{
			HandTIDRxData(&devPointer->Feature.data8.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 9:
		{
			HandTIDRxData(&devPointer->Feature.data9.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		case 10:
		{
			HandTIDRxData(&devPointer->Feature.data10.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Feature.datalen[i],&k);
			break;
		}
		}
	}
	for(i=0; i<64; i++)
	{
		I2CMS_SlvRxData[i]=0;
	}
}

void HandSetFeat()
{
	HandSetFeat_sub(pTidList[devNum]);

	if (devNum != 7)
		// update Flash except for IR
		UpdateFlashTID(pTidList[devNum]->Feature, devNum, false);
}

// =========================================================================
// =========================================================================
//                  Handle Output Report
// =========================================================================
// =========================================================================
void HandOutRpt_sub(TID_Device* devPointer)
{
	uint8_t i=0,k=2;

	for(i=0; i<5; i++)
	{
		switch(devPointer->Output.arg[i])
		{
		case 1:							//Sleep period
		{
			HandTIDRxData(&devPointer->Output.data1.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Output.datalen[i],&k);
			break;
		}
		case 2:							//Vibration Level
		{
			HandTIDRxData(&devPointer->Output.data2.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Output.datalen[i],&k);
			break;
		}
		case 3:							//Alerm time
		{
			HandTIDRxData(&devPointer->Output.data3.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Output.datalen[i],&k);
			break;
		}
		case 4:							//Alerm time
		{
			HandTIDRxData(&devPointer->Output.data4.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Output.datalen[i],&k);
			break;
		}
		case 5:							//Alerm time
		{
			HandTIDRxData(&devPointer->Output.data5.value,I2CMS_SlvRxData[k],I2CMS_SlvRxData[k+1],devPointer->Output.datalen[i],&k);
			break;
		}
		}
	}
	for(i=0; i<64; i++)
	{
		I2CMS_SlvRxData[i]=0;
	}
}

void HandOutRpt()
{
	HandOutRpt_sub(pTidList[devNum]);
}


// =========================================================================
// =========================================================================
//                  			Slave deal with received Data
// =========================================================================
// =========================================================================
void TID_SlvRxUpdate()
{
	switch(I2CSlvCmd)
	{
	case 4:																			//handle input report
		HandOutRpt();
		I2CSlvCmd = 0;
		break;

	case 6:																			//handle Get Feature
		HandSetFeat();
		I2CSlvCmd = 0;
		break;
	}
}


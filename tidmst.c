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

#include "tidmst.h"

uint8_t TIDDevEnFlag[8];
uint8_t TIDMstHandFlag;
uint8_t TIDMstStage;
uint8_t TIDMstInitFIN;
uint8_t TIDMstFirstInitFIN;
uint16_t TIDMstInitDevState;
uint16_t TIDMstEnReadDevState;
uint8_t TIDMstDevState[16];
int8_t reportAPFlag;

/*
 *			1:get input
 *			2:set output
 *			4:set feature
 *			8:get feature
 */
uint8_t TIDMstTRxCounter;

// ==============================================================
//					Slvae Store data to TID REG
// ==============================================================
void MstDataInit()
{
	TIDMstHandFlag = 0;
	BuzDev.dTod.dTHR = 1;
	LedDev.dTod.dTHR = 1;
	BuzDev.dTod.dFOUR = 0;
	LedDev.dTod.dFOUR = 1;
	BuzDev.dTod.dFIVE = 0;
	LedDev.dTod.dFIVE = 1;
	BuzDev.dTod.dSIX = 0;
	LedDev.dTod.dSIX = 1;
	BuzDev.dTod.dSEV = 0;
	LedDev.dTod.dSEV = 1;
	BuzDev.dTod.dEIG = 0;
	LedDev.dTod.dEIG = 1;
	BuzDev.dTod.dNINE = 0;
	LedDev.dTod.dNINE = 1;
	BuzDev.dTod.dX = 0;
	LedDev.dTod.dX = 0;
	BuzDev.dTod.dXI = 0;
	LedDev.dTod.dXI = 0;
	BuzDev.dTod.dXII = 0;
	LedDev.dTod.dXII = 0;
	BuzDev.dTod.dXIII = 0;
	LedDev.dTod.dXIII = 0;
	BuzDev.dTod.dXIV = 0;
	LedDev.dTod.dXIV = 0;
}
// **************************************************************
//					Master Store data to TID REG
// **************************************************************
void MstDataStore_sub(TID_Device* devPointer)
{
	uint8_t i=0, j=0;
	/* Store Feature  in I2CMS_MstTxSetFTData*/
	if((*devPointer).Feature.arg[0] != 0)																												//Feature
	{
		i=0;
		j=0;
		I2CMS_MstTxSetFTData[i++] = (int8_t)devPointer->DevDesc.GetFeatLeng;
		I2CMS_MstTxSetFTData[i++] = (int8_t)(devPointer->DevDesc.GetFeatLeng>>8);
		while(devPointer->Feature.arg[j] != 0)
		{
			switch(devPointer->Feature.arg[j])
			{
			case 1:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data1.value, devPointer->Feature.datalen[0],&i);
				j++;
				break;
			}
			case 2:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data2.value, devPointer->Feature.datalen[1],&i);
				j++;
				break;
			}
			case 3:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data3.value, devPointer->Feature.datalen[2],&i);
				j++;
				break;
			}
			case 4:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data4.value, devPointer->Feature.datalen[3],&i);
				j++;
				break;
			}
			case 5:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data5.value, devPointer->Feature.datalen[4],&i);
				j++;
				break;
			}
			case 6:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data6.value, devPointer->Feature.datalen[5],&i);
				j++;
				break;
			}
			case 7:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data7.value, devPointer->Feature.datalen[6],&i);
				j++;
				break;
			}
			case 8:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data8.value, devPointer->Feature.datalen[7],&i);
				j++;
				break;
			}
			case 9:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data9.value, devPointer->Feature.datalen[8],&i);
				j++;
				break;
			}
			case 10:
			{
				StoreTIDTxData(&I2CMS_MstTxSetFTData[i], devPointer->Feature.data10.value, devPointer->Feature.datalen[9],&i);
				j++;
				break;
			}
			}
		}
	}
	/* Store Feature  in I2CMS_MstTxOutRPData */
	/* Output */
	if((*devPointer).Output.arg[0] != 0)
	{
		i=0;
		j=0;
		I2CMS_MstTxOutRPData[i++] = (int8_t)devPointer->DevDesc.OutRptLeng;
		I2CMS_MstTxOutRPData[i++] = (int8_t)(devPointer->DevDesc.OutRptLeng>>8);
		while((*devPointer).Output.arg[j] != 0)
		{
			switch((*devPointer).Output.arg[j])
			{
			case 1:
			{
				StoreTIDTxData(&I2CMS_MstTxOutRPData[i], devPointer->Output.data1.value, devPointer->Output.datalen[0],&i);
				j++;
				break;
			}
			case 2:
			{
				StoreTIDTxData(&I2CMS_MstTxOutRPData[i], devPointer->Output.data2.value, devPointer->Output.datalen[1],&i);
				j++;
				break;
			}
			case 3:
			{
				StoreTIDTxData(&I2CMS_MstTxOutRPData[i], devPointer->Output.data3.value, devPointer->Output.datalen[2],&i);
				j++;
				break;
			}
			case 4:
			{
				StoreTIDTxData(&I2CMS_MstTxOutRPData[i], devPointer->Output.data4.value, devPointer->Output.datalen[3],&i);
				j++;
				break;
			}
			case 5:
			{
				StoreTIDTxData(&I2CMS_MstTxOutRPData[i], devPointer->Output.data5.value, devPointer->Output.datalen[4],&i);
				j++;
				break;
			}
			}
		}
	}
}

void MstDataStore()
{
#if 1
	MstDataStore_sub(pTidList[I2CMstDev]);
#else	
	switch (I2CMstDev)
	{
	case 1:
	{
		MstDataStore_sub(&BuzDev);
		break;
	}
	case 2:
	{
		MstDataStore_sub(&LedDev);
		break;
	}
	case 3:
	{
		MstDataStore_sub(&AHRSDev);
		break;
	}
	case 4:
	{
		MstDataStore_sub(&SonDev);
		break;
	}
	case 5:
	{
		MstDataStore_sub(&TempDev);
		break;
	}
	case 6:
	{
		MstDataStore_sub(&GasDev);
		break;
	}
	case 7:
	{
		MstDataStore_sub(&IRDev);
		break;
	}
	case 8:
	{
		MstDataStore_sub(&KeyDev);
		break;
	}
	case 9:
	{
		MstDataStore_sub(&ResDev9);
		break;
	}
	case 10:
	{
		MstDataStore_sub(&ResDev10);
		break;
	}
	case 11:
	{
		MstDataStore_sub(&ResDev11);
		break;
	}
	case 12:
	{
		MstDataStore_sub(&ResDev12);
		break;
	}
	case 13:
	{
		MstDataStore_sub(&ResDev13);
		break;
	}
	case 14:
	{
		MstDataStore_sub(&ResDev14);
		break;
	}
	}
#endif	
}

// *******************************************************
//							Master Handle Device state
// *******************************************************
void TIDMstUpdateDevState()
{

//	if((AHRSDev.Input.data2.value==1) ||
//			(SonDev.Input.data2.value==1) ||
//			(TempDev.Input.data3.value==1) ||
//			(TempDev.Input.data4.value==1) ||
//			(GasDev.Input.data2.value==1))				//over flag
//	{
//		if(BuzDev.dTod.dTHR==1)		BuzDev.Output.data1.value = 1;				//Song
//		if(LedDev.dTod.dTHR==1)		LedDev.Output.data1.value = 1;				//Period
//		if(AHRSDev.dTod.dTHR==1)	AHRSDev.Input.data2.value = 0;
//		if(SonDev.dTod.dTHR==1)		SonDev.Input.data2.value = 0;
//		if(TempDev.dTod.dTHR==1)	TempDev.Input.data3.value = 0;
//		if(TempDev.dTod.dTHR==1)	TempDev.Input.data4.value = 0;
//		if(GasDev.dTod.dTHR==1)		GasDev.Input.data2.value = 0;
//	}
	if((AHRSDev.Input.data2.value==1) && (TIDMstInitDevState & (1<<AHRSDEV)))
	{
		if(BuzDev.dTod.dTHR==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dTHR==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	else if((SonDev.Input.data2.value==1) && (TIDMstInitDevState & (1<<SONARDEV)))
	{
		if(BuzDev.dTod.dFOUR==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dFOUR==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	else if(((TempDev.Input.data3.value==1) || (TempDev.Input.data4.value==1)) && (TIDMstInitDevState & (1<<TEMPDEV)))
	{
		if(BuzDev.dTod.dFIVE==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dFIVE==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	else if((GasDev.Input.data2.value==1) && (TIDMstInitDevState & (1<<GASDEV)))
	{
		if(BuzDev.dTod.dSIX==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dSIX==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	else if((IRDev.Input.data1.value & 1) && (TIDMstInitDevState & (1<<IRDEV)))
	{
		if(BuzDev.dTod.dSEV==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dSEV==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	else if((KeyDev.Input.data1.value > 0) && (TIDMstInitDevState & (1<<KEYDEV)))
	{
		if(BuzDev.dTod.dEIG==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dEIG==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	else if((ResDev9.Input.data2.value == 1) && (TIDMstInitDevState & (1<<RESDEV9)))
	{
		if(BuzDev.dTod.dNINE==1)
		{
			BuzDev.Output.data1.value = 1;				//Song
			TIDMstDevState[BUZZERDEV] |= 2;				//buzzer operate:				output(2)
		}
		if(LedDev.dTod.dNINE==1)
		{
			LedDev.Output.data1.value = 1;				//Period
			TIDMstDevState[LEDDEV] |= 2;				//led operate: 					output(2)
		}
	}
	if (TIDMstInitDevState & (1<<RESDEV9))	PA2 = 0;
	else PA2 = 1;

	if((TIDMstDevState[BUZZERDEV] & 2)==0 && (reportAPFlag == 0))				//I2C output part & AP received data that are done
	{
		BuzDev.Output.data1.value = 0;				//Song
	}

	if((TIDMstDevState[LEDDEV] & 2)==0 && (reportAPFlag == 0))					//I2C output part & AP received data that are done
	{
		LedDev.Output.data1.value = 0;		//Period
	}

	if((TIDMstDevState[IRDEV] & 2)==0 && (reportAPFlag == 0))					//I2C output part & AP received data that are done
	{
		IRDev.Output.data1.value = 0;		//send flag
	}
	if((TIDMstDevState[IRDEV] & 2)==0 && (reportAPFlag == 0))					//I2C output part & AP received data that are done
	{
		IRDev.Output.data2.value = 0;		//send flag
	}

	TIDMstDevState[BUZZERDEV] |= 1;				//buzzer operate:		read input(1)
	TIDMstDevState[LEDDEV] |= 1;				//led operate: 			read input(1)
	TIDMstDevState[AHRSDEV] |= 1;				//AHRS operate: 		read input(1)
	TIDMstDevState[SONARDEV] |= 1;				//sonar operate: 		read input(1)
	TIDMstDevState[TEMPDEV] |= 1;				//temperature operate:	read input(1)
	TIDMstDevState[GASDEV] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[IRDEV] |= 1;					//IR operate:			read input(1)
	TIDMstDevState[KEYDEV] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[RESDEV9] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[RESDEV10] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[RESDEV11] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[RESDEV12] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[RESDEV13] |= 1;				//gas operate:			read input(1)
	TIDMstDevState[RESDEV14] |= 1;				//gas operate:			read input(1)

}

// *************************************************************
//					TID initialize device state
// *************************************************************

void getDev_DevDes(uint8_t devNum, uint8_t DevReg)
{
	if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
	{
		if(I2CMstTimeOutFlag==1)
		{
			TIDMstInitDevState &= ~(1<<devNum);
			TIDMstStage+=2;
		}
		else if(I2CMS_MstRxData[0]==DEV_NOT_INIT)
		{
			TIDMstInitDevState &= ~(1<<devNum);
			TIDMstStage+=2;
		}
		else
		{
			HandDevDesc();
			TIDMstDevState[devNum] |= 4;				//device operate:				get feature(4)
			if(TIDMstInitDevState & (1<<devNum))
				TIDMstStage+=2;
			else
				TIDMstStage++;
		}
		TIDMstHandFlag=2;
	}
	else
	{
		I2CMstDev = devNum;
		I2C_MASTER_Read_Write_Start(DevReg, 0);		//21
		I2C_MASTER_Read_Write_Con(DevReg, 0x01, I2C_READ);
		TIDMstHandFlag=0;
	}
}

void getDev_RepDes(uint8_t devNum, uint8_t DevReg)
{
	if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
	{
		if(I2CMstTimeOutFlag==1)
		{
			TIDMstInitDevState &= ~(1<<devNum);
			TIDMstStage++;
		}
		else
		{
			HandRptDesc();
			TIDMstInitDevState |= (1<<devNum);
			TIDMstStage++;
		}
		TIDMstHandFlag=2;
	}
	else
	{
		I2CMstDev = devNum;
		I2C_MASTER_Read_Write_Start(DevReg, 0);
		I2C_MASTER_Read_Write_Con(DevReg, 0x02, I2C_READ);
		TIDMstHandFlag=0;
	}
}

void TIDMst_GetDev()
{
	if((I2CMstEndFlag==1) && (TIDMstHandFlag==0))
	{
		TIDMstHandFlag=1;
	}
	switch(TIDMstStage)
	{
	case 0:																		//Buzzer Device Descriptor
	{
		getDev_DevDes(BUZZERDEV, 0x15);											//21:Buzzer
		break;
	}
	case 1:																		//Buzzer Report Descriptor
	{
		getDev_RepDes(BUZZERDEV, 0x15);											//21:Buzzer
		break;
	}
	case 2:																		//LED Device Descriptor
	{
		getDev_DevDes(LEDDEV, 0x16);											//22:LED
		break;
	}
	case 3:																		//LED Report Descriptor
	{
		getDev_RepDes(LEDDEV, 0x16);											//22:LED
		break;
	}
	case 4:																		//AHRS Device Descriptor
	{
		getDev_DevDes(AHRSDEV, 0x17);											//23:AHRS
		break;
	}
	case 5:																		//AHRS Report Descriptor
	{
		getDev_RepDes(AHRSDEV, 0x17);											//23:AHRS
		break;
	}
	case 6:																		//Sonar Device Descriptor
	{
		getDev_DevDes(SONARDEV, 0x18);											//24:SONAR
		break;
	}
	case 7:																		//Sonar Report Descriptor
	{
		getDev_RepDes(SONARDEV, 0x18);											//24:SONAR
		break;
	}
	case 8:																		//Temperature Device Descriptor
	{
		getDev_DevDes(TEMPDEV, 0x19);											//25:TEMPERATURE
		break;
	}
	case 9:																		//Temperature Report Descriptor
	{
		getDev_RepDes(TEMPDEV, 0x19);											//25:TEMPERATURE
		break;
	}
	case 10:																		//Gas Device Descriptor
	{
		getDev_DevDes(GASDEV, 0x1a);											//26:GAS
		break;
	}
	case 11:																		//Gas Report Descriptor
	{
		getDev_RepDes(GASDEV, 0x1a);						//26:GAS
		break;
	}
	case 12:												//IR Device Descriptor
	{
		getDev_DevDes(IRDEV, 0x1b);							//27:IR
		break;
	}
	case 13:												//IR Report Descriptor
	{
		getDev_RepDes(IRDEV, 0x1b);							//27:IR
		break;
	}
	case 14:												//KEY Device Descriptor
	{
		getDev_DevDes(KEYDEV, 0x1c);						//28:KEY
		break;
	}
	case 15:												//KEY Report Descriptor
	{
		getDev_RepDes(KEYDEV, 0x1c);						//28:KEY
		break;
	}
	case 16:												//RESERVE Device 9 Descriptor
	{
		getDev_DevDes(RESDEV9, 0x1d);						//29:RESERVE Device 9
		break;
	}
	case 17:												//RESERVE Device 9 Report Descriptor
	{
		getDev_RepDes(RESDEV9, 0x1d);						//29:RESERVE Device 9
		break;
	}
	case 18:												//RESERVE Device 10 Descriptor
	{
		getDev_DevDes(RESDEV10, 0x1e);						//30:RESERVE Device 10
		break;
	}
	case 19:												//RESERVE Device 10 Report Descriptor
	{
		getDev_RepDes(RESDEV10, 0x1e);						//30:RESERVE Device 10
		break;
	}
	case 20:												//RESERVE Device 11 Descriptor
	{
		getDev_DevDes(RESDEV11, 0x1f);						//31:RESERVE Device 11
		break;
	}
	case 21:												//RESERVE Device 11 Report Descriptor
	{
		getDev_RepDes(RESDEV11, 0x1f);						//32:RESERVE Device 11
		break;
	}
	case 22:												//RESERVE Device 12 Descriptor
	{
		getDev_DevDes(RESDEV12, 0x20);						//32:RESERVE Device 12
		break;
	}
	case 23:												//RESERVE Device 12 Report Descriptor
	{
		getDev_RepDes(RESDEV12, 0x20);						//32:RESERVE Device 12
		break;
	}
	case 24:												//RESERVE Device 13 Descriptor
	{
		getDev_DevDes(RESDEV13, 0x21);						//33:RESERVE Device 13
		break;
	}
	case 25:												//RESERVE Device 13 Report Descriptor
	{
		getDev_RepDes(RESDEV13, 0x21);						//33:RESERVE Device 13
		break;
	}
	case 26:												//RESERVE Device 14 Descriptor
	{
		if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
		{
			if(I2CMstTimeOutFlag==1)
			{
				TIDMstInitDevState &= ~(1<<RESDEV14);
				TIDMstStage=0;
				TIDMstInitFIN=1;
				TIDMstFirstInitFIN=1;
			}
			else if(I2CMS_MstRxData[0]==DEV_NOT_INIT)
			{
				TIDMstInitDevState &= ~(1<<RESDEV14);
				TIDMstStage=0;
				TIDMstInitFIN=1;
				TIDMstFirstInitFIN=1;
			}
			else
			{
				HandDevDesc();
				TIDMstStage++;
			}
			TIDMstHandFlag=2;
		}
		else
		{
			I2CMstDev = RESDEV14;
			I2C_MASTER_Read_Write_Start(0x22, 0);		//34
			I2C_MASTER_Read_Write_Con(0x22, 0x01, I2C_READ);
			TIDMstHandFlag=0;
		}
		break;
	}
	case 27:													//RESERVE Device 14 Report Descriptor
	{
		if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
		{
			if(I2CMstTimeOutFlag==1)
			{
				TIDMstInitDevState &= ~(1<<RESDEV14);
				TIDMstStage=0;
				TIDMstInitFIN=1;
				TIDMstFirstInitFIN=1;
			}
			else
			{
				HandRptDesc();
				if((TIDMstInitDevState & (1<<RESDEV14))==0)
					TIDMstDevState[RESDEV14] |= 4;				//device operate:				get feature(4)
				TIDMstInitDevState |= 1<<RESDEV14;
				TIDMstStage=0;
				TIDMstInitFIN=1;
				TIDMstFirstInitFIN=1;
			}
			TIDMstHandFlag=2;
		}
		else
		{
			I2CMstDev = RESDEV14;
			I2C_MASTER_Read_Write_Start(0x22, 0);		//34
			I2C_MASTER_Read_Write_Con(0x22, 0x02, I2C_READ);
			TIDMstHandFlag=0;
		}
		break;
	}
	}
}

// ***************************************************************************************
//							Master Transfer/Receive Data
// ***************************************************************************************
//	***********  Subfunction   *****************
void DevTRx_In(uint8_t devNum, uint8_t devNum_next, uint8_t DevReg)
{
	/* check device is connected */
	if(TIDMstInitDevState & (1<<devNum))
	{
		/* check device input command need execute */
		if(TIDMstDevState[devNum] & 1)
		{
			if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
			{
				if(I2CMstTimeOutFlag==0)
				{
					HandInRpt();
					TIDMstDevState[devNum] = TIDMstDevState[devNum] & (~1);
					TIDMstTRxCounter=4*devNum+1;
				}
				else
				{
					TIDMstDevState[devNum] = TIDMstDevState[devNum] & (~1);
					TIDMstTRxCounter=4*devNum+1;
				}
				TIDMstHandFlag=2;
			}
			else
			{
				I2CMstDev = devNum;
				I2C_MASTER_Read_Write_Start(DevReg, 0);
				I2C_MASTER_Read_Write_Con(DevReg, 0x03, I2C_READ);
				TIDMstHandFlag=0;
			}
		}
		else TIDMstTRxCounter=4*devNum+1;
	}
	else TIDMstTRxCounter=4*devNum_next;
}

void DevTRx_Out(uint8_t devNum, uint8_t devNum_next, uint8_t DevReg)
{

	/* check device is connected */
	if(TIDMstInitDevState & (1<<devNum))
	{
		/* check device output command need execute */
		if(TIDMstDevState[devNum] & 2)
		{
			if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
			{
				TIDMstDevState[devNum] = TIDMstDevState[devNum] & (~(2));
				TIDMstTRxCounter=4*devNum+2;
				TIDMstHandFlag=2;
			}
			else
			{
				I2CMstDev = devNum;
				MstDataStore();
				I2C_MASTER_Read_Write_Start(DevReg, 0);
				I2C_MASTER_Read_Write_Con(DevReg, 0x04, I2C_WRITE);
				TIDMstHandFlag=0;
			}
		}
		else TIDMstTRxCounter=4*devNum+2;
	}
	else TIDMstTRxCounter=4*devNum_next;
}

void DevTRx_GetFeat(uint8_t devNum, uint8_t devNum_next, uint8_t DevReg)
{

	/* check device is connected */
	if(TIDMstInitDevState & (1<<devNum))
	{
		/* check device input command need execute */
		if((TIDMstDevState[devNum] & 4) && ((TIDMstDevState[devNum] & 8) ==0))
		{
			if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
			{
				if(I2CMstTimeOutFlag==0)
				{
					HandGetFeat();
					TIDMstDevState[devNum] = TIDMstDevState[devNum] & (~4);
					TIDMstTRxCounter=4*devNum+3;
				}
				else
				{
					TIDMstDevState[devNum] = TIDMstDevState[devNum] & (~4);
					TIDMstTRxCounter=4*devNum+3;
				}
				TIDMstHandFlag=2;
			}
			else
			{
				I2CMstDev = devNum;
				I2C_MASTER_Read_Write_Start(DevReg, 0);
				I2C_MASTER_Read_Write_Con(DevReg, 0x05, I2C_READ);			//Buzzer get feature
				TIDMstHandFlag=0;
			}
		}
		else TIDMstTRxCounter=4*devNum+3;
	}
	else TIDMstTRxCounter=4*devNum_next;
}

void DevTRx_SetFeat(uint8_t devNum, uint8_t devNum_next, uint8_t DevReg)
{

	/* check device is connected */
	if(TIDMstInitDevState & (1<<devNum))
	{
		/* check device output command need execute */
		if(TIDMstDevState[devNum] & 8)
		{
			if((I2CMstEndFlag==1) && (TIDMstHandFlag==1))
			{
				TIDMstDevState[devNum] |= 4;
				TIDMstDevState[devNum] = TIDMstDevState[devNum] & (~8);
				TIDMstTRxCounter=4*devNum_next;
				TIDMstHandFlag=2;
			}
			else
			{
				I2CMstDev = devNum;
				MstDataStore();
				I2C_MASTER_Read_Write_Start(DevReg, 0);
				I2C_MASTER_Read_Write_Con(DevReg, 0x06, I2C_WRITE);			//Buzzer set feature
				TIDMstHandFlag=0;
			}
		}
		else TIDMstTRxCounter=4*devNum_next;
	}
	else TIDMstTRxCounter=4*devNum_next;
}

void TIDMstDevTRx()
{
	if((I2CMstEndFlag==1) && (TIDMstHandFlag==0))
	{
		TIDMstHandFlag=1;
	}
	switch(TIDMstTRxCounter)
	{
	case 0:
	{
		TIDMstTRxCounter = 4;
		TIDMstInitFIN = 2;
		break;
	}
	/* Buzzer Input(4) */
	case (4*BUZZERDEV):
	{
		DevTRx_In(BUZZERDEV, LEDDEV, 0x15);
		break;
	}
	/* Buzzer Output(5) */
	case (4*BUZZERDEV+1):
	{
		DevTRx_Out(BUZZERDEV, LEDDEV, 0x15);
		break;
	}
	/* Buzzer Get Feature(6) */
	case (4*BUZZERDEV+2):
	{
		DevTRx_GetFeat(BUZZERDEV, LEDDEV, 0x15);
		break;
	}
	/* Buzzer Set Feature(7) */
	case (4*BUZZERDEV+3):
	{
		DevTRx_SetFeat(BUZZERDEV, LEDDEV, 0x15);
		break;
	}
	/* LED Input(8) */
	case (4*LEDDEV):
	{
		DevTRx_In(LEDDEV, AHRSDEV, 0x16);
		break;
	}
	/* LED Output(9) */
	case (4*LEDDEV+1):
	{
		DevTRx_Out(LEDDEV, AHRSDEV, 0x16);
		break;
	}
	/* LED Get Feature(10) */
	case (4*LEDDEV+2):
	{
		DevTRx_GetFeat(LEDDEV, AHRSDEV, 0x16);
		break;
	}
	/* LED Set Feature(11) */
	case (4*LEDDEV+3):
	{
		DevTRx_SetFeat(LEDDEV, AHRSDEV, 0x16);
		break;
	}
	/* AHRS Input(12) */
	case (4*AHRSDEV):
	{
		DevTRx_In(AHRSDEV, SONARDEV, 0x17);
		break;
	}
	/* AHRS Output(13) */
	case (4*AHRSDEV+1):
	{
		DevTRx_Out(AHRSDEV, SONARDEV, 0x17);
		break;
	}
	/* AHRS Get Feature(14) */
	case (4*AHRSDEV+2):
	{
		DevTRx_GetFeat(AHRSDEV, SONARDEV, 0x17);
		break;
	}
	/* AHRS Set Feature(15) */
	case (4*AHRSDEV+3):
	{
		DevTRx_SetFeat(AHRSDEV, SONARDEV, 0x17);
		break;
	}
	/* Sonar Input(16) */
	case (4*SONARDEV):
	{
		DevTRx_In(SONARDEV, TEMPDEV, 0x18);
		break;
	}
	/* Sonar Output(17) */
	case (4*SONARDEV+1):
	{
		DevTRx_Out(SONARDEV, TEMPDEV, 0x18);
		break;
	}
	/* Sonar Get Feature(18) */
	case (4*SONARDEV+2):
	{
		DevTRx_GetFeat(SONARDEV, TEMPDEV, 0x18);
		break;
	}
	/* Sonar Set Feature(19) */
	case (4*SONARDEV+3):
	{
		DevTRx_SetFeat(SONARDEV, TEMPDEV, 0x18);
		break;
	}
	/* Temperature Input(20) */
	case (4*TEMPDEV):
	{
		DevTRx_In(TEMPDEV, GASDEV, 0x19);
		break;
	}
	/* Temperature Output(21) */
	case (4*TEMPDEV+1):
	{
		DevTRx_Out(TEMPDEV, GASDEV, 0x19);
		break;
	}
	/* Temperature Get Feature(22) */
	case (4*TEMPDEV+2):
	{
		DevTRx_GetFeat(TEMPDEV, GASDEV, 0x19);
		break;
	}
	/* Temperature Set Feature(23) */
	case (4*TEMPDEV+3):
	{
		DevTRx_SetFeat(TEMPDEV, GASDEV, 0x19);
		break;
	}
	/* Gas Input(24) */
	case (4*GASDEV):
	{
		DevTRx_In(GASDEV, IRDEV, 0x1a);
		break;
	}
	/* Gas Output(25) */
	case (4*GASDEV+1):
	{
		DevTRx_Out(GASDEV, IRDEV, 0x1a);
		break;
	}
	/* Gas Get Feature(26) */
	case (4*GASDEV+2):
	{
		DevTRx_GetFeat(GASDEV, IRDEV, 0x1a);
		break;
	}
	/* Gas Set Feature(27) */
	case (4*GASDEV+3):
	{
		DevTRx_SetFeat(GASDEV, IRDEV, 0x1a);
		break;
	}
	/* IR Input(28) */
	case (4*IRDEV):
	{
		DevTRx_In(IRDEV, KEYDEV, 0x1b);
		break;
	}
	/* IR Output(29) */
	case (4*IRDEV+1):
	{
		DevTRx_Out(IRDEV, KEYDEV, 0x1b);
		break;
	}
	/* IR Get Feature(30) */
	case (4*IRDEV+2):
	{
		DevTRx_GetFeat(IRDEV, KEYDEV, 0x1b);
		break;
	}
	/* IR Set Feature(31) */
	case (4*IRDEV+3):
	{
		DevTRx_SetFeat(IRDEV, KEYDEV, 0x1b);
		break;
	}
	/* KEY Input(32) */
	case (4*KEYDEV):
	{
		DevTRx_In(KEYDEV, RESDEV9, 0x1c);
		break;
	}
	/* KEY Output(33) */
	case (4*KEYDEV+1):
	{
		DevTRx_Out(KEYDEV, RESDEV9, 0x1c);
		break;
	}
	/* KEY Get Feature(34) */
	case (4*KEYDEV+2):
	{
		DevTRx_GetFeat(KEYDEV, RESDEV9, 0x1c);
		break;
	}
	/* KEY Set Feature(35) */
	case (4*KEYDEV+3):
	{
		DevTRx_SetFeat(KEYDEV, RESDEV9, 0x1c);
		break;
	}
	/* RESDEV9 Input(36) */
	case (4*RESDEV9):
	{
		DevTRx_In(RESDEV9, RESDEV10, 0x1d);
		break;
	}
	/* RESDEV9 Output(37) */
	case (4*RESDEV9+1):
	{
		DevTRx_Out(RESDEV9, RESDEV10, 0x1d);
		break;
	}
	/* RESDEV9 Get Feature(38) */
	case (4*RESDEV9+2):
	{
		DevTRx_GetFeat(RESDEV9, RESDEV10, 0x1d);
		break;
	}
	/* RESDEV9 Set Feature(39) */
	case (4*RESDEV9+3):
	{
		DevTRx_SetFeat(RESDEV9, RESDEV10, 0x1d);
		break;
	}
	/* RESDEV10 Input(40) */
	case (4*RESDEV10):
	{
		DevTRx_In(RESDEV10, RESDEV11, 0x1e);
		break;
	}
	/* RESDEV10 Output(41) */
	case (4*RESDEV10+1):
	{
		DevTRx_Out(RESDEV10, RESDEV11, 0x1e);
		break;
	}
	/* RESDEV10 Get Feature(42) */
	case (4*RESDEV10+2):
	{
		DevTRx_GetFeat(RESDEV10, RESDEV11, 0x1e);
		break;
	}
	/* RESDEV10 Set Feature(43) */
	case (4*RESDEV10+3):
	{
		DevTRx_SetFeat(RESDEV10, RESDEV11, 0x1e);
		break;
	}
	/* RESDEV11 Input(44) */
	case (4*RESDEV11):
	{
		DevTRx_In(RESDEV11, RESDEV12, 0x1f);
		break;
	}
	/* RESDEV11 Output(45) */
	case (4*RESDEV11+1):
	{
		DevTRx_Out(RESDEV11, RESDEV12, 0x1f);
		break;
	}
	/* RESDEV11 Get Feature(46) */
	case (4*RESDEV11+2):
	{
		DevTRx_GetFeat(RESDEV11, RESDEV12, 0x1f);
		break;
	}
	/* RESDEV11 Set Feature(47) */
	case (4*RESDEV11+3):
	{
		DevTRx_SetFeat(RESDEV11, RESDEV12, 0x1f);
		break;
	}
	/* RESDEV12 Input(48) */
	case (4*RESDEV12):
	{
		DevTRx_In(RESDEV12, RESDEV13, 0x20);
		break;
	}
	/* RESDEV12 Output(49) */
	case (4*RESDEV12+1):
	{
		DevTRx_Out(RESDEV12, RESDEV13, 0x20);
		break;
	}
	/* RESDEV12 Get Feature(50) */
	case (4*RESDEV12+2):
	{
		DevTRx_GetFeat(RESDEV12, RESDEV13, 0x20);
		break;
	}
	/* RESDEV12 Set Feature(51) */
	case (4*RESDEV12+3):
	{
		DevTRx_SetFeat(RESDEV12, RESDEV13, 0x20);
		break;
	}
	/* RESDEV13 Input(52) */
	case (4*RESDEV13):
	{
		DevTRx_In(RESDEV13, RESDEV14, 0x21);
		break;
	}
	/* RESDEV13 Output(53) */
	case (4*RESDEV13+1):
	{
		DevTRx_Out(RESDEV13, RESDEV14, 0x21);
		break;
	}
	/* RESDEV13 Get Feature(54) */
	case (4*RESDEV13+2):
	{
		DevTRx_GetFeat(RESDEV13, RESDEV14, 0x21);
		break;
	}
	/* RESDEV13 Set Feature(55) */
	case (4*RESDEV13+3):
	{
		DevTRx_SetFeat(RESDEV13, RESDEV14, 0x21);
		break;
	}
	/* RESDEV14 Input(56) */
	case (4*RESDEV14):
	{
		DevTRx_In(RESDEV14, 0, 0x22);
		break;
	}
	/* RESDEV14 Output(57) */
	case (4*RESDEV14+1):
	{
		DevTRx_Out(RESDEV14, 0, 0x22);
		break;
	}
	/* RESDEV14 Get Feature(58) */
	case (4*RESDEV14+2):
	{
		DevTRx_GetFeat(RESDEV14, 0, 0x22);
		break;
	}
	/* RESDEV14 Set Feature(59) */
	case (4*RESDEV14+3):
	{
		DevTRx_SetFeat(RESDEV14, 0, 0x22);
		break;
	}
	}
}

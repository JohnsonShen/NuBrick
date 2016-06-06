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

#include "tiddev.h"

uint16_t tidReg[10] = {TID_REG_ONE, TID_REG_TWO, TID_REG_THR, TID_REG_FOUR, TID_REG_FIVE, TID_REG_SIX, TID_REG_SEV, TID_REG_EIG, TID_REG_NINE, TID_REG_TEN};

// ==============================================================
//										Slvae Store data to TID REG
// ==============================================================
void SlvDevInit(TID_Device* devPointer)
{
	int i,j,k;					//i:total number, j:data number in class, k:class number

	I2CMS_SlvTxDevDesData[0] = devPointer->DevDesc.DevDesc_leng;				//((devPointer)->DevDesc.DevDesc_leng)
	I2CMS_SlvTxDevDesData[1] = (devPointer->DevDesc.DevDesc_leng)>>8;
	I2CMS_SlvTxDevDesData[2] = devPointer->DevDesc.RptDesc_leng;				//Report descriptor
	I2CMS_SlvTxDevDesData[3] = (devPointer->DevDesc.RptDesc_leng)>>8;
	I2CMS_SlvTxDevDesData[4] = devPointer->DevDesc.InRptLeng;					//Input report
	I2CMS_SlvTxDevDesData[5] = (devPointer->DevDesc.InRptLeng)>>8;
	I2CMS_SlvTxDevDesData[6] = devPointer->DevDesc.OutRptLeng;					//Output report
	I2CMS_SlvTxDevDesData[7] = (devPointer->DevDesc.OutRptLeng)>>8;
	I2CMS_SlvTxDevDesData[8] = devPointer->DevDesc.GetFeatLeng;					//Get feature
	I2CMS_SlvTxDevDesData[9] = (devPointer->DevDesc.GetFeatLeng)>>8;
	I2CMS_SlvTxDevDesData[10] = devPointer->DevDesc.SetFeatLeng;				//Set feature
	I2CMS_SlvTxDevDesData[11] = (devPointer->DevDesc.SetFeatLeng)>>8;
	I2CMS_SlvTxDevDesData[12] = devPointer->DevDesc.CID;						//manufacturers ID
	I2CMS_SlvTxDevDesData[13] = (devPointer->DevDesc.CID)>>8;
	I2CMS_SlvTxDevDesData[14] = devPointer->DevDesc.DID;						//Product ID
	I2CMS_SlvTxDevDesData[15] = (devPointer->DevDesc.DID)>>8;
	I2CMS_SlvTxDevDesData[16] = devPointer->DevDesc.PID;						//Device firmware revision
	I2CMS_SlvTxDevDesData[17] = (devPointer->DevDesc.PID)>>8;
	I2CMS_SlvTxDevDesData[18] = devPointer->DevDesc.UID;						//Device Class type
	I2CMS_SlvTxDevDesData[19] = (devPointer->DevDesc.UID)>>8;
	I2CMS_SlvTxDevDesData[20] = devPointer->DevDesc.UCID;						//UCID
	I2CMS_SlvTxDevDesData[21] = (devPointer->DevDesc.UCID)>>8;
	I2CMS_SlvTxDevDesData[22] = devPointer->DevDesc.reserve1;					//reserve
	I2CMS_SlvTxDevDesData[23] = (devPointer->DevDesc.reserve1)>>8;
	I2CMS_SlvTxDevDesData[24] = devPointer->DevDesc.reserve2;					//reserve
	I2CMS_SlvTxDevDesData[25] = (devPointer->DevDesc.reserve2)>>8;

	/* Reprot Descriptor Data */
	/*I2CMS_SlvTxRPDesData*/
	I2CMS_SlvTxRPDesData[0] = (int8_t)devPointer->DevDesc.RptDesc_leng;
	I2CMS_SlvTxRPDesData[1] = (int8_t)(devPointer->DevDesc.RptDesc_leng)>>8;
	i=2;
	for(k=1; k<=3; k++)
	{
		if(k==1)																																			//Feature type
		{
			if(devPointer->Feature.dataNum != 0)
			{
				I2CMS_SlvTxRPDesData[i++] = TID_CHANGE_STATE_FLAG;												//Change type sign
				I2CMS_SlvTxRPDesData[i++] = TID_FEATURE_FLAG;
				for(j=0; j< (devPointer->Feature.dataNum); j++)																				//import data form
				{
					if(j==0)
					{
						slvTxRpDes((devPointer->Feature.data1), (devPointer->Feature.datalen[0]), &i, tidReg[0]);
					}
					else if(j==1)
					{
						slvTxRpDes((devPointer->Feature.data2), (devPointer->Feature.datalen[1]), &i, tidReg[1]);
					}
					else if(j==2)
					{
						slvTxRpDes((devPointer->Feature.data3), (devPointer->Feature.datalen[2]), &i, tidReg[2]);
					}
					else if(j==3)
					{
						slvTxRpDes((devPointer->Feature.data4), (devPointer->Feature.datalen[3]), &i, tidReg[3]);
					}
					else if(j==4)
					{
						slvTxRpDes((devPointer->Feature.data5), (devPointer->Feature.datalen[4]), &i, tidReg[4]);
					}
					else if(j==5)
					{
						slvTxRpDes((devPointer->Feature.data6), (devPointer->Feature.datalen[5]), &i, tidReg[5]);
					}
					else if(j==6)
					{
						slvTxRpDes((devPointer->Feature.data7), (devPointer->Feature.datalen[6]), &i, tidReg[6]);
					}
					else if(j==7)
					{
						slvTxRpDes((devPointer->Feature.data8), (devPointer->Feature.datalen[7]), &i, tidReg[7]);
					}
					else if(j==8)
					{
						slvTxRpDes((devPointer->Feature.data9), (devPointer->Feature.datalen[8]), &i, tidReg[8]);
					}
					else if(j==9)
					{
						slvTxRpDes((devPointer->Feature.data10), (devPointer->Feature.datalen[9]), &i, tidReg[9]);
					}
				}
			}
		}
		else if(k==2)																																	//Input type
		{
			if(devPointer->Input.dataNum != 0)
			{
				I2CMS_SlvTxRPDesData[i++] = TID_CHANGE_STATE_FLAG;												//Change type sign
				I2CMS_SlvTxRPDesData[i++] = TID_INPUT_FLAG;
				for(j=0; j < devPointer->Input.dataNum; j++)																				//import data form
				{
					if(j==0)
					{
						slvTxRpDes((devPointer->Input.data1), (devPointer->Input.datalen[0]), &i, tidReg[0]);
					}
					else if(j==1)
					{
						slvTxRpDes((devPointer->Input.data2), (devPointer->Input.datalen[1]), &i, tidReg[1]);
					}
					else if(j==2)
					{
						slvTxRpDes((devPointer->Input.data3), (devPointer->Input.datalen[2]), &i, tidReg[2]);
					}
					else if(j==3)
					{
						slvTxRpDes((devPointer->Input.data4), (devPointer->Input.datalen[3]), &i, tidReg[3]);
					}
					else if(j==4)
					{
						slvTxRpDes((devPointer->Input.data5), (devPointer->Input.datalen[4]), &i, tidReg[4]);
					}
				}
			}
		}
		else if(k==3)																																	//Output type
		{
			if(devPointer->Output.dataNum != 0)
			{
				I2CMS_SlvTxRPDesData[i++] = TID_CHANGE_STATE_FLAG;												//Change type sign
				I2CMS_SlvTxRPDesData[i++] = TID_OUTPUT_FLAG;
				for(j=0; j<devPointer->Output.dataNum; j++)																				//import data form
				{
					if(j==0)
					{
						slvTxRpDes((devPointer->Output.data1), (devPointer->Output.datalen[0]), &i, tidReg[0]);
					}
					else if(j==1)
					{
						slvTxRpDes((devPointer->Output.data2), (devPointer->Output.datalen[1]), &i, tidReg[1]);
					}
					else if(j==2)
					{
						slvTxRpDes((devPointer->Output.data3), (devPointer->Output.datalen[2]), &i, tidReg[2]);
					}
					else if(j==3)
					{
						slvTxRpDes((devPointer->Output.data4), (devPointer->Output.datalen[3]), &i, tidReg[3]);
					}
					else if(j==4)
					{
						slvTxRpDes((devPointer->Output.data5), (devPointer->Output.datalen[4]), &i, tidReg[4]);
					}
				}
			}
		}
	}
	I2CSlv_InitFlag = 1;
}

void SlvDataInit()
{
	if (devNum >= 1 && devNum < MAX_TID_DEV)
	{
		if (pTidList[devNum])
			SlvDevInit(pTidList[devNum]);
	}

}

// **************************************************************
//						Slvae Store data to TID REG
// **************************************************************
void SlvDevDataStore(TID_Device* devPointer)
{
	uint8_t i=0, j=0;
	/* Output */
	if(devPointer->Feature.dataNum != 0)																												//Feature
	{
		i=0;
		I2CMS_SlvTxGetFTData[i++] = (int8_t)devPointer->DevDesc.GetFeatLeng;
		I2CMS_SlvTxGetFTData[i++] = (int8_t)(devPointer->DevDesc.GetFeatLeng>>8);
		for(j=0; j < devPointer->Feature.dataNum; j++)																							//import data
		{
			if(j==0)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data1.value, devPointer->Feature.datalen[0],&i);
			}
			if(j==1)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data2.value, devPointer->Feature.datalen[1],&i);
			}
			if(j==2)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data3.value, devPointer->Feature.datalen[2],&i);
			}
			if(j==3)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data4.value, devPointer->Feature.datalen[3],&i);
			}
			if(j==4)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data5.value, devPointer->Feature.datalen[4],&i);
			}
			if(j==5)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data6.value, devPointer->Feature.datalen[5],&i);
			}
			if(j==6)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data7.value, devPointer->Feature.datalen[6],&i);
			}
			if(j==7)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data8.value, devPointer->Feature.datalen[7],&i);
			}
			if(j==8)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data9.value, devPointer->Feature.datalen[8],&i);
			}
			if(j==9)
			{
				StoreTIDTxData(&I2CMS_SlvTxGetFTData[i], devPointer->Feature.data10.value, devPointer->Feature.datalen[9],&i);
			}
		}
	}
	/* Input */
	if(devPointer->Input.dataNum != 0)
	{
		i=0;
		I2CMS_SlvTxInRPData[i++] = (int8_t)devPointer->DevDesc.InRptLeng;
		I2CMS_SlvTxInRPData[i++] = (int8_t)(devPointer->DevDesc.InRptLeng>>8);
		for(j=0; j<devPointer->Input.dataNum; j++)																							//import data
		{
			if(j==0)
			{
				StoreTIDTxData(&I2CMS_SlvTxInRPData[i], devPointer->Input.data1.value, devPointer->Input.datalen[0],&i);
			}
			if(j==1)
			{
				StoreTIDTxData(&I2CMS_SlvTxInRPData[i], devPointer->Input.data2.value, devPointer->Input.datalen[1],&i);
			}
			if(j==2)
			{
				StoreTIDTxData(&I2CMS_SlvTxInRPData[i], devPointer->Input.data3.value, devPointer->Input.datalen[2],&i);
			}
			if(j==3)
			{
				StoreTIDTxData(&I2CMS_SlvTxInRPData[i], devPointer->Input.data4.value, devPointer->Input.datalen[3],&i);
			}
			if(j==4)
			{
				StoreTIDTxData(&I2CMS_SlvTxInRPData[i], devPointer->Input.data5.value, devPointer->Input.datalen[4],&i);
			}
		}
	}
}

void SlvDataStore()
{
#if 1
	SlvDevDataStore(pTidList[devNum]);
#else	
	if(devNum == 1)
		SlvDevDataStore(&BuzDev);
	else if(devNum == 2)
		SlvDevDataStore(&LedDev);
	else if(devNum == 3)
		SlvDevDataStore(&AHRSDev);
	else if(devNum == 4)
		SlvDevDataStore(&SonDev);
	else if(devNum == 5)
		SlvDevDataStore(&TempDev);
	else if(devNum == 6)
		SlvDevDataStore(&GasDev);
	else if(devNum == 7)
		SlvDevDataStore(&IRDev);
	else if(devNum == 8)
		SlvDevDataStore(&KeyDev);
	else if(devNum == 9)
		SlvDevDataStore(&ResDev9);
	else if(devNum == 10)
		SlvDevDataStore(&ResDev10);
	else if(devNum == 11)
		SlvDevDataStore(&ResDev11);
	else if(devNum == 12)
		SlvDevDataStore(&ResDev12);
	else if(devNum == 13)
		SlvDevDataStore(&ResDev13);
	else if(devNum == 14)
		SlvDevDataStore(&ResDev14);
#endif	
}

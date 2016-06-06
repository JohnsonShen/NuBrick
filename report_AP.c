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

#include "report_AP.h"

int8_t reportNewTabFlag;

// ========================================
//						Report Index Data
// ========================================

void report_index_binary()
{
	
#if 0
	// not test yet.
	uint16_t Report_state[18];

	if(reportNewTabFlag==0)
	{
		Report_state[0] = 0x8585;
		Report_state[1] = INDEX_REPORT_LENGTH;				//total length about Index
		Report_state[2] = DEVICE_NUMBER;						//how many device is prepared to be used
		Report_state[3] = TIDMstInitDevState;					//how many device is connected to be used
		Report_state[4] = INDEX_DATA_LENGTH;					// received data length on ondex

		Serial_write((char*)Report_state, INDEX_REPORT_LENGTH+2);

		reportNewTabFlag = 1;
	}
	else if(reportNewTabFlag==1)
	{
		Report_state[0] = INDEX_DATA_LENGTH;
		Report_state[1] = BatDev.Input.data1.value;			//Battery value
		Report_state[2] = BatDev.Input.data2.value;			//Battery over flag
		Report_state[3] = BuzDev.Input.data1.value;			//buzzer execute state
		Report_state[4] = LedDev.Input.data1.value;			//LED execute state
		Report_state[5] = AHRSDev.Input.data1.value;		//AHRS Vibration Value
		Report_state[6] = AHRSDev.Input.data2.value;		//AHRS Over flag
		Report_state[7] = SonDev.Input.data1.value;			//Sonar Value
		Report_state[8] = SonDev.Input.data2.value;			//Sonar Over flag
		Report_state[9] = TempDev.Input.data1.value;		//Temperature Value
		Report_state[10] = TempDev.Input.data2.value;		//Huminity Value
		Report_state[11] = TempDev.Input.data3.value;		//Temperature Over flag
		Report_state[12] = TempDev.Input.data4.value;		//Huminity Over flag
		Report_state[13] = GasDev.Input.data1.value;		//Gas Value
		Report_state[14] = GasDev.Input.data2.value;		//Gas Over flag
		Report_state[15] = IRDev.Input.data1.value;			//IR Receive data
		Report_state[16] = KeyDev.Input.data1.value;		//Key state
		Serial_write((char*)Report_state, INDEX_DATA_LENGTH);
	}
#else
	
	uint8_t Report_state[36];
	uint8_t i;

	if(reportNewTabFlag==0)
	{
		i=0;
		Report_state[i++] = 85;
		Report_state[i++] = 85;
		Report_state[i++] = INDEX_REPORT_LENGTH;				//total length about Index
		Report_state[i++] = INDEX_REPORT_LENGTH >> 8;
		Report_state[i++] = DEVICE_NUMBER;						//how many device is prepared to be used
		Report_state[i++] = DEVICE_NUMBER >> 8;
		Report_state[i++] = TIDMstInitDevState;					//how many device is connected to be used
		Report_state[i++] = TIDMstInitDevState >> 8;
		Report_state[i++] = INDEX_DATA_LENGTH;					// received data length on ondex
		Report_state[i++] = INDEX_DATA_LENGTH >> 8;
		Serial_write((char*)Report_state, INDEX_REPORT_LENGTH+2);

		reportNewTabFlag = 1;
	}
	else if(reportNewTabFlag==1)
	{
		i=0;
		Report_state[i++] = INDEX_DATA_LENGTH;
		Report_state[i++] = 0;
		Report_state[i++] = BatDev.Input.data1.value;			//Battery value
		Report_state[i++] = BatDev.Input.data1.value>>8;
		Report_state[i++] = BatDev.Input.data2.value;			//Battery over flag
		Report_state[i++] = BatDev.Input.data2.value>>8;
		Report_state[i++] = BuzDev.Input.data1.value;			//buzzer execute state
		Report_state[i++] = BuzDev.Input.data1.value>>8;
		Report_state[i++] = LedDev.Input.data1.value;			//LED execute state
		Report_state[i++] = LedDev.Input.data1.value>>8;
		Report_state[i++] = AHRSDev.Input.data1.value;			//AHRS Vibration Value
		Report_state[i++] = AHRSDev.Input.data1.value>>8;
		Report_state[i++] = AHRSDev.Input.data2.value;			//AHRS Over flag
		Report_state[i++] = AHRSDev.Input.data2.value>>8;
		Report_state[i++] = SonDev.Input.data1.value;			//Sonar Value
		Report_state[i++] = SonDev.Input.data1.value>>8;
		Report_state[i++] = SonDev.Input.data2.value;			//Sonar Over flag
		Report_state[i++] = SonDev.Input.data2.value>>8;
		Report_state[i++] = TempDev.Input.data1.value;			//Temperature Value
		Report_state[i++] = TempDev.Input.data1.value>>8;
		Report_state[i++] = TempDev.Input.data2.value;			//Huminity Value
		Report_state[i++] = TempDev.Input.data2.value>>8;
		Report_state[i++] = TempDev.Input.data3.value;			//Temperature Over flag
		Report_state[i++] = TempDev.Input.data3.value>>8;
		Report_state[i++] = TempDev.Input.data4.value;			//Huminity Over flag
		Report_state[i++] = TempDev.Input.data4.value>>8;
		Report_state[i++] = GasDev.Input.data1.value;			//Gas Value
		Report_state[i++] = GasDev.Input.data1.value>>8;
		Report_state[i++] = GasDev.Input.data2.value;			//Gas Over flag
		Report_state[i++] = GasDev.Input.data2.value>>8;
		Report_state[i++] = IRDev.Input.data1.value;			//IR Receive data
		Report_state[i++] = IRDev.Input.data1.value>>8;
		Report_state[i++] = KeyDev.Input.data1.value;			//Key state
		Report_state[i++] = KeyDev.Input.data1.value>>8;
		Serial_write((char*)Report_state, INDEX_DATA_LENGTH);
	}
#endif	
}

void report_devLink_binary()
{
	__align(2) uint8_t Report_state[24];
	
#if 0
	// not test yet.
	if(reportNewTabFlag==0)
	{
		*(uint16_t *)(&(Report_state[0])) = 0x8585;
		//Report_state[1] = 85;
		*(uint16_t *)(&(Report_state[2])) = DEVICELINK_REPORT_LENGTH;			//total length about Index
		*(uint16_t *)(&(Report_state[4])) = DEVICE_NUMBER;						//how many device is prepared to be used
		*(uint16_t *)(&(Report_state[6])) = TIDMstInitDevState;					//how many device is connected to be used
		*(uint16_t *)(&(Report_state[8])) = DEVICELINK_DATA_LENGTH;				// received data length on ondex
		Serial_write((char*)Report_state, DEVICELINK_REPORT_LENGTH+2);

		reportNewTabFlag = 1;
	}
	else if(reportNewTabFlag==1)
	{
		uint16_t devLinkState[2] = {0};

		//devLinkState[0] = 0;
		if(TIDMstInitDevState & (1<<BUZZERDEV))
		{
			devLinkState[0] = BuzDev.dTod.dZERO;
			devLinkState[0] |= BuzDev.dTod.dONE<<1;
			devLinkState[0] |= BuzDev.dTod.dTWO<<2;
			devLinkState[0] |= BuzDev.dTod.dTHR<<3;
			devLinkState[0] |= BuzDev.dTod.dFOUR<<4;
			devLinkState[0] |= BuzDev.dTod.dFIVE<<5;
			devLinkState[0] |= BuzDev.dTod.dSIX<<6;
			devLinkState[0] |= BuzDev.dTod.dSEV<<7;
			devLinkState[0] |= BuzDev.dTod.dEIG<<8;
			devLinkState[0] |= BuzDev.dTod.dNINE<<9;
			devLinkState[0] |= BuzDev.dTod.dX<<10;
			devLinkState[0] |= BuzDev.dTod.dXI<<11;
			devLinkState[0] |= BuzDev.dTod.dXII<<12;
			devLinkState[0] |= BuzDev.dTod.dXIII<<13;
			devLinkState[0] |= BuzDev.dTod.dXIV<<14;
		}

		//devLinkState[1] = 0;
		if(TIDMstInitDevState & (1<<LEDDEV))
		{
			devLinkState[1] = LedDev.dTod.dZERO;
			devLinkState[1] |= LedDev.dTod.dONE<<1;
			devLinkState[1] |= LedDev.dTod.dTWO<<2;
			devLinkState[1] |= LedDev.dTod.dTHR<<3;
			devLinkState[1] |= LedDev.dTod.dFOUR<<4;
			devLinkState[1] |= LedDev.dTod.dFIVE<<5;
			devLinkState[1] |= LedDev.dTod.dSIX<<6;
			devLinkState[1] |= LedDev.dTod.dSEV<<7;
			devLinkState[1] |= LedDev.dTod.dEIG<<8;
			devLinkState[1] |= LedDev.dTod.dNINE<<9;
			devLinkState[1] |= LedDev.dTod.dX<<10;
			devLinkState[1] |= LedDev.dTod.dXI<<11;
			devLinkState[1] |= LedDev.dTod.dXII<<12;
			devLinkState[1] |= LedDev.dTod.dXIII<<13;
			devLinkState[1] |= LedDev.dTod.dXIV<<14;
		}

		*(uint16_t *)(&(Report_state[0])) = DEVICELINK_DATA_LENGTH;
		//Report_state[1] = 0;
		*(uint16_t *)(&(Report_state[2])) = devLinkState[0];			//Vibration Sensor
		//Report_state[3] = devLinkState[0]>>8;
		*(uint16_t *)(&(Report_state[4])) = devLinkState[1];			//Over flag
		//Report_state[5] = devLinkState[1]>>8;
		Serial_write((char*)Report_state, DEVICELINK_DATA_LENGTH);
	}

#else
	uint8_t i;

	if(reportNewTabFlag==0)
	{
		i=0;
		Report_state[i++] = 85;
		Report_state[i++] = 85;
		Report_state[i++] = DEVICELINK_REPORT_LENGTH;			//total length about Index
		Report_state[i++] = DEVICELINK_REPORT_LENGTH >> 8;
		Report_state[i++] = DEVICE_NUMBER;						//how many device is prepared to be used
		Report_state[i++] = DEVICE_NUMBER >> 8;
		Report_state[i++] = TIDMstInitDevState;					//how many device is connected to be used
		Report_state[i++] = TIDMstInitDevState >> 8;
		Report_state[i++] = DEVICELINK_DATA_LENGTH;				// received data length on ondex
		Report_state[i++] = DEVICELINK_DATA_LENGTH >> 8;
		Serial_write((char*)Report_state, DEVICELINK_REPORT_LENGTH+2);

		reportNewTabFlag = 1;
	}
	else if(reportNewTabFlag==1)
	{
		uint16_t devLinkState[2] = {0};

		i=0;

		//devLinkState[0] = 0;
		if(TIDMstInitDevState & (1<<BUZZERDEV))
		{
			devLinkState[0] = BuzDev.dTod.dZERO;
			devLinkState[0] |= BuzDev.dTod.dONE<<1;
			devLinkState[0] |= BuzDev.dTod.dTWO<<2;
			devLinkState[0] |= BuzDev.dTod.dTHR<<3;
			devLinkState[0] |= BuzDev.dTod.dFOUR<<4;
			devLinkState[0] |= BuzDev.dTod.dFIVE<<5;
			devLinkState[0] |= BuzDev.dTod.dSIX<<6;
			devLinkState[0] |= BuzDev.dTod.dSEV<<7;
			devLinkState[0] |= BuzDev.dTod.dEIG<<8;
			devLinkState[0] |= BuzDev.dTod.dNINE<<9;
			devLinkState[0] |= BuzDev.dTod.dX<<10;
			devLinkState[0] |= BuzDev.dTod.dXI<<11;
			devLinkState[0] |= BuzDev.dTod.dXII<<12;
			devLinkState[0] |= BuzDev.dTod.dXIII<<13;
			devLinkState[0] |= BuzDev.dTod.dXIV<<14;
		}

		//devLinkState[1] = 0;
		if(TIDMstInitDevState & (1<<LEDDEV))
		{
			devLinkState[1] = LedDev.dTod.dZERO;
			devLinkState[1] |= LedDev.dTod.dONE<<1;
			devLinkState[1] |= LedDev.dTod.dTWO<<2;
			devLinkState[1] |= LedDev.dTod.dTHR<<3;
			devLinkState[1] |= LedDev.dTod.dFOUR<<4;
			devLinkState[1] |= LedDev.dTod.dFIVE<<5;
			devLinkState[1] |= LedDev.dTod.dSIX<<6;
			devLinkState[1] |= LedDev.dTod.dSEV<<7;
			devLinkState[1] |= LedDev.dTod.dEIG<<8;
			devLinkState[1] |= LedDev.dTod.dNINE<<9;
			devLinkState[1] |= LedDev.dTod.dX<<10;
			devLinkState[1] |= LedDev.dTod.dXI<<11;
			devLinkState[1] |= LedDev.dTod.dXII<<12;
			devLinkState[1] |= LedDev.dTod.dXIII<<13;
			devLinkState[1] |= LedDev.dTod.dXIV<<14;
		}

		Report_state[i++] = DEVICELINK_DATA_LENGTH;
		Report_state[i++] = 0;
		Report_state[i++] = devLinkState[0];			//Vibration Sensor
		Report_state[i++] = devLinkState[0]>>8;
		Report_state[i++] = devLinkState[1];			//Over flag
		Report_state[i++] = devLinkState[1]>>8;
		Serial_write((char*)Report_state, DEVICELINK_DATA_LENGTH);
	}
#endif	
}

// ====== subfunction for handle transfer descriptor ======
void HandRepDesc(uint8_t* storeData, uint16_t min, uint16_t max, uint8_t DataLen, uint8_t* position)
{
	switch(DataLen)
	{
	case 1:
	{
		*storeData = 1;
		*(storeData+1) = 9;
		*(storeData+2) = min;
		*(storeData+3) = 13;
		*(storeData+4) = max;
		*position += 5;
		break;
	}
	case 2:
	{
		*storeData = 2;
		*(storeData+1) = 10;
		*(storeData+2) = min;
		*(storeData+3) = min>>8;
		*(storeData+4) = 14;
		*(storeData+5) = max;
		*(storeData+6) = max>>8;
		*position += 7;
		break;
	}
	}
}
//	============= subfunction for handle transfer Data Max/Min ===========
void HandRepData(uint8_t* storeData, uint16_t data, uint8_t DataLen, uint8_t* position)
{
	switch(DataLen)
	{
	case 1:
	{
		*storeData = data;
		*position += 1;
		break;
	}
	case 2:
	{
		*storeData = data;
		*(storeData+1) = data>>8;
		*position += 2;
		break;
	}
	}
}

// ========================================
//				Report AP Data
// ========================================
void report_binary(TID_Device devName)
{
	uint8_t Report_state[80];
	uint8_t i=0, j=0;

	/* Report device descript */
	if(reportNewTabFlag==0)
	{
		Report_state[0] = 85;
		Report_state[1] = 85;
		Report_state[2] = devName.DevDesc.DevDesc_leng;
		Report_state[3] = devName.DevDesc.DevDesc_leng>>8;
		Report_state[4] = devName.DevDesc.RptDesc_leng;
		Report_state[5] = devName.DevDesc.RptDesc_leng>>8;
		Report_state[6] = devName.DevDesc.InRptLeng;
		Report_state[7] = devName.DevDesc.InRptLeng>>8;
		Report_state[8] = devName.DevDesc.OutRptLeng;
		Report_state[9] = devName.DevDesc.OutRptLeng>>8;
		Report_state[10] = devName.DevDesc.GetFeatLeng;
		Report_state[11] = devName.DevDesc.GetFeatLeng>>8;
		Report_state[12] = devName.DevDesc.SetFeatLeng;
		Report_state[13] = devName.DevDesc.SetFeatLeng>>8;
		Report_state[14] = devName.DevDesc.CID;
		Report_state[15] = devName.DevDesc.CID>>8;
		Report_state[16] = devName.DevDesc.DID;
		Report_state[17] = devName.DevDesc.DID>>8;
		Report_state[18] = devName.DevDesc.PID;
		Report_state[19] = devName.DevDesc.PID>>8;
		Report_state[20] = devName.DevDesc.UID;
		Report_state[21] = devName.DevDesc.UID>>8;
		Report_state[22] = devName.DevDesc.UCID;
		Report_state[23] = devName.DevDesc.UCID>>8;
		Report_state[24] = 0;
		Report_state[25] = 0;
		Report_state[26] = 0;
		Report_state[27] = 0;
		Serial_write((char*)Report_state, devName.DevDesc.DevDesc_leng+2);

		reportNewTabFlag = 1;
	}
	/* Report data form*/
	else if(reportNewTabFlag==1)
	{
		uint16_t min = 0, max = 0;
		
		j=0;
		Report_state[j++] = devName.DevDesc.RptDesc_leng;
		Report_state[j++] = devName.DevDesc.RptDesc_leng>>8;
		/* Feature */
		if(devName.Feature.arg[0] != 0)
		{
			Report_state[j++] = 1;
			Report_state[j++] = 1;
			for(i=0; i<10; i++)
			{
				switch(devName.Feature.arg[i])
				{
				case 1:
				{
					Report_state[j++] = TID_REG_ONE;										//Feature data1
					min = devName.Feature.data1.minimum;
					max = devName.Feature.data1.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data1.minimum,devName.Feature.data1.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 2:
				{
					Report_state[j++] = TID_REG_TWO;										//Feature data2
					min = devName.Feature.data2.minimum;
					max = devName.Feature.data2.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data2.minimum,devName.Feature.data2.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 3:
				{
					Report_state[j++] = TID_REG_THR;										//Feature data3
					min = devName.Feature.data3.minimum;
					max = devName.Feature.data3.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data3.minimum,devName.Feature.data3.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 4:
				{
					Report_state[j++] = TID_REG_FOUR;										//Feature data4
					min = devName.Feature.data4.minimum;
					max = devName.Feature.data4.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data4.minimum,devName.Feature.data4.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 5:
				{
					Report_state[j++] = TID_REG_FIVE;										//Feature data5
					min = devName.Feature.data5.minimum;
					max = devName.Feature.data5.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data5.minimum,devName.Feature.data5.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 6:
				{
					Report_state[j++] = TID_REG_SIX;										//Feature data6
					min = devName.Feature.data6.minimum;
					max = devName.Feature.data6.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data6.minimum,devName.Feature.data6.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 7:
				{
					Report_state[j++] = TID_REG_SEV;										//Feature data7
					min = devName.Feature.data7.minimum;
					max = devName.Feature.data7.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data7.minimum,devName.Feature.data7.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 8:
				{
					Report_state[j++] = TID_REG_EIG;										//Feature data8
					min = devName.Feature.data8.minimum;
					max = devName.Feature.data8.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data8.minimum,devName.Feature.data8.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 9:
				{
					Report_state[j++] = TID_REG_NINE;										//Feature data9
					min = devName.Feature.data9.minimum;
					max = devName.Feature.data9.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data9.minimum,devName.Feature.data9.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				case 10:
				{
					Report_state[j++] = TID_REG_TEN;										//Feature data10
					min = devName.Feature.data10.minimum;
					max = devName.Feature.data10.maximum;
					//HandRepDesc(&Report_state[j],devName.Feature.data10.minimum,devName.Feature.data10.maximum,devName.Feature.datalen[i],&j);
					break;
				}
				}
				
				HandRepDesc(&Report_state[j], min, max, devName.Feature.datalen[i], &j);
			}
		}
		if(devName.Input.arg[0] != 0)
		{
			/* Input */
			Report_state[j++] = 1;
			Report_state[j++] = 2;
			for(i=0; i<5; i++)
			{
				switch(devName.Input.arg[i])
				{
				case 1:
				{
					Report_state[j++] = TID_REG_ONE;										//Input data1
					min = devName.Input.data1.minimum;
					max = devName.Input.data1.maximum;
					//HandRepDesc(&Report_state[j],devName.Input.data1.minimum,devName.Input.data1.maximum,devName.Input.datalen[i],&j);
					break;
				}
				case 2:
				{
					Report_state[j++] = TID_REG_TWO;										//Input data2
					min = devName.Input.data2.minimum;
					max = devName.Input.data2.maximum;
					//HandRepDesc(&Report_state[j],devName.Input.data2.minimum,devName.Input.data2.maximum,devName.Input.datalen[i],&j);
					break;
				}
				case 3:
				{
					Report_state[j++] = TID_REG_THR;										//Input data3
					min = devName.Input.data3.minimum;
					max = devName.Input.data3.maximum;
					//HandRepDesc(&Report_state[j],devName.Input.data3.minimum,devName.Input.data3.maximum,devName.Input.datalen[i],&j);
					break;
				}
				case 4:
				{
					Report_state[j++] = TID_REG_FOUR;										//Input data4
					min = devName.Input.data4.minimum;
					max = devName.Input.data4.maximum;
					//HandRepDesc(&Report_state[j],devName.Input.data4.minimum,devName.Input.data4.maximum,devName.Input.datalen[i],&j);
					break;
				}
				case 5:
				{
					Report_state[j++] = TID_REG_FIVE;										//Input data5
					min = devName.Input.data5.minimum;
					max = devName.Input.data5.maximum;
					//HandRepDesc(&Report_state[j],devName.Input.data5.minimum,devName.Input.data5.maximum,devName.Input.datalen[i],&j);
					break;
				}
				}
				
				HandRepDesc(&Report_state[j], min, max, devName.Input.datalen[i], &j);
			}
		}
		if(devName.Output.arg[0] != 0)
		{
			/* Output */
			Report_state[j++] = 1;
			Report_state[j++] = 3;
			for(i=0; i<5; i++)
			{
				switch(devName.Output.arg[i])
				{
				case 1:
				{
					Report_state[j++] = TID_REG_ONE;										//Output data1
					min = devName.Output.data1.minimum;
					max = devName.Output.data1.maximum;					
					//HandRepDesc(&Report_state[j],devName.Output.data1.minimum,devName.Output.data1.maximum,devName.Output.datalen[i],&j);
					break;
				}
				case 2:
				{
					Report_state[j++] = TID_REG_TWO;										//Output data2
					min = devName.Output.data2.minimum;
					max = devName.Output.data2.maximum;					
					//HandRepDesc(&Report_state[j],devName.Output.data2.minimum,devName.Output.data2.maximum,devName.Output.datalen[i],&j);
					break;
				}
				case 3:
				{
					Report_state[j++] = TID_REG_THR;										//Output data3
					min = devName.Output.data3.minimum;
					max = devName.Output.data3.maximum;					
					//HandRepDesc(&Report_state[j],devName.Output.data3.minimum,devName.Output.data3.maximum,devName.Output.datalen[i],&j);
					break;
				}
				case 4:
				{
					Report_state[j++] = TID_REG_FOUR;										//Output data4
					min = devName.Output.data4.minimum;
					max = devName.Output.data4.maximum;					
					//HandRepDesc(&Report_state[j],devName.Output.data4.minimum,devName.Output.data4.maximum,devName.Output.datalen[i],&j);
					break;
				}
				case 5:
				{
					Report_state[j++] = TID_REG_FIVE;										//Output data5
					min = devName.Output.data5.minimum;
					max = devName.Output.data5.maximum;					
					//HandRepDesc(&Report_state[j],devName.Output.data5.minimum,devName.Output.data5.maximum,devName.Output.datalen[i],&j);
					break;
				}
				}
				
				HandRepDesc(&Report_state[j], min, max, devName.Output.datalen[i], &j);
			}
		}
		Serial_write((char*)Report_state, j);

		reportNewTabFlag=2;
	}
	/* Report data */
	else if(reportNewTabFlag==2)
	{
		j=0;
		Report_state[j++]= devName.DevDesc.InRptLeng +devName.DevDesc.OutRptLeng +devName.DevDesc.GetFeatLeng;
		Report_state[j++]= 0;
		/* Feature */
		if(devName.Feature.arg[0] != 0)
		{
			for(i=0; i<10; i++)
			{
				switch(devName.Feature.arg[i])
				{
				case 1:													//Feature data1
				{
					HandRepData(&Report_state[j],devName.Feature.data1.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 2:													//Feature data2
				{
					HandRepData(&Report_state[j],devName.Feature.data2.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 3:													//Feature data3
				{
					HandRepData(&Report_state[j],devName.Feature.data3.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 4:													//Feature data4
				{
					HandRepData(&Report_state[j],devName.Feature.data4.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 5:													//Feature data5
				{
					HandRepData(&Report_state[j],devName.Feature.data5.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 6:													//Feature data6
				{
					HandRepData(&Report_state[j],devName.Feature.data6.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 7:													//Feature data7
				{
					HandRepData(&Report_state[j],devName.Feature.data7.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 8:													//Feature data8
				{
					HandRepData(&Report_state[j],devName.Feature.data8.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 9:													//Feature data9
				{
					HandRepData(&Report_state[j],devName.Feature.data9.value,devName.Feature.datalen[i],&j);
					break;
				}
				case 10:													//Feature data10
				{
					HandRepData(&Report_state[j],devName.Feature.data10.value,devName.Feature.datalen[i],&j);
					break;
				}
				}
			}
		}
		/* Input */
		if(devName.Input.arg[0] != 0)
		{
			for(i=0; i<5; i++)
			{
				switch(devName.Input.arg[i])
				{
				case 1:													//Input data1
				{
					HandRepData(&Report_state[j],devName.Input.data1.value,devName.Input.datalen[i],&j);
					break;
				}
				case 2:													//Input data2
				{
					HandRepData(&Report_state[j],devName.Input.data2.value,devName.Input.datalen[i],&j);
					break;
				}
				case 3:													//Input data3
				{
					HandRepData(&Report_state[j],devName.Input.data3.value,devName.Input.datalen[i],&j);
					break;
				}
				case 4:													//Input data4
				{
					HandRepData(&Report_state[j],devName.Input.data4.value,devName.Input.datalen[i],&j);
					break;
				}
				case 5:													//Input data5
				{
					HandRepData(&Report_state[j],devName.Input.data5.value,devName.Input.datalen[i],&j);
					break;
				}
				}
			}
		}
		/* Output */
		if(devName.Output.arg[0] != 0)
		{
			for(i=0; i<5; i++)
			{
				switch(devName.Output.arg[i])
				{
				case 1:													//Output data1
				{
					HandRepData(&Report_state[j],devName.Output.data1.value,devName.Output.datalen[i],&j);
					break;
				}
				case 2:													//Output data2
				{
					HandRepData(&Report_state[j],devName.Output.data2.value,devName.Output.datalen[i],&j);
					break;
				}
				case 3:													//Output data3
				{
					HandRepData(&Report_state[j],devName.Output.data3.value,devName.Output.datalen[i],&j);
					break;
				}
				case 4:													//Output data4
				{
					HandRepData(&Report_state[j],devName.Output.data4.value,devName.Output.datalen[i],&j);
					break;
				}
				case 5:													//Output data5
				{
					HandRepData(&Report_state[j],devName.Output.data5.value,devName.Output.datalen[i],&j);
					break;
				}
				}
			}
		}
//		Report_state[j++] = devName.dTod.dZERO;
//		Report_state[j++] = devName.dTod.dONE;
//		Report_state[j++] = devName.dTod.dTWO;
//		Report_state[j++] = devName.dTod.dTHR;
//		Report_state[j++] = devName.dTod.dFOUR;
//		Report_state[j++] = devName.dTod.dFIVE;
//		Report_state[j++] = devName.dTod.dSIX;
//		Report_state[j++] = devName.dTod.dSEV;
		Serial_write((char*)Report_state, j);
	}
}


// ****************************************
//						Set parameter
// ****************************************
int GetUARTValue_Binary()
{
	char value_s[4]= {0};
	int8_t i=0;
	int dataOut=0;

	while(value_s[i-1] != '\r')
	{
		value_s[i] = GetChar();
		i++;
	}
	sscanf(value_s,"%d",&dataOut);

	return dataOut;
}

// ======================================
// 				set value to parameter
// ======================================
void getPara_feature_binary(TID_Device* devPointer, uint8_t devNum)
{
	char para;
	int paraNum;
	para = getchar();
	sscanf(&para, "%d", &paraNum);
	if(para == '1')
	{
		devPointer->Feature.data1.value = GetUARTValue_Binary();
		reportAPFlag = 1;
		if(devPointer->Feature.arg[1]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '2')
	{
		devPointer->Feature.data2.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[2]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '3')
	{
		devPointer->Feature.data3.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[3]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '4')
	{
		devPointer->Feature.data4.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[4]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '5')
	{
		devPointer->Feature.data5.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[5]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '6')
	{
		devPointer->Feature.data6.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[6]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '7')
	{
		devPointer->Feature.data7.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[7]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '8')
	{
		devPointer->Feature.data8.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[8]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == '9')
	{
		devPointer->Feature.data9.value = GetUARTValue_Binary();
		if(devPointer->Feature.arg[9]==0)
		{
			TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
			reportAPFlag = 0;
		}
	}
	else if (para == 'a')
	{
		devPointer->Feature.data10.value = GetUARTValue_Binary();
		TIDMstDevState[devNum] |= 8;					//device operate:				set feature(4)
		reportAPFlag = 0;
	}
}

void getPara_input_binary(TID_Device* devPointer, uint8_t devNum)
{
	char para;
	int paraNum;
	para = getchar();
	sscanf(&para, "%d", &paraNum);
	if(para == '1')
	{
		devPointer->Input.data1.value = GetUARTValue_Binary();
		reportAPFlag = 1;
		if(devPointer->Input.arg[1]==0)
		{
			//TIDMstDevState[devNum] |= 1;					//device operate:				set input(1)
			reportAPFlag = 0;
		}
	}
	else if (para == '2')
	{
		devPointer->Input.data2.value = GetUARTValue_Binary();
		if(devPointer->Input.arg[2]==0)
		{
			//TIDMstDevState[devNum] |= 1;					//device operate:				set input(1)
			reportAPFlag = 0;
		}
	}
	else if (para == '3')
	{
		devPointer->Input.data3.value = GetUARTValue_Binary();
		if(devPointer->Input.arg[3]==0)
		{
			//TIDMstDevState[devNum] |= 1;					//device operate:				set input(1)
			reportAPFlag = 0;
		}
	}
	else if (para == '4')
	{
		devPointer->Input.data4.value = GetUARTValue_Binary();
		if(devPointer->Input.arg[4]==0)
		{
			//TIDMstDevState[devNum] |= 1;					//device operate:				set input(1)
			reportAPFlag = 0;
		}
	}
	else if (para == '5')
	{
		devPointer->Input.data5.value = GetUARTValue_Binary();
		//TIDMstDevState[devNum] |= 1;					//device operate:				set input(1)
		reportAPFlag = 0;
	}
}

void getPara_output_binary(TID_Device* devPointer, uint8_t devNum)
{
	char para;
	int paraNum;
	para = getchar();
	sscanf(&para, "%d", &paraNum);
	if(para == '1')
	{
		devPointer->Output.data1.value = GetUARTValue_Binary();
		reportAPFlag = 1;
		if(devPointer->Output.arg[1]==0)
		{
			TIDMstDevState[devNum] |= 2;					//device operate:				set output(2)
			reportAPFlag = 0;
		}
	}
	else if (para == '2')
	{
		devPointer->Output.data2.value = GetUARTValue_Binary();
		if(devPointer->Output.arg[2]==0)
		{
			TIDMstDevState[devNum] |= 2;					//device operate:				set output(2)
			reportAPFlag = 0;
		}
	}
	else if (para == '3')
	{
		devPointer->Output.data3.value = GetUARTValue_Binary();
		if(devPointer->Output.arg[3]==0)
		{
			TIDMstDevState[devNum] |= 2;					//device operate:				set output(2)
			reportAPFlag = 0;
		}
	}
	else if (para == '4')
	{
		devPointer->Output.data4.value = GetUARTValue_Binary();
		if(devPointer->Output.arg[4]==0)
		{
			TIDMstDevState[devNum] |= 2;					//device operate:				set output(2)
			reportAPFlag = 0;
		}
	}
	else if (para == '5')
	{
		devPointer->Output.data5.value = GetUARTValue_Binary();
		{
			TIDMstDevState[devNum] |= 2;					//device operate:				set output(2)
			reportAPFlag = 0;
		}
	}
}
// ======================================
// 		set connect to other device
// ======================================
void getConData_binary(uint8_t* Para1)
{
	char para;
	int paraNum;
	para = getchar();
	sscanf(&para, "%d", &paraNum);

#if 1
	
	{
		int index;
		
		if (para >= '0' && para <= '9')
			index = para - '0';
		else if (para >= 'a' && para <= 'e')
			index = para - 'a' + 10;
		else
			return;
		
		*(Para1+index) = GetUARTValue_Binary();
	}
	
#else
	
	if(para == '0')
	{
		*Para1 = GetUARTValue_Binary();
	}
	else if (para == '1')
	{
		*(Para1+1) = GetUARTValue_Binary();
	}
	else if (para == '2')
	{
		*(Para1+2) = GetUARTValue_Binary();
	}
	else if (para == '3')
	{
		*(Para1+3) = GetUARTValue_Binary();
	}
	else if (para == '4')
	{
		*(Para1+4) = GetUARTValue_Binary();
	}
	else if (para == '5')
	{
		*(Para1+5) = GetUARTValue_Binary();
	}
	else if (para == '6')
	{
		*(Para1+6) = GetUARTValue_Binary();
	}
	else if (para == '7')
	{
		*(Para1+7) = GetUARTValue_Binary();
	}
	else if (para == '8')
	{
		*(Para1+8) = GetUARTValue_Binary();
	}
	else if (para == '9')
	{
		*(Para1+9) = GetUARTValue_Binary();
	}
	else if (para == 'a')
	{
		*(Para1+10) = GetUARTValue_Binary();
	}
	else if (para == 'b')
	{
		*(Para1+11) = GetUARTValue_Binary();
	}
	else if (para == 'c')
	{
		*(Para1+12) = GetUARTValue_Binary();
	}
	else if (para == 'd')
	{
		*(Para1+13) = GetUARTValue_Binary();
	}
	else if (para == 'e')
	{
		*(Para1+14) = GetUARTValue_Binary();
	}
#endif	
}

void SetPara_Binary()
{
	static 
	char device;
	char type;
	device = getchar();
	/* BATTERY */
	if(device=='b')
	{
		type = GetChar();
		if(type=='1')				//Feature
		{
			getPara_feature_binary(&BatDev, 0);
			if(TIDMstDevState[0] & 8)
			{
				UpdateFlashTID(BatDev.Feature, 0, false);
			}
			TIDMstDevState[0] = 0;
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&BatDev, 0);
			TIDMstDevState[0] = 0;
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&BatDev, 0);
			TIDMstDevState[0] = 0;
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&BatDev.dTod.dZERO);
		}
	}
	/* BUZZER */
	if(device=='z')
	{
		type = GetChar();
		if(type=='1')					//Feature
		{
			getPara_feature_binary(&BuzDev, BUZZERDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&BuzDev, BUZZERDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&BuzDev.dTod.dZERO);
		}
	}
	/* LED */
	else if(device=='l')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&LedDev, LEDDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&LedDev, LEDDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&LedDev.dTod.dZERO);
		}
	}
	/* AHRS */
	else if(device=='a')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&AHRSDev, AHRSDEV);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&AHRSDev, AHRSDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&AHRSDev, AHRSDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&AHRSDev.dTod.dZERO);
		}
	}
	/* SONAR */
	else if(device=='s')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&SonDev, SONARDEV);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&SonDev, SONARDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&SonDev, SONARDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&SonDev.dTod.dZERO);
		}
	}
	/* TEMPERATURE */
	else if(device=='t')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&TempDev, TEMPDEV);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&TempDev, TEMPDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&TempDev, TEMPDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&TempDev.dTod.dZERO);
		}
	}
	/* GAS */
	else if(device=='g')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&GasDev, GASDEV);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&GasDev, GASDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&GasDev, GASDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&GasDev.dTod.dZERO);
		}
	}
	/* IR */
	else if(device=='i')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&IRDev, IRDEV);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&IRDev, IRDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&IRDev, IRDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&IRDev.dTod.dZERO);
		}
	}
	/* KEY */
	else if(device=='k')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&KeyDev, KEYDEV);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&KeyDev, KEYDEV);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&KeyDev, KEYDEV);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&KeyDev.dTod.dZERO);
		}
	}
	/* RESERVED DEVICE 9 */
	else if(device=='A')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&ResDev9, RESDEV9);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&ResDev9, RESDEV9);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&ResDev9, RESDEV9);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&ResDev9.dTod.dZERO);
		}
	}
	/* RESERVED DEVICE 10 */
	else if(device=='B')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&ResDev10, RESDEV10);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&ResDev10, RESDEV10);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&ResDev10, RESDEV10);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&ResDev10.dTod.dZERO);
		}
	}
	/* RESERVED DEVICE 11 */
	else if(device=='C')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&ResDev11, RESDEV11);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&ResDev11, RESDEV11);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&ResDev11, RESDEV11);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&ResDev11.dTod.dZERO);
		}
	}
	/* RESERVED DEVICE 12 */
	else if(device=='D')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&ResDev12, RESDEV12);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&ResDev12, RESDEV12);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&ResDev12, RESDEV12);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&ResDev12.dTod.dZERO);
		}
	}
	/* RESERVED DEVICE 13 */
	else if(device=='E')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&ResDev13, RESDEV13);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&ResDev13, RESDEV13);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&ResDev13, RESDEV13);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&ResDev13.dTod.dZERO);
		}
	}
	/* RESERVED DEVICE 14 */
	else if(device=='F')
	{
		type = GetChar();
		if(type == '1')				//Feature
		{
			getPara_feature_binary(&ResDev14, RESDEV14);
		}
		else if(type=='2')			//Input
		{
			getPara_input_binary(&ResDev14, RESDEV14);
		}
		else if(type=='3')			//Output
		{
			getPara_output_binary(&ResDev14, RESDEV14);
		}
		else if(type=='4')			//Connect device
		{
			getConData_binary(&ResDev14.dTod.dZERO);
		}
	}
}

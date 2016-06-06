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
* HCHEISH@nuvoton.com                                         *
*=============================================================*
*/

#include "temp_HTU21D.h"

float Humi_HTU21D;
float Temp_HTU21D;
uint32_t HTU21DWaitFlag;


void HTU21D_Init(void)
{
	uint8_t tmp[1],temp[1];

	TempDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	TempDev.DevDesc.RptDesc_leng = 58;						//Report descriptor
	TempDev.DevDesc.InRptLeng = 8;							//Input report
	TempDev.DevDesc.OutRptLeng = 0;							//Output report
	TempDev.DevDesc.GetFeatLeng = 8;						//Get feature
	TempDev.DevDesc.SetFeatLeng = 8;						//Set feature
	TempDev.DevDesc.CID = 0;								//manufacturers ID
	TempDev.DevDesc.DID = 0;								//Product ID
	TempDev.DevDesc.PID = 0;								//Device firmware revision
	TempDev.DevDesc.UID = 0;								//Device Class type
	TempDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	TempDev.Feature.data1.minimum = 0;						//Sleep period
	TempDev.Feature.data1.maximum = 1024;
	TempDev.Feature.data1.value = 100;
	TempDev.Feature.data2.minimum = 0;						//temperature alerm value
	TempDev.Feature.data2.maximum = 100;
	TempDev.Feature.data2.value = 35;
	TempDev.Feature.data3.minimum = 0;						//humidity alerm value
	TempDev.Feature.data3.maximum = 100;
	TempDev.Feature.data3.value = 70;
	TempDev.Feature.arg[0] = 1;
	TempDev.Feature.arg[1] = 2;
	TempDev.Feature.arg[2] = 3;
	TempDev.Feature.datalen[0] = 2;
	TempDev.Feature.datalen[1] = 2;
	TempDev.Feature.datalen[2] = 2;
	TempDev.Feature.dataNum = 3;
	/* Input */
	TempDev.Input.data1.minimum = 0;						//temperature value
	TempDev.Input.data1.maximum = 100;
	TempDev.Input.data1.value = 25;
	TempDev.Input.data2.minimum = 0;						//humidity value
	TempDev.Input.data2.maximum = 100;
	TempDev.Input.data2.value = 50;
	TempDev.Input.data3.minimum = 0;						//temperature over flag
	TempDev.Input.data3.maximum = 1;
	TempDev.Input.data3.value = 0;
	TempDev.Input.data4.minimum = 0;						//humidity over flag
	TempDev.Input.data4.maximum = 1;
	TempDev.Input.data4.value = 0;
	TempDev.Input.arg[0] = 1;
	TempDev.Input.arg[1] = 2;
	TempDev.Input.arg[2] = 3;
	TempDev.Input.arg[3] = 4;
	TempDev.Input.datalen[0] = 2;
	TempDev.Input.datalen[1] = 2;
	TempDev.Input.datalen[2] = 1;
	TempDev.Input.datalen[3] = 1;
	TempDev.Input.dataNum = 4;
	/* Output */
	TempDev.Output.dataNum = 0;


	I2CTempTRxEndFlag = 1;
	I2C_Temp_Init();
	temp[0] = 0x03;
	I2CTemp_writeBytes(I2C1, HTU21D_I2C_SLA, HTU21D_Userregister, 1, temp);
	I2CTemp_readBytes(I2C1, HTU21D_I2C_SLA, HTU21D_Userregister, 1, tmp);
}

uint8_t HTU21D_ReadHumid(void)
{
	uint8_t tmp[1];
	I2CTemp_readBytes(I2C1, HTU21D_I2C_SLA, HTU21D_TriggerHumidHold, 1,tmp); // trigger Temperature Hold master
	return(tmp[0]);
}

uint16_t HTU21D_ReadTemp(void)
{
	uint8_t tmp[2];
	I2CTemp_readBytes(I2C1, HTU21D_I2C_SLA, HTU21D_TriggerTempHold, 2,tmp); // trigger Temperature Hold master
	return((tmp[0]<<4) + (tmp[1]>>4));
}

float HTU21D_Humidity(void)
{
	uint8_t sRH   = HTU21D_ReadHumid(); //8-bit
	float   Humid =  -6    + ((125 * sRH)/256);
	return (Humid);
}

float HTU21D_Temperature(void)
{
	uint16_t sTemp = HTU21D_ReadTemp();  //12-bit
	float    Temp  = -46.85 + ((175.72 * sTemp)/4096);
	HTU21DWaitFlag = 1;
	return(Temp);
}

void GetHTU21DTemp(void)
{
	if(HTU21DWaitFlag == 0)
	{
		TempDev.Input.data2.value = HTU21D_Humidity();
		TempDev.Input.data1.value = HTU21D_Temperature();
	}

	if(TempDev.Input.data2.value > TempDev.Feature.data3.value)
	{
		TempDev.Input.data4.value = 1;
	}
	else
	{
		TempDev.Input.data4.value = 0;
	}

	if(TempDev.Input.data1.value > TempDev.Feature.data2.value)
	{
		TempDev.Input.data3.value = 1;
	}
	else
	{
		TempDev.Input.data3.value = 0;
	}
}

void WaitHTU21D(void)
{
	if(HTU21DWaitFlag > 0)
		HTU21DWaitFlag ++;
	if(HTU21DWaitFlag > 10)
	{
		HTU21DWaitFlag = 0;
		I2C_Close(I2C1);
		/* Open I2C module and set bus clock */
		I2C_Open(I2C1, 100000);
	}
	else HTU21DWaitFlag++;

}

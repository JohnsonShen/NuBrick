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
 * HSHSIEH@nuvoton.com                                         *
 *=============================================================*
 */

#include "i2cdev.h"
#include "sensors.h"
#include "ahrs.h"

float Gyro[3];
float GyroValue;
int8_t VibrationOverFlag;
int32_t VibrationOverTimeCounter;

void AhrsParaInit()
{
	//LED
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk);
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_GPIO;
	GPIO_SetMode(PA,BIT2,GPIO_MODE_OUTPUT);
	PA2 = 1;

	AHRSDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	AHRSDev.DevDesc.RptDesc_leng = 34;						//Report descriptor
	AHRSDev.DevDesc.InRptLeng = 5;							//Input report
	AHRSDev.DevDesc.OutRptLeng = 0;							//Output report
	AHRSDev.DevDesc.GetFeatLeng = 5;						//Get feature
	AHRSDev.DevDesc.SetFeatLeng = 5;						//Set feature
	AHRSDev.DevDesc.CID = 0;								//manufacturers ID
	AHRSDev.DevDesc.DID = 0;								//Product ID
	AHRSDev.DevDesc.PID = 0;								//Device firmware revision
	AHRSDev.DevDesc.UID = 0;								//Device Class type
	AHRSDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	AHRSDev.Feature.data1.minimum = 0;						//Sleep period
	AHRSDev.Feature.data1.maximum = 1024;
	AHRSDev.Feature.data1.value = 100;
	AHRSDev.Feature.data2.minimum = 0;						//Vibration
	AHRSDev.Feature.data2.maximum = 10;
	AHRSDev.Feature.data2.value = 1;
	AHRSDev.Feature.arg[0] = 1;
	AHRSDev.Feature.arg[1] = 2;
	AHRSDev.Feature.datalen[0] = 2;
	AHRSDev.Feature.datalen[1] = 1;
	AHRSDev.Feature.dataNum = 2;
	/* Input */
	AHRSDev.Input.data1.minimum = 0;						//Vibration sensor
	AHRSDev.Input.data1.maximum = 720;
	AHRSDev.Input.data1.value = 0;
	AHRSDev.Input.data2.minimum = 0;						//Over flag
	AHRSDev.Input.data2.maximum = 1;
	AHRSDev.Input.data2.value = 0;
	AHRSDev.Input.arg[0] = 1;
	AHRSDev.Input.arg[1] = 2;
	AHRSDev.Input.datalen[0] = 2;
	AHRSDev.Input.datalen[1] = 1;
	AHRSDev.Input.dataNum = 2;
	/* Output */
	AHRSDev.Output.dataNum = 0;
}

// ===========================================================
//
//	Function:Read ahrs value and compare
//
// ===========================================================
void AhrsRead(uint8_t GYRO_Sentivity, uint16_t VibrationAlermTime, uint8_t VibrationClearFlag)
{
	int i;
	GyroValue=0;
	// Get x,y,z velocity
	//nvtGetEulerRPY(Euler);
	//nvtGetAccZWithoutGravity(&Ve[0],&Ve[1]);
	//nvtGetVelocity(Ve);
	//nvtGetMove(Move);
	//nvtGetCalibratedACC(Ve);
	// Get x,y,z Angeler velocity
	//nvtGetCalibratedGYRO(Gyro);
	nvtGetAccZWithoutGravity(&Gyro[0], &Gyro[1]);
	/* Update TID value */
	GyroValue = 0;
	for(i=0; i<3; i++)
	{
		if(GyroValue<Gyro[i])
			GyroValue=Gyro[i];
	}
	GyroValue = GyroValue*30;
	AHRSDev.Input.data1.value = GyroValue;
	//printf("Vibration=%f.\n", GyroValue);
	/* Alerm if angular is bigger than stander */
	if (GyroValue > (GYRO_PRESCALE*GYRO_Sentivity))
	{
		PA2 = 0;
		AHRSDev.Input.data2.value = 1;
		//VibrationOverTimeCounter = getTickCount()+ VibrationAlermTime*1000;
	}
	else
	{
		PA2 = 1;
		AHRSDev.Input.data2.value = 0;
	}

	/* reset alerm flag after 10s */
//	if(AHRSDev.Input.data2.value == 1)
//	{
//		if(getTickCount() > VibrationOverTimeCounter)
//			AHRSDev.Input.data2.value = 0;
//		if(VibrationClearFlag == 1)
//			AHRSDev.Input.data2.value = 0;
//	}

}

void AHRS_Init(void)
{
	I2C_Init();
	nvtAHRSInit();
	SensorsInit();
	AhrsParaInit();
}

void AHRS_Control(void)
{
	SensorsRead(SENSOR_ACC|SENSOR_GYRO,1);
	nvtUpdateAHRS(SENSOR_ACC|SENSOR_GYRO);
	AhrsRead(AHRSDev.Feature.data2.value, AHRSDev.Feature.data3.value, AHRSDev.Output.data1.value);				//(Vibration Level, alerm time, clear flag)
}

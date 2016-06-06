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

#ifndef TID_H_
#define TID_H_

#include <stdio.h>
#include "M451Series.h"
#include "devCheck.h"
#include "flash.h"
#include "I2C_ms.h"

#define MAX_TID_DEV         (15)
#define EMBEDDED_TID_DEV    (9)

/****************************************************************/
/*                    Sensor Type definitions                   */
/****************************************************************/
#define BATTERYDEV	0
#define BUZZERDEV		1
#define LEDDEV 			2
#define AHRSDEV			3
#define SONARDEV		4
#define TEMPDEV			5
#define GASDEV			6
#define IRDEV			7
#define KEYDEV			8
#define RESDEV9			9
#define RESDEV10		10
#define RESDEV11		11
#define RESDEV12		12
#define RESDEV13		13
#define RESDEV14		14

#define TID_CHANGE_STATE_FLAG 1
#define TID_FEATURE_FLAG 1
#define TID_INPUT_FLAG 2
#define TID_OUTPUT_FLAG 3
#define TID_REG_MIN 8
#define TID_REG_MAX 12
#define TID_REG_ONE 5
#define TID_REG_TWO 17
#define TID_REG_THR 29
#define TID_REG_FOUR 41
#define TID_REG_FIVE 53
#define TID_REG_SIX 65
#define TID_REG_SEV 77
#define TID_REG_EIG 89
#define TID_REG_NINE 101
#define TID_REG_TEN 113

//==============================================================
//==============================================================
//											Device descriptor
//==============================================================
//==============================================================
typedef struct
{
	uint16_t DevDesc_leng;				//Report descriptor length
	uint16_t RptDesc_leng;				//Report descriptor length
	uint16_t InRptLeng;					//input report length
	uint16_t OutRptLeng;					//output report length
	uint16_t GetFeatLeng;				//get feature length
	uint16_t SetFeatLeng;				//set feature length
	uint16_t CID;								//Company ID
	uint16_t DID;								//Device ID
	uint16_t PID;								//product ID
	uint16_t UID;								//product ID
	uint16_t UCID;							//product ID
	uint16_t reserve1;					//reserved
	uint16_t reserve2;					//reserved
} DEVICE_DESCRIPTOR;

typedef struct
{
	uint16_t value;
	uint16_t minimum;
	uint16_t maximum;
} TID_DATA;

typedef struct
{
	uint8_t dZERO;			//battery
	uint8_t dONE;			//Buzzer
	uint8_t dTWO;			//Led
	uint8_t dTHR;			//AHRS
	uint8_t dFOUR;			//SONAR
	uint8_t dFIVE;			//Temperature
	uint8_t dSIX;			//Gas
	uint8_t dSEV;			//IR
	uint8_t dEIG;			//KEY
	uint8_t dNINE;			//Reserved device 9
	uint8_t dX;			//Reserved device 10
	uint8_t dXI;			//Reserved device 11
	uint8_t dXII;			//Reserved device 12
	uint8_t dXIII;			//Reserved device 13
	uint8_t dXIV;			//Reserved device 14
} DEVICE_TO_DEVICE;

//==============================================================
//==============================================================
//											TID Parameter
//==============================================================
//==============================================================
// ************************************************************
//						TID Class subject
// ************************************************************
typedef struct
{
	TID_DATA data1;									//Class data 1
	TID_DATA data2;									//Class data 2
	TID_DATA data3;									//Class data 3
	TID_DATA data4;									//Class data 4
	TID_DATA data5;									//Class data 5
	TID_DATA data6;									//Class data 1
	TID_DATA data7;									//Class data 2
	TID_DATA data8;									//Class data 3
	TID_DATA data9;									//Class data 4
	TID_DATA data10;									//Class data 5
	uint8_t arg[10];									//arrangement of data
	uint8_t datalen[10];
	uint8_t dataNum;
} TID_FEATURE;

typedef struct
{
	TID_DATA data1;									//Class data 1
	TID_DATA data2;									//Class data 2
	TID_DATA data3;									//Class data 3
	TID_DATA data4;									//Class data 4
	TID_DATA data5;									//Class data 5
	uint8_t arg[5];									//arrangement of data
	uint8_t datalen[5];
	uint8_t dataNum;
} TID_INPUT;

typedef struct
{
	TID_DATA data1;									//Class data 1
	TID_DATA data2;									//Class data 2
	TID_DATA data3;									//Class data 3
	TID_DATA data4;									//Class data 4
	TID_DATA data5;									//Class data 5
	uint8_t arg[5];								//arrangement of data
	uint8_t datalen[5];
	uint8_t dataNum;
} TID_OUTPUT;

typedef void (*PFN_FUNC_T)(void);
typedef PFN_FUNC_T APFN_FUNC_T[4];

typedef struct
{
	PFN_FUNC_T  pfnSetup;           // function to configure GPIO and initial sensor
	PFN_FUNC_T  pfnPeriod;          // function to process sensor data and flow control once per 0.1 seconds
	PFN_FUNC_T  pfnPulling;         // function to process sensor data and flow control frequently
	PFN_FUNC_T  pfnReport;          // function to report data
} DEVICE_FUNC_T;

// ************************************************************
//										TID Device
// ************************************************************
typedef struct
{
	DEVICE_DESCRIPTOR DevDesc;
	TID_FEATURE Feature;
	TID_INPUT Input;
	TID_OUTPUT Output;
	DEVICE_TO_DEVICE dTod;
	DEVICE_FUNC_T   func;
} TID_Device;

typedef TID_Device *PTID_Device_T;

extern PTID_Device_T pTidList[MAX_TID_DEV];
extern APFN_FUNC_T pfnDevFunc[MAX_TID_DEV];

extern TID_Device BatDev;
extern TID_Device BuzDev;
extern TID_Device LedDev;
extern TID_Device AHRSDev;
extern TID_Device SonDev;
extern TID_Device TempDev;
extern TID_Device GasDev;
extern TID_Device IRDev;
extern TID_Device KeyDev;
extern TID_Device ResDev9;
extern TID_Device ResDev10;
extern TID_Device ResDev11;
extern TID_Device ResDev12;
extern TID_Device ResDev13;
extern TID_Device ResDev14;

extern uint16_t TID_loadFromFlash;

void SetDeviceFunction(TID_Device *tid, APFN_FUNC_T *apfn_func);

//master
void HandDevDesc(void);
void HandRptDesc(void);
void HandInRpt(void);
void HandGetFeat(void);
void HandGetFeat_sub(TID_Device*);
void HandInRpt_sub(TID_Device*);
/* Handle report descripture */
void HandRptDesc_sub(TID_Device*);
//slave
void TID_SlvRxUpdate(void);

//subfunction
void StoreTIDTxData(uint8_t*, uint16_t, uint8_t, uint8_t*);
void slvTxRpDes(TID_DATA, uint8_t, int*, int16_t);
void TID_MstRxUpdate(void);
/* Handle Input report */

void GetFlashTID(TID_FEATURE*, int8_t);
void UpdateFlashTID(TID_FEATURE, int8_t, bool);

#endif //TID

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

#include "Report.h"

volatile uint32_t ReportTimeCounter;		//10 time from Timer1
char report_mode = REPORT_INDEX;
char report_format = REPORT_FORMAT_TEXT;
char stream_mode = STREAM_PAUSE;


// *******************************************************
//						Report Data
// *******************************************************
// ========================================
//						Report Data Index
// ========================================

void report_index()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		if(TIDMstInitDevState & (1<<BUZZERDEV))
		{
			printf("Buzzer is connected\n");
		}
		else
		{
			printf("Buzzer is not connected\n");
		}
		if(TIDMstInitDevState & (1<<LEDDEV))
		{
			printf("LED is connected\n");
		}
		else
		{
			printf("LED is not connected\n");
		}
		if(TIDMstInitDevState & (1<<AHRSDEV))
		{
			printf("AHRS is connected, Vibration value:%d, Over Boundary:%d\n",AHRSDev.Input.data1.value,AHRSDev.Input.data2.value);
		}				//Vibration sensor, Over flag
		else
		{
			printf("AHRS is not connected\n");
		}
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_index_binary();
	}
}
// ========================================
//						Report Data Index
// ========================================
void report_devLink()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_devLink_binary();
	}
}

// ==========================================
//						Report BUZZER Data
// ==========================================

void report_buzzer()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		if(TIDMstInitDevState & (1<<BUZZERDEV))
		{
			printf("Buzzer Feature:Sleep period:%d, volume:%d, Tone:%d\n",
				   BuzDev.Feature.data1.value,								//SleepPrd
				   BuzDev.Feature.data2.value,								//Volume
				   BuzDev.Feature.data3.value);								//Tone
			printf("Buzzer Output:Song:%d, Period:%d, Duty:%d\n",
				   BuzDev.Output.data1.value,								//Song
				   BuzDev.Output.data2.value,								//Period
				   BuzDev.Output.data3.value);								//Duty
			printf("Buzzer Connect device:One:%d, Two:%d, Three:%d\n",
				   BuzDev.dTod.dONE,
				   BuzDev.dTod.dTWO,
				   BuzDev.dTod.dTHR);
		}
		else if((TIDMstInitDevState & (1<<BUZZERDEV))==0)
		{
			printf("Buzzer is not connected\n");
		}
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(BuzDev);
		//report_buzzer_binary();
	}
}

// ==========================================
//						Report LED Data
// ==========================================

void report_led()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		if(TIDMstInitDevState & (1<<LEDDEV))
		{
			printf("LED Feature:Sleep period:%d, Bright:%d, Color:%d\n",
				   LedDev.Feature.data1.value,							//SleepPrd
				   LedDev.Feature.data2.value,							//Brightness
				   LedDev.Feature.data3.value);							//Color
			printf("LED Output:Period:%d, Duty:%d\n",
				   LedDev.Output.data1.value,							//Prd
				   LedDev.Output.data2.value);							//Duty
			printf("LED Connect device:One:%d, Two:%d, Three:%d\n",
				   LedDev.dTod.dONE,
				   LedDev.dTod.dTWO,
				   LedDev.dTod.dTHR);
		}
		else if((TIDMstInitDevState & (1<<LEDDEV))==0)
		{
			printf("LED is not connected\n");
		}
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(LedDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_ahrs()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		if(TIDMstInitDevState & (1<<AHRSDEV))
		{
			printf("AHRS Feature:Sleep period:%d, Vibration Level:%d, Alerm time:%d\n",
				   AHRSDev.Feature.data1.value,							//SleepPrd
				   AHRSDev.Feature.data2.value,							//Vibration Level
				   AHRSDev.Feature.data3.value);						//Alerm time
			printf("AHRS Input:Vibration value:%d, Over Flag:%d\n",
				   AHRSDev.Input.data1.value,							//Vibration Sensor
				   AHRSDev.Input.data2.value);							//Over Flag
			printf("AHRS Output:Clear Flag:%d\n",
				   AHRSDev.Output.data1.value);							//clear flag
			printf("LED Connect device:One:%d, Two:%d, Three:%d\n",
				   AHRSDev.dTod.dONE,
				   AHRSDev.dTod.dTWO,
				   AHRSDev.dTod.dTHR);
		}
		else if((TIDMstInitDevState & (1<<AHRSDEV))==0)
		{
			printf("AHRS is not connected\n");
		}
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(AHRSDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_battery()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(BatDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_sonar()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(SonDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_temp()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(TempDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_gas()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(GasDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_ir()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(IRDev);
	}
}

// ==========================================
//						Report AHRS Data
// ==========================================

void report_key()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(KeyDev);
	}
}

void report_resDev9()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(ResDev9);
	}
}

void report_resDev10()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(ResDev10);
	}
}

void report_resDev11()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(ResDev11);
	}
}

void report_resDev12()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(ResDev12);
	}
}

void report_resDev13()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(ResDev13);
	}
}

void report_resDev14()
{
	if (report_format == REPORT_FORMAT_TEXT)
	{
		/* TBD */
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		report_binary(ResDev14);
	}
}

void report_sensors()
{
	if(stream_mode==STREAM_PAUSE)
		return;

	if (report_mode == REPORT_INDEX)
	{
		report_index();
	}
	if (report_mode == REPORT_DEVLINK)
	{
		report_devLink();
	}
	else if (/* report_mode >= 0 && */ report_mode < MAX_TID_DEV)
	{
		if (pTidList[report_mode]->func.pfnReport)
			pTidList[report_mode]->func.pfnReport();
	}
}

// *******************************************************
//					set device parameter
// *******************************************************
// ======================================
// 				caculate value from UART
// ======================================
int16_t GetUARTValue()
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

	return (int16_t)dataOut;
}

// ======================================
// 				set value to parameter
// ======================================
void getPara_text(uint16_t* Para1, uint8_t number)
{
	char para;
	int paraNum;
	printf("Please select parameter:\n");
	para = getchar();
	sscanf(&para, "%d", &paraNum);
	if(paraNum <= number)
	{
		if(para == '1')
		{
			printf("Please set value:(minimun:%d,maxium:%d)\n",*(Para1+1), *(Para1+2));
			*Para1 = GetUARTValue();
			printf("Value=%d\n",*Para1);
		}
		else if (para == '2')
		{
			printf("Please set value:(minimun:%d,maxium:%d)\n",*(Para1+4), *(Para1+5));
			*(Para1+3) = GetUARTValue();
			printf("Value=%d\n",*(Para1+3));
		}
		else if (para == '3')
		{
			printf("Please set value:(minimun:%d,maxium:%d)\n",*(Para1+7), *(Para1+8));
			*(Para1+6) = GetUARTValue();
		}
	}
	else
	{
		printf("WRONG CHOISE.\n");
	}
}
// ======================================
// 				set connect to other device
// ======================================
void getConData_text(uint8_t* Para1, uint8_t number)
{
	char para;
	int paraNum;
	printf("Please select device:\n");
	para = getchar();
	sscanf(&para, "%d", &paraNum);
	if(paraNum <= number)
	{
		if(para == '1')
		{
			printf("Please select whether connect to device one. 1/0\n");
			*Para1 = GetUARTValue();
			if(*Para1==1)
				printf("Connect to device one.\n");
			else if(*Para1==0)
				printf("Not connect to device one.\n");
		}
		else if (para == '2')
		{
			printf("Please select whether connect to device one. 1/0\n");
			*(Para1+1) = GetUARTValue();
			if(*(Para1+1)==1)
				printf("Connect to device one.\n");
			else if(*(Para1+1)==0)
				printf("Not connect to device one.\n");
		}
		else if (para == '3')
		{
			printf("Please select whether connect to device one. 1/0\n");
			*(Para1+2) = GetUARTValue();
			if(*(Para1+2)==1)
				printf("Connect to device one.\n");
			else if(*(Para1+2)==0)
				printf("Not connect to device one.\n");
		}
	}
	else
	{
		printf("WRONG CHOISE.\n");
	}
}

void SetPara()
{
	char device;
	char type;
	if (report_format == REPORT_FORMAT_TEXT)
	{
		printf("Please select device:BUZZER:1, LED:2, AHRS:3\n");
		device = getchar();
		/* BUZZER */
		if(device=='1')
		{
			printf("Please select type:Feature:1, Output:3, Connect:4\n");
			type = GetChar();
			if(type=='1')
			{
				getPara_text(&BuzDev.Feature.data1.value,3);				//Sleep period
			}
			else if(type=='3')
			{
				getPara_text(&BuzDev.Output.data1.value,3);					//Song
			}
			else if(type=='4')
			{
				getConData_text(&BuzDev.dTod.dZERO,8);
			}
			else
			{
				printf("WRONG CHOISE.\n");
			}
		}
		/* LED */
		else if(device=='2')
		{
			printf("Please select type:Feature:1, Output:3, Connect:4\n");
			type = GetChar();
			if(type == '1')
			{
				getPara_text(&LedDev.Feature.data1.value,3);				//Sleep period
			}
			else if(type=='3')
			{
				getPara_text(&LedDev.Output.data1.value,2);					//period
			}
			else if(type=='4')
			{
				getConData_text(&LedDev.dTod.dZERO,8);
			}
			else
			{
				printf("WRONG CHOISE.\n");
			}
		}
		/* AHRS */
		else if(device=='3')
		{
			printf("Please select type:Feature:1, Output:3, Connect:4\n");
			type = GetChar();
			if(type == '1')
			{
				getPara_text(&AHRSDev.Feature.data1.value,3);				//Sleep period
			}
			else if(type=='3')
			{
				getPara_text(&AHRSDev.Output.data1.value,1);				//Clear flag
			}
			else if(type=='4')
			{
				getConData_text(&AHRSDev.dTod.dZERO,8);
			}
			else
			{
				printf("WRONG CHOISE.\n");
			}
		}
		else
		{
			printf("WRONG CHOISE.\n");
		}
	}
	else if (report_format == REPORT_FORMAT_BINARY)
	{
		SetPara_Binary();
	}
}

void CommandProcess()
{
	//read data if data number more than two
	if (Serial_available() >= 2)
	{
		char start=Serial_read();
		if (start == '@')
		{
			// Start of new control message
			int command = Serial_read(); // Commands
			if (command == 'h')  //Hook AHRS Stack Device
			{
				// Read ID
				char id[2];
				id[0] = GetChar();
				id[1] = GetChar();
				// Reply with synch message
				printf("@HOOK");
				Serial_write(id, 2);
			}
			/* report start */
			else if (command == 's')
			{
				char mode = GetChar();
				if (mode == 's')  // 's'tart stream
				{
					stream_mode = STREAM_START;
				}
				else if (mode == 'p')  // 'p'ause stream
				{
					stream_mode = STREAM_PAUSE;
				}
			}
			/* Get version */
			else if (command == 'v')
			{
				// Read ID
				unsigned char Version[2];
				Version[0] = '@';
				Version[1] = VERSION_CODE;
				if (report_format == REPORT_FORMAT_BINARY)
					Serial_write((char*)&Version, 2);
				else
					printf("Version:%d\n",Version);
			}
			/* Tab selection */
			else if (command == 't')
			{
				char mode = GetChar();
				if (mode == 'x')							//index
				{
					report_mode = REPORT_INDEX;
					reportNewTabFlag = 0;
				}
				if (mode == 'd')							//device link
				{
					report_mode = REPORT_DEVLINK;
					reportNewTabFlag = 0;
				}
				else if(mode == 'b')						//buzzer
				{
					report_mode = REPORT_BATTERY;
					reportNewTabFlag = 0;
				}
				else if(mode == 'z')						//buzzer
				{
					report_mode = REPORT_BUZZER;
					reportNewTabFlag = 0;
				}
				else if(mode == 'l')						//Led
				{
					report_mode = REPORT_LED;
					reportNewTabFlag = 0;
				}
				else if(mode == 'a')						//AHRS
				{
					report_mode = REPORT_AHRS;
					reportNewTabFlag = 0;
				}
				else if(mode == 's')						//SONAR
				{
					report_mode = REPORT_SONAR;
					reportNewTabFlag = 0;
				}
				else if(mode == 't')						//TEMPERATURE
				{
					report_mode = REPORT_TEMP;
					reportNewTabFlag = 0;
				}
				else if(mode == 'g')						//GAS
				{
					report_mode = REPORT_GAS;
					reportNewTabFlag = 0;
				}
				else if(mode == 'i')						//IR
				{
					report_mode = REPORT_IR;
					reportNewTabFlag = 0;
				}
				else if(mode == 'k')						//KEY
				{
					report_mode = REPORT_KEY;
					reportNewTabFlag = 0;
				}
				else if(mode == 'A')						//Reserved device 9
				{
					report_mode = REPORT_RESDEV9;
					reportNewTabFlag = 0;
				}
				else if(mode == 'B')						//Reserved device 10
				{
					report_mode = REPORT_RESDEV10;
					reportNewTabFlag = 0;
				}
				else if(mode == 'C')						//Reserved device 11
				{
					report_mode = REPORT_RESDEV11;
					reportNewTabFlag = 0;
				}
				else if(mode == 'D')						//Reserved device 12
				{
					report_mode = REPORT_RESDEV12;
					reportNewTabFlag = 0;
				}
				else if(mode == 'E')						//Reserved device 13
				{
					report_mode = REPORT_RESDEV13;
					reportNewTabFlag = 0;
				}
				else if(mode == 'F')						//Reserved device 14
				{
					report_mode = REPORT_RESDEV14;
					reportNewTabFlag = 0;
				}
				else if(mode == 'r')						//Restart
				{
					reportNewTabFlag = 0;
				}
			}
			/* report format selection */
			else if (command == 'f')
			{
				char format = GetChar();
				if (format == 't')
				{
					report_format = REPORT_FORMAT_TEXT;
				}
				else if (format == 'b')
				{
					report_format = REPORT_FORMAT_BINARY;
				}
			}
			/* enter modify parameter mode */
			else if (command == 'm')
			{
				SetPara();
			}
		}
	}
	else if(TMR1INTCount >= ReportTimeCounter)
	{
		report_sensors();
		ReportTimeCounter = TMR1INTCount+1;
	}
}

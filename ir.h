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
#ifndef IR_H_
#define IR_H_

#include <stdio.h>
#include <stdlib.h>
#include "M451Series.h"
#include "timerctrl.h"
#include "tid.h"

void IR_Init(void);
void SendIR(void);
void ReceiveIR(void);
void IR_Control(void);
void IR_Check(void);

extern uint8_t TxIR_CODE[4];
extern uint8_t RxIR_CODE[4];
extern uint8_t IR_Tx_stage;
extern uint8_t IR_TX_Bitn;
extern uint8_t IR_TX_Byten;
extern uint8_t IR_bit_state;
extern uint8_t IR_byte_state;
extern uint8_t IR_Tx_LearnedDataByten;
extern uint8_t IR_TxExecuteFLAG;
extern uint8_t IRTx_StartFlag;
extern uint8_t IRTx_StopFlag;
extern volatile uint16_t IR_rawDATA[68];
extern volatile uint32_t IR_DATA;		//IR receive data
extern volatile uint32_t IR_DATA_OUT;	//IR receive data output
extern uint16_t IR_capture_count;				//IR receiver Data counter
extern uint8_t IR_RxExecute_Flag;				//IR start receive data flag
extern uint8_t IR_RxComplete_Flag;		//IR complete receiving data
extern uint8_t IR_RxReveice_Bit;		//IR complete receiving data
extern uint16_t IR_caprure_timer_falling;			//PWM capture time between rising and falling
extern uint16_t IR_caprure_timer_rising;			//PWM capture time between rising and falling
extern uint32_t IR_init_counter;				//IR start receive time
//learned mode variable
extern uint8_t IR_LearnMode;								//IR learning mode ON/OFF
extern uint8_t IR_LearnedFlag;							//IR learned Flag
extern uint8_t IR_LearnedByte;							//IR learning current Byte
extern uint8_t IR_LearnedDataLeng[6];					//IR Rx learning mode data base
extern uint16_t IR_FallingMaxTime;					//IR Rx Falling Max Time
extern uint16_t IR_FallingMinTime;					//IR Rx Falling Min Time
extern uint16_t IR_RisingMaxTime;					//IR Rx Rising Max Time
extern uint16_t IR_RisingMinTime;					//IR Rx Rising Min Time
extern uint16_t IR_FallingMaxFreq;					//IR Rx Falling Max Time
extern uint16_t IR_FallingMinFreq;					//IR Rx Falling Min Time
extern uint16_t IR_RisingMaxFreq;					//IR Rx Rising Max Time
extern uint16_t IR_RisingMinFreq;					//IR Rx Rising Min Time
extern uint32_t IR_LearnedData[12];					//IR Rx learning mode data base

// ------transfer state------
#define TRANSFER_START	0
#define TRANSFER_CONT		1
#define IR_DATA_BIT_NUMBER		8

// ------transfer ticker-------
#define	START_TRAN_FIRST	111		//9ms, start transfer bit
#define START_TRAN_SECOND	222		//4.5ms,
#define TRAN_BIT_FIRST		1786	//560us,
#define TRAN_BIT_ZERO			1786	//560us, bit data "0"
#define TRAN_BIT_ONE			595		//1680us,bit data "1"

// ------receive ticker-------
#define	IRRX_FIRST_BIT					100		//500us for pwm capture base at 5us
#define IRRX_DATA_BIT_JUDGE			200		//1ms for pwm capture base at 5us
#define Receive_DATA_BIT			64		//1ms for pwm capture base at 5us

#endif /* IR_H_ */

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

void IR_Init(void);
void SendIR(void);
void ReceiveIR(void);

extern uint8_t TxIR_CODE[4];
extern uint8_t RxIR_CODE[4];
extern uint8_t	IR_Tx_stage;
extern uint8_t	IR_TX_Bitn;
extern uint8_t	IR_TX_Byten;
extern uint8_t IR_bit_state;
extern uint8_t IR_bits_counter;
extern uint8_t IR_bytes_counter;
extern uint8_t IR_TxExecuteFLAG;
extern uint8_t IRTx_StopFlag;
extern volatile uint16_t IR_rawDATA[34];
extern volatile uint16_t IR_DATA[4];		//IR receive data
extern volatile uint16_t IR_DATA_OUT[4];	//IR receive data output
extern uint16_t IR_capture_count;				//IR receiver Data counter
extern uint8_t IR_RxExecute_Flag;				//IR start receive data flag
extern uint8_t IR_RxComplete_Flag;		//IR complete receiving data
extern uint32_t IR_init_counter;				//IR start receive time
extern uint32_t PWM_caprure_timer;			//PWM capture time between rising and falling

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
#define	IRRX_FIRST_BIT_MAX			950		//5ms for pwm capture base at 5us
#define IRRX_FIRST_BIT_MIN			850		//4ms for pwm capture base at 5us
#define IRRX_DATA_BIT_JUDGE			200		//1ms for pwm capture base at 5us

#endif /* IR_H_ */

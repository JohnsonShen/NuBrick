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

#include "timer3IRQ.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  IR Transfer Data, Call by TMR3_IRQHandler()                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void IR_TranDATA(void)
{
	if(IR_bit_state==0)
	{
		// Stop PWM0 CH3 output
		PWM_EnableOutput(PWM0, PWM_CH_3_MASK);
		//Initial Timer0 to periodic mode with 1786Hz(560us)
		TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, TRAN_BIT_FIRST);
		TIMER_EnableInt(TIMER3);
		TIMER_Start(TIMER3);
		IR_bit_state=1;
	}
	else if (IR_bit_state==1)
	{
		if(TxIR_CODE[IR_TX_Byten] & (1 << (IR_DATA_BIT_NUMBER-IR_TX_Bitn-1)))
		{
			// Stop PWM0 CH3 output
			PWM_DisableOutput(PWM0, PWM_CH_3_MASK);
			//Initial Timer0 to periodic mode with 595Hz(1680us)
			TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, TRAN_BIT_ONE);
			TIMER_EnableInt(TIMER3);
			TIMER_Start(TIMER3);
		}
		else
		{
			// Enable PWM0 CH3 output
			PWM_DisableOutput(PWM0, PWM_CH_3_MASK);
			//Initial Timer0 to periodic mode with 1786Hz(560us)
			TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, TRAN_BIT_ZERO);
			TIMER_EnableInt(TIMER3);
			TIMER_Start(TIMER3);
		}
		if(IR_TX_Bitn<7)
		{
			IR_TX_Bitn++;
		}
		else
		{
			IR_TX_Bitn=0;
			IR_TX_Byten++;
		}
		IR_bit_state=0;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/*  TMR0 IRQ handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void TMR3_IRQHandler(void)
{
	uint8_t LearnedDataByten;							//LearnedDataByten transfered from IR_Tx_LearnedDataByten
	// clear Timer0 interrupt flag
	TIMER_ClearIntFlag(TIMER3);

	if(IR_Tx_stage==TRANSFER_START && IR_bit_state==1)
	{
		// Enable PWM0 CH2 output
		PWM_DisableOutput(PWM0, PWM_CH_3_MASK);
		//Initial Timer0 to periodic mode with 222Hz(4.5ms)
		TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, START_TRAN_SECOND);
		TIMER_EnableInt(TIMER3);
		TIMER_Start(TIMER3);
		IR_bit_state=0;
		IR_Tx_stage=TRANSFER_CONT;
	}
	else if(IR_Tx_stage==TRANSFER_CONT)
	{
		if(IR_LearnedFlag==0)
		{
			if(IR_TX_Byten<4)
			{
				IR_TranDATA();
			}
			else
			{
				if(IRTx_StopFlag==0)
				{
					PWM_EnableOutput(PWM0, PWM_CH_3_MASK);
					//Initial Timer0 to periodic mode with 1786Hz(560us)
					TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, TRAN_BIT_ZERO);
					TIMER_EnableInt(TIMER3);
					TIMER_Start(TIMER3);
					IRTx_StopFlag=1;
				}
				else if(IRTx_StopFlag==1)
				{
					PWM_DisableOutput(PWM0, PWM_CH_3_MASK);
					IR_TX_Bitn = 0;
					IR_TX_Byten = 0;
					IR_Tx_stage=TRANSFER_START;
					IRTx_StopFlag = 0;
					IR_TxExecuteFLAG = 0;
				}
			}
		}
		else if(IR_LearnedFlag==1)
		{
			if(IRTx_StopFlag==1)
			{
				PWM_DisableOutput(PWM0, PWM_CH_3_MASK);
				IR_Tx_stage=TRANSFER_START;
				IR_TX_Bitn=0;
				IR_byte_state = 0;
				IRTx_StopFlag	=	0;
				IR_TxExecuteFLAG = 0;
			}
			else if(IR_TX_Bitn%2==0)
			{
				LearnedDataByten = IR_Tx_LearnedDataByten*2+IR_byte_state;
				PWM_EnableOutput(PWM0, PWM_CH_3_MASK);
				if(IR_LearnedData[LearnedDataByten] & (1 << (IR_LearnedDataLeng[IR_Tx_LearnedDataByten]/2 - IR_TX_Bitn - 1)))//IR_TX_Bitn*2
					//Initial Timer0 to periodic mode with 1786Hz(560us)
					TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, IR_FallingMaxFreq);
				else
					TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, IR_FallingMinFreq);
				TIMER_EnableInt(TIMER3);
				TIMER_Start(TIMER3);
				IR_TX_Bitn++;
			}
			else if(IR_TX_Bitn%2==1)
			{
				LearnedDataByten = IR_Tx_LearnedDataByten*2+IR_byte_state;
				PWM_DisableOutput(PWM0, PWM_CH_3_MASK);
				if(IR_LearnedData[LearnedDataByten] & (1 << (IR_LearnedDataLeng[IR_Tx_LearnedDataByten]/2-IR_TX_Bitn-1)))//IR_TX_Bitn*2
					//Initial Timer0 to periodic mode with 1786Hz(560us)
					TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, IR_RisingMaxFreq);
				else
					TIMER_Open(TIMER3, TIMER_ONESHOT_MODE, IR_RisingMinFreq);
				TIMER_EnableInt(TIMER3);
				TIMER_Start(TIMER3);
				IR_TX_Bitn++;
			}
			if(IR_TX_Bitn==32 && IR_byte_state == 0)
			{
				IR_TX_Bitn=0;
				IR_byte_state=1;
			}
			else if((IR_TX_Bitn+31==IR_LearnedDataLeng[IR_Tx_LearnedDataByten]) && (IR_byte_state == 1))
			{
				IRTx_StopFlag = 1;
			}
		}
	}
}

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
	// clear Timer0 interrupt flag
	TIMER_ClearIntFlag(TIMER3);
	
	if(IR_Tx_stage==TRANSFER_START)
	{
		if(IR_bit_state==1)
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
	}
	else if(IR_Tx_stage==TRANSFER_CONT)
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
				IR_TX_Bitn=0;
				IR_TX_Byten=0;
				IR_Tx_stage=TRANSFER_START;
				IRTx_StopFlag	=	0;
				IR_TxExecuteFLAG=0;
			}
		}
	}
}

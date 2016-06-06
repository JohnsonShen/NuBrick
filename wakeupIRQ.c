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

#include "wakeupIRQ.h"

void I2CWakeUpInit()
{
	/* Initialize I2C Slave Mode */
	I2C_MS_Slave_Init();

	/* Unlock protected registers */
	SYS_UnlockReg();

	/* Enable power wake-up interrupt */
	CLK->PWRCTL |= CLK_PWRCTL_PDWKIEN_Msk;
	NVIC_EnableIRQ(PWRWU_IRQn);

	/* Enable I2C wake-up */
	I2C_EnableWakeup(I2C_MS_PORT);

	/* Lock protected registers */
	SYS_LockReg();

	/* Set I2C0 enter Not Address SLAVE mode */
	I2C_SET_CONTROL_REG(I2C_MS_PORT, I2C_CTL_SI_AA);

}

/*---------------------------------------------------------------------------------------------------------*/
/*  Power Wake-up IRQ Handler                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void PWRWU_IRQHandler(void)
{
	/* Check system power down mode wake-up interrupt flag */
	if(((CLK->PWRCTL) & CLK_PWRCTL_PDWKIF_Msk) != 0)
	{
		/* Clear system power down wake-up interrupt flag */
		CLK->PWRCTL |= CLK_PWRCTL_PDWKIF_Msk;
	}
}

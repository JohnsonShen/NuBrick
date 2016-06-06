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

#include "key.h"

uint8_t keyState;

void key_init()
{

	KeyDev.DevDesc.DevDesc_leng = 26;						//Report descriptor
	KeyDev.DevDesc.RptDesc_leng = 22;						//Report descriptor
	KeyDev.DevDesc.InRptLeng = 3;							//Input report
	KeyDev.DevDesc.OutRptLeng = 0;							//Output report
	KeyDev.DevDesc.GetFeatLeng = 4;							//Get feature
	KeyDev.DevDesc.SetFeatLeng = 4;							//Set feature
	KeyDev.DevDesc.CID = 0;									//manufacturers ID
	KeyDev.DevDesc.DID = 0;									//Product ID
	KeyDev.DevDesc.PID = 0;									//Device firmware revision
	KeyDev.DevDesc.UID = 0;									//Device Class type
	KeyDev.DevDesc.UCID = 0;								//reserve
	/* Feature */
	KeyDev.Feature.data1.minimum = 0;						//Sleep period
	KeyDev.Feature.data1.maximum = 1024;
	KeyDev.Feature.data1.value = 100;
	KeyDev.Feature.arg[0] = 1;
	KeyDev.Feature.datalen[0] = 2;
	KeyDev.Feature.dataNum = 1;
	/* Input */
	KeyDev.Input.data1.minimum = 0;							//sensored disance
	KeyDev.Input.data1.maximum = 127;
	KeyDev.Input.data1.value = 0;
	KeyDev.Input.arg[0] = 1;
	KeyDev.Input.datalen[0] = 2;
	KeyDev.Input.dataNum = 1;
	/* Output */
	KeyDev.Output.dataNum = 0;

	/* Configure PC.0 as Input mode and enable interrupt by rising edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC0MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC0MFP_GPIO;
	GPIO_SetMode(PC, BIT0, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 0, GPIO_INT_BOTH_EDGE);

	/* Configure PC.1 as Input mode and enable interrupt by rising edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC1MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC1MFP_GPIO;
	GPIO_SetMode(PC, BIT1, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 1, GPIO_INT_BOTH_EDGE);

	/* Configure PC.2 as Input mode and enable interrupt by rising edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC2MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC2MFP_GPIO;
	GPIO_SetMode(PC, BIT2, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 2, GPIO_INT_BOTH_EDGE);

	/* Configure PC.3 as Input mode and enable interrupt by rising edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC3MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC3MFP_GPIO;
	GPIO_SetMode(PC, BIT3, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 3, GPIO_INT_BOTH_EDGE);

	/* Configure PC.4 as Input mode and enable interrupt by rising edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC4MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC4MFP_GPIO;
	GPIO_SetMode(PC, BIT4, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 4, GPIO_INT_BOTH_EDGE);

	/* Configure PC.5 as Quasi-bidirection mode and enable interrupt by falling edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC5MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC5MFP_GPIO;
	GPIO_SetMode(PC, BIT5, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 5, GPIO_INT_BOTH_EDGE);

	/* Configure PC.6 as Quasi-bidirection mode and enable interrupt by falling edge trigger */
	SYS->GPC_MFPL &= ~SYS_GPC_MFPL_PC6MFP_Msk;
	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC6MFP_GPIO;
	GPIO_SetMode(PC, BIT6, GPIO_MODE_QUASI);
	GPIO_EnableInt(PC, 6, GPIO_INT_BOTH_EDGE);
	NVIC_EnableIRQ(GPC_IRQn);

	/* Configure PE.0 as Quasi-bidirection mode and enable interrupt by falling edge trigger */
	/* Configure PC.3 as Input mode and enable interrupt by rising edge trigger */
	SYS->GPE_MFPL &= ~SYS_GPE_MFPL_PE0MFP_Msk;
	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE0MFP_GPIO;
	GPIO_SetMode(PE, BIT0, GPIO_MODE_QUASI);
	GPIO_EnableInt(PE, 0, GPIO_INT_BOTH_EDGE);
	NVIC_EnableIRQ(GPE_IRQn);

	//LED
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA2MFP_Msk);
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_GPIO;
	GPIO_SetMode(PA,BIT2,GPIO_MODE_OUTPUT);
	PA2 = 1;

	/* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
	GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);
	GPIO_ENABLE_DEBOUNCE(PC, BIT0);
	GPIO_ENABLE_DEBOUNCE(PC, BIT1);
	GPIO_ENABLE_DEBOUNCE(PC, BIT2);
	GPIO_ENABLE_DEBOUNCE(PC, BIT3);
	GPIO_ENABLE_DEBOUNCE(PC, BIT4);
	GPIO_ENABLE_DEBOUNCE(PC, BIT5);
	GPIO_ENABLE_DEBOUNCE(PE, BIT0);

	keyState = 0;

}

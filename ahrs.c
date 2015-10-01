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
 
#include "ahrs.h"

// ===========================================================
//
//	Function:Read ahrs value and compare 
//
// ===========================================================
uint8_t AhrsRead(uint8_t ACC_Sentivity, uint8_t GYRO_Sentivity)
{
	float gyro[3], Ve[3], Move[3], Euler[3];
	
	// Get x,y,z velocity
	nvtGetEulerRPY(Euler);
	nvtGetVelocity(Ve);		
	nvtGetMove(Move);
	//nvtGetCalibratedACC(Ve);
	//nvtGetAccZWithoutGravity(&Ve[0], &Ve[1]);
	// Get x,y,z Angeler velocity
	nvtGetCalibratedGYRO(gyro);
	printf("Vx=%f, Vy=%f, Vz=%f, Ax=%f, Ay=%f, Az=%f\n", Ve[0], Ve[1], Ve[2], gyro[0], gyro[1], gyro[2]);
	if (((Ve[0]*ACC_Sentivity)> ACC_PRESCALE) || 
			((Ve[1]*ACC_Sentivity)> ACC_PRESCALE) || 
			((Ve[2]*ACC_Sentivity)> ACC_PRESCALE) ||
			((gyro[0]*GYRO_Sentivity)> GYRO_PRESCALE) ||
			((gyro[1]*GYRO_Sentivity)> GYRO_PRESCALE) ||
			((gyro[2]*GYRO_Sentivity)> GYRO_PRESCALE))
		return 1;
	else
		return 0;
}

/**
 *******************************************************************************
 * @file	util_Checksum.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	11 dec. 2013
 * @brief   Calcul de checksums.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "util_Checksum.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**-----------------------------------------------------------------------------
 * @brief	Calcul du Complement a 2 sur un buffer.
 *
 * @param[in]	pBuffer		Pointeur vers le buffer
 * @param[in]	Size		Taille du buffer
 *
 * return 		Checksum	Checkum calcule
 *
 */
uint32_t Checksum_CalculComplementA2(uint8_t* pBuffer, uint32_t Size)
{
	uint32_t Checksum=0;

	// Somme
	while (Size)
	{
		Checksum += pBuffer[--Size];
	}

	/* Complement a 2 */
	Checksum = ~Checksum + 1;

	return Checksum;
}









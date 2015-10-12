/**
 *******************************************************************************
 * @file	util_Checksum.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	11 dec. 2013
 * @brief   Calcul de checksums.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef UTIL_CHECKSUM_H_
#define UTIL_CHECKSUM_H_


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

uint32_t Checksum_CalculComplementA2(uint8_t* pBuffer, uint32_t Size);


#endif /* UTIL_CHECKSUM_H_ */





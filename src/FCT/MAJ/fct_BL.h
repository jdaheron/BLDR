/**
 *******************************************************************************
 * @file	fct_BL
 * @author	j.daheron
 * @version	1.0.0
 * @date	11 dec. 2013
 * @brief   Gestion du bootloader.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef FCT_BL_H_
#define FCT_BL_H_


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void VALIDATION_SEQUENCE_FCT_BL_CAN_HOST();

uint8_t BL_GetID(uint16_t* ID);
uint8_t BL_ReadMemory(uint32_t Address, uint8_t* pData, uint16_t NbData);
uint8_t BL_GO(uint32_t Address);
uint8_t BL_WriteMemory(uint32_t Address, uint8_t* pData, uint16_t NbData);
uint8_t BL_Erase(uint8_t FlashSector);

#endif /* FCT_BL_H_ */





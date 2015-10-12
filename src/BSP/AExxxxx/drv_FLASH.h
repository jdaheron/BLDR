/**
 *******************************************************************************
 * @file	drv_FLASH
 * @author	j.daheron
 * @version	1.0.0
 * @date	21 oct. 2013
 * @brief   Gestion de la Flash interne.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_FLASH_H_
#define DRV_FLASH_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "stm32f2xx_flash.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void FLASH_Read(uint32_t Address, uint16_t Size, uint8_t* pBuffer);
uint8_t FLASH_Write(uint32_t Address, uint8_t* pBuffer, uint16_t Size);
uint8_t FLASH_Erase(uint8_t FlashSector);
uint8_t FLASH_EraseAddress(uint32_t Address);
int8_t FLASH_GetSectorNum(uint32_t Address);

#endif /* DRV_FLASH_H_ */





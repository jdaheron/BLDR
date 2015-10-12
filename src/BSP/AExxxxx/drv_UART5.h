/**
 *******************************************************************************
 * @file	drv_UART5.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'UART5.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_UART5_H_
#define DRV_UART5_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void UART5_Init(uint32_t baudrate);
void UART5_DeInit();
uint16_t UART5_Read(uint8_t* pBuffer, uint16_t Taille);
void UART5_Write(uint8_t* pBuffer, uint16_t Taille);


#endif /* DRV_UART5_H_ */





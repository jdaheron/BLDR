/**
 *******************************************************************************
 * @file	drv_USART1.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'USART1.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_USART1_H_
#define DRV_USART1_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void USART1_Init(uint32_t baudrate);
void USART1_DeInit();
uint16_t USART1_Read(uint8_t* pBuffer, uint16_t Taille);
void USART1_Write(uint8_t* pBuffer, uint16_t Taille);


#endif /* DRV_USART1_H_ */





/**
 *******************************************************************************
 * @file	drv_USART6.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'USART6.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_USART6_H_
#define DRV_USART6_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void USART6_Init(uint32_t baudrate);
void USART6_DeInit();
uint16_t USART6_Read(uint8_t* pBuffer, uint16_t Taille);
void USART6_Write(uint8_t* pBuffer, uint16_t Taille);


#endif /* DRV_USART6_H_ */





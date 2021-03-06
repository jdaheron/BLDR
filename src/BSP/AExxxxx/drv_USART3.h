/**
 *******************************************************************************
 * @file	drv_USART3.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'USART3.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_USART3_H_
#define DRV_USART3_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void USART3_Init(uint32_t baudrate);
void USART3_DeInit();
uint16_t USART3_Read(uint8_t* pBuffer, uint16_t Taille);
void USART3_Write(uint8_t* pBuffer, uint16_t Taille);


#endif /* DRV_USART3_H_ */





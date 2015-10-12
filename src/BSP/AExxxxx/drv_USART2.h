/**
 *******************************************************************************
 * @file	drv_USART2.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'USART2.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_USART2_H_
#define DRV_USART2_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void USART2_Init(uint32_t baudrate);
void USART2_DeInit();
uint16_t USART2_Read(uint8_t* pBuffer, uint16_t Taille);
void USART2_Write(uint8_t* pBuffer, uint16_t Taille);


#endif /* DRV_USART2_H_ */





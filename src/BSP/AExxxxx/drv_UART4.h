/**
 *******************************************************************************
 * @file	drv_UART4.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'UART4.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_UART4_H_
#define DRV_UART4_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void UART4_Init(uint32_t baudrate);
void UART4_DeInit();
uint16_t UART4_Read(uint8_t* pBuffer, uint16_t Taille);
void UART4_Write(uint8_t* pBuffer, uint16_t Taille);


#endif /* DRV_UART4_H_ */





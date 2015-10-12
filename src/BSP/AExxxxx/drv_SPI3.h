/**
 *******************************************************************************
 * @file	drv_SPI3.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 oct. 2013
 * @brief   Gestion du SPI3.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_SPI3_H_
#define DRV_SPI3_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


void SPI3_Init(uint32_t Vitesse_kHz);
void SPI3_DeInit();
uint8_t SPI3_Read();
uint8_t SPI3_Write(uint8_t Data);


#endif /* DRV_SPI3_H_ */





/**
 *******************************************************************************
 * @file	drv_SPI1.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 oct. 2013
 * @brief   Gestion du SPI1.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_SPI1_H_
#define DRV_SPI1_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


void SPI1_Init(uint32_t Vitesse_kHz);
void SPI1_DeInit();
uint8_t SPI1_Read();
uint8_t SPI1_Write(uint8_t Data);


#endif /* DRV_SPI1_H_ */





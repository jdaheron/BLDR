/**
 *******************************************************************************
 * @file	drv_SPI2.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 oct. 2013
 * @brief   Gestion du SPI2.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_SPI2_H_
#define DRV_SPI2_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


void SPI2_Init();
void SPI2_DeInit();
uint8_t SPI2_Read();
uint8_t SPI1_Write(uint8_t Data);


#endif /* DRV_SPI2_H_ */





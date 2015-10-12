/**
 *******************************************************************************
 * @file	drv_I2C3.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 dec. 2013
 * @brief   Gestion de l'I2C3.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_I2C3_H_
#define DRV_I2C3_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void I2C3_Init(uint32_t ClockSpeed);
void I2C3_DeInit();

uint8_t  I2C3_ReadByte(uint8_t SlaveAddress, uint8_t DataAddress);
uint16_t I2C3_ReadWord(uint8_t SlaveAddress, uint8_t DataAddress);
void I2C3_ReadBuffer(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t* pData, uint16_t Taille);

void I2C3_WriteByte(uint8_t SlaveAddress, uint8_t Address, uint8_t Data);
void I2C3_WriteWord(uint8_t SlaveAddress, uint8_t Address, uint16_t Data);
void I2C3_WriteBuffer(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t* pData, uint16_t Taille);

#endif /* DRV_I2C3_H_ */





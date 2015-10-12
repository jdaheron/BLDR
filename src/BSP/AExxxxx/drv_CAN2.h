/**
 *******************************************************************************
 * @file	drv_CAN2.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 oct. 2013
 * @brief   Gestion du CAN2.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_CAN2_H_
#define DRV_CAN2_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_CAN.h"


/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void CAN2_Init();
void CAN2_DeInit();
uint8_t CAN2_Read(CAN_Msg_t* pTrame);
uint8_t CAN2_GestionEnvoiTrame();
uint8_t CAN2_Write(CAN_Msg_t Trame);

#endif /* DRV_CAN2_H_ */





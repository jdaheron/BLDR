/**
 *******************************************************************************
 * @file	drv_CAN1.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 oct. 2013
 * @brief   Gestion du CAN1.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_CAN1_H_
#define DRV_CAN1_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_CAN.h"


/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void CAN1_Init();
void CAN1_DeInit();
uint8_t CAN1_Read(CAN_Msg_t* pTrame);
uint8_t CAN1_GestionEnvoiTrame();
uint8_t CAN1_Write(CAN_Msg_t Trame);

#endif /* DRV_CAN1_H_ */





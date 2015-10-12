/**
 *******************************************************************************
 * @file	drv_CAN.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	18 oct. 2013
 * @brief   Types lies au CAN.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_CAN_H_
#define DRV_CAN_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/

/** Type ID de trame CAN. */
typedef enum {

	ID_Type_Std	= 0,
	ID_Type_Ext

} CAN_ID_Type_e;

/** Type ID de trame CAN. */
typedef enum {

	RTR_Type_Data	= 0,
	RTR_Type_Remote

} CAN_RTR_Type_e;

/** Definition d'une trame CAN. */
typedef struct {

	uint32_t ID;
	CAN_ID_Type_e ID_Type;
	CAN_RTR_Type_e RTR_Type;
	uint8_t DLC;
	uint8_t Data[8];

} CAN_Msg_t;

/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


#endif /* DRV_CAN_H_ */





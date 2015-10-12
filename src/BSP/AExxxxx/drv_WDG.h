/**
 *******************************************************************************
 * @file	drv_WDG.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	06 dec. 2013
 * @brief   Gestion du Watchdog.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_WDG_H_
#define DRV_WDG_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void WDG_InitIWDG(uint16_t Timeout_ms);
void WDG_InitWWDG(uint16_t Timeout_ms);
void WDG_Refresh();
void WDG_Disable();


#endif /* DRV_WDG_H_ */





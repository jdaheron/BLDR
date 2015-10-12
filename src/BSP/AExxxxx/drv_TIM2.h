/**
 *******************************************************************************
 * @file	drv_TIM2.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	09 dec. 2013
 * @brief   Gestion du Timer TIM2.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_TIM2_H_
#define DRV_TIM2_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void TIM2_Init(uint32_t Periode_us);
void TIM2_DeInit();
void TIM2_PWM_Configurer(MappingGpio_e PinId, Etat_e Etat, uint8_t Ratio_pr100);
void TIM2_PWM_ReadConfiguration(MappingGpio_e PinId, uint32_t* pPeriode_us, uint8_t* pRatio_pr100);
void TIM2_PWM_Activer(MappingGpio_e PinId);
void TIM2_PWM_Desactiver(MappingGpio_e PinId);


#endif /* DRV_TIM2_H_ */





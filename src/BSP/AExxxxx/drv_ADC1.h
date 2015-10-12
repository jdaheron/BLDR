/**
 *******************************************************************************
 * @file	drv_ADC1.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 avr. 2013
 * @brief   Gestion de l'ADC1.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_ADC1_H_
#define DRV_ADC1_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/

typedef enum {
	ADC_Ready = 0,
	ADC_ConvEnCours
}ADC1_ConvStatus_e;

typedef struct {

	ADC_TypeDef*	ADCx;				// Numero de l'ADC
	uint8_t			ADC_Channel;		// Numero du Channel
	uint8_t			ConversionContinue;	// Conversion Continue du canal
	uint32_t		LastValue_mV;		// Valeur de la derniere conversion
	uint32_t*		pStoreValue_mV;		// Pointeur ou stocker la donnee convertie
	pFunction		pFct_CallbackEOC;	// Pointeur vers la fonction callback fin de conversion du Channel

} ADC1_ChannelConf_t;


/* Exported constants --------------------------------------------------------*/

#define ADC_NB_ChannelConf_MAX	10


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void ADC1_Tick();
void ADC1_Init();
void ADC1_DeInit();
uint8_t ADC1_ChannelConfigure(ADC1_ChannelConf_t ADC_Channel);

void ADC1_ConversionContinue_Activer(uint8_t ID_Channel);
void ADC1_ConversionContinue_Desactiver(uint8_t ID_Channel);

void ADC1_StartConversion(uint8_t ID_ADC_Channel);
ADC1_ConvStatus_e ADC1_GetConversionStatus();
uint32_t ADC1_Get_mV(uint8_t ID_Channel);


#endif /* DRV_ADC1_H_ */





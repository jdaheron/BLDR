/**
 *******************************************************************************
 * @file	drv_TIM2.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	09 dec. 2013
 * @brief   Gestion du Timer TIM2.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_TIM2.h"


/* External variables --------------------------------------------------------*/

static uint32_t TIM_Prescaler = 1;
static uint32_t TIM_Periode = 1;
static uint32_t SavePeriode_us = 0;
static uint8_t SaveRatio_pr100[4] = {0};


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define TIMER_SIZE	0xFFFF	// 16 bits


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**-----------------------------------------------------------------------------
 * @brief	Initialisation du Timer.
 *
 * @param[in]	Periode_us		Periode en us.
 *
 */
void TIM2_Init(uint32_t Periode_us) {

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_ClocksTypeDef RCC_Clocks;
	uint32_t Clock_MHz;

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//----------------------------------
	// Initialisation base de temps

	// Lecture horloge Bus
	RCC_GetClocksFreq(&RCC_Clocks);
	Clock_MHz = RCC_Clocks.PCLK1_Frequency;

	// Calcul Prescaler et Periode
	TIM_Prescaler = Clock_MHz / (1000 * 1000);
	TIM_Periode = Periode_us * 2;
	while (TIM_Periode >= TIMER_SIZE)
	{
		TIM_Prescaler <<= 1;
		TIM_Periode >>= 1;
	}
	TIM_TimeBaseInitStruct.TIM_Prescaler		= TIM_Prescaler;
	TIM_TimeBaseInitStruct.TIM_Period			= TIM_Periode;
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	//----------------------------------
	// Activation Timer
	TIM_Cmd(TIM2, ENABLE);

	// Sauvegarde
	SavePeriode_us = Periode_us;
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void TIM2_DeInit() {

	TIM_Cmd(TIM2, DISABLE);									// Desactivation peripherique
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Configuration d'une sortie PWM.
 *
 * @param[in]	PinId			Numero d'identification de la pin.
 * @param[in]	Etat			Etat de la sortie (Actif / Inactif).
 * @param[in]	Ratio_pr100		Ratio en pourcent.
 *
 */
void TIM2_PWM_Configurer(MappingGpio_e PinId, Etat_e Etat, uint8_t Ratio_pr100)
{
	TIM_OCInitTypeDef TIM_OCInit;
	uint32_t Pulse;

	TIM_OCStructInit(&TIM_OCInit);

	if (PinId >= NB_PORT_GPIO)
		return;

	// Definition de l'etat
	if (Etat == Etat_ACTIF)
	{
		TIM_OCInit.TIM_OutputState = TIM_OutputState_Enable;
	}
	else
	{
		TIM_OCInit.TIM_OutputState = TIM_OutputState_Disable;
	}


	// Inversion etat actif si necessaire
	if (MappingGpio[PinId].Inverse == FALSE)
	{
		TIM_OCInit.TIM_OCPolarity = TIM_OCPolarity_High;
	}
	else
	{
		TIM_OCInit.TIM_OCPolarity = TIM_OCPolarity_Low;
	}


	// Calcul Ratio
	if (Ratio_pr100 >= 100)
	{
		TIM_OCInit.TIM_Pulse = TIM_Periode;
	}
	else
	{
		Pulse  = Ratio_pr100 * TIM_Periode;
		Pulse /= 100;
		TIM_OCInit.TIM_Pulse = Pulse;
	}
	TIM_OCInit.TIM_OCMode = TIM_OCMode_PWM1;


	// Configuration du canal en fonction du mapping
	switch (MappingGpio[PinId].PARAM)
	{
		case TIM_Channel_1:
			SaveRatio_pr100[0] = Ratio_pr100;
			TIM_OC1Init(TIM2, &TIM_OCInit);
			break;

		case TIM_Channel_2:
			SaveRatio_pr100[1] = Ratio_pr100;
			TIM_OC2Init(TIM2, &TIM_OCInit);
			break;

		case TIM_Channel_3:
			SaveRatio_pr100[2] = Ratio_pr100;
			TIM_OC3Init(TIM2, &TIM_OCInit);
			break;

		case TIM_Channel_4:
			SaveRatio_pr100[3] = Ratio_pr100;
			TIM_OC4Init(TIM2, &TIM_OCInit);
			break;

		default:
			return;
	}
}


/**-----------------------------------------------------------------------------
 * @brief	Configuration d'une sortie PWM.
 *
 * @param[in]	PinId			Numero d'identification de la pin.
 * @param[in]	pPeriode_us		Periode en us.
 * @param[in]	pEtat			Etat de la sortie (Actif / Inactif).
 * @param[in]	pRatio_pr100	Ratio en pourcent.
 *
 */
void TIM2_PWM_ReadConfiguration(MappingGpio_e PinId, uint32_t* pPeriode_us, uint8_t* pRatio_pr100)
{

	*pPeriode_us = SavePeriode_us;

	// Configuration du canal en fonction du mapping
	switch (MappingGpio[PinId].PARAM)
	{
		case TIM_Channel_1:	*pRatio_pr100 = SaveRatio_pr100[0];	break;
		case TIM_Channel_2:	*pRatio_pr100 = SaveRatio_pr100[1];	break;
		case TIM_Channel_3:	*pRatio_pr100 = SaveRatio_pr100[2];	break;
		case TIM_Channel_4:	*pRatio_pr100 = SaveRatio_pr100[3];	break;
	}
}


/**-----------------------------------------------------------------------------
 * @brief	Activation d'une sortie PWM.
 *
 * @param[in]	PinId	Numero d'identification de la pin.
 *
 */
void TIM2_PWM_Activer(MappingGpio_e PinId)
{
	TIM_CCxCmd(TIM2, MappingGpio[PinId].PARAM, TIM_CCx_Enable);
	TIM_CtrlPWMOutputs(TIM2, ENABLE); // TIM2 et TIM8 seulement
}


/**-----------------------------------------------------------------------------
 * @brief	Desactivation d'une sortie PWM.
 *
 * @param[in]	PinId	Numero d'identification de la pin.
 *
 */
void TIM2_PWM_Desactiver(MappingGpio_e PinId)
{
	TIM_CCxCmd(TIM2, MappingGpio[PinId].PARAM, TIM_CCx_Disable);
	TIM_CtrlPWMOutputs(TIM2, DISABLE); // TIM2 et TIM8 seulement
}









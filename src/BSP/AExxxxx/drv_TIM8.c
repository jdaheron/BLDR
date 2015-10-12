/**
 *******************************************************************************
 * @file	drv_TIM8.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	09 dec. 2013
 * @brief   Gestion du Timer TIM8.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_TIM8.h"


/* External variables --------------------------------------------------------*/

static uint32_t TIM_Prescaler = 1;
static uint32_t TIM_Periode = 1;
static uint32_t SavePeriode_us = 0;
static uint8_t SaveRatio_pr100[4] = {0};
TIM_OCInitTypeDef TIM_OCInit[4];

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
void TIM8_Init(uint32_t Periode_us) {

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_ClocksTypeDef RCC_Clocks;
	uint32_t Clock_MHz;

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);

	// Activation horloges
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	//----------------------------------
	// Initialisation base de temps

	// Lecture horloge Bus
	RCC_GetClocksFreq(&RCC_Clocks);
	Clock_MHz = RCC_Clocks.PCLK2_Frequency;

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
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);

	TIM_OCStructInit(&TIM_OCInit[0]);
	TIM_OCStructInit(&TIM_OCInit[1]);
	TIM_OCStructInit(&TIM_OCInit[2]);
	TIM_OCStructInit(&TIM_OCInit[3]);

	//----------------------------------
	// Activation Timer
	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE); // TIM1 et TIM8 seulement

	// Sauvegarde
	SavePeriode_us = Periode_us;
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void TIM8_DeInit() {

	TIM_Cmd(TIM8, DISABLE);									// Desactivation peripherique
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Configuration d'une sortie PWM.
 *
 * @param[in]	PinId			Numero d'identification de la pin.
 * @param[in]	Etat			Etat de la sortie (Actif / Inactif).
 * @param[in]	Ratio_pr100		Ratio en pourcent.
 *
 */
void TIM8_PWM_Configurer(MappingGpio_e PinId, Etat_e Etat, uint8_t Ratio_pr100)
{
	uint32_t Pulse;
	uint8_t iConf;
	pFunction pInit;

	if (PinId >= NB_PORT_GPIO)
		return;

	// Configuration du canal en fonction du mapping
	switch (MappingGpio[PinId].PARAM)
	{
		case TIM_Channel_1:
			iConf = 0;
			pInit = (pFunction)TIM_OC1Init;
			break;

		case TIM_Channel_2:
			iConf = 1;
			pInit = (pFunction)TIM_OC2Init;
			break;

		case TIM_Channel_3:
			iConf = 2;
			pInit = (pFunction)TIM_OC3Init;
			break;

		case TIM_Channel_4:
			iConf = 3;
			pInit = (pFunction)TIM_OC4Init;
			break;

		default:
			return;
	}

	// Calcul Ratio
	if (Ratio_pr100 >= 100)
	{
		TIM_OCInit[iConf].TIM_Pulse = TIM_Periode;
	}
	else
	{
		Pulse  = Ratio_pr100 * TIM_Periode;
		Pulse /= 100;
		TIM_OCInit[iConf].TIM_Pulse = Pulse;
	}
	TIM_OCInit[iConf].TIM_OCMode = TIM_OCMode_PWM1;


	// Sortie standard
	if (MappingGpio[PinId].EtatInit == TIM_CCx_Enable)
	{
		// Definition de l'etat
		if (Etat == Etat_ACTIF)
			TIM_OCInit[iConf].TIM_OutputState = TIM_OutputState_Enable;
		else
			TIM_OCInit[iConf].TIM_OutputState = TIM_OutputState_Disable;

		// Inversion etat actif si necessaire
		if (MappingGpio[PinId].Inverse == FALSE)
			TIM_OCInit[iConf].TIM_OCPolarity = TIM_OCPolarity_High;
		else
			TIM_OCInit[iConf].TIM_OCPolarity = TIM_OCPolarity_Low;
	}
	else
	{// Sortie inverse

		// Definition de l'etat
		if (Etat == Etat_ACTIF)
			TIM_OCInit[iConf].TIM_OutputNState = TIM_OutputNState_Enable;
		else
			TIM_OCInit[iConf].TIM_OutputNState = TIM_OutputNState_Disable;

		// Inversion etat actif si necessaire
		if (MappingGpio[PinId].Inverse == FALSE)
			TIM_OCInit[iConf].TIM_OCNPolarity = TIM_OCNPolarity_Low;
		else
			TIM_OCInit[iConf].TIM_OCNPolarity = TIM_OCNPolarity_High;
	}

	SaveRatio_pr100[iConf] = Ratio_pr100;

	pInit(TIM8, &TIM_OCInit[iConf]);
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
void TIM8_PWM_ReadConfiguration(MappingGpio_e PinId, uint32_t* pPeriode_us, uint8_t* pRatio_pr100)
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
void TIM8_PWM_Activer(MappingGpio_e PinId)
{
	if (MappingGpio[PinId].EtatInit == TIM_CCx_Enable)
		TIM_CCxCmd(TIM8, MappingGpio[PinId].PARAM, MappingGpio[PinId].EtatInit);
	else
		TIM_CCxNCmd(TIM8, MappingGpio[PinId].PARAM, MappingGpio[PinId].EtatInit);
}


/**-----------------------------------------------------------------------------
 * @brief	Desactivation d'une sortie PWM.
 *
 * @param[in]	PinId	Numero d'identification de la pin.
 *
 */
void TIM8_PWM_Desactiver(MappingGpio_e PinId)
{
	if (MappingGpio[PinId].EtatInit == TIM_CCx_Enable)
		TIM_CCxCmd(TIM8, MappingGpio[PinId].PARAM, TIM_CCx_Disable);
	else
		TIM_CCxNCmd(TIM8, MappingGpio[PinId].PARAM, TIM_CCxN_Disable);

}


/**
 *******************************************************************************
 * @file	drv_ADC1.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	11 avr. 2013
 * @brief   Gestion de l'ADC1.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_ADC1.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define ADC1_RANGE_MAX_mV	3300
#define ADC1_RANGE_MAX_BITS	4096


/* Private macros ------------------------------------------------------------*/

#define _ADC1_CONSOLE	//_printf


/* Private variables ---------------------------------------------------------*/

static volatile ADC1_ConvStatus_e ConvStatus=ADC_Ready;
static volatile uint8_t CurrentID=0;
static volatile uint8_t SequenceID=0;
static volatile uint8_t SequenceEnCours=FALSE;
static volatile ADC1_ChannelConf_t ADC1_ChannelConf[ADC_NB_ChannelConf_MAX];
static uint8_t ADC1_NbChannelConf=0;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

int8_t prvADC1_GetNextActif();
uint32_t prvADCvalue_to_mV(uint32_t ulADCvalue);


/**-----------------------------------------------------------------------------
 * @brief	Tick ADC.
 *
 */
void ADC1_Tick() {

	// Verification pas de sequence en cours
	if (SequenceEnCours)
	{
		_ADC1_CONSOLE("--STO--\n");
		return;
	}

	// Verification ADC dispo
	if (ConvStatus != ADC_Ready)
		return;

	// Lancement d'une nouvelle sequence
	SequenceID = 0;
	if (prvADC1_GetNextActif() >= 0)
	{
		_ADC1_CONSOLE("--SS--\n");
		SequenceEnCours = TRUE;
		ADC1_StartConversion(SequenceID);

	}
}

/**-----------------------------------------------------------------------------
 * @brief	Initialisation de l'ADC1.
 *
 */
void ADC1_Init() {

	// Initialisation commune à tous les ADC
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonStructInit(&ADC_CommonInitStruct);
	ADC_CommonInitStruct.ADC_Mode				= ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler			= ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_DMAAccessMode		= ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);

	// Activation horloges
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Config peripheriqueENABLE
	ADC_InitTypeDef xADC_Init;
	ADC_StructInit(&xADC_Init);
	xADC_Init.ADC_Resolution			= ADC_Resolution_12b;
	xADC_Init.ADC_ScanConvMode			= DISABLE;
	xADC_Init.ADC_ContinuousConvMode	= DISABLE;
	xADC_Init.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;
	xADC_Init.ADC_ExternalTrigConv		= 0;
	xADC_Init.ADC_DataAlign				= ADC_DataAlign_Right;
	xADC_Init.ADC_NbrOfConversion		= 1;

	ADC_Init(ADC1, &xADC_Init);

	// Configuration interruption
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(ADC_IRQn);
	#endif

	// Initialisation des objets
	for (int i=0; i<ADC_NB_ChannelConf_MAX; i++) {
		ADC1_ChannelConf[i].ADCx				= 0;
		ADC1_ChannelConf[i].ADC_Channel			= 0;
		ADC1_ChannelConf[i].ConversionContinue	= FALSE;
		ADC1_ChannelConf[i].LastValue_mV		= 0;
		ADC1_ChannelConf[i].pStoreValue_mV		= 0;
		ADC1_ChannelConf[i].pFct_CallbackEOC	= 0;
	}
	ADC1_NbChannelConf = 0;
	ConvStatus = ADC_Ready;
	CurrentID = 0;
	SequenceID = 0;
	SequenceEnCours = FALSE;

	// Activation peripherique
	ADC_Cmd(ADC1, ENABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation de l'ADC1.
 *
 */
void ADC1_DeInit() {

	ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
	ADC_Cmd(ADC1, DISABLE);									// Desactivation peripherique
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);	// Desactivation horloge
}


/**-----------------------------------------------------------------------------
 * @brief	Configuration d'un  de l'ADC.
 *
 * @param[in]	ADC_Channel		Structure de configuration d'un  ADC.
 * @retval		ID_ADC_Channel	Numero de l'ID attribue au  configure.
 */
uint8_t ADC1_ChannelConfigure(ADC1_ChannelConf_t ADC_Channel) {

	ADC1_ChannelConf_t* pConf=0;
	uint8_t ID_ADC_Channel=0;
	uint8_t Exist=FALSE;

	// Verification des parametres d'entree
	if (ADC1_NbChannelConf >= ADC_NB_ChannelConf_MAX)
		return ADC_NB_ChannelConf_MAX;

	// On verifie que le canal n'est pas deja configure
	while ((ID_ADC_Channel < ADC1_NbChannelConf) && (Exist == FALSE)) {

		if ((ADC1_ChannelConf[ID_ADC_Channel].ADCx == ADC_Channel.ADCx)
		&&	(ADC1_ChannelConf[ID_ADC_Channel].ADC_Channel == ADC_Channel.ADC_Channel))
		{
			Exist = TRUE;
		}
		else
			ID_ADC_Channel++;
	}

	// Creation d'une nouvelle config si besoin
	if (Exist == FALSE)
	{
		ID_ADC_Channel = ADC1_NbChannelConf;

		// Incrementation du pointeur
		if (ADC1_NbChannelConf < ADC_NB_ChannelConf_MAX)
			ADC1_NbChannelConf++;

		// Copie de la configuration
		pConf = (ADC1_ChannelConf_t*) &(ADC1_ChannelConf[ID_ADC_Channel]);
		*pConf = ADC_Channel;
		pConf->LastValue_mV			= 0;
		pConf->ConversionContinue	= FALSE;
	}

	return ID_ADC_Channel;
}


/**-----------------------------------------------------------------------------
 * @brief	Activation de la conversion continue d'un ADC.
 *
 * @param[in]	ID_Channel		ID du canal concerne.
 */
void ADC1_ConversionContinue_Activer(uint8_t ID_Channel) {

	// Verification des parametres d'entree
	if (ID_Channel >= ADC1_NbChannelConf) return;

	// Ecriture de l'etat
	ADC1_ChannelConf[ID_Channel].ConversionContinue = TRUE;
}


/**-----------------------------------------------------------------------------
 * @brief	Activation de la conversion continue d'un ADC.
 *
 * @param[in]	ID_Channel		ID du canal concerne.
 */
void ADC1_ConversionContinue_Desactiver(uint8_t ID_Channel) {

	// Verification des parametres d'entree
	if (ID_Channel >= ADC1_NbChannelConf) return;

	// Ecriture de l'etat
	ADC1_ChannelConf[ID_Channel].ConversionContinue = FALSE;
}


/**-----------------------------------------------------------------------------
 * @brief	Lancement de la sequence de conversion sur l'ADC.
 *
 * @param[in]	ID_Channel	ID du  concerne.
 */
void ADC1_StartConversion(uint8_t ID_Channel) {

	// Atente ADC dispo
	uint32_t timeout = 0xFFFF;
	while ((ADC1_GetConversionStatus() != ADC_Ready) && (timeout--)) __NOP();

	if (!timeout)
		return;

	ADC_RegularChannelConfig(
			ADC1_ChannelConf[ID_Channel].ADCx,
			ADC1_ChannelConf[ID_Channel].ADC_Channel,
			1,
			ADC_SampleTime_3Cycles
	);

	_ADC1_CONSOLE("SC\n");
	CurrentID = ID_Channel;
	ConvStatus = ADC_ConvEnCours;

	ADC_SoftwareStartConv(ADC1_ChannelConf[ID_Channel].ADCx);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du Status de l'ADC.
 *
 * @return	ConvStatus		Status de la conversion.
 */
ADC1_ConvStatus_e ADC1_GetConversionStatus() {
	return ConvStatus;
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'une valeur de conversion de l'ADC.
 *
 * @param[in]	ID_Channel	ID du  concerne.
 */
uint32_t ADC1_Get_mV(uint8_t ID_Channel) {

	return ADC1_ChannelConf[ID_Channel].LastValue_mV;
}


/**-----------------------------------------------------------------------------
 * @brief	ADC_IRQHandler.
 *
 */
void ADC_IRQHandler(void) {

	if (ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET) {

		// Sauvegarde de la nouvelle valeur d'acquistion
		ADC1_ChannelConf[CurrentID].LastValue_mV = prvADCvalue_to_mV(ADC_GetConversionValue(ADC1));

		// Stockage "distant" et appel fonction callback
		if (ADC1_ChannelConf[CurrentID].pStoreValue_mV != 0)
			*(ADC1_ChannelConf[CurrentID].pStoreValue_mV) = ADC1_ChannelConf[CurrentID].LastValue_mV;
		if (ADC1_ChannelConf[CurrentID].pFct_CallbackEOC != 0)
			ADC1_ChannelConf[CurrentID].pFct_CallbackEOC(ADC1_ChannelConf[CurrentID].LastValue_mV);

		_ADC1_CONSOLE("EC\n");
		ConvStatus = ADC_Ready;

		if (SequenceEnCours)
		{
			// Lancement conversion suivante
			SequenceID++;
			if (prvADC1_GetNextActif() >= 0)
				ADC1_StartConversion(SequenceID);
			else
			{
				_ADC1_CONSOLE("--ES--\n");
				SequenceEnCours = FALSE;
			}
		}
	}
}

/**-----------------------------------------------------------------------------
 * @brief	Lancement de la conversion sur le  suivant.
 *
 * @retval	ucID_ADC_Channel		Numero de l'ID attrubue au  configure ((-1) si aucun).
 */
int8_t prvADC1_GetNextActif() {

	uint8_t Found=FALSE;

	// Recherche du  suivant a lancer
	while ((SequenceID < ADC1_NbChannelConf) && (Found == FALSE)) {

		if (ADC1_ChannelConf[SequenceID].ConversionContinue == Etat_ACTIF)
			Found = TRUE;
		else
			SequenceID++;
	}

	// Lancement de la conversion si  trouve
	if (Found)
		return Found;

	return (-1);
}


/**-----------------------------------------------------------------------------
 * @brief	Conversion valeur d'ADC en mV.
 *
 * @param[in]	ulADCvalue	Valeur de l'ADC.
 * @retval		ulAdc_mV	Valeur en mV.
 */
uint32_t prvADCvalue_to_mV(uint32_t ADCvalue) {

	uint32_t Adc_mV;

	Adc_mV = ADCvalue;
	Adc_mV *= ADC1_RANGE_MAX_mV;
	Adc_mV /= (ADC1_RANGE_MAX_BITS - 1);

	return Adc_mV;
}


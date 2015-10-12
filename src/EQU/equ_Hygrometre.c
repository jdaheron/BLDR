/**
 ***************************************************************************************************
 * @file	equ_Hygrometre.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	21 sept. 2014
 * @brief   Gestion de l'Hygrometrie.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "equ_Hygrometre.h"


/* External Variables *****************************************************************************/


/** @addtogroup equ_Hygrometre
 * @{
 */ 

 
/** 
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

/**
 * @}
 */ 

 
/** 
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */
 
#define NB_ECHANTILLONS		100


/**
 * @}
 */ 

 
/** 
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */
  
/**
 * @}
 */ 

  
/** 
 ***************************************************************************************************
 * @defgroup Private_FunctionPrototypes Private FunctionPrototypes
 * @{
 */


/**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Private_Variables Private Variables
 * @{
 */
 
static uint32_t Acq_Values[NB_ECHANTILLONS] = {0};


/**
 * @}
 */ 
  	
	
/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 


/**
 ***************************************************************************************************
 * @brief	Ajout d'une valeur d'acquisition.
 * @return 	void.
 */
void
AddAcquisitionValue(
		uint32_t ADC_Value		/**<[in] Valeur d'ADC.*/
)
{
	static int iBuffer = 0;

	Acq_Values[iBuffer] = ADC_Value;

	iBuffer++;
	if (iBuffer >= NB_ECHANTILLONS)
		iBuffer = 0;
}

 
/**
 * @}
 */ 
 

/** 
 ***************************************************************************************************
 * @brief	Initialisation de l'acquisition d'Hygrometrie.
 * @return 	void
 */
void
Hygrometre_Init(
		void
)
{
	ADC1_ChannelConf_t 	ADC_Channel = {0};
	MappingGpio_s 		PinConfig = {0};
	uint32_t			ID_ADC_Channel;
	uint32_t 			Index = 0;
	uint32_t 			port;

	PinConfig = GPIO_GetPinConfiguration(PORT_TRIM);

	// Effacement du buffer de filtrage
	for (int i=0; i<NB_ECHANTILLONS; i++)
		Acq_Values[i] = 0;

	// Initialisation de l'ADC
	ADC_Channel.ADCx				= (ADC_TypeDef *) PinConfig.PERIPH;
	ADC_Channel.ADC_Channel			= PinConfig.PARAM;
	ADC_Channel.pStoreValue_mV		= NULL;
	ADC_Channel.pFct_CallbackEOC	= AddAcquisitionValue;
	ID_ADC_Channel = ADC1_ChannelConfigure(ADC_Channel);
	ADC1_ConversionContinue_Activer(ID_ADC_Channel);
}


/**
 ***************************************************************************************************
 * @brief	Lecture de la valeur actuelle d'Hygrometrie.
 * @return 	Hygrometrie en pr100.
 */
uint16_t
Hygrometre_GetValue(
		void
)
{
	uint32_t MoyenneADC = 0;

	// Moyennage
	for (int i=0; i<NB_ECHANTILLONS; i++)
		MoyenneADC += Acq_Values[i];

	MoyenneADC /= NB_ECHANTILLONS;


	return (uint16_t) MoyenneADC;
}


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

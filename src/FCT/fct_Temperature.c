/**
 ***************************************************************************************************
 * @file	fct_Temperature.h
 * @author	Julien
 * @version	1.0.0
 * @date	12 oct. 2014
 * @brief   Gestion de la temperature.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "fct_Temperature.h"
#include "drv_MAX6675.h"


/* External Variables *****************************************************************************/


/** @addtogroup fct_Temperature
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
 
#define TEMPERATURE_OFFSET_DegC100	(-140)
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

static Temperature_s Temp;

/**
 * @}
 */ 
  	
	
/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 
 


void Temperature_Read()
{
	int16_t CurrentTemp;

	CurrentTemp = MAX6675_GetTemperature();

	if (CurrentTemp == MAX6675_TEMP_NON_VALIDE)
	{
		Temp.IsValide = FALSE;
	}
	else
	{
		if (Temp.IsValide == FALSE)
		{
			for (uint16_t i=0; i<TEMPERATURE_NB_ECH; i++)
				Temp.TableAcquisition[i] = CurrentTemp + TEMPERATURE_OFFSET_DegC100;
			Temp.iAcquisition = 0;
			Temp.IsValide = TRUE;
		}

		Temp.TableAcquisition[Temp.iAcquisition] = CurrentTemp + TEMPERATURE_OFFSET_DegC100;

		Temp.iAcquisition++;
		if (Temp.iAcquisition >= TEMPERATURE_NB_ECH)
			Temp.iAcquisition = 0;
	}
}

 /**
 * @}
 */ 
 

/**
 ***************************************************************************************************
 * @brief	Initialisation du gestionnaire de temperature.
 * @return 	void
 */
void
Temperature_Init(
		uint32_t PeriodeAcquisition_ms
)
{
	// Init des variables
	Temp.IsValide				= FALSE;
	Temp.CurrentTemp_DegC100	= 0;
	Temp.iAcquisition			= 0;
	Temp.PeriodeAcquisition_ms	= PeriodeAcquisition_ms;

	// Premiere acquisition
	for (uint16_t i=0; i<TEMPERATURE_NB_ECH; i++)
		Temperature_Read();

	// Lancement du Timer d'acquisition
	TSW_Start(&Temp.TmrAcquisition, Temp.PeriodeAcquisition_ms);
}

/**
 ***************************************************************************************************
 * @brief	Thread du gestionnaire de temperature.
 * @return 	void
 */
void
Temperature_Thread(
	void
)
{
	if (TSW_IsRunning(&Temp.TmrAcquisition) == FALSE)
	{
		Temperature_Read();
		TSW_ReStart(&Temp.TmrAcquisition);
	}
}

/**
 ***************************************************************************************************
 * @brief	Verification de la validite de la temperature.
 * @return 	void
 */
Bool_e
Temperature_IsValide(
	void
)
{
	return Temp.IsValide;
}

/**
 ***************************************************************************************************
 * @brief	Verification de la validite de la temperature.
 * @return 	void
 */
int16_t
Temperature_Get_DegC100(
	void
)
{
	int32_t Moyenne;
	if (Temp.IsValide == FALSE)
	{
		return (-1);
	}

	Moyenne = 0;
	for (uint16_t i=0; i<TEMPERATURE_NB_ECH; i++)
		Moyenne += Temp.TableAcquisition[i];
	Moyenne /= TEMPERATURE_NB_ECH;

	return (int16_t) Moyenne;
}





 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

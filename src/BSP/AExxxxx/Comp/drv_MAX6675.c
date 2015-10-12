/**
 ***************************************************************************************************
 * @file	drv_MAX6675.c
 * @author	Julien
 * @version	1.0.0
 * @date	11 oct. 2014
 * @brief   Gestion du convertisseur Thermocouple K MAX6675.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "drv_MAX6675.h"


/* External Variables *****************************************************************************/


/** @addtogroup drv_MAX6675
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
 
/**
 * @}
 */ 
  	
	
/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 
 
 /**
 * @}
 */ 
 


/**
 ***************************************************************************************************
 * @brief	Lecture de la temperature sur le MAX6675.
 * @return 	Temperature en 100eme de Degres Celsuis.
 * 			Si KO, retourne MAX6675_TEMP_NON_VALIDE
 */
int16_t
MAX6675_GetTemperature(
		void
)
{
	uint16_t RegistreValue;
	int16_t Valeur_DegC100;

	HAL_MAX6675_Select();
	Delay_ms(1);

	RegistreValue = HAL_MAX6675_Read();
	RegistreValue <<= 8;
	RegistreValue |= HAL_MAX6675_Read();

	HAL_MAX6675_Deselect();

	// Verif TK present
	if (RegistreValue & 0x4)
	{
		return MAX6675_TEMP_NON_VALIDE;
	}

	RegistreValue >>= 3;
	RegistreValue &= 0x0FFF;

	Valeur_DegC100 = 25 * RegistreValue;

	return Valeur_DegC100;
}




 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

/**
 ***************************************************************************************************
 * @file	equ_ThermocoupleK.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	21 sept. 2014
 * @brief   Gestion de Thermocouple K.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "equ_ThermocoupleK.h"


/* External Variables *****************************************************************************/


/** @addtogroup equ_ThermocoupleK
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
 * @brief	Initialisation de l'acquisition de temperature.
 * @return 	void
 */
void
ThermocoupleK_Init(
		void
)
{

}


/**
 ***************************************************************************************************
 * @brief	Lecture de la valeur actuelle de temperature.
 * @return 	Temperature en Degres Celius.
 */
int16_t
ThermocoupleK_GetTemperature(
		void
)
{
	uint16_t ValeurDegC;

	HAL_TempK_Select();
	Delay_ms(1);

	ValeurDegC = HAL_TempK_Read();
	ValeurDegC <<= 8;
	ValeurDegC |= HAL_TempK_Read();

	HAL_TempK_Deselect();

	// Verif TK present
	if (ValeurDegC & 0x4)
	{
		return -100;
	}

	ValeurDegC >>= 3;
	return (ValeurDegC * 0.25);
}


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

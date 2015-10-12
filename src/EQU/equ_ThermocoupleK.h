/**
 ***************************************************************************************************
 * @file	equ_ThermocoupleK.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	21 sept. 2014
 * @brief   Gestion de Thermocouple K.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef EQU_THERMOCOUPLEK_H_
#define EQU_THERMOCOUPLEK_H_


/* Includes ***************************************************************************************/

#include "BSP.h"


/** @defgroup equ_ThermocoupleK equ_ThermocoupleK
  * @brief Gestion de Thermocouple K.
  * @{
  */ 

/** 
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */
 
/**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */


/**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Exported_Macros Exported Macros 
 * @{
 */
 
/**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Exported_Variables Exported Variables
 * @{
 */
 
/**
 * @}
 */ 

 
/** 
 ***************************************************************************************************
 * @defgroup Exported_Functions Exported Functions  
 * @{
 */


/**
 * @brief	Initialisation de l'acquisition de temperature.
 * @return 	void
 */
void
ThermocoupleK_Init(
		void
);

/**
 * @brief	Lecture de la valeur actuelle de temperature.
 * @return 	Temperature en Degres Celius.
 */
int16_t
ThermocoupleK_GetTemperature(
		void
);
	 
/**
 * @}
 */ 
 
 
/**
 * @}
 */ 


#endif /* EQU_THERMOCOUPLEK_H_ */

/**
 ***************************************************************************************************
 * @file	drv_MAX6675.c
 * @author	Julien
 * @version	1.0.0
 * @date	11 oct. 2014
 * @brief   Gestion du convertisseur Thermocouple K MAX6675.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef DRV_MAX6675_H_
#define DRV_MAX6675_H_


/* Includes ***************************************************************************************/

#include "BSP.h"


/** @defgroup drv_MAX6675 drv_MAX6675
  * @brief Gestion du convertisseur Thermocouple K MAX6675.
  * @{
  */ 

/** 
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */

#define MAX6675_TEMP_NON_VALIDE	(-10000)
 
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

int16_t MAX6675_GetTemperature(void);

	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* DRV_MAX6675_H_ */

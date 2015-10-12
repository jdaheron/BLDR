/**
 ***************************************************************************************************
 * @file	fct_Temperature.h
 * @author	Julien
 * @version	1.0.0
 * @date	12 oct. 2014
 * @brief   Gestion de la temperature.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef FCT_TEMPERATURE_H_
#define FCT_TEMPERATURE_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "util_TSW.h"


/** @defgroup fct_Temperature fct_Temperature
  * @brief Gestion de la temperature.
  * @{
  */ 

/** 
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */

#define TEMPERATURE_NB_ECH		15


 /**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */


/** Definition breve de la structure. */
typedef struct {

	Bool_e		IsValide;
	int16_t		CurrentTemp_DegC100;
	int16_t		TableAcquisition[TEMPERATURE_NB_ECH];
	uint16_t	iAcquisition;
	uint32_t	PeriodeAcquisition_ms;
	TSW_s		TmrAcquisition;

} Temperature_s;


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

void	Temperature_Init(uint32_t PeriodeAcquisition_ms);
void	Temperature_Thread(void);
Bool_e	Temperature_IsValide(void);
int16_t	Temperature_Get_DegC100(void);
	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* FCT_TEMPERATURE_H_ */

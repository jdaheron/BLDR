/**
 ***************************************************************************************************
 * @file	mod_Demarrage.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	25 aout 2014
 * @brief   Gestion du mode Demarrage.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef MOD_DEMARRAGE_H_
#define MOD_DEMARRAGE_H_


/* Includes ***************************************************************************************/

#include "BSP.h"


/** @defgroup mod_Demarrage mod_Demarrage
  * @brief Composant permettant la gestion du mode Demarrage de l'applicatif Master
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
* -------------------------------------------------------------------------------------------------
* @brief	Initialisation du Mode Demarrage
* @return	void
*/
void
Demarrage_Init(
		void
);


/**
* -------------------------------------------------------------------------------------------------
* @brief	Fonction permettant la gestion du mode Demarrage
* @return	Status
*/
Status_e
Demarrage(
		uint32_t AppAddress
);






 /**
 * @}
 */



#endif /* MOD_DEMARRAGE_H_ */

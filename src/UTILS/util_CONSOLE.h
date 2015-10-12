/**
 ***************************************************************************************************
 * @file	util_CONSOLE.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	26 sept. 2014
 * @brief   Gestion de la Console.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef UTIL_CONSOLE_H_
#define UTIL_CONSOLE_H_


/* Includes ***************************************************************************************/

#include "BSP.h"



/** @defgroup util_CONSOLE util_CONSOLE
  * @brief Gestion de la Console.
  * @{
  */

/**
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */

#define NO_LOG		((const char*)(-1))
#define NO_NAME		((const char*)(0))

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
 * @brief	Definition breve de la fonction Foo.
 * @return 	void
 */
void
_CONSOLE(
		const char*	Name,			/**<[in] Nom du Log (0=Pas de nom; (-1)=Log desactive).*/
		const char* Chaine,			/**<[in] Chaine de log.*/
		...							/**<[in] Parametres pour la chaine.*/
);


 /**
 * @}
 */


 /**
 * @}
 */


#endif /* UTIL_CONSOLE_H_ */

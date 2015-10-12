/**
 ***************************************************************************************************
 * @file	util_LogFile.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	29 juil. 2014
 * @brief   Gestion du fichier de log.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef UTIL_LOGFILE_H_
#define UTIL_LOGFILE_H_


/* Includes ***************************************************************************************/

#include "BSP.h"



/** @defgroup util_LogFile util_LogFile
  * @brief Gestion du fichier de log.
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
 * @brief	Ecriture dans le fichier de log.
 * @return 	void
 */
void
LogFile_Write(
		char*	ComposantName,		/**<[in] Nom du composant.*/
		uint8_t	LogLevel,			/**<[in] Niveau de Log.*/
		char*	str					/**<[in] chaine a logger.*/
);


/**
 * @brief	Envoi du contenu du log vers la console.
 * @return 	void
 */
void
LogFile_Print(
		void
);

	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* UTIL_LOGFILE_H_ */

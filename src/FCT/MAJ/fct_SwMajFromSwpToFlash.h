/**
 ***************************************************************************************************
 * @file	fct_SwMajFromSwpToFlash.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	12 sept. 2014
 * @brief   Gestion de la MAJ d'une carte depuis un SWP vers la flash interne.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef FCT_SWMAJFROMSWPTOFLASH_H_
#define FCT_SWMAJFROMSWPTOFLASH_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "fct_SwHex.h"


/** @defgroup fct_SwMajFromSdToFlash fct_SwMajFromSdToFlash
  * @brief Gestion de la MAJ d'une carte depuis un SWP vers la flash interne.
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
 * @brief	Definition breve de la fonction Foo.
 * @return 	Status
 */
Status_e
SwMajFromSwpToFlash_Main(
		char* Filename			/**<[in] Nom du fichier d'entree. */
);

/**
 * @brief	Lecture de l'etat d'avancement de la MAJ.
 * @return 	Avancement (pr1000)
 */
uint16_t
SwMajFromSwpToFlash_GetAvancement_pr1000(
		void
);

	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* FCT_SWMAJFROMSWPTOFLASH_H_ */

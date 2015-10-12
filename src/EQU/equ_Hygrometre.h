/**
 ***************************************************************************************************
 * @file	equ_Hygrometre.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	21 sept. 2014
 * @brief   Gestion de l'Hygrometrie.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef EQU_HYGROMETRE_H_
#define EQU_HYGROMETRE_H_


/* Includes ***************************************************************************************/

#include "BSP.h"


/** @defgroup equ_Hygrometre equ_Hygrometre
  * @brief Gestion de l'Hygrometrie.
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
 * @brief	Initialisation de l'acquisition d'Hygrometrie.
 * @return 	void
 */
void
Hygrometre_Init(
		void
);

/**
 * @brief	Lecture de la valeur actuelle d'Hygrometrie.
 * @return 	Hygrometrie en pr100.
 */
uint16_t
Hygrometre_GetValue(
		void
);
	 
/**
 * @}
 */ 
 
 
/**
 * @}
 */ 


#endif /* EQU_HYGROMETRE_H_ */

/**
 *******************************************************************************
 * @file	BSP.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	19 mars 2013
 * @brief   Board Support Package
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_H_
#define BSP_H_


/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


/* Exported types ------------------------------------------------------------*/

/** Pointeur de fonction */
typedef int (*pFunction)(void*, ...);

/** Status fonctions */
typedef enum{

	Status_OK = 0,
	Status_KO,
	Status_EnCours,
	Status_Fini,

} Status_e;


/** Etats */
typedef enum{

	Etat_INACTIF = 0,
	Etat_ACTIF

} Etat_e;


/** Booleen */
typedef enum{

	FALSE = 0,
	TRUE

} Bool_e;

/** Booleen Non/oui */
typedef enum{

	OUI = TRUE,
	NON = FALSE

} NONOUI_e;

/* Structure ---------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

#undef NULL
#ifndef NULL
	#define NULL	0
#endif


/* Exported macro ------------------------------------------------------------*/

#define __PACKED	__attribute__ ((packed))
#define __IN_FLASH	__attribute__ ((section(".text")))


/* Exported functions ------------------------------------------------------- */


// Les fonctions du BSP sont inclues dans un fichier spécifique à la carte.

#define CONSOLE_ENABLE	0

#include "AExxxxx/BSP_CARTE.h"


#endif /* BSP_H_ */






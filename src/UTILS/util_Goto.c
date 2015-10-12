/**
 *******************************************************************************
 * @file	util_Goto.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	18 dec. 2013
 * @brief   Fonction Goto.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "util_Goto.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**-----------------------------------------------------------------------------
 * @brief	Goto.
 *
 * @param[in]	Address		Adresse cible.
 *
 */
Status_e GOTO(uint32_t Address)
{
	pFunction Jump_To_Application;
	uint32_t JumpAddress;

	// On verifie que le la table de vecteurs se trouve dans la RAM
	if (((*(uint32_t*) Address) & 0x2FFD0000 ) == 0x20000000){

		JumpAddress = *(uint32_t*) (Address + 4);		// Configuration du pointeur de fonction
		Jump_To_Application = (pFunction) JumpAddress;
		__disable_irq();								// Desactivation des interruptions

		BSP_DeInit();									// ReInitialisation du uC

		__set_MSP(*(uint32_t*) Address);				// Initialisation du Stack Pointer
		Jump_To_Application(0);							// JUMP

		return Status_OK;
	}

	return Status_KO;
}


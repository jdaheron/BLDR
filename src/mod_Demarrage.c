/**
 ***************************************************************************************************
 * @file	mod_Demarrage.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	25 aout 2014
 * @brief   Gestion du mode Demarrage.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "mod_Demarrage.h"

#include "util_TSW.h"
#include "util_printf.h"
#include "util_Goto.h"


/* External Variables *****************************************************************************/


/** @addtogroup mod_Demarrage
  * @{
  */


 /**
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

/** Liste des Etapes du mode. */
typedef enum
{
	Demarrage_Etape_Init = 0,

	Demarrage_Etape_AttenteFinTempo,
	Demarrage_Etape_SystemeModeDemarrage,
	Demarrage_Etape_StartApplication,

} Demarrage_Etape_e;


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */

#define DUREE_TEMPO_DEMARRAGE_ms	300


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */

/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_FunctionPrototypes Private FunctionPrototypes
 * @{
 */



/**
 * @}
 */


  /**
 ***************************************************************************************************
 * @defgroup Private_Variables Private Variables
 * @{
 */

static Demarrage_Etape_e Etape = Demarrage_Etape_Init;
static TSW_s TmrDemarrage;


/**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */

/**
 * @}
 */



/**
****************************************************************************************************
* TODO VALIDATION ...
*/
void
Demarrage_Init(
		void
)
{
	TSW_Start(&TmrDemarrage, DUREE_TEMPO_DEMARRAGE_ms);
}



/**
****************************************************************************************************
* TODO VALIDATION ...
*/
Status_e
Demarrage(
		uint32_t AppAddress
)
{
	Status_e Status = Status_EnCours;

	switch (Etape)
	{
		//--------------------------------------------------------------
		case Demarrage_Etape_Init:

			Etape = Demarrage_Etape_AttenteFinTempo;
			break;

		//--------------------------------------------------------------
		case Demarrage_Etape_AttenteFinTempo:

			if (TSW_IsRunning(&TmrDemarrage))
				break;

			Etape = Demarrage_Etape_SystemeModeDemarrage;

			break;

		//--------------------------------------------------------------
		case Demarrage_Etape_SystemeModeDemarrage:

			Etape = Demarrage_Etape_StartApplication;

			break;

		//--------------------------------------------------------------
		case Demarrage_Etape_StartApplication:

			_printf("START APPLICATION 0x%08X\n\n\n", AppAddress);
			TSW_Delay(5);
			GOTO(AppAddress);

			Status = Status_KO;
			break;

		//--------------------------------------------------------------
		default:
			Etape = Demarrage_Etape_Init;
			break;
	}

	// Gestion de la sortie du Mode
	if (Status != Status_EnCours)
	{
		Etape = Demarrage_Etape_Init;
	}

	return Status;
}


 /**
 * @}
 */

/* End Of File ************************************************************************************/


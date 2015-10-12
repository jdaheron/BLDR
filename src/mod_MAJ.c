/**
 ***************************************************************************************************
 * @file	mod_MAJ.c
 * @author	f.larthomas
 * @version	1.0.0
 * @date	17 juil. 2014
 * @brief   Composant permettant la gestion du mode MAJ de l'applicatif Master
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "mod_MAJ.h"
#include "util_printf.h"
#include "util_TSW.h"

#include "fct_SwMajFromSwpToFlash.h"
#include "fct_SwFlash.h"


/* External Variables *****************************************************************************/


/** @addtogroup mod_MAJ
  * @{
  */


 /**
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */

#define CARTE_MASTER 0


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

static uint16_t AffichAvancement=1000;
static char Filename[255] = "";
static char FileVersion[255] = "";


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
Status_e
MAJ(
		MajInfo_s* pMajInfo
)
{
	static MAJ_Etape_e Etape = MAJ_Initialisation;
	static Bool_e MasterMiseAJour;
	Status_e Status = Status_EnCours;

	switch (Etape)
	{
		//--------------------------------------------------------------
		case MAJ_Initialisation:

			_printf("------------------------------\n");
			_printf("Mode MAJ\n");

			appMAJ_Init(pMajInfo);
			MasterMiseAJour = FALSE;

			// Recuperation des infos Master
			strcpy(pMajInfo->SW_Carte[CARTE_MASTER].V_Carte, SwFlash_GetAppSwVersion());


			Etape = MAJ_FindHex;
//			if (pMajInfo->BootFromAPP == TRUE)
//			{
//				if ((pMajInfo->ForcageMAJ == TRUE) || (pMajInfo->SW_Carte[CARTE_MASTER].MAJ_Requise == TRUE))
//					Etape = MAJ_SystemModeAttente;
//				else
//					Etape = MAJ_StartMasterWithNewSw;
//			}
//			else
//				Etape = MAJ_ReadSwpInfo;

			break;

		//--------------------------------------------------------------
		case MAJ_FindHex:


			switch (appMAJ_FindHexFile("AL", Filename))
			{
				case Status_OK:
					_printf("Fichier hex trouve : %s\n", Filename);

					memset(pMajInfo->SW_Carte[CARTE_MASTER].V_Memoire, 0, VERSION_SW_SIZE);
					strncpy(pMajInfo->SW_Carte[CARTE_MASTER].V_Memoire, Filename, strlen(Filename) - 4);	// Suppression extension
					_printf("V_Memoire : %s\n", pMajInfo->SW_Carte[CARTE_MASTER].V_Memoire);

					Etape = MAJ_VerifNecessiteMAJ;
					break;

				default:
					Etape = MAJ_TryToStartMaster;
					_printf("Fichier hex non trouve\n");
					break;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_VerifNecessiteMAJ:

			// Comparaison des versions
			if (strcmp(pMajInfo->SW_Carte[CARTE_MASTER].V_Carte, pMajInfo->SW_Carte[CARTE_MASTER].V_Memoire) != 0)
			{
				_printf("MAJ necessaire.\n");
				Etape = MAJ_UpdateMaster;
			}
			else
				Etape = MAJ_TryToStartMaster;

			break;

		//--------------------------------------------------------------
		case MAJ_TryToStartMaster:

			switch (SwFlash_PreparerApplication())
			{
				case Status_OK:
					Etape = MAJ_Status_OK;
					break;

				case Status_KO:
					_printf("MAJ_TryToStartMaster KO...\n");
					Etape = MAJ_UpdateMaster;
					break;

				default:
					break;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_UpdateMaster:

			switch (SwMajFromSwpToFlash_Main(Filename))
			{
				case Status_OK:
					MasterMiseAJour = TRUE;
					Etape = MAJ_StartMasterWithNewSw;
					break;

				case Status_KO:
					Etape = MAJ_Status_KO;
					break;

				default:
					break;
			}

//			pMajInfo->SW_Carte[CARTE_MASTER].Avancement_pr1000 = SwMajFromSwpToFlash_GetAvancement_pr1000();
//
//			if (AffichAvancement != pMajInfo->SW_Carte[CARTE_MASTER].Avancement_pr1000 / 10)
//			{
//				AffichAvancement = pMajInfo->SW_Carte[CARTE_MASTER].Avancement_pr1000 / 10;
//				_printf("Avancement = %d %c\n", AffichAvancement, '%');
//			}

			break;

		//--------------------------------------------------------------
		case MAJ_StartMasterWithNewSw:

			switch (SwFlash_PreparerApplication())
			{
				case Status_OK:
					Etape = MAJ_Status_OK;
					break;

				case Status_KO:
					Etape = MAJ_Status_KO;
					break;

				default:
					break;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_Status_OK:
			_printf("MAJ OK\n");
			Status = Status_OK;
			break;

		//--------------------------------------------------------------
		case MAJ_Status_KO:
			_printf("MAJ KO\n");
			Status = Status_KO;
			break;

		//--------------------------------------------------------------
		default:
			Etape = MAJ_Initialisation;
			break;

	}


	// Gestion de la sortie du Mode
	if (Status != Status_EnCours)
	{
		_printf("FIN MAJ\n");

		Etape = MAJ_Initialisation;

		if ((MasterMiseAJour == TRUE) && (Status == Status_OK))
			return Status_Fini;

		return Status;
	}

	return Status;
};



 /**
 * @}
 */

/* End Of File ************************************************************************************/

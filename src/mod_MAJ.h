/**
 ***************************************************************************************************
 * @file	mod_MAJ.h
 * @author	f.larthomas
 * @version	1.0.0
 * @date	17 juil. 2014
 * @brief   Composant permettant la gestion du mode MAJ de l'applicatif Master
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef MOD_MAJ_H_
#define MOD_MAJ_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "app_MAJ.h"



/** @defgroup mod_MAJ mod_MAJ
  * @brief Composant permettant la gestion du mode MAJ de l'applicatif Master
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

/** Liste des sorties de modes . */
typedef enum
{
	MAJ_Initialisation = 0,

	MAJ_FindHex,
	MAJ_VerifNecessiteMAJ,
	MAJ_SystemModeAttente,

	MAJ_VerifSWP,

	MAJ_UpdateMaster,

	MAJ_TryToStartMaster,
	MAJ_StartMasterWithNewSw,

	MAJ_Status_OK,
	MAJ_Status_KO,

	MAJ_Fin,

} MAJ_Etape_e;



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
* @brief	Fonction permettant la gestion du mode MAJ
* @return	Status (OK/KO/EnCours).
*/
Status_e
MAJ(
		MajInfo_s* pMajInfo
);


 /**
 * @}
 */



#endif /* MOD_MAJ_H_ */

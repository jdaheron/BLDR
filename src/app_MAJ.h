/**
 ***************************************************************************************************
 * @file	app_MAJ.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	25 août 2014
 * @brief   Gestion des MAJ
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef APP_MAJ_H_
#define APP_MAJ_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "fct_SwHex.h"


/** @defgroup app_MAJ app_MAJ
  * @brief Gestion des MAJ.
  * @{
  */ 

/** 
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */
 
#define NOM_BASE_SWP			"GMAX_*"
#define VERSION_SW_SIZE			14	// ex:"AL00056AAA"
#define VERSION_SW_BASE_SIZE	7	// ex:"AL00056"


 /**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */

/** Definition d'un SW d'une carte. */
typedef struct {

	char		V_Base[VERSION_SW_SIZE+2];		/**< Version de base.*/
	char		V_Carte[VERSION_SW_SIZE+2];		/**< Version lue sur la carte.*/
	char		V_Memoire[VERSION_SW_SIZE+2];	/**< Version contenue dans la carte SD.*/
	Bool_e		IsPresente;						/**< status de presence de la carte.*/
	Bool_e		MAJ_Requise;					/**< Flag de MAJ requise.*/
	uint32_t	Taille_MAJ;						/**< Taille de la MAJ.*/
	uint8_t		iSwp;							/**< Indice du hex dans le SWP.*/
	uint16_t	Avancement_pr1000;				/**< Avancement de la MAJ.*/

} SW_Carte_s;

/** Definition de l'appMAJ. */
typedef struct {

	Bool_e			ForcageMAJ;				/**< Flag de forcage de MAJ.*/
	Bool_e			BootFromAPP;			/**< Flag indiquant un reboot demande par l'APP.*/
	Swp_s			Swp;					/**< Donnees du Software Package.*/
	uint32_t		SwpCheckTS;				/**< Timestamp de verification du SWP.*/
	SW_Carte_s		SW_Carte[5];			/**< Donnees des cartes du systeme.*/

} MajInfo_s;


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
 * @brief	Initialisation d'une structure de gestion de MAJ.
 * @return 	void.
 */
void
appMAJ_Init(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
);

/**
 * @brief	Verification des informations du SWP.
 * @return 	Status (OK/KO/EnCours)
 */
Status_e
appMAJ_CheckSWPInfo(
		MajInfo_s* pMajInfo,		/**<[in] Informations de MAJ.*/
		char* FileName_Prefix		/**<[in] Prefix du fichier recherche.*/
);

/**
 * @brief	Recherche d'un fichier HEX.
 * @return 	Status (OK/KO)
 */
Status_e
appMAJ_FindHexFile(
		char* FileName_Prefix,		/**<[in] Prefix du fichier recherche.*/
		char* FoundFilename		/**<[Out] Fichier trouve.*/
);

/**
 * @brief	Association de le version de base a la version memoire.
 * @return 	void
 */
void
appMAJ_Associer_VBase_VMemoire(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
);

/**
 * @brief	Verification de la necessite de mettre a jour (au moins une carte).
 * @return 	TRUE/FALSE
 */
Bool_e
appMAJ_IsMajNeeded(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
);

/**
 * @brief	Impression des informations du gestionnaire de MAJ.
 * @return 	void
 */
void
appMAJ_PrintInfos(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
);

/**
 * @brief	Sauvegarde des donnees du gestionnaire de MAJ.
 * @return 	void
 */
void
appMAJ_SaveMajInfo(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
);



 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* APP_MAJ_H_ */

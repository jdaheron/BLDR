/**
 ***************************************************************************************************
 * @file	app_MAJ.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	25 août 2014
 * @brief   Gestion des MAJ.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "app_MAJ.h"

#include "fct_ihex.h"
#include "fct_MemoireFAT.h"
//#include "fct_BL_CAN_Host.h"
//#include "fct_BL.h"

#include "util_printf.h"
#include "util_TSW.h"


/* External Variables *****************************************************************************/


/** @addtogroup app_MAJ
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


#define NB_CARTES	5

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

#if 0
#define NB_SECTORS				12	// TODO JD


/** Definition de l'Objet UpdateSW. */
typedef struct {

	uint8_t		FileName[32];
	FIL			HexFile;
	IHexRecord	HexRec;
	int8_t		CurrentSector;
	uint8_t		SectorErased[NB_SECTORS];
	uint32_t	StartAddress;
	uint32_t	BaseAddress;
	uint32_t	CurrentAddress;
	uint32_t	ExpectedAddress;
	uint32_t	Checksum;

} UpdateSW_s;
#endif

 
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
 ***************************************************************************************************
 * @todo Validation
 */
void
appMAJ_Init(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
)
{
	MajInfo_s tmpConf;
	Bool_e ConfValide = TRUE;

	_printf("appMAJ_Init\n");

	//----------------------------------------------------------
	// Init de la structure
	for (int iCarte=0; iCarte<NB_CARTES; iCarte++)
	{
		pMajInfo->SW_Carte[iCarte].V_Memoire[0]			= '\0';
		pMajInfo->SW_Carte[iCarte].V_Carte[0]			= '\0';
		pMajInfo->SW_Carte[iCarte].MAJ_Requise			= FALSE;
		pMajInfo->SW_Carte[iCarte].IsPresente			= FALSE;
		pMajInfo->SW_Carte[iCarte].iSwp					= 0;
		pMajInfo->SW_Carte[iCarte].Taille_MAJ			= 0;
		pMajInfo->SW_Carte[iCarte].Avancement_pr1000	= 0;
	}
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
Status_e
appMAJ_CheckSWPInfo(
		MajInfo_s* pMajInfo,		/**<[in] Informations de MAJ.*/
		char* FileName_Prefix		/**<[in] Prefix du fichier recherche.*/
)
{
	FRESULT res;
	static FIL File;
	char FileNamePrefix[_MAX_LFN]={0};
	char FileNameExtension[_MAX_LFN]={0};
	int8_t NbFilePrefix;
	int8_t NbFileExtension;

	// Init
	pMajInfo->Swp.IsValide = FALSE;
	pMajInfo->Swp.FileName[0] = 0;

	// Recherche fichier avec le prefixe attendu
	NbFilePrefix = MemoireFAT_SearchFileByName("", FileName_Prefix, 1, FileNamePrefix);

	// Recherche fichier avec l'extension attendue
	NbFileExtension = MemoireFAT_SearchFileByExtension("", "*.swp", 1, FileNameExtension);

	// Si un seul fichier trouve
	if ((NbFilePrefix == 1) && (NbFileExtension == 1)
	&&	(strcmp(FileNamePrefix, FileNameExtension) == 0))
	{
		strcpy(pMajInfo->Swp.FileName, FileNamePrefix);

		_printf("Fichier SWP trouve: %s\n", pMajInfo->Swp.FileName);

		// Tentative d'ouverture du fichier
		res = f_open(&File, pMajInfo->Swp.FileName, FA_READ);
		if (res != FR_OK)
		{
			f_close(&File);
			_printf("f_open KO: %d\n", res);
			return Status_KO;
		}

		// Verification du contenu du fichier
		if (SwHex_ReadSwpInfo(&File, &pMajInfo->Swp) == Status_KO)
		{
			_printf("SwHex_ReadSwpInfo KO\n");
			f_close(&File);
			return Status_KO;
		}

		f_close(&File);
	}
	else
		return Status_KO;

	return Status_OK;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
Status_e
appMAJ_FindHexFile(
		char* FileName_Prefix,		/**<[in] Prefix du fichier recherche.*/
		char* FoundFilename		/**<[Out] Fichier trouve.*/
)
{
	FRESULT res;
	static FIL File;
	char FileNamePrefix[_MAX_LFN]={0};
	char FileNameExtension[_MAX_LFN]={0};
	int8_t NbFilePrefix;
	int8_t NbFileExtension;

	// Recherche fichier avec le prefixe attendu
	NbFilePrefix = MemoireFAT_SearchFileByName("", FileName_Prefix, 1, FileNamePrefix);

	// Recherche fichier avec l'extension attendue
	NbFileExtension = MemoireFAT_SearchFileByExtension("", "*.hex", 1, FileNameExtension);

	// Si un seul fichier trouve
	if ((NbFilePrefix == 1) && (NbFileExtension == 1)
	&&	(strcmp(FileNamePrefix, FileNameExtension) == 0))
	{
		strcpy(FoundFilename, FileNamePrefix);
		return Status_OK;
	}

	return Status_KO;
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
void
appMAJ_Associer_VBase_VMemoire(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
)
{
	// Parcours des cartes
	for (int iCarte=0; iCarte<NB_CARTES; iCarte++)
	{
		// Recuperation des versions de base du systeme
		strcpy(pMajInfo->SW_Carte[iCarte].V_Base, "ALJ00001");

		// Recherche du Sw assacie
		for (int iSwpSW=0; iSwpSW<pMajInfo->Swp.NbSw; iSwpSW++)
		{
			// Si nom de base de la version identiques, copie vers V_Memoire
			if (strncmp(pMajInfo->SW_Carte[iCarte].V_Base, pMajInfo->Swp.Sw[iSwpSW].Version, VERSION_SW_BASE_SIZE) == 0)
			{
				strncpy(pMajInfo->SW_Carte[iCarte].V_Memoire, pMajInfo->Swp.Sw[iSwpSW].Version, VERSION_SW_SIZE);
				pMajInfo->SW_Carte[iCarte].V_Memoire[VERSION_SW_SIZE] = 0;
				pMajInfo->SW_Carte[iCarte].iSwp			= iSwpSW;
				pMajInfo->SW_Carte[iCarte].Taille_MAJ	= 0;
			}
		}
	}

}

/**
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
appMAJ_IsMajNeeded(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
)
{
	Bool_e MajNecessaire = FALSE;

	// Parcours des cartes
	for (uint8_t iCarte=0; iCarte<NB_CARTES; iCarte++)
	{
		// Verification carte presente
		if (pMajInfo->SW_Carte[iCarte].IsPresente == FALSE)
			continue;

		// Comparaison des versions
		if ((strcmp(pMajInfo->SW_Carte[iCarte].V_Carte, pMajInfo->SW_Carte[iCarte].V_Memoire) != 0)
		||	(pMajInfo->ForcageMAJ == TRUE))
		{
			pMajInfo->SW_Carte[iCarte].MAJ_Requise = TRUE;
			MajNecessaire = TRUE;
		}
	}

	return MajNecessaire;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
appMAJ_PrintInfos(
		MajInfo_s* pMajInfo			/**<[in] Informations de MAJ.*/
)
{
	// Affichage infos
	_printf("------------------------------\n");
	_printf("appMAJ Infos\n");
	_printf("------------------------------\n");
	_printf("ForcageMAJ     = %d\n",	pMajInfo->ForcageMAJ);
//	_printf("ValidationUser = %d\n",	MAJ.ValidationUser);
	_printf("SWP :\n");
	_printf(" Version        = %s\n",		pMajInfo->Swp.Version);
	_printf(" HeaderChecksum = 0x%08X\n",	pMajInfo->Swp.HeaderChecksum);
	_printf(" HeaderIsValide = %d\n",		pMajInfo->Swp.HeaderIsValide);
	_printf(" NbSw           = %d\n",		pMajInfo->Swp.NbSw);
	for (int i=0; i<pMajInfo->Swp.NbSw; i++)
	{
		_printf(" SW %d :\n",		i);
		_printf("  Version      = %s\n",		pMajInfo->Swp.Sw[i].Version);
		_printf("  BufferOffset = %d\n",		pMajInfo->Swp.Sw[i].BufferOffset);
		_printf("  HexSize      = %d\n",		pMajInfo->Swp.Sw[i].HexSize);
		_printf("  Checksum     = 0x%08X\n",	pMajInfo->Swp.Sw[i].Checksum);
		_printf("  IsValide     = %d\n",		pMajInfo->Swp.Sw[i].IsValide);
	}

	_printf("SW_Carte :\n");
	for (int iCarte=0; iCarte<NB_CARTES; iCarte++)
	{
		_printf(" Carte %d :\n", iCarte);
		_printf("  IsPresente  : %d\n", pMajInfo->SW_Carte[iCarte].IsPresente);
		_printf("  MAJ_Requise : %d\n", pMajInfo->SW_Carte[iCarte].MAJ_Requise);
		_printf("  V_Base      : %s\n", pMajInfo->SW_Carte[iCarte].V_Base);
		_printf("  V_Carte     : %s\n", pMajInfo->SW_Carte[iCarte].V_Carte);
		_printf("  V_Memoire   : %s\n", pMajInfo->SW_Carte[iCarte].V_Memoire);
		_printf("  iSwp        : %d\n", pMajInfo->SW_Carte[iCarte].iSwp);
	}
	_printf("------------------------------\n");
}







 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

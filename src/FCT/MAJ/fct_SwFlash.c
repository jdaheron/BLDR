/**
 ***************************************************************************************************
 * @file	fct_SwFlash.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	15 sept. 2014
 * @brief   Gestion de logiciels en Flash.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "fct_SwFlash.h"

#include "util_printf.h"
#include "util_Checksum.h"


/* External Variables *****************************************************************************/


/** @addtogroup fct_SwFlash
  * @{
  */


 /**
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

/** Etapes de la gestion de sauvegarde d'application. */
typedef enum
{
	SwFlash_Initialisation = 0,

	SwFlash_VerifAPP,
	SwFlash_VerifBKP,

} SwFlash_Etape_e;

/** Liste des SwFlash. */
typedef enum
{
	APP = 0,
	BKP,
	NB_SwFlash,

} SwFlash_e;


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */

#define SW_FLASH_CONSOLE_ENABLE	1


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */

#if SW_FLASH_CONSOLE_ENABLE
	#define _SW_FLASH_CONSOLE	_printf
#else
	#define _SW_FLASH_CONSOLE
#endif


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

extern uint32_t _AppStartAddr;
#if USE_BKP
	extern uint32_t _BkpStartAddr;
#endif

static SwFlash_Info_s SwFlash_Info_Default[NB_SwFlash] = {{0}};
static SwFlash_Info_s SwInfo[NB_SwFlash];
static Bool_e SaveNeeded = FALSE;
static Bool_e InitNeeded = TRUE;

/**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */


/**-----------------------------------------------------------------------------
 * @brief	Impression des informations SwFlash.
 *
 */
void PrintInfos()
{
	_SW_FLASH_CONSOLE("------------------------------\n");
	_SW_FLASH_CONSOLE("SwFlash Infos\n");
	_SW_FLASH_CONSOLE("------------------------------\n");

	_SW_FLASH_CONSOLE(" _AppStartAddr: %d\n",		&_AppStartAddr);
	#if USE_BKP
	_SW_FLASH_CONSOLE(" _BkpStartAddr: %d\n",		&_BkpStartAddr);
	#endif
	_SW_FLASH_CONSOLE(" UseBkp       : %d\n",		USE_BKP);
	_SW_FLASH_CONSOLE(" AppInfo      :\n"			);
	_SW_FLASH_CONSOLE("  StartAdress  : 0x%08X\n",	SwInfo[APP].StartAdress);
	_SW_FLASH_CONSOLE("  Size         : %d\n",		SwInfo[APP].Size);
	_SW_FLASH_CONSOLE("  Checksum     : 0x%08X\n",	SwInfo[APP].Checksum);
	_SW_FLASH_CONSOLE("  Version      : %s\n",		SwInfo[APP].Version);
	_SW_FLASH_CONSOLE("  IsValide     : %d\n",		SwInfo[APP].IsValide);
	#if USE_BKP
	_SW_FLASH_CONSOLE(" BkpInfo      :\n"			);
	_SW_FLASH_CONSOLE("  StartAdress  : 0x%08X\n",	SwInfo[BKP].StartAdress);
	_SW_FLASH_CONSOLE("  Size         : %d\n",		SwInfo[BKP].Size);
	_SW_FLASH_CONSOLE("  Checksum     : 0x%08X\n",	SwInfo[BKP].Checksum);
	_SW_FLASH_CONSOLE("  Version      : %s\n",		SwInfo[BKP].Version);
	_SW_FLASH_CONSOLE("  IsValide     : %d\n",		SwInfo[BKP].IsValide);
	_SW_FLASH_CONSOLE("------------------------------\n");
	#endif
}


/**-----------------------------------------------------------------------------
 * @brief	Verification des donnees d'un SwInfo.
 *
 */
Bool_e IsSwInfoValid(SwFlash_e Type)
{
	Bool_e DonneesValides = TRUE;

	// Verification adresse de depart
	if ((SwInfo[Type].StartAdress < 0x08000000) || (SwInfo[Type].StartAdress > 0x08100000))
		DonneesValides = FALSE;

	// Verification taille
	if ((SwInfo[Type].Size == 0) || (SwInfo[Type].Size > 1024*1024))	// Taille nulle ou > 1Mo
		DonneesValides = FALSE;

	if (strncmp(SwInfo[Type].Version, "AL", 2) != 0)
		DonneesValides = FALSE;

	if ((SwInfo[Type].IsValide != TRUE) && (SwInfo[Type].IsValide != FALSE))
		DonneesValides = FALSE;

	return DonneesValides;
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture de la version d'un Sw.
 *
 */
Bool_e
ReadSwVersion(SwFlash_e Type, char* Version)
{
	Bool_e VersionValide;
	uint32_t ReadAddress;
	char TmpVersion[VERSION_SIZE];
	uint32_t BaseAddress;
	uint32_t Offset;

	//----------------------------------------
	// DEFINITION ADRESSE DE BASE ET OFFSET
	//----------------------------------------
	BaseAddress = SwInfo[Type].StartAdress;
	if (Type == APP)
		Offset = 0;
	else
		Offset = SwInfo[BKP].StartAdress - SwInfo[APP].StartAdress;

	//----------------------------------------
	// LECTURE VERSION
	//----------------------------------------

	// Verification pointeur valide
	ReadAddress = BaseAddress + (100 * sizeof(uint32_t));
	if (( (*((uint32_t*) ReadAddress) + Offset ) & 0xFF000000) != 0x08000000)	// La version SW doit etre en Flash
	{
		// Pointeur non valide
		VersionValide = FALSE;
	}
	else
	{
		// Lecture version
		strcpy((char*) TmpVersion, (char*) "AL");
		strncat((char*) TmpVersion, (char*) (*((uint32_t*) ReadAddress) + Offset ), VERSION_SIZE - 2);

		// Verification
		VersionValide = TRUE;
		for (uint32_t i=0; i<strlen(TmpVersion); i++)
		{
			if (isalnum(TmpVersion[i]) == FALSE)
				VersionValide = FALSE;
		}
	}

	if ((VersionValide == TRUE) && (Version != NULL))
	{
		strncpy((char*) Version, (char*) TmpVersion, VERSION_SIZE-1);
	}
	else
		Version[0] = 0;

	return VersionValide;
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture de la taille d'un Sw.
 *
 */
Bool_e
ReadSwSize(SwFlash_e Type, uint32_t* Size)
{
	Bool_e SizeValide;
	uint32_t ReadAddress;
	uint32_t TmpSize;
	uint32_t BaseAddress;
	uint32_t Offset;

	//----------------------------------------
	// DEFINITION ADRESSE DE BASE ET OFFSET
	//----------------------------------------
	BaseAddress = SwInfo[Type].StartAdress;

	//----------------------------------------
	// LECTURE TAILLE
	//----------------------------------------

	// Verification pointeur valide
	ReadAddress = BaseAddress + (101 * sizeof(uint32_t));

	// Lecture taille
	TmpSize = (uint32_t) *((uint32_t*) ReadAddress);

	// Verification
	if ((TmpSize == 0) || (TmpSize > 1024*1024))	// Taille non nulle et inferieure ou egale a 1Mo
		SizeValide = FALSE;
	else
		SizeValide = TRUE;

	if ((SizeValide == TRUE) && (Size != NULL))
	{
		*Size = TmpSize;
	}

	return SizeValide;
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture de la taille d'un Sw.
 *
 */
void ChercheSwValide(SwFlash_e Type)
{
	char TmpVersion[VERSION_SIZE];
	uint32_t TmpSize;
	uint32_t ChecksumAttendu;
	uint32_t ChecksumCalcule;


	//--------------------------------------------------------------
	// Verif VersionSW
	if (ReadSwVersion(Type, TmpVersion) == FALSE)
	{
		SwInfo[Type].IsValide = FALSE;
		return;
	}
	else
	{
		// Si version lue !=  version sauvegardee, on considere la sauvegarde non valide
//		if (strncmp(SwInfo[Type].Version, TmpVersion, VERSION_SIZE) != 0)
//		{
			SwInfo[Type].IsValide = TRUE;
			strncpy(SwInfo[Type].Version, TmpVersion, VERSION_SIZE);
//		}
	}
	return;


	//--------------------------------------------------------------
	// Verif Size
	if (ReadSwSize(Type, &TmpSize) == FALSE)
	{
		SwInfo[Type].IsValide = FALSE;
		return;
	}
	else
	{
		// Si taille lue != taille sauvegardee, on considere la sauvegarde non valide
		if (SwInfo[Type].Size != TmpSize)
		{
			SwInfo[Type].IsValide = FALSE;
			SwInfo[Type].Size = TmpSize;
		}
	}


	//--------------------------------------------------------------
	// Verif Checksum

	ChecksumCalcule = Checksum_CalculComplementA2((uint8_t*) SwInfo[Type].StartAdress, SwInfo[Type].Size);	// ~30ms pour 128ko

	// Si le Sw est considere comme valide, on verifie le checksum
	if (SwInfo[Type].IsValide == TRUE)
		ChecksumAttendu = SwInfo[Type].Checksum;
	else
	{
		ChecksumAttendu = ChecksumCalcule;
		SaveNeeded = TRUE;
	}

	// Verification
	if (ChecksumCalcule == ChecksumAttendu)
		SwInfo[Type].IsValide = TRUE;
	else
	{
		_SW_FLASH_CONSOLE("Checksum Error\n");
		SwInfo[Type].IsValide = FALSE;
	}
}



/**-----------------------------------------------------------------------------
 * @brief	Copy d'un espace flash vers l'autre.
 *
 */
void FlashCopy(SwFlash_e to, SwFlash_e from)
{
	uint32_t Add;
	uint8_t SectorErased[12];
	uint8_t buf[128];
	uint32_t Src;
	uint32_t Dest;
	uint32_t Size;

	Src = SwInfo[from].StartAdress;
	Dest = SwInfo[to].StartAdress;
	Size = SwInfo[from].Size;

	_SW_FLASH_CONSOLE("FlashCopy From %d To %d (%do)\n", from, to, Size);

	// Init
	for (int i=0; i<12; i++)
		SectorErased[i] = FALSE;

	// Copie flash
	for (Add = 0; Add < Size; Add += 128)
	{
		// Lecture source
		FLASH_Read(Src + Add, 128, buf);

		// Verification necessite effacement secteur
		if (SectorErased[FLASH_GetSectorNum(Dest + Add)] == FALSE)
		{
			FLASH_EraseAddress(Dest + Add);
			SectorErased[FLASH_GetSectorNum(Dest + Add)] = TRUE;
		}

		// Ecriture
		FLASH_Write(Dest + Add, buf, 128);
	}

	// Copie infos
	SwInfo[to].Size = SwInfo[from].Size;
	SwInfo[to].Checksum = SwInfo[from].Checksum;
	strncpy(SwInfo[to].Version, SwInfo[from].Version, VERSION_SIZE);
	SaveNeeded = TRUE;
}



 /**
 * @}
 */


/**
 ***************************************************************************************************
 * @todo Validation
 */
void
SwFlash_Init(
		void
)
{
	_SW_FLASH_CONSOLE("SwFlash_Init\n");

	SaveNeeded = FALSE;

	SwFlash_Info_Default[APP].StartAdress = (uint32_t) &_AppStartAddr;
	#if USE_BKP
		SwFlash_Info_Default[BKP].StartAdress = (uint32_t) &_BkpStartAddr;
	#else
		SwFlash_Info_Default[BKP].StartAdress = 0;
	#endif

	// Verification des donnees de l'eeprom externe
	SwInfo[APP].IsValide = IsSwInfoValid(APP);
	SwInfo[BKP].IsValide = IsSwInfoValid(BKP);

	_SW_FLASH_CONSOLE("APP(IsValide=%d|Size=%d|V=%s) | BKP(IsValide=%d|Size=%d|V=%s)\n",
			SwInfo[APP].IsValide, SwInfo[APP].Size, SwInfo[APP].Version,
			SwInfo[BKP].IsValide, SwInfo[BKP].Size, SwInfo[BKP].Version);

	// Si donnees de l'eeprom non  valides, restauration des adresses par defaut
	if (SwInfo[APP].IsValide == FALSE)
		SwInfo[APP].StartAdress = (uint32_t) &_AppStartAddr;
	#if USE_BKP
		if (SwInfo[BKP].IsValide == FALSE)
			SwInfo[BKP].StartAdress = (uint32_t) &_BkpStartAddr;
	#endif

	//--------------------------------------------------------------
	// Verif APP et BKP

	_SW_FLASH_CONSOLE("SwFlash_Verif APP et BKP\n");
	ChercheSwValide(APP);
	ChercheSwValide(BKP);

	_SW_FLASH_CONSOLE("APP(IsValide=%d|Size=%d|V=%s) | BKP(IsValide=%d|Size=%d|V=%s)\n",
			SwInfo[APP].IsValide, SwInfo[APP].Size, SwInfo[APP].Version,
			SwInfo[BKP].IsValide, SwInfo[BKP].Size, SwInfo[BKP].Version);

	// Effacement Versions si sw non valides
	if (SwInfo[APP].IsValide == FALSE)
		SwInfo[APP].Version[0] = 0;
	if (SwInfo[BKP].IsValide == FALSE)
		SwInfo[BKP].Version[0] = 0;

	InitNeeded = FALSE;
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
Status_e
SwFlash_PreparerApplication(
		void
)
{
	Status_e Status = Status_KO;

	//--------------------------------------------------------------
	// Init
	if (InitNeeded)
		SwFlash_Init();

	//--------------------------------------------------------------
	// Verif APP et BKP
	_SW_FLASH_CONSOLE("SwFlash_Choix Action\n");

	#if USE_BKP

		// 2 SW valides, on conserve l'APP
		if (SwInfo[APP].IsValide && SwInfo[BKP].IsValide)
		{
			_SW_FLASH_CONSOLE("APP valide & BKP valide => Comparaison versions\n");

			if (strcmp(SwInfo[APP].Version, SwInfo[BKP].Version) != 0)
			{
				_SW_FLASH_CONSOLE("  Versions differentes => Copie APP=>BKP\n");
				FlashCopy(BKP, APP);
				Status = Status_OK;
			}
			else
			{
				_SW_FLASH_CONSOLE("  Versions identiques => Pas d'action a mener\n");
				Status = Status_OK;
			}
		}
		// Seul BKP valide
		else if (!SwInfo[APP].IsValide && SwInfo[BKP].IsValide)
		{
			_SW_FLASH_CONSOLE("APP non valide & BKP valide => Copie BKP=>APP\n");
			FlashCopy(APP, BKP);
			Status = Status_OK;
		}
		// Seule APP valide
		else if (SwInfo[APP].IsValide && !SwInfo[BKP].IsValide)
		{
			_SW_FLASH_CONSOLE("APP valide & BKP non valide => Copie APP=>BKP\n");
			FlashCopy(BKP, APP);
			Status = Status_OK;
		}
		else	// Aucun SW valide
		{
			_SW_FLASH_CONSOLE("Aucun SW valide => Pas d'action a mener\n");
			Status = Status_KO;
		}

	#else

		if (SwInfo[APP].IsValide)
		{
			_SW_FLASH_CONSOLE("APP valide\n");
			Status = Status_OK;
		}
		else
		{
			_SW_FLASH_CONSOLE("APP non valide\n");
			Status = Status_KO;
		}

	#endif

	return Status;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
uint32_t
SwFlash_GetAppAddress(
		void
)
{
	// Init du gestionnaire si necessaire
	if (InitNeeded)
		SwFlash_Init();

	return SwInfo[APP].StartAdress;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
char*
SwFlash_GetAppSwVersion(
		void
)
{
	// Init du gestionnaire si necessaire
	if (InitNeeded)
		SwFlash_Init();

	return SwInfo[APP].Version;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
SwFlash_SaveAppInfo(
		SwFlash_Info_s* pSwInfo
)
{
	_SW_FLASH_CONSOLE("SwFlash_SaveAppInfo\n");

	// Init du gestionnaire si necessaire
	if (InitNeeded)
		SwFlash_Init();

	// Si la version SW ou la taille n'est pas a jour, on la lit
	if (pSwInfo->Version[0] == 0)
		ReadSwVersion(APP, pSwInfo->Version);

	if (pSwInfo->Size == 0)
		ReadSwSize(APP, &pSwInfo->Size);

	// Si le flag IsValide n'est pas positionne, on l'efface
	if ((pSwInfo->IsValide != TRUE) && (pSwInfo->IsValide != FALSE))
		pSwInfo->IsValide = FALSE;

	// Copie des structures pour l'enregistrement
	memcpy(&SwInfo[0], pSwInfo, sizeof(SwFlash_Info_s));

	// Une nouvelle init est necessaire
	InitNeeded = TRUE;
}



 /**
 * @}
 */

/* End Of File ************************************************************************************/

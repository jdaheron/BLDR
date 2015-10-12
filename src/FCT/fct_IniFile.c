/**
 ***************************************************************************************************
 * @file	fct_IniFile.C
 * @author	j.daheron
 * @version	1.0.0
 * @date	28 août 2014
 * @brief   Gestion de fichier INI.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "fct_IniFile.h"
#include "minIni.h"


/* External Variables *****************************************************************************/


/** @addtogroup fct_IniFile
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
 
#define MAX_STR_SIZE	100
#define MODE_READ		0
#define MODE_WRITE		1

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
 
static FIL File;
static FIL FileTmp;
static int Mode = 0;
static Bool_e FileModified = FALSE;
static FIL* pCurrentWrFile;
static FIL* pCurrentRdFile;

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
 * Validation OK - JD le 28/08/2014
 */
Bool_e
IsKeyValid(
		Key_s*	pKey	/**<[in] Cle a controler.*/
)
{
	Bool_e IsValid = FALSE;
	uint8_t car;

	switch(pKey->KeyType)
	{
		case KeyType_Int:
			if (pKey->MaxValue <  pKey->MinValue)
			{
				if (((int) pKey->Value <= (int)pKey->MaxValue) && ((int)pKey->Value >= (int)pKey->MinValue))
					IsValid = TRUE;
			}
			else
			{
				if ((pKey->Value <= pKey->MaxValue) && (pKey->Value >= pKey->MinValue))
					IsValid = TRUE;
			}
			break;

		case KeyType_Str:
			car = *((uint8_t*) pKey->Value);
			if (car <= 127)
				IsValid = TRUE;
			break;

		//case KeyType_Float:
		//	ini_putf(pKeyValue[iKey].SectionName, pKeyValue[iKey].KeyName, (float) pKeyValue[iKey].DefaultValue, FileName);
		//	break;
	}

	return IsValid;
}

/** 
 ***************************************************************************************************
 * Validation OK - JD le 28/08/2014
 */
Bool_e
IsFileValid(
		const IniFile_s*	IniFile		/**<[in] Fichier Ini.*/
)
{
	Bool_e IsValid = TRUE;
	char buff[MAX_STR_SIZE];
	Key_s Key;

	for (uint32_t iKey=0; iKey<IniFile->NbKey; iKey++)
	{
		// Copie de la valeur par defaut de la cle en local
		memcpy(&Key, &(IniFile->KeyTable[iKey]), sizeof(Key_s));

		// Lecture de la valeur presente dans le fichier
		if (IniFile_Read(&Key, buff) == Status_KO)
		{
			IsValid = FALSE;
			break;
		}

		// Verification de la validite de la cle
		if (Key.KeyType == KeyType_Str)
			Key.Value = &buff;

		if (IsKeyValid(&Key) == FALSE)
		{
			IsValid = FALSE;
			break;
		}
	}

	return IsValid;
}

/**
 ***************************************************************************************************
 * Validation OK - JD le 28/08/2014
 */
IniFile_Status_e
IniFile_Init(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
)
{
	IniFile_Status_e Status = IniFile_StatusOK;

	// Ouverture du fichier, creation si besoin
	if (IniFile_OpenWrite(IniFile) == IniFile_StatusKO)
		return IniFile_StatusKO;

	// Verification du contenu du fichier
	if (IsFileValid(IniFile) == FALSE)
		Status = IniFile_StatusKO;

	IniFile_Close(IniFile);

	return Status;
}

/**
 ***************************************************************************************************
 * Validation OK - JD le 01/09/2014
 */
IniFile_Status_e
IniFile_OpenRead(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
)
{
	IniFile_Status_e Status = IniFile_StatusOK;

	// Verif presence repertoire, creation si besoin
	if (MemoireFAT_IsDirectoryPresent((char*) IniFile->FileName, TRUE) == MemFAT_Error)
		return IniFile_StatusKO;

	// Ouverture du fichier, creation si besoin
	switch (MemoireFAT_IsFilePresent(&File, (char*) IniFile->FileName, FA_READ | FA_OPEN_EXISTING, TRUE, TRUE))
	{
		case MemFAT_OK:
			Status = IniFile_StatusOK;
			break;

		case MemFAT_FileCreated:
			Status = IniFile_StatusFileCreated;
			break;

		case MemFAT_Error:
		default:
			return IniFile_StatusKO;
	}

	Mode = MODE_READ;

	return Status;
}

/**
 ***************************************************************************************************
 * Validation OK - JD le 01/09/2014
 */
IniFile_Status_e
IniFile_OpenWrite(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
)
{
	IniFile_Status_e Status = IniFile_StatusOK;

	// Verif presence repertoire, creation si besoin
	if (MemoireFAT_IsDirectoryPresent((char*) IniFile->FileName, TRUE) == MemFAT_Error)
		return IniFile_StatusKO;

	// Ouverture du fichier, creation si besoin
	switch (MemoireFAT_IsFilePresent(&File, (char*) IniFile->FileName, FA_READ | FA_WRITE, TRUE, TRUE))
	{
		case MemFAT_OK:
			Status = IniFile_StatusOK;
			break;

		case MemFAT_FileCreated:
			Status = IniFile_StatusFileCreated;
			break;

		case MemFAT_Error:
		default:
			return IniFile_StatusKO;
	}

	// Creation d'un fichier temporaire
	char buffer[100] = {0};
	strcpy(buffer, IniFile->FileName);
	strcat(buffer, "~\0");
	f_unlink(buffer);
	switch (MemoireFAT_IsFilePresent(&FileTmp, buffer, FA_READ | FA_WRITE, TRUE, TRUE))
	{
		case MemFAT_OK:
			Status = IniFile_StatusOK;
			break;

		case MemFAT_FileCreated:
			Status = IniFile_StatusFileCreated;
			break;

		case MemFAT_Error:
		default:
			return IniFile_StatusKO;
	}

	pCurrentRdFile = &File;
	pCurrentWrFile = &FileTmp;
	FileModified = FALSE;
	Mode = MODE_WRITE;

	return Status;
}

/**
 ***************************************************************************************************
 * Validation OK - JD le 01/09/2014
 */
void
IniFile_Close(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
)
{
	// Fermeture des fichiers
	f_close(&File);
	f_close(&FileTmp);

	// Creation du nom de fichier temporaire
	char buffer[100] = {0};
	strcpy(buffer, IniFile->FileName);
	strcat(buffer, "~\0");

	if ((Mode == MODE_WRITE) && (FileModified == TRUE))
	{
		if (pCurrentRdFile == &FileTmp)
		{
			f_unlink(IniFile->FileName);
			f_rename(buffer, IniFile->FileName);
		}
	}
	f_unlink(buffer);

	Mode = MODE_READ;
}

/**
 ***************************************************************************************************
 * Validation OK - JD le 28/08/2014
 */
void
IniFile_SetDefaultValue(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
)
{
	Key_s* pKey;

	for (uint32_t iKey=0; iKey<IniFile->NbKey; iKey++)
	{
		pKey = (Key_s*) &(IniFile->KeyTable[iKey]);
		IniFile_Write(pKey, pKey->Value);
	}
}


/**
 ***************************************************************************************************
 * Validation OK - JD le 28/08/2014
 */
Status_e
IniFile_Read(
		Key_s*				pKey,		/**<[in] Cle a lire.*/
		void*				pValue		/**<[in] Pointeur vers la valeur lue.*/
)
{
	char buff[4];

	if (pValue == NULL)
		return Status_KO;

	// Verification de l'existance de la cle
	if (ini_getkey_OpenedFile(pKey->SectionName, 0, buff, 2, &File) == 0)
		return Status_KO;

	// Ecriture de la donnee
	switch(pKey->KeyType)
	{
		case KeyType_Int:
			*((long*) pValue) = ini_getl_OpenedFile(pKey->SectionName, pKey->KeyName,(long) pKey->Value, &File);
			break;

		case KeyType_Str:
			ini_gets_OpenedFile(pKey->SectionName, pKey->KeyName, (TCHAR*) pKey->Value, (TCHAR*) pValue, MAX_STR_SIZE, &File);
			break;

		//case KeyType_Float:
		//	ini_putf(pKeyValue[iKey].SectionName, pKeyValue[iKey].KeyName, (float) pKeyValue[iKey].DefaultValue, FileName);
		//	break;
	}

	return Status_OK;
}

/**
 ***************************************************************************************************
 * Validation OK - JD le 28/08/2014
 */
Status_e
IniFile_Write(
		Key_s*				pKey,		/**<[in] Cle a ecrire.*/
		void*				Value		/**<[in] Valeur a ecrire.*/
)
{
	Key_s Key;
//	FIL tmpFile;
	Bool_e WriteStatus;

	// Copie du contenu de la cle en local
	memcpy(&Key, pKey, sizeof(Key_s));

	// Pointeur vers la nouvelle valeur
	Key.Value = Value;

	// Verification de la validite de la donnees
	if (IsKeyValid(&Key) == FALSE)
		return Status_KO;

	// Ecriture de la donnee
	switch(pKey->KeyType)
	{
		case KeyType_Int:
			WriteStatus = ini_putl_OpenedFile(pKey->SectionName, pKey->KeyName, (long) Value, pCurrentRdFile, pCurrentWrFile);
			break;

		case KeyType_Str:
			WriteStatus = ini_puts_OpenedFile(pKey->SectionName, pKey->KeyName, (TCHAR*) Value, pCurrentRdFile, pCurrentWrFile);
			break;

		//case KeyType_Float:
		//	ini_putf(pKeyValue[iKey].SectionName, pKeyValue[iKey].KeyName, (float) pKeyValue[iKey].DefaultValue, FileName);
		//	break;
	}

	if (WriteStatus == TRUE)
	{
		FileModified = TRUE;

		// Changement du fichier temporaire cible
		if (pCurrentWrFile == &FileTmp)
		{
			pCurrentWrFile = &File;
			pCurrentRdFile = &FileTmp;
		}
		else
		{
			pCurrentWrFile = &FileTmp;
			pCurrentRdFile = &File;
		}
	}

	return Status_OK;
}


/**
 * @brief	Ecriture d'une valeur brute dans un fichier ouvert.
 * @return 	Status.
 */
Status_e
IniFile_WriteRawData(
		Key_s*				pKey,		/**<[in] Cle a ecrire.*/
		void*				Value		/**<[in] Valeur a ecrire.*/
)
{
	char Buffer[32];

	FileModified = TRUE;

	// Ecriture de la donnee
	switch(pKey->KeyType)
	{
		case KeyType_SectionName:
			f_puts("\n[", pCurrentRdFile);
			f_puts(pKey->SectionName, pCurrentRdFile);
			f_puts("]\n", pCurrentRdFile);
			break;

		case KeyType_Int:
			f_puts(pKey->KeyName, pCurrentRdFile);
			f_putc('=', pCurrentRdFile);
			_sprintf(Buffer, "%d\n", (int) Value);
			f_puts(Buffer, pCurrentRdFile);
			break;

		case KeyType_Str:
			f_puts(pKey->KeyName, pCurrentRdFile);
			f_putc('=', pCurrentRdFile);
			f_puts((char*) Value, pCurrentRdFile);
			f_putc('\n', pCurrentRdFile);
			break;
	}

	return Status_OK;
}




 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

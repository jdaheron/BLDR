/**
 ***************************************************************************************************
 * @file	fct_MemoireFAT.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	25 juil. 2014
 * @brief   Gestion de la memoire FAT.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "fct_MemoireFAT.h"
#include "util_TSW.h"


/* External Variables *****************************************************************************/


/** @addtogroup fct_MemoireFAT
  * @{
  */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

/** Definition de l'objet Memoire. */
typedef struct{

	Diskio_drvTypeDef*	DiskDriver;		/**< Driver de disque. */
	Bool_e				InitOk;			/**< Status d'initialisation de la memoire. */
	Bool_e				AccesAutorise;	/**< Autorisation de l'acces a la memoire pour l'applicatif. */
	FATFS				Fatfs;			/**< Systeme de fichier. */
	char				DrivePath[4];	/**< Chemin de la memoire. */

} Memoire_t;


/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */
 
/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */
  
#define MEMOIRE_CONSOLE_ENABLE	1

#if MEMOIRE_CONSOLE_ENABLE
	#define _MEMOIRE_CONSOLE	_printf
#else
	#define _MEMOIRE_CONSOLE	//
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
 
static Memoire_t Memoire = {
		.DiskDriver		= NULL,
		.InitOk			= FALSE,
		.AccesAutorise	= TRUE,
};
#if _USE_LFN
	static char Lfname[_MAX_LFN];
	static FILINFO FileInfo = {
			.lfname = Lfname,
			.lfsize = _MAX_LFN
	};
#else
	FILINFO FileInfo;
#endif


/**
 * @}
 */ 
  	
	
/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 
 
#define  VALIDATION_FCT_MEMOIRE 1

#if VALIDATION_FCT_MEMOIRE

void VALIDATION_SEQUENCE_FCT_MEMOIRE(){
#if 0
	uint8_t test=0;
	uint8_t buffer[32];
	uint32_t FileSize;
	uint32_t br;
	uint32_t index;
	FIL File;

	Memoire_Init();
	Memoire_ScanFiles("");

	while (1) {

		if (test)
		{
			// Lecture info fichier
			f_stat("AL00056_Base0x0802_SW0.hex", &FileInfo);
			_MEMOIRE_CONSOLE("--- AL00056_Base0x0802_SW0.hex ---\n");
			_MEMOIRE_CONSOLE("size = %d\n", FileSize = FileInfo.fsize);

			TimerSW_Start(TIMER_Generique, 30*1000);
			// Lecture fichier
			f_open(&File, "AL00056_Base0x0802_SW0.hex", FA_READ);

			//_MEMOIRE_CONSOLE("data = \n");
			index = 0;
			while (FileSize)
			{
				// Lecture
				f_lseek(&File, index);
				f_read(&File, buffer, 10, &br);

				// Affichage
				//buffer[br]=0;
				//_MEMOIRE_CONSOLE("%s",buffer);

				index += br;
				FileSize -= br;
			}
			f_close(&File);

			_MEMOIRE_CONSOLE("Duree lecture = %dms\n", TimerSW_GetTempsPasse(TIMER_Generique));

			test = 0;
		}
	}
#endif
}
#endif


 /**
 * @}
 */ 
 

/** 
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
MemoireFAT_Init(
		Diskio_drvTypeDef* DiskDriver		/**<[in] Driver du disque a initialiser.*/
)
{
	FIL File;
	FRESULT FRes;
	uint16_t block_size;

	if (DiskDriver == NULL)
		return FALSE;

	Memoire.DiskDriver = DiskDriver;

	if (MemoireFAT_IsReady() == TRUE)
		return TRUE;

	// Montage FileSystem
	if (FATFS_LinkDriver(Memoire.DiskDriver, Memoire.DrivePath) == 0)
	{
		if(f_mount(&Memoire.Fatfs, Memoire.DrivePath, 0) == FR_OK)
		{
			// Essai de création de fichier pour tester le FS
			FRes = f_open(&File, "TestDisk.txt", FA_READ);
			switch (FRes)
			{
				case FR_OK:
				case FR_NO_FILE:
					f_close(&File);
					f_unlink("TestDisk.txt");
					FRes = FR_OK;
					break;

				case FR_NO_FILESYSTEM:
					_MEMOIRE_CONSOLE("[Memoire] Creation du FS\n");
					Memoire.DiskDriver->disk_ioctl(GET_SECTOR_SIZE, &block_size);
					FRes = f_mkfs(Memoire.DrivePath, 0, block_size);
					break;

				case FR_DISK_ERR:				// (1) A hard error occurred in the low level disk I/O layer
				case FR_INT_ERR:				// (2) Assertion failed
				case FR_NOT_READY:				// (3) The physical drive cannot work
				case FR_NO_PATH:				// (5) Could not find the path
				case FR_INVALID_NAME:			// (6) The path name format is invalid
				case FR_DENIED:					// (7) Access denied due to prohibited access or directory full
				case FR_EXIST:					// (8) Access denied due to prohibited access
				case FR_INVALID_OBJECT:			// (9) The file/directory object is invalid
				case FR_WRITE_PROTECTED:		// (10) The physical drive is write protected
				case FR_INVALID_DRIVE:			// (11) The logical drive number is invalid
				case FR_NOT_ENABLED:			// (12) The volume has no work area
				case FR_MKFS_ABORTED:			// (14) The f_mkfs() aborted due to any parameter error
				case FR_TIMEOUT:				// (15) Could not get a grant to access the volume within defined period
				case FR_LOCKED:					// (16) The operation is rejected according to the file sharing policy
				case FR_NOT_ENOUGH_CORE:		// (17) LFN working buffer could not be allocated
				case FR_TOO_MANY_OPEN_FILES:	// (18) Number of open files > _FS_SHARE
				case FR_INVALID_PARAMETER:		// (19) Given parameter is invalid
				default:
					break;
			}
		}
	}

	if (FRes == FR_OK){

		_MEMOIRE_CONSOLE("[Memoire] FS OK\n");
		Memoire.InitOk = TRUE;
	}
	else
	{
		_MEMOIRE_CONSOLE("[Memoire] FS ERROR\n");
		MemoireFAT_DeInit();
	}

	return Memoire.InitOk;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
MemoireFAT_DeInit(
		void
)
{

	Memoire.InitOk = FALSE;

	f_mount(NULL, Memoire.DrivePath, 0);	// Unmout
	FATFS_UnLinkDriver(Memoire.DrivePath);
	TSW_Delay(10);
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
MemoireFAT_IsReady(
		void
)
{
	return Memoire.InitOk;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
MemoireFAT_AutoriserAcces(
		void
)
{
	Memoire.AccesAutorise = TRUE;
	MemoireFAT_Init(Memoire.DiskDriver);
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
MemoireFAT_InterdireAcces(
		void
)
{
	MemoireFAT_DeInit();
	Memoire.AccesAutorise = FALSE;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
MemoireFAT_Main(
		void
)
{
	if (Memoire.InitOk)
	{
		if (Memoire.DiskDriver->disk_status() & STA_NODISK)			// Memoire absente
			MemoireFAT_DeInit();
	}
	else
	{
		if (((Memoire.DiskDriver->disk_status() & STA_NODISK) == 0)	// Memoire presente
		&&	(Memoire.AccesAutorise == TRUE))
			MemoireFAT_Init(Memoire.DiskDriver);
	}

}


/**
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
MemoireFAT_Erase(
		void
) {
#if 0	//TODO JD
	uint8_t buff[MEMORY_SECTOR_SIZE];
	uint32_t i;

	// Verification memoire disponible
	if (eMemoire_IsReady() == FALSE)
		return FR_DENIED;

	_MEMOIRE_CONSOLE("[Memoire] Erase\n");

	// Init du disque si necessaire
	disk_initialize(MEMOIRE_TYPE);

	// Remplissage buffer
	for (i=0; i<MEMORY_SECTOR_SIZE; i++) buff[i] = 0xFF;

	// Effacement des 100 premiers secteurs pour corrompre la FAT.
	for (i=0; i<100; i++){
		disk_write(MEMOIRE_TYPE, buff, i, 1);
		vDelay_ms(2);
	}
#endif
	return FR_OK;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void
MemoireFAT_PrintFileList(
		char* path		/**<[in] Repertoire a scanner.*/
)
{
	FRESULT res;
	DIR dir;
	char *fn;   // This function is assuming non-Unicode cfg.

	if (Memoire.InitOk == FALSE)
		return;

	// Ouverture répertoire a scanner
	res = f_opendir(&dir, path);
	if (res == FR_OK) {

		_MEMOIRE_CONSOLE("--- DIR : \"%s\" ---\n", path);

		// Balayage du répertoire
		while (1) {

			// Lecture de l'objet Repertoire
			res = f_readdir(&dir, &FileInfo);
			if (res != FR_OK || FileInfo.fname[0] == 0) break;	// Break on error or end of dir
			if (FileInfo.fname[0] == '.') continue;				// Ignore dot entry

			fn = FileInfo.fname;
			if (_USE_LFN && (FileInfo.lfname[0]))
				fn = FileInfo.lfname;

			// Envoi vers la console
			_MEMOIRE_CONSOLE("%s/%s", path, fn);

			if (FileInfo.fattrib & AM_DIR)
				_MEMOIRE_CONSOLE("/");
			_MEMOIRE_CONSOLE("\n");
		}

		_MEMOIRE_CONSOLE("--- END OF DIR ---\n");

		f_closedir(&dir);
	}
	else
		_MEMOIRE_CONSOLE("--- DIR ERROR---\n");

}

/**
 ***************************************************************************************************
 * @todo Validation
 */
void MemoireFAT_PrintOpenedFile(
		FIL* 	pFile	/**<[in] Fichier*/
)
{
	char Buffer[256];
	int SaveCursor;

	SaveCursor = f_tell(pFile);
	f_lseek(pFile, 0);

	_MEMOIRE_CONSOLE("--- FILE ---\n");

	while (f_gets(Buffer, 256, pFile))
		_MEMOIRE_CONSOLE("%s", Buffer);

	_MEMOIRE_CONSOLE("--- END OF FILE ---\n");

	f_lseek(pFile, SaveCursor);
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
void MemoireFAT_PrintFile(
		char* 	FileName	/**<[in] Nom du fichier*/
)
{
	FIL File;
	char Buffer[256];

	if (f_open(&File, FileName, FA_READ) == FR_OK)
	{
		_MEMOIRE_CONSOLE("--- FILE : \"%s\" ---\n", FileName);

		while (f_gets(Buffer, 256, &File))
			_MEMOIRE_CONSOLE("%s", Buffer);

		_MEMOIRE_CONSOLE("--- END OF FILE ---\n");

		f_close(&File);
	}
	else
		_MEMOIRE_CONSOLE("--- FILE ERROR---\n");
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
int8_t
MemoireFAT_SearchFileByName(
		char* Dir,				/**<[in] Repertoire a scanner.*/
		char* FileName,			/**<[in] Nom de fichier a trouver.*/
		uint8_t RetFileNum,		/**<[in] Numero de fichier a retourner.*/
		char* RetFileName		/**<[out] Nom de fichier trouve.*/
)
{
	FRESULT res;
	DIR dir;
	char *fn;   // This function is assuming non-Unicode cfg.
	uint8_t NbFile = 0;
	uint8_t NameSize = 0;

	// Ouverture répertoire a scanner
	if (f_opendir(&dir, Dir) == FR_OK) {

		while (NameSize < strlen(FileName))
		{
			if (FileName[NameSize] == '*')
				break;
			NameSize++;
		}

		// Balayage du répertoire
		while (1) {

			// Lecture de l'objet Repertoire
			res = f_readdir(&dir, &FileInfo);
			if (res != FR_OK || FileInfo.fname[0] == 0)
				break;
			if (FileInfo.fname[0] == '.')
				continue;				// Ignore dot entry

			fn = FileInfo.fname;
			#if _USE_LFN
				fn = FileInfo.lfname;
			#endif

			if (strncmp(FileName, fn, NameSize) == 0)
			{
				NbFile++;
				if ((RetFileNum == NbFile) && (RetFileName != NULL))
				{
					RetFileName[0] = 0;
					strcpy(RetFileName, fn);
				}
			}
		}
	}
	else
		return (-1);

	return NbFile;
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
int8_t
MemoireFAT_SearchFileByExtension(
		char* Dir,				/**<[in] Repertoire a scanner.*/
		char* FileExtension,	/**<[in] Nom de fichier a trouver.*/
		uint8_t RetFileNum,		/**<[in] Numero de fichier a retourner.*/
		char* RetFileName		/**<[out] Nom de fichier trouve.*/
)
{
	FRESULT res;
	DIR dir;
	char *fn;   // This function is assuming non-Unicode cfg.
	uint8_t NbFile = 0;
	uint8_t CompareSize = 0;
	uint8_t Invert = FALSE;
	uint8_t fTrouve;
	uint8_t car1, car2;
	uint8_t NameSize;

	// Gestion de l'inversion
	if (FileExtension[0] == '*')
		Invert = TRUE;

	// Ouverture répertoire a scanner
	if (f_opendir(&dir, Dir) == FR_OK) {

		// Calcul de la taille du fichier a trouve en fonction de l'inversion
		if (Invert == FALSE)
		{
			while (CompareSize < strlen(FileExtension))
			{
				if (FileExtension[CompareSize] == '*')
					break;
				CompareSize++;
			}
		}
		else
		{
			CompareSize =  strlen(FileExtension) - 1;
		}

		// Balayage du répertoire
		while (1) {

			// Lecture de l'objet Repertoire
			res = f_readdir(&dir, &FileInfo);
			if (res != FR_OK || FileInfo.fname[0] == 0)
				break;
			if (FileInfo.fname[0] == '.')
				continue;				// Ignore dot entry

			fn = FileInfo.fname;
			NameSize = strlen(FileInfo.fname);
			#if _USE_LFN
				fn = FileInfo.lfname;
				NameSize = (uint32_t) memchr(fn, 0, _MAX_LFN) - (uint32_t) fn;
			#endif

			if (NameSize == 0)
				continue;

			// Comparaison avec le fichier cherche
			fTrouve = FALSE;
			if (Invert == FALSE)
			{
				if (strncmp(FileExtension, fn, CompareSize) == 0)
					fTrouve = TRUE;
			}
			else{

				fTrouve = TRUE;
				for (int i=0; i<CompareSize; i++){

					car1 = FileExtension[CompareSize-i];
					car2 = fn[NameSize - 1 - i];
					if (car1 != car2)
					{
						fTrouve = FALSE;
						break;
					}
				}
			}

			if (fTrouve)
			{
				NbFile++;
				if ((RetFileNum == NbFile) && (RetFileName != NULL))
				{
					RetFileName[0] = 0;
					strcpy(RetFileName, fn);
				}
			}
		}

	}
	else
		return (-1);

	return NbFile;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
MemFAT_Status_e
MemoireFAT_IsDirectoryPresent(
		char* 	DirName,			/**<[in] Nom du repertoire recherche.*/
		Bool_e	CreateIfMissing		/**<[in] Creation du repertoire si absent.*/
)
{
	DIR	Dir;
	FRESULT res;
	char buffer[100];
	char* pChaineRestante;

	// Tentative d'ouverture du repertoire
	res = f_opendir(&Dir, DirName);
	if (res == FR_OK)
	{
		f_closedir(&Dir);
		return MemFAT_OK;
	}

	if (CreateIfMissing == FALSE)
		return MemFAT_Error;

	// Creation de l'arborescence des repertoires si necessaire
	pChaineRestante = strchr(DirName,'/');
	while (pChaineRestante != NULL)
	{
		memset(buffer, 0, 100);
		strncpy(buffer, DirName, pChaineRestante - DirName);

		// Si le dossier n'existe pas, on le cree
		res = f_opendir(&Dir, buffer);
		if (res != FR_OK)
		{
			if (res != FR_NO_PATH)
				return MemFAT_Error;

			_MEMOIRE_CONSOLE("Creation repertoire \"%s\"\n", buffer);
			if (f_mkdir(buffer) != FR_OK)
				return MemFAT_Error;
		}
		f_closedir(&Dir);

		pChaineRestante = strchr(pChaineRestante + 1, '/');
	}

	return MemFAT_DirCreated;
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
MemFAT_Status_e
MemoireFAT_IsFilePresent(
		FIL*	pFile,				/**<[in] Handle du fichier.*/
		char* 	FileName,			/**<[in] Nom du fichier recherche.*/
		int		ModeAcces,			/**<[in] Mode d'acces au fichier */
		Bool_e	CreateIfMissing,	/**<[in] Creation du fichier si absent.*/
		Bool_e	KeepOpened			/**<[in] Conserver la fichier ouvert.*/
)
{
	Bool_e FileOk = FALSE;
	FRESULT res;

	// Tentative d'ouverture du fichier
	if (f_open(pFile, FileName, ModeAcces | FA_OPEN_EXISTING) == FR_OK)
	{
		if (KeepOpened == FALSE)
			f_close(pFile);
		return MemFAT_OK;
	}

	// Creation du fichier si demande
	if (CreateIfMissing == TRUE)
	{
		if (f_open(pFile, FileName, ModeAcces | FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
		{
			_MEMOIRE_CONSOLE("Creation Fichier \"%s\"\n", FileName);
			if (KeepOpened == FALSE)
				f_close(pFile);
			return MemFAT_FileCreated;
		}
	}

	return MemFAT_Error;
}


#if 0	//TODO JD : revoir la methode d'acces aux infos fichiers.
/**-----------------------------------------------------------------------------
 * @brief	Recupération du pointeur d'information fichier.
 *
 * @retval	pFileInfo		Pointeur vers FileInfo.
 */
FILINFO* Memoire_GetpFileInfo()
{
	return &FileInfo;
}
#endif


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

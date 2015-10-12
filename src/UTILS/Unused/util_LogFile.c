/**
 ***************************************************************************************************
 * @file	util_LogFile.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	29 juil. 2014
 * @brief   Gestion du fichier de log.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "util_LogFile.h"
#include "util_printf.h"
#include "util_TSW.h"
#include "FatFs/ff.h"


/* External Variables *****************************************************************************/


/** @addtogroup util_LogFile
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

#define DIRECTORY_NAME	"LOGS"
#define BUFFER_SIZE		256


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

FIL File;
Horodatage_s Time;
char Buffer[BUFFER_SIZE];

#define _OUT(a)	f_puts(a, &File)
//#define _OUT	_printf

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
 * Validation OK - JD 31/07/14
 */
void
LogFile_Write(
		char*	ComposantName,		/**<[in] Nom du composant.*/
		uint8_t	LogLevel,			/**<[in] Niveau de Log.*/
		char*	str					/**<[in] chaine a logger.*/
)
{
	DIR dir;
	Bool_e FileOk = FALSE;
	FRESULT res;

	// Lecture de l'heure actuelle
	RTC_Lire(&Time);

	// Si le dossier n'existe pas, on le cree
	res = f_opendir(&dir, DIRECTORY_NAME);
	if (res != FR_OK)
	{
		if (res != FR_NO_PATH)
			return;

		_printf("Creation repertoire %s\n", DIRECTORY_NAME);
		f_mkdir(DIRECTORY_NAME);
	}
	f_closedir(&dir);

	// Construction du nom de fichier
	_sprintf(Buffer, "%s/LOG_%d-%02d-%02d.csv",	DIRECTORY_NAME, Time.Annee, Time.Mois, Time.Jour);

	// Si le fichier n'existe pas, on le cree et on ajoute les noms de colonnes
	if (f_open(&File, Buffer, FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
	{
		if (f_open(&File, Buffer, FA_WRITE | FA_CREATE_NEW) == FR_OK)
		{
			f_puts("AAAA-MM-JJ;HH:MM:SS;TS;COMPO;LEVEL;LOG\n", &File);
			FileOk = TRUE;
		}
	}
	else
		FileOk = TRUE;

	// Le fichier existe et a ete ouvert
	if (FileOk)
	{
		// Positionnement en fin de fichier
		f_lseek(&File, f_size(&File));

		// Construction entete de ligne
		_sprintf(Buffer, "%d-%02d-%02d;%02d:%02d:%02d;%08d;",
				Time.Annee, Time.Mois, Time.Jour,
				Time.Heure, Time.Minute, Time.Seconde,
				TSW_GetTimestamp_ms()
		);
		f_puts(Buffer, &File);			// Ecriture Entete de ligne
		f_puts(ComposantName, &File);	// Ecriture Nom Composant

		_sprintf(Buffer, ";%02d;", LogLevel);
		f_puts(Buffer, &File);			// Ecriture LogLevel
		f_puts(str, &File);				// Ecriture Texte de log
		f_puts("\n", &File);			// Fin de ligne

	}
	f_close(&File);
	//f_sync(&File);
}


/**
 ***************************************************************************************************
 * Validation OK - JD 31/07/14
 */
void
LogFile_Print(
		void
)
{
	uint32_t Cursor=0;
	uint32_t br;

	// Lecture de l'heure actuelle
	RTC_Lire(&Time);

	// Construction du nom de fichier
	_sprintf(Buffer, "%s/LOG_%d-%02d-%02d.csv",	DIRECTORY_NAME, Time.Annee, Time.Mois, Time.Jour);

	if (f_open(&File, Buffer, FA_READ) != FR_OK)
		return;

	_printf("--- LOGFILE : \"%s\" ---\n", Buffer);

	while (f_gets(Buffer, BUFFER_SIZE, &File))
		_printf("%s", Buffer);

	_printf("--- FIN LOGFILE ---\n", Buffer);

	f_close(&File);
}


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

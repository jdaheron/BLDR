/**
 ***************************************************************************************************
 * @file	fct_MemoireFAT.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	25 juil. 2014
 * @brief   Gestion de la memoire FAT.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef FCT_MEMOIREFAT_H_
#define FCT_MEMOIREFAT_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "FatFs/ff.h"
#include "ff_gen_drv.h"


/** @defgroup fct_MemoireFAT fct_MemoireFAT
  * @brief Gestion de la memoire FAT.
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

/** Definition des status de verification de fichiers et repertoires. */
typedef enum
{
	MemFAT_OK = 0,
	MemFAT_Error,
	MemFAT_FileCreated,
	MemFAT_DirCreated

} MemFAT_Status_e;

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
 * @brief	Initialisation de la memoire.
 * @return 	TRUE=Init OK, FALSE=Init KO
 */
Bool_e
MemoireFAT_Init(
		Diskio_drvTypeDef* DiskDriver		/**<[in] Driver du disque a initialiser.*/
);

/**
 * @brief	De-Initialisation de la memoire FAT.
 * @return 	void
 */
void
MemoireFAT_DeInit(
		void
);

/**
 * @brief	Lecture de l'etat de disponibilite de la memoire.
 * @return 	void
 */
Bool_e
MemoireFAT_IsReady(
		void
);

/**
 * @brief	Autorisation de l'acces a la memoire pour l'applicatif.
 * @return 	void
 */
void
MemoireFAT_AutoriserAcces(
		void
);

/**
 * @brief	Interdiction de l'acces a la memoire pour l'applicatif.
 * @return 	void
 */
void
MemoireFAT_InterdireAcces(
		void
);

/**
 * @brief	Fonction principale de la fonctionnalite Memoire FAT.
 * @return 	void
 */
void
MemoireFAT_Main(
		void
);

/**
 * @brief	Effacement de la memoire.
 * @return 	TRUE=Effacement OK, FALSE=Effacement KO
 */
Bool_e
MemoireFAT_Erase(
		void
);

/**
 * @brief	Envoi de la liste de fichiers sur la console.
 * @return 	void
 */
void
MemoireFAT_PrintFileList(
		char* path		/**<[in] Repertoire a scanner.*/
);

/**
 * @brief	Ecriture du contenu d'un fichier ouvert dans la console.
 * @return 	void
 */
void MemoireFAT_PrintOpenedFile(
		FIL* 	pFile	/**<[in] Fichier*/
);

/**
 * @brief	Ecriture du contenu d'un fichier dans la console.
 * @return 	void
 */
void
MemoireFAT_PrintFile(
		char* 	FileName	/**<[in] Nom du fichier*/
);

/**
 * @brief	Recherche d'un fichier par son Nom.
 * @return 	Nombre de fichier trouve.
 */
int8_t
MemoireFAT_SearchFileByName(
		char* Dir,				/**<[in] Repertoire a scanner.*/
		char* FileName,			/**<[in] Nom de fichier a trouver.*/
		uint8_t RetFileNum,		/**<[in] Numero de fichier a retourner.*/
		char* RetFileName		/**<[out] Nom de fichier trouve.*/
);

/**
 * @brief	Recherche d'un fichier par son Extension.
 * @return 	Nombre de fichier trouve.
 */
int8_t
MemoireFAT_SearchFileByExtension(
		char* Dir,				/**<[in] Repertoire a scanner.*/
		char* FileExtension,	/**<[in] Nom de fichier a trouver.*/
		uint8_t RetFileNum,		/**<[in] Numero de fichier a retourner.*/
		char* RetFileName		/**<[out] Nom de fichier trouve.*/
);


/**
 * @brief	Verification de la presence d'un repertoire.
 * @return 	Status
 */
MemFAT_Status_e
MemoireFAT_IsDirectoryPresent(
		char* 	DirName,			/**<[in] Nom du repertoire recherche.*/
		Bool_e	CreateIfMissing		/**<[in] Creation du repertoire si absent.*/
);

/**
 * @brief	Verification de la presence d'un fichier.
 * @return 	Status
 */
MemFAT_Status_e
MemoireFAT_IsFilePresent(
		FIL*	pFile,				/**<[in] Handle du fichier.*/
		char* 	FileName,			/**<[in] Nom du fichier recherche.*/
		int		ModeAcces,			/**<[in] Mode d'acces au fichier */
		Bool_e	CreateIfMissing,	/**<[in] Creation du fichier si absent.*/
		Bool_e	KeepOpened			/**<[in] Conserver la fichier ouvert.*/
);


	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* FCT_MEMOIREFAT_H_ */

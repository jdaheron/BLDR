/**
 ***************************************************************************************************
 * @file	fct_IniFile.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	28 août 2014
 * @brief   Gestion de fichier INI.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef FCT_INIFILE_H_
#define FCT_INIFILE_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "fct_MemoireFAT.h"


/** @defgroup fct_IniFile fct_IniFile
  * @brief Gestion de fichier INI.
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

/** Definition des types de cle. */
typedef enum
{
	IniFile_StatusOK = 0,			/**< Status OK.*/
	IniFile_StatusKO,				/**< Status KO.*/
	IniFile_StatusDefaultValue,		/**< Valeurs par defaut restaurees.*/
	IniFile_StatusFileCreated,		/**< Fichier cree (avec les valeurs par defaut).*/

} IniFile_Status_e;

/** Definition des types de cle. */
typedef enum
{
	KeyType_SectionName = 0,	/**< Type nom de section.*/
	KeyType_Int,				/**< Type entier.*/
	KeyType_Str,				/**< Type chaine.*/
//	KeyType_Float,				/**< Type flottant.*/

} KeyType_e;

/** Definition d'une cle. */
typedef struct {

	KeyType_e	KeyType;		/**< Type de cle.*/
	const char*	SectionName;	/**< Nom de la section.*/
	const char*	KeyName;		/**< Nom de la cle.*/
	void*		Value;			/**< Valeur par defaut.*/
	void*		MinValue;		/**< Valeur min.*/
	void*		MaxValue;		/**< Valeur max.*/

} Key_s;

/** Contenu d'un fichier Ini. */
typedef struct {

	const char*		FileName;	/**< Nom du fichier.*/
	const Key_s*	KeyTable;	/**< Table de donnees du fichier.*/
	const uint32_t	NbKey;		/**< Nombre de cles.*/

} IniFile_s;

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
 * @brief	Initilisation d'un fichier INI (Le fichier doit etre ferme).
 * @return 	Status.
 */
IniFile_Status_e
IniFile_Init(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
);

/**
 * @brief	Ouverture d'un fichier INI pour lecture.
 * @return 	Status.
 */
IniFile_Status_e
IniFile_OpenRead(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
);

/**
 * @brief	Ouverture d'un fichier INI pour ecriture.
 * @return 	Status.
 */
IniFile_Status_e
IniFile_OpenWrite(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
);

/**
 * @brief	Fermeture d'un fichier INI.
 * @return 	void.
 */
void
IniFile_Close(
		const IniFile_s*	IniFile		/**<[in] Descripteur du contenu du fichier.*/
);

/**
 * @brief	Lecture d'une valeur dans un fichier ouvert.
 * @return 	Status.
 */
Status_e
IniFile_Read(
		Key_s*				pKey,		/**<[in] Cle a lire.*/
		void*				pValue		/**<[in] Pointeur vers la valeur lue.*/
);

/**
 * @brief	Ecriture d'une valeur dans un fichier ouvert.
 * @return 	Status.
 */
Status_e
IniFile_Write(
		Key_s*				pKey,		/**<[in] Cle a ecrire.*/
		void*				Value		/**<[in] Valeur a ecrire.*/
);

/**
 * @brief	Ecriture d'une valeur brute dans un fichier ouvert.
 * @return 	Status.
 */
Status_e
IniFile_WriteRawData(
		Key_s*				pKey,		/**<[in] Cle a ecrire.*/
		void*				Value		/**<[in] Valeur a ecrire.*/
);


	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* FCT_INIFILE_H_ */

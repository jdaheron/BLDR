/**
 ***************************************************************************************************
 * @file	util_CSV.h
 * @author	user01
 * @version	1.0.0
 * @date	28 juil. 2014
 * @brief   Parseur de fichier CSV.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef UTIL_CSV_H_
#define UTIL_CSV_H_


/* Includes ***************************************************************************************/
#include "ff.h"
#include "BSP.h"




/** @defgroup util_CSV util_CSV
  * @brief Parseur de fichier CSV.
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

/** @brief Status CSV */
typedef enum {
	
	CSV_Status_Operation_KO = 0,
	CSV_Status_Operation_OK,

	CSV_Status_RenommageImpossible,
	CSV_Status_FermetureFichierImpossible,
	CSV_Status_OuvertureImpossible,
	CSV_Status_FichierNonCSV,

} CSV_Status_e;


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
 * @brief	Lecture d'une chaine dans une fichier CSV
 * @return 	Status
 */
CSV_Status_e
CSV_ReadString(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	char*			str				/**<[out] Chaine lue. */
);

/**
 * @brief	Ecriture d'une chaine dans une fichier CSV
 * @return 	Status
 */
CSV_Status_e
CSV_WriteString(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	const char*		str				/**<[in] Chaine a ecrire. */
);

/**
 * @brief	Lecture d'une chaine dans une fichier CSV
 * @return 	Status de la lecture
 */
CSV_Status_e
CSV_ReadInteger(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	uint32_t*		integer			/**<[out] Donnee a lire. */
);

/**
 * @brief	Ecriture d'une chaine dans une fichier CSV
 * @return 	Status
 */
CSV_Status_e
CSV_WriteInteger(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	uint32_t*		integer			/**<[in] Donnee a ecrire. */
);

	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* UTIL_CSV_H_ */

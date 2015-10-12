/**
 ***************************************************************************************************
 * @file	util_CSV.c
 * @author	user01
 * @version	1.0.0
 * @date	28 juil. 2014
 * @brief   Parseur de fichier CSV.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "util_CSV.h"
#include "util_printf.h"


/* External Variables *****************************************************************************/


/** @addtogroup util_CSV
  * @{
  */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

typedef enum {
	Etape_Placement_Ligne = 0,
	Etape_Placement_Colonne,
}Etape_Placement_e;

/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */

/** Configuration du Parseur CSV */
#define TAILLE_LIGNE_MAX				50
#define INDICE_PREMIERE_LIGNE			1
#define INDICE_PREMIERE_COLONNE			1
#define NOMBRE_LIGNE_MAX				100
#define CSV_SEPARATOR					';'
#define NB_CHAR_CSV_MAX					NOMBRE_LIGNE_MAX*TAILLE_LIGNE_MAX


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */

/** Tests */
//---------- Test fichier
#define __IsFileAskedForNotA_CSV(Path)	((strstr(Path, ".csv") == NULL) && (strstr(Path, ".txt") == NULL))



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
 
/**
 * @}
 */ 

/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 

/***************************************************************************************************
* @brief	Placement du curseur ligne
* @return 	Taille de la ligne devant laquelle le curseur est placee
*/
uint32_t
CSV_PlaceLine(
		FIL*			pFile,			/**<[in] Pointeur Fichier.*/
		uint32_t		Indice_Ligne	/**<[in] Numero de ligne.*/
) {

	//------------------------------- Declaration des variables
	uint8_t 	Ligne[TAILLE_LIGNE_MAX];
	uint8_t* 	pLigne = Ligne;
	uint32_t	Line_Length[NOMBRE_LIGNE_MAX];
	uint32_t	cmpt_lgn;
	uint32_t 	b_Ligne;
	uint32_t	b_Calcul_Offset;
	uint32_t	offset;

	offset = 0;
	memset(pLigne, 		0, TAILLE_LIGNE_MAX);
	memset(Line_Length, 0, NOMBRE_LIGNE_MAX);


		//------------------------------- Placement sur la ligne Version B
		cmpt_lgn = 0;
		for(b_Ligne=INDICE_PREMIERE_LIGNE; b_Ligne<NB_CHAR_CSV_MAX; b_Ligne++) {
			if(cmpt_lgn == Indice_Ligne) {
				b_Ligne = NB_CHAR_CSV_MAX;
				for(b_Calcul_Offset=0; b_Calcul_Offset<cmpt_lgn-1; b_Calcul_Offset++)
					offset += Line_Length[b_Calcul_Offset];
				if(Indice_Ligne == 1)
					f_lseek(pFile, 0);
				else
					f_lseek(pFile, offset+cmpt_lgn);
			} else {
				f_gets((char*)pLigne, 2, pFile);

				if(*pLigne == '\n') {
					cmpt_lgn++; Line_Length[cmpt_lgn]++;
					if(cmpt_lgn != Indice_Ligne) {
						pLigne = Ligne;
						memset(pLigne, 0, TAILLE_LIGNE_MAX);
					}
				} else {
					pLigne++; Line_Length[cmpt_lgn]++;
				}
			}
		}

	return Line_Length[cmpt_lgn-1];
}

/***************************************************************************************************
* @brief	Placement du curseur colonne
* @return 	Taille de la colonne devant laquelle le curseur est placee
*/
uint32_t
CSV_PlaceColumn(
		FIL* 		pFile,				/**<[in] Pointeur Fichier.*/
		uint32_t	Line_Length,		/**<[in] longueur de la ligne.*/
		uint32_t	Indice_Colonne		/**<[in] Numero de colonne.*/
) {

	Bool_e		LastColumn 	= FALSE;
	Bool_e		FirstLine	= FALSE;
	uint8_t 	Ligne[TAILLE_LIGNE_MAX];
	uint8_t* 	pLigne = Ligne;
	uint32_t 	b_Column;
	uint32_t	cmpt_column;
	uint32_t	Column_Length;

	Column_Length 	= 0;
	cmpt_column 	= 0;
	memset(pLigne, 0, TAILLE_LIGNE_MAX);

	if(f_tell(pFile) == 0)
		FirstLine = TRUE;

	for(b_Column= INDICE_PREMIERE_COLONNE; b_Column<=Line_Length+1; b_Column++) {
		if(cmpt_column == Indice_Colonne) {
			b_Column = Line_Length+1;
			f_lseek(pFile, f_tell(pFile)-Column_Length);
		} else {
			f_gets((char*)pLigne, 2, pFile);
			if( (*pLigne == CSV_SEPARATOR) || (*pLigne == '\n') ) {
				 cmpt_column++;
				if(b_Column >= Line_Length && FirstLine == FALSE) {
					Column_Length++;
					LastColumn = TRUE;
				}
				Column_Length++;
				if(cmpt_column != Indice_Colonne)
					Column_Length = 0;
			} else {
				Column_Length++; pLigne++;
			}
		}
	}

	if(LastColumn == TRUE)
		Column_Length--;

	return Column_Length;
}

/***************************************************************************************************
 * @brief	Placement du curseur dans le fichier CSV
 * @return 	Taille de la colonne devant laquelle le curseur est placee
 */
uint32_t
CSV_Lecture_PlaceCursor(
		FIL* 		pFile,			/**<[in] Pointeur Fichier.*/
		uint32_t	Indice_Ligne,	/**<[in] Numero de ligne.*/
		uint32_t	Indice_Colonne	/**<[in] Numero de colonne.*/
) {

	//------------------------------- Declaration des variables
	uint32_t	Line_Length;
	uint32_t	Column_Length;

	if(Indice_Ligne == 0 || Indice_Colonne == 0)
		return 0;
	//------------------------------- Placement du curseur
	Line_Length = CSV_PlaceLine(pFile, Indice_Ligne);

	if(Line_Length == 0)
		return Line_Length;

	Column_Length = CSV_PlaceColumn(pFile, Line_Length, Indice_Colonne);

	return Column_Length;
}

/***************************************************************************************************
 * @brief	Placement du curseur dans le fichier CSV
 * @return 	Taille de la colonne devant laquelle le curseur est placee
 */
uint32_t
CSV_Ecriture_PlaceCursor(
		FIL* 		pFile,			/**<[in] Pointeur Fichier.*/
		uint32_t	Indice_Ligne,	/**<[in] Numero de ligne.*/
		uint32_t	Indice_Colonne	/**<[in] Numero de colonne.*/
) {

	// Declaration des variables
	Etape_Placement_e	Etape = 0;
	char	 	car[2];
	uint32_t	cpt_line;
	uint32_t 	CurrentLine;
	uint32_t	cpt_column;

	if(Indice_Ligne < INDICE_PREMIERE_LIGNE || Indice_Colonne < INDICE_PREMIERE_COLONNE)
		return 0;

	cpt_line 	= INDICE_PREMIERE_LIGNE;
	cpt_column = INDICE_PREMIERE_COLONNE;

	// Placement du curseur
	for(CurrentLine=INDICE_PREMIERE_LIGNE; CurrentLine<NB_CHAR_CSV_MAX; CurrentLine++) {

		switch(Etape) {

			//--------------------------------------------------------------------------------------
			case Etape_Placement_Ligne:
				if(cpt_line == Indice_Ligne)
					Etape = Etape_Placement_Colonne;
				else
				{
					f_gets(car, 2, pFile);
					if(car[0] == '\n')
						cpt_line++;
				}
				break;

			//-----------------------------------------------------------------------------------
			case Etape_Placement_Colonne:
				if(cpt_column == Indice_Colonne)
					return f_tell(pFile);
				else
				{
					f_gets(car, 2, pFile);
					if(car[0] == CSV_SEPARATOR)
						cpt_column++;
				}
				break;
		}
	}

	return f_tell(pFile);
}

 /**
 * @}
 */ 

/**
 ***************************************************************************************************
 * @todo Validation
 */
CSV_Status_e
CSV_ReadString(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	char*			str				/**<[out] Chaine a lue. */
)
{
	// Declaration des variables
	uint32_t	Column_Length;
	FRESULT		Test_Retour;
	FIL 		File_Pointer;

	// Test sur le fichier demande
	if(__IsFileAskedForNotA_CSV(FileName))
		return CSV_Status_FichierNonCSV;

	// Ouverture du fichier
	Test_Retour = f_open(&File_Pointer, FileName, FA_OPEN_EXISTING | FA_READ);
	if(Test_Retour != FR_OK)
		return CSV_Status_OuvertureImpossible;

	// Placement du curseur ligne
	Column_Length = CSV_Lecture_PlaceCursor(&File_Pointer, Indice_Ligne, Indice_Colonne);

	// Lecture de la ligne
	if(Column_Length != 0)
		f_gets(str, Column_Length, &File_Pointer);
	else
		return CSV_Status_Operation_KO;

	// Fermeture du fichier
	Test_Retour = f_close(&File_Pointer);
	if(Test_Retour != FR_OK)
		return CSV_Status_FermetureFichierImpossible;

	return CSV_Status_Operation_OK;
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
CSV_Status_e
CSV_WriteString(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	const char*		str				/**<[in] Chaine a ecrire. */
)
{
	// Declaration des variables
	uint32_t	b_reecriture;
	uint32_t	ToCursorLength;
	uint32_t	ColumnLength;
	FRESULT		Test_Retour;
	FIL 		File_Pointer_Origine;
	FIL 		File_Pointer_Destination;
	uint8_t 	Ligne[1];
	uint8_t* 	pLigne = Ligne;
	UINT		Nb_Reads = 0, Nb_Writes = 0;

	memset(pLigne, 0, 1);

	// Test sur le fichier demande
	if(__IsFileAskedForNotA_CSV(FileName))
		return CSV_Status_FichierNonCSV;

	// Ouverture du fichier
	Test_Retour = f_open(&File_Pointer_Origine, FileName, FA_OPEN_EXISTING | FA_READ);
	if(Test_Retour != FR_OK)
		return CSV_Status_OuvertureImpossible;

	// Creation d'un fichier temporaire
	Test_Retour = f_open(&File_Pointer_Destination, "csv.tmp", FA_CREATE_ALWAYS | FA_WRITE);
	if(Test_Retour != FR_OK)
		return CSV_Status_OuvertureImpossible;

	// Placement du curseur ligne
	ToCursorLength = CSV_Ecriture_PlaceCursor(&File_Pointer_Origine, Indice_Ligne, Indice_Colonne);

	// Ecriture jusqu'a l'emplacement choisi
	f_lseek(&File_Pointer_Origine, 0);
	for(b_reecriture=0; b_reecriture<ToCursorLength ;b_reecriture++) {
		f_read(&File_Pointer_Origine, pLigne, 1, &Nb_Reads);
		f_write(&File_Pointer_Destination, pLigne, 1, &Nb_Writes);
	}

	// Ecriture de la nouvelle donnee
	//f_puts(";", &File_Pointer_Destination);
	f_puts(str, &File_Pointer_Destination);
	f_puts(";", &File_Pointer_Destination);

	// Ecriture de la fin
	f_lseek(&File_Pointer_Origine, 0);
	ColumnLength = CSV_Lecture_PlaceCursor(&File_Pointer_Origine, Indice_Ligne, Indice_Colonne);
	f_lseek(&File_Pointer_Origine, f_tell(&File_Pointer_Origine) + ColumnLength /*+ 1*/);
	for(b_reecriture=0; b_reecriture<NB_CHAR_CSV_MAX-ToCursorLength ;b_reecriture++) {
		f_read(&File_Pointer_Origine, pLigne, 1, &Nb_Reads);
		f_write(&File_Pointer_Destination, pLigne, 1, &Nb_Writes);
		if(f_eof(&File_Pointer_Origine) == 1)
			b_reecriture = NB_CHAR_CSV_MAX - ToCursorLength;
	}

	// Fermeture des fichier
	Test_Retour = f_close(&File_Pointer_Origine);
	if(Test_Retour != FR_OK)
		return CSV_Status_FermetureFichierImpossible;
	Test_Retour = f_close(&File_Pointer_Destination);
	if(Test_Retour != FR_OK)
		return CSV_Status_FermetureFichierImpossible;

	// Renomage du fichier de sortie et effacement du fichier temporaire
	Test_Retour = f_unlink(FileName);
	if(Test_Retour != FR_OK)
		return CSV_Status_RenommageImpossible;
	Test_Retour = f_rename("csv.tmp", FileName);
	if(Test_Retour != FR_OK)
		return CSV_Status_RenommageImpossible;

	return CSV_Status_Operation_OK;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
CSV_Status_e
CSV_ReadInteger(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	uint32_t*		integer			/**<[out] Donnee lue. */
)
{
	char Buffer[16];
	CSV_Status_e Status;

	Status = CSV_ReadString(FileName, Indice_Ligne, Indice_Colonne, Buffer);

	// Si lecture effectuee
	if (Status == CSV_Status_Operation_OK)
		*integer = strtoul(Buffer, 0, 10);

	return Status;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
CSV_Status_e
CSV_WriteInteger(
	const TCHAR* 	FileName,		/**<[in] Nom vers le fichier CSV.*/
	uint32_t		Indice_Ligne,	/**<[in] Numero de ligne.*/
	uint32_t		Indice_Colonne,	/**<[in] Numero de colonne.*/
	uint32_t*		integer			/**<[in] Donnee a ecrire. */
)
{
	char Buffer[16];

	_sprintf(Buffer, "%d", *integer);

	// Ecriture dans la fichier
	return CSV_WriteString(FileName, Indice_Ligne, Indice_Colonne, Buffer);
}

 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

/**
 ***************************************************************************************************
 * @file	fct_SwFlash.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	15 sept. 2014
 * @brief   Gestion de logiciels en Flash.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef FCT_SWFLASH_H_
#define FCT_SWFLASH_H_


/* Includes ***************************************************************************************/

#include "BSP.h"


/** @defgroup fct_SwFlash fct_SwFlash
  * @brief Gestion de logiciels en Flash.
  * @{
  */

/**
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */

#define USE_BKP			0
#define VERSION_SIZE	14	// "AL12345XYZ"


 /**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */


/** Definition d'un logiciel en flash. */
typedef struct {

	uint32_t StartAdress;
	uint32_t Size;
	uint32_t Checksum;
	char	Version[VERSION_SIZE+1];
	uint8_t	IsValide;

} SwFlash_Info_s;


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
 * @brief	Preparation au demarrage de l'application.
 * @return 	Status (OK/KO/EnCours)
 */
Status_e
SwFlash_PreparerApplication(
		void
);


/**
 * @brief	Lecture de l'adresse de debut de l'application.
 * @return 	App Start Address.
 */
uint32_t
SwFlash_GetAppAddress(
		void
);

/**
 * @brief	Lecture de la version SW de l'application.
 * @return 	Version SW.
 */
char*
SwFlash_GetAppSwVersion(
		void
);

/**
 * @brief	Sauvegarde des informations de l'applicatif.
 * @return 	void.
 */
void
SwFlash_SaveAppInfo(
		SwFlash_Info_s* pSwInfo
);



 /**
 * @}
 */


 /**
 * @}
 */


#endif /* FCT_SWFLASH_H_ */

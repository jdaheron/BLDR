/**
 *******************************************************************************
 * @file	drv_CAT24AA16.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	02 dec. 2013
 * @brief   Gestion de l'EEPROM I2C CAT24AA16.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_CAT24AA16_H_
#define DRV_CAT24AA16_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"

/**
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */



/**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */

 /**
 * @}
 */

#define CAT24AA16_SLAVE_ADDRESS		0xA0
#define CAT24AA16_PAGE_SIZE			16
#define CAT24AA16_SIZE				2048

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
 * @defgroup Exported_Functions Exported Functions
 * @{
 */


/**
 * @brief	Lecture d'une donnee depuis la flash
 * @return 	void
 */
void
CAT24AA16_Read(
		uint32_t Address,	/**<[in] Adresse dans l'EEPROM*/
		uint8_t* pData,		/**<[in] Pointeur vers l'emplacement de la donnee a ecrire*/
		uint16_t Taille		/**<[in] Taille des donnees a ecrire*/
);


/**
 * @brief	Ecriture d'une donnee dans la flash
 * @return 	void
 */
void
CAT24AA16_Write(
		uint32_t Address,	/**<[in] Adresse dans l'EEPROM*/
		uint8_t* pData,		/**<[in] Pointeur vers l'emplacement pour enregistrer la valeur lue*/
		uint16_t Taille		/**<[in] Taille des donnees a lire*/
);



#endif /* DRV_CAT24AA16_H_ */

/**
* @}
*/



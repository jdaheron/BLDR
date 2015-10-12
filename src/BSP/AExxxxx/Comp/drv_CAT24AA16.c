/**
 ***************************************************************************************************
 * @file	drv_CAT24AA16.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	1 juil. 2014
 * @brief   Gestion de l'EEPROM I2C CAT24AA16.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "drv_CAT24AA16.h"


/* External Variables *****************************************************************************/


/** @addtogroup drv_CAT24AA16
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
 * @todo Validation
 */
void CAT24AA16_Read(uint32_t Address,	uint8_t* pData, uint16_t Taille)
{
	uint16_t NbRW = 1;
	uint16_t iRW;
	uint16_t TailleRW = CAT24AA16_PAGE_SIZE;
	uint8_t AddressMSB;
	uint8_t AddressLSB;

	if (Address + Taille >= CAT24AA16_SIZE)
		return;

	// Calcul du nombre de lecture a effectuer
	while (Taille > CAT24AA16_PAGE_SIZE) {
		NbRW++;
		Taille -= CAT24AA16_PAGE_SIZE;
	}

	// Lecture des pages une par une
	for (iRW=0; iRW<NbRW; iRW++)
	{
		if (iRW == (NbRW - 1)) {
			TailleRW = Taille;
		}

		// Calcul de l'adresse
		AddressMSB = (uint8_t) ((Address >> 7) & 0x0E);
		AddressLSB = (uint8_t) (Address & 0x00FF);

		HAL_EEPROM_ReadBuffer(CAT24AA16_SLAVE_ADDRESS + AddressMSB, AddressLSB, pData, TailleRW);

		Address += TailleRW;
		pData += TailleRW;
	}
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
void CAT24AA16_Write(uint32_t Address, uint8_t* pData, uint16_t Taille)
{
	uint16_t NbRW = 1;
	uint16_t iRW;
	uint16_t TailleRW;
	uint8_t AddressMSB;
	uint8_t AddressLSB;

	if (Address + Taille >= CAT24AA16_SIZE)
		return;

	// Si adresse de debut non alignee sur un debut de page, ecriture de la fin de page precedente
	if (Address % CAT24AA16_PAGE_SIZE != 0)
	{
		// Calcul de la taille a ecrire
		TailleRW  = ((uint16_t) Address + CAT24AA16_PAGE_SIZE) / CAT24AA16_PAGE_SIZE;
		TailleRW *= CAT24AA16_PAGE_SIZE;
		TailleRW -= Address;
		if (TailleRW > Taille)
			TailleRW = Taille;

		// Calcul de l'adresse
		AddressMSB = (uint8_t) ((Address >> 7) & 0x0E);
		AddressLSB = (uint8_t) (Address & 0x00FF);

		HAL_EEPROM_WriteBuffer(CAT24AA16_SLAVE_ADDRESS + AddressMSB, AddressLSB, pData, TailleRW);
		Delay_ms(10);

		Address += TailleRW;
		pData += TailleRW;
		Taille -= TailleRW;
	}

	// Si d'autres pages sont a ecrire
	if (Taille > 0)
	{
		// Calcul du nombre d'ecriture a effectuer
		while (Taille > CAT24AA16_PAGE_SIZE) {
			NbRW++;
			Taille -= CAT24AA16_PAGE_SIZE;
		}

		// Ecriture des pages une par une
		TailleRW = CAT24AA16_PAGE_SIZE;
		for (iRW=0; iRW<NbRW; iRW++)
		{
			if (iRW == (NbRW - 1)) {
				TailleRW = Taille;
			}

			// Calcul de l'adresse
			AddressMSB = (uint8_t) ((Address >> 7) & 0x0E);
			AddressLSB = (uint8_t) (Address & 0x00FF);

			HAL_EEPROM_WriteBuffer(CAT24AA16_SLAVE_ADDRESS + AddressMSB, AddressLSB, pData, TailleRW);
			Delay_ms(10);

			Address += TailleRW;
			pData += TailleRW;
		}
	}
}







 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

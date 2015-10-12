/**
 *******************************************************************************
 * @file	drv_FLASH
 * @author	j.daheron
 * @version	1.0.0
 * @date	21 oct. 2013
 * @brief   Gestion de la Flash interne.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_FLASH.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un buffer en Flash interne.
 *
 * @param[in]	Address	Adresse du debut de lecture.
 * @param[in]	Size	Nombre de donnees a lire.
 * @param[out]	pBuffer	Donnees lues (NULL si vide).
 *
 */
void FLASH_Read(uint32_t Address, uint16_t Size, uint8_t* pBuffer)
{
	memcpy(pBuffer, (uint32_t *) Address, Size);
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un buffer.
 *
 * @param[in]	Address	Adresse du debut de l'ecriture.
 * @param[in]	pBuffer	Donnees a ecrire.
 * @param[in]	Size	Nombre de donnees a ecrire.
 *
 * @return		Status	Statut de l'ecriture.
 */
uint8_t FLASH_Write(uint32_t Address, uint8_t* pBuffer, uint16_t Size)
{
	uint8_t Status = Status_OK;

	// Desactivation des interruptions
	__disable_irq();

	// Deverrouillage de la flash
	FLASH_Unlock();

	// Ecriture des octets
	for (int i=0; i<Size; i++)
	{
		//TODO : OPTIMISATION DU TIMING POSSIBLE EN UTILISANT : FLASH_ProgramWord(uint32_t Address, uint32_t Data);

		if (FLASH_ProgramByte(Address + i, (uint8_t) *(pBuffer + i)) != FLASH_COMPLETE)
			Status = Status_KO;
	}

	// Verrouillage de la flash
	FLASH_Lock();

	// ReActivation des interruptions
	__enable_irq();

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Effacement d'un secteur de Flash.
 *
 * @param[in]	FlashSector	Secteur a effacer.
 *
 * @return		Status	Statut de l'ecriture.
 */
uint8_t FLASH_Erase(uint8_t FlashSector)
{
	uint32_t FLASH_Sector;
	FLASH_Status Status;

	// Effacement du secteur
	switch (FlashSector)
	{
		case 0:		FLASH_Sector = FLASH_Sector_0;	break;
		case 1:		FLASH_Sector = FLASH_Sector_1;	break;
		case 2:		FLASH_Sector = FLASH_Sector_2;	break;
		case 3:		FLASH_Sector = FLASH_Sector_3;	break;
		case 4:		FLASH_Sector = FLASH_Sector_4;	break;
		case 5:		FLASH_Sector = FLASH_Sector_5;	break;
		case 6:		FLASH_Sector = FLASH_Sector_6;	break;
		case 7:		FLASH_Sector = FLASH_Sector_7;	break;
		case 8:		FLASH_Sector = FLASH_Sector_8;	break;
		case 9:		FLASH_Sector = FLASH_Sector_9;	break;
		case 10:	FLASH_Sector = FLASH_Sector_10;	break;
		case 11:	FLASH_Sector = FLASH_Sector_11;	break;
		case 0xFF:	FLASH_Sector = 0xFF;			break;
		default:	return Status_KO;				break;

	}

	// Desactivation des interruptions
	__disable_irq();

	// Deverrouillage de la flash
	FLASH_Unlock();

	if (FLASH_Sector == 0xFF)
		Status = FLASH_EraseAllSectors(VoltageRange_3);
	else
		Status = FLASH_EraseSector(FLASH_Sector, VoltageRange_3);

	// Verrouillage de la flash
	FLASH_Lock();

	// ReActivation des interruptions
	__enable_irq();

	if (Status == FLASH_COMPLETE)
		return Status_OK;

	return Status_KO;
}


/**-----------------------------------------------------------------------------
 * @brief	Effacement d'un secteur de Flash en fonction de l'adresse.
 *
 * @param[in]	Address	Adresse du debut de l'ecriture.
 *
 * @return		Status	Statut de l'effacement.
 */
uint8_t FLASH_EraseAddress(uint32_t Address)
{
	int32_t FlashSector;

	FlashSector = FLASH_GetSectorNum(Address);
	if (FlashSector >= 0)
		return FLASH_Erase(FlashSector);

	return Status_KO;
}


/**-----------------------------------------------------------------------------
 * @brief	Conversion d'une adresse en numero de secteur.
 */
int8_t FLASH_GetSectorNum(uint32_t Address)
{
	if		(Address < 0x08000000)	return (-1);
	else if	(Address < 0x08004000)	return 0;
	else if	(Address < 0x08008000)	return 1;
	else if	(Address < 0x0800C000)	return 2;
	else if	(Address < 0x08010000)	return 3;
	else if	(Address < 0x08020000)	return 4;
	else if	(Address < 0x08040000)	return 5;
	else if	(Address < 0x08060000)	return 6;
	else if	(Address < 0x08080000)	return 7;
	else if	(Address < 0x080A0000)	return 8;
	else if	(Address < 0x080C0000)	return 9;
	else if	(Address < 0x080E0000)	return 10;
	else if	(Address < 0x08100000)	return 11;

	return (-1);
}




/**-----------------------------------------------------------------------------
 * @brief	FLASH_IRQHandler.
 *
 */
void FLASH_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}



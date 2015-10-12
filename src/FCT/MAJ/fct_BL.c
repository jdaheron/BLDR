/**
 *******************************************************************************
 * @file	fct_BL_CAN_Host
 * @author	j.daheron
 * @version	1.0.0
 * @date	11 dec. 2013
 * @brief   Gestion du bootloader CAN (Host).
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "fct_BL.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define BL_CONSOLE_ENABLE	0
#define PID_ADDRESS			0xE0042000	// "DBGMCU_IDCODE"


/* Private macros ------------------------------------------------------------*/

#if BL_CONSOLE_ENABLE
	#define _BL_CONSOLE	_printf
#else
	#define _BL_CONSOLE
#endif


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Validation des fonctions.
 *
 */
#define  VALIDATION_FCT_BL 0

#if VALIDATION_FCT_BL
void VALIDATION_SEQUENCE_FCT_BL(){

	_printf("VALIDATION_SEQUENCE_FCT_BL\n");
	while (1) {
	}

}
#endif


/**-----------------------------------------------------------------------------
 * @brief	Lecture du Device ID.
 */
uint8_t BL_GetID(uint16_t* ID)
{
	_BL_CONSOLE("BL_GetID\n");

	FLASH_Read(PID_ADDRESS, 2, ID);

	return Status_OK;
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture Memoire.
 */
uint8_t BL_ReadMemory(uint32_t Address, uint8_t* pData, uint16_t NbData)
{
	if (NbData > 256)
		return Status_KO;

	_BL_CONSOLE("BL_ReadMemory\n");

	FLASH_Read(Address, NbData, pData) ;

	return Status_OK;
}


/**-----------------------------------------------------------------------------
 * @brief	GOTO.
 */
uint8_t BL_GO(uint32_t Address)
{
	_BL_CONSOLE("BL_GO\n");

	GOTO(Address);

	return Status_OK;
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture Memoire.
 */
uint8_t BL_WriteMemory(uint32_t Address, uint8_t* pData, uint16_t NbData)
{
	_BL_CONSOLE("BL_WriteMemory\n");

	if (NbData > 256)
		return Status_KO;

	FLASH_Write(Address, pData, NbData);

	return Status_OK;
}


/**-----------------------------------------------------------------------------
 * @brief	Erase.
 */
uint8_t BL_Erase(uint8_t FlashSector)
{
	uint32_t tmp_u32;

	_BL_CONSOLE("BL_CAN_Erase\n");

	if ((FlashSector > 11) && (FlashSector != 0xFF))
		return Status_KO;

	switch (tmp_u32 = FLASH_GetStatus())
	{
		case FLASH_BUSY:			_BL_CONSOLE("FLASH_BUSY\n");			break;
		case FLASH_ERROR_PROGRAM:	_BL_CONSOLE("FLASH_ERROR_PROGRAM\n");	break;
		case FLASH_ERROR_WRP:		_BL_CONSOLE("FLASH_ERROR_WRP\n");		break;
		case FLASH_ERROR_OPERATION:	_BL_CONSOLE("FLASH_ERROR_OPERATION\n");	break;
		case FLASH_COMPLETE:		_BL_CONSOLE("FLASH_COMPLETE\n");		break;
	}
	if (tmp_u32 != FLASH_COMPLETE)
		FLASH_ClearFlag(tmp_u32);

	return FLASH_Erase(FlashSector);
}

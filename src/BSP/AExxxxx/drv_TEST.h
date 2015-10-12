/**
 *******************************************************************************
 * @file	drv_TEST.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	06 dec. 2013
 * @brief   Gestion des fonctionnalites de Test.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef DRV_TEST_H_
#define DRV_TEST_H_


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"


/* Exported types ------------------------------------------------------------*/


/** Liste des fonctions de Test. */
typedef enum {

	Test_Fct_AccesBSP		= 0x00,
	Test_Fct_Goto			= 0x01,
	Test_Fct_GpioRead		= 0x02,
	Test_Fct_GpioWrite		= 0x03,
	Test_Fct_AnalogIn		= 0x04,
	Test_Fct_AnalogOut		= 0x05,
	Test_Fct_PwmOut			= 0x06,
	Test_Fct_MemoryConfig	= 0x07,
	Test_Fct_MemoryRead		= 0x08,
	Test_Fct_MemoryWrite	= 0x09,
	Test_Fct_Rtc			= 0x0A,
	Test_Fct_UartRead		= 0x0B,
	Test_Fct_UartWrite		= 0x0C,
	Test_Fct_CanConfig		= 0x0D,
	Test_Fct_CanData		= 0x0E,
	//Test_Fct_,

} Test_Fct_e;

/** Liste des types de memoire. */
typedef enum {

	Test_MemType_Inconnu		= 0x0,
	Test_MemType_FlashInterne	= 0x1,
	Test_MemType_FlashExterne	= 0x2,
	Test_MemType_EepromExterne	= 0x3,
	Test_MemType_FlashSpi		= 0x4,
	Test_MemType_CarteSD		= 0x5,
	Test_MemType_BQ2060			= 0x6,

	Test_MemType_NB

} Test_MemType_e;

/** Definition de l'objet Test. */
typedef struct {

	Etat_e Etat;		/**< Etat de la fonction. */

	uint8_t Mem_Type;

	uint8_t CAN_TestType;
	uint32_t CAN_Periph;
	CAN_Msg_t CAN_Msg;
	CAN_Msg_t CAN_MsgCpy;


} Test_t;


/* Exported constants --------------------------------------------------------*/

#define TEST_TRAME_SIZE	15


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void VALIDATION_SEQUENCE_DRV_TEST();

void TEST_Activer();
void TEST_Desactiver();
uint8_t TEST_ExecuterFct(uint8_t Fct, CAN_Msg_t* pMsg);

void TEST_CAN_Msg_to_buffer(CAN_Msg_t* pMsg, uint8_t* Buffer);
void TEST_buffer_to_CAN_Msg(uint8_t* Buffer, CAN_Msg_t* pMsg);

#endif /* DRV_TEST_H_ */





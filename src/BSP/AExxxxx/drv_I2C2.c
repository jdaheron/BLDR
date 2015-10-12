/**
 *******************************************************************************
 * @file	drv_I2C2.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 dec. 2013
 * @brief   Gestion de l'I2C2.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_I2C2.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 * @param[in]	ClockSpeed	Vitesse en Hz (400kHz max).
 */
void I2C2_Init(uint32_t ClockSpeed)
{
	if (ClockSpeed > 400000)
		ClockSpeed = 400000;

	I2C_InitTypeDef xI2C2_Init = {

			.I2C_ClockSpeed = ClockSpeed,
			.I2C_Mode = I2C_Mode_I2C,
			.I2C_DutyCycle = I2C_DutyCycle_2,
			.I2C_OwnAddress1 = 0,
			.I2C_Ack = I2C_Ack_Enable,
			.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,
	};

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	// Config peripherique
	I2C_Init(I2C2, &xI2C2_Init);

	// Activation peripherique
	I2C_Cmd(I2C2, ENABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void I2C2_DeInit()
{
	I2C_Cmd(I2C2, DISABLE);									// Desactivation peripherique
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);	// Desactivation horloge
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * @param[in]	SlaveAddress	Adresse du composant.
 * @param[in]	DataAddress		Adresse de la donnee a lire.
 *
 * @return		Data			Donnnee lue.
 */

uint8_t I2C2_ReadByte(uint8_t SlaveAddress, uint8_t DataAddress)
{
	uint8_t Data;

	I2C2_ReadBuffer(SlaveAddress, DataAddress, &Data, 1);

	return Data;
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture de 2 octets (LSB first).
 *
 * @param[in]	SlaveAddress	Adresse du composant a lire.
 * @param[in]	DataAddress		Adresse des donnees a lire.
 *
 * @return		Data			Donnnee lue.
 */
uint16_t I2C2_ReadWord(uint8_t SlaveAddress, uint8_t DataAddress)
{
#if 1

	uint8_t Data[2];

	I2C2_ReadBuffer(SlaveAddress, DataAddress, Data, 2);

	return (uint16_t) ( ((uint16_t) Data[0] & 0x00FF) + (((uint16_t) Data[1] << 8) & 0xFF00) );

#else

	uint16_t Data = 0xFFFF;
	uint32_t TO;
	const uint32_t TO_Value=0x800;


	//--------------------------------------------------
	// START
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait EV5
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) && (TO--));

	// ADDRESS
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Transmitter);

	// Wait EV8
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));

	// COMMAND CODE
	I2C_SendData(I2C2, DataAddress);

	// Wait EV8_2
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (TO--));


	//--------------------------------------------------
	//RE-START
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait EV5
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) && (TO--));

	// ADDRESS
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Receiver);

	// Wait EV8
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));

	// READ WORD (LSB)
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	I2C_NACKPositionConfig(I2C2, I2C_NACKPosition_Next);
	Data  = I2C_ReceiveData(I2C2);

	// Wait EV7
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (TO--));

	// READ WORD (MSB)
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	I2C_NACKPositionConfig(I2C2, I2C_NACKPosition_Current);
	Data += (I2C_ReceiveData(I2C2) << 8) & 0xFF00;

	// Wait EV7
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (TO--));

	// STOP
	I2C_GenerateSTOP(I2C2, ENABLE);
	TO = TO_Value;
	while ((!I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE)) && (TO--));

	return Data;

#endif
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un buffer.
 *
 * @param[in]	SlaveAddress	Adresse du composant a lire.
 * @param[in]	DataAddress		Adresse de debut de la lecture.
 * @param[out]	pData			Pointeur vers les donnees lues.
 * @param[in]	Taille			Nombre de donnes a lire.
 *
 */
void I2C2_ReadBuffer(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t* pData, uint16_t Taille)
{
	uint16_t iBuffer=0;
	uint32_t TO;
	const uint32_t TO_Value=0x400;


	//--------------------------------------------------
	// START
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait EV5
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) && (TO--));

	// SLAVE ADDRESS + Write
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Transmitter);

	// Wait EV8
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));

	// DATA ADDRESS
	I2C_SendData(I2C2, DataAddress);

	// Wait EV8_2
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (TO--));


	//--------------------------------------------------
	//RE-START
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait EV5
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) && (TO--));

	// SLAVE ADDRESS + Read
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Receiver);

	// Wait EV6
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (TO--));

	I2C_AcknowledgeConfig(I2C2, ENABLE);

	// READ
	while (Taille > 0)
	{
		if (Taille == 1)	// Dernier octet a lire
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE);	// Pas d'envoi ACK

			// STOP
			I2C_GenerateSTOP(I2C2, ENABLE);
		}

		// Wait EV7
		TO = TO_Value;
		while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (TO--));

		pData[iBuffer] = I2C_ReceiveData(I2C2);

		Taille--;
		iBuffer++;
	}
}



/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un octet.
 *
 * @param[in]	SlaveAddress	Adresse du composant.
 * @param[in]	DataAddress		Adresse de la donnee a ecrire.
 * @param[in]	Data			Donnee a ecrire.
 */
void I2C2_WriteByte(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t Data)
{
	I2C2_WriteBuffer(SlaveAddress, DataAddress, &Data, 1);
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture de deux octets (LSB first).
 *
 * @param[in]	SlaveAddress	Adresse du composant.
 * @param[in]	DataAddress		Adresse de la donnee a ecrire.
 * @param[in]	Data			Donnee a ecrire.
 */
void I2C2_WriteWord(uint8_t SlaveAddress, uint8_t DataAddress, uint16_t Data)
{
#if 1

	uint8_t buffer[2];

	buffer[0] = (uint8_t) ((Data >> 8) & 0x00FF);
	buffer[1] = (uint8_t) (Data & 0x00FF);

	I2C2_WriteBuffer(SlaveAddress, DataAddress, buffer, 2);

#else
	uint32_t TO;
	const uint32_t TO_Value=0x1000;


	//--------------------------------------------------
	// START
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait EV5
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) && (TO--));

	// ADDRESS
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Transmitter);

	// Wait EV6
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (TO--));

	// COMMAND CODE
	I2C_SendData(I2C2, SlaveAddress);

	// Wait EV8
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));

	// WRITE WORD (LSB)
	I2C_SendData(I2C2, (uint8_t) (Data & 0x00FF));

	// Wait EV8
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));

	// WRITE WORD (MSB)
	I2C_SendData(I2C2, (uint8_t) ((Data >> 8) & 0x00FF));

	// Wait EV8_2
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (TO--));

	// STOP
	I2C_GenerateSTOP(I2C2, ENABLE);
	TO = TO_Value;
	while ((!I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE)) && (TO--));

#endif
}



/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un buffer.
 *
 * @param[in]	SlaveAddress	Adresse du composant.
 * @param[in]	DataAddress		Adresse de debut d'ecriture.
 * @param[in]	pData			Pointeur vers les donnees a ecrire.
 * @param[in]	Taille			Nombre de donnes a erire.
 *
 */
void I2C2_WriteBuffer(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t* pData, uint16_t Taille)
{
	uint16_t iBuffer=0;
	uint32_t TO;
	const uint32_t TO_Value=0x1000;


	//--------------------------------------------------
	// START
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait EV5
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) && (TO--));

	// SLAVE ADDRESS + Write
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Transmitter);

	// Wait EV6
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (TO--));

	// DATA ADDRESS
	I2C_SendData(I2C2, DataAddress);

	// Wait EV8
	TO = TO_Value;
	while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));

	// WRITE
	while (Taille > 0)
	{
		I2C_SendData(I2C2, pData[iBuffer]);

		if (Taille != 1)
		{
			// Wait EV8
			TO = TO_Value;
			while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) && (TO--));
		}
		else // Dernier octet a ecrire
		{
			// Wait EV8_2
			TO = TO_Value;
			while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (TO--));
		}

		Taille--;
		iBuffer++;
	}

	// STOP
	I2C_GenerateSTOP(I2C2, ENABLE);
	TO = TO_Value;
	while ((!I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE)) && (TO--));

}



/**-----------------------------------------------------------------------------
 * @brief	I2C2_EV_IRQHandler.
 *
 */
void I2C2_EV_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/**-----------------------------------------------------------------------------
 * @brief	I2C2_ER_IRQHandler.
 *
 */
void I2C2_ER_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}







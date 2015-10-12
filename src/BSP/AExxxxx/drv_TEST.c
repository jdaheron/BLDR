/**
 *******************************************************************************
 * @file	drv_TEST.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	06 dec. 2013
 * @brief   Gestion des fonctionnalites de Test.
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_Test.h"
#include "util_Conversions.h"
#include "util_Goto.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static Test_t Test = { .Etat = Etat_INACTIF, };


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**-----------------------------------------------------------------------------
 * @brief	Validation des fonctions.
 *
 */
#define  VALIDATION_DRV_TEST 0

#if VALIDATION_DRV_TEST

void VALIDATION_SEQUENCE_DRV_TEST() {

	uint8_t Test=0;

	uint8_t ResetTest=0;
	uint32_t Periode=1000;

	uint8_t Fct=0;
	uint8_t Param[8];

	uint32_t Address=0;

	while (1) {

		if (Test)
		{
			TEST_ExecuterFct(Fct, Param);
			if (ResetTest)
				Test = 0;
		}

		if (GPIO_Get(PORT_BTN_WKUP))
		{
			//_printf("PORT_BTN_WKUP\n");
			Fct_GOTO(0x1FFF0000);
		}

		if (GPIO_Get(PORT_BTN_TAMPER))
		{
			_printf("PORT_BTN_TAMPER\n");
			Fct_GOTO(0x08020000);
		}

	}
}
#endif



/**-----------------------------------------------------------------------------
 * @brief	Ecriture Memoire (Fonction dependante de l'application).
 *
 */
uint8_t TEST_MemoireWrite(uint32_t Address, uint8_t* pData)
{
	uint8_t Status = Status_KO;

	switch (Test.Mem_Type)
	{
		case Test_MemType_FlashInterne:
			break;

		case Test_MemType_FlashExterne:
			break;

		case Test_MemType_EepromExterne:
			Status = Status_OK;
			break;

		case Test_MemType_FlashSpi:
			break;

		case Test_MemType_CarteSD:
			break;

		case Test_MemType_BQ2060:
			break;
	}

	return Status;
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture Memoire (Fonction dependante de l'application).
 *
 */
uint8_t TEST_MemoireRead(uint32_t Address, uint8_t* pData)
{
	uint8_t Status = Status_KO;
	uint8_t tmp_u8 = 0;
	uint16_t tmp_u16 = 0;
	uint32_t tmp_u32 = 0;

	switch (Test.Mem_Type)
	{
		case Test_MemType_FlashInterne:
			tmp_u32 = *(uint32_t*) Address;
			Conv_u32_to_pu8(tmp_u32, pData, MSB_First);
			Status = Status_OK;
			break;

		case Test_MemType_FlashExterne:
			// (FONCTION NON GEREE)
			break;

		case Test_MemType_EepromExterne:
			Status = Status_OK;
			break;

		//TODO JD
		#if 0
		case Test_MemType_CarteSD
			if (disk_read(SD, buff, Address, 1) == RES_OK)
				{
					pData[0] = buff[0];
					pData[1] = buff[1];
					pData[2] = buff[2];
					pData[3] = buff[3];
					Status = Status_OK;
				}
		break;
		#endif

		case Test_MemType_BQ2060:
			Conv_u16_to_pu8(tmp_u16, &pData[2], MSB_First);
			Status = Status_OK;
			break;
	}

	return Status;
}



/**-----------------------------------------------------------------------------
 * @brief	Activation des fonctionnalites de Test.
 *
 */
void TEST_Activer()
{
	Test.Mem_Type = Test_MemType_Inconnu;
	Test.CAN_Periph = 0;
	Test.CAN_TestType = 0;

	Test.Etat = Etat_ACTIF;
}

/**-----------------------------------------------------------------------------
 * @brief	Desactivation des fonctionnalites de Test.
 *
 */
void TEST_Desactiver()
{
	Test.Etat = Etat_INACTIF;
}


/**-----------------------------------------------------------------------------
 * @brief	Execution d'une fonction de Test.
 *
 * @param[in]	Fct		Fonction a executer.
 * @param[in]	pMsg	Message contenant les parametres de la fcontion.
 *
 */
uint8_t TEST_ExecuterFct(uint8_t Fct, CAN_Msg_t* pMsg)
{
	uint8_t Status = Status_KO;
	uint8_t tmp_u8 = 0;
	uint16_t tmp_u16 = 0;
	uint32_t tmp_u32 = 0;
	uint32_t MemAddr = 0;
	Horodatage_s tmp_RTC;
	MappingGpio_s PinConfig;

	// Verification test actif
	if ((Fct > Test_Fct_AccesBSP) && (Test.Etat == Etat_INACTIF))
		return Status_KO;

	switch (Fct)
	{
		//--------------------------------------------------------------
		case Test_Fct_AccesBSP:

			Test.Etat = Etat_INACTIF;

			if ((pMsg->RTR_Type == RTR_Type_Data) && (pMsg->DLC == 3))
			{
				if ((pMsg->Data[0] == 0xCA)
				&&	(pMsg->Data[1] == 0xFE)
				&&	(pMsg->Data[2] == 1))
				{
					TEST_Activer();
					Status = Status_OK;
				}
				else if (pMsg->Data[2] == 0)
				{
					TEST_Desactiver();
					Status = Status_OK;
				}
			}
			break;

		//--------------------------------------------------------------
		case Test_Fct_Goto:

			if ((pMsg->RTR_Type == RTR_Type_Data) && (pMsg->DLC == 6))
			{
				// Verification cle.
				if ((pMsg->Data[0] == 0xCA) && (pMsg->Data[1] == 0xFE))
				{
					tmp_u32 = Conv_pu8_to_u32(&pMsg->Data[2], MSB_First);
					GOTO(tmp_u32);
					Status = Status_OK;
				}
			}

			break;

		//--------------------------------------------------------------
		case Test_Fct_GpioRead:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 1)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			// Execution de la fonction
			pMsg->Data[1] = GPIO_Get(pMsg->Data[0]);
			pMsg->DLC = 2;

			Status = Status_OK;
			break;

		//--------------------------------------------------------------
		case Test_Fct_GpioWrite:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 2)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			if ((pMsg->Data[1] != Etat_ACTIF) && (pMsg->Data[1] != Etat_INACTIF))
				break;

			// Execution de la fonction
			GPIO_Set(pMsg->Data[0], pMsg->Data[1]);

			Status = Status_OK;
			break;

		//--------------------------------------------------------------
		case Test_Fct_AnalogIn:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 1)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			// Lecture de la configuration de la pin
			PinConfig = GPIO_GetPinConfiguration(pMsg->Data[0]);

			// Execution de la fonction
			ADC1_ChannelConf_t ADC_Channel = {
					.ADCx				= (ADC_TypeDef*) PinConfig.PERIPH,
					.ADC_Channel		= PinConfig.PARAM,
					.pStoreValue_mV		= NULL,
					.pFct_CallbackEOC	= NULL, };

			tmp_u8 = ADC1_ChannelConfigure(ADC_Channel);

			tmp_u32 = 0xFFFF;	// Timeout
			ADC1_StartConversion(tmp_u8);
			while ((ADC1_GetConversionStatus(tmp_u8) != ADC_Ready) && (tmp_u32--)) __NOP();

			tmp_u16 = (uint16_t) ADC1_Get_mV(tmp_u8);

			Conv_u16_to_pu8(tmp_u16, &pMsg->Data[1], MSB_First);
			pMsg->DLC = 3;

			Status = Status_OK;
			break;

		//--------------------------------------------------------------
		case Test_Fct_AnalogOut:

			// (FONCTION NON GEREE)

			Status = Status_KO;
			break;

		//--------------------------------------------------------------
		case Test_Fct_PwmOut:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			// Lecture de la configuration de la pin
			PinConfig = GPIO_GetPinConfiguration(pMsg->Data[0]);

			// Lecture de la configuration actuelle
			if (pMsg->DLC == 1)
			{
				Status = Status_OK;

				switch (PinConfig.PERIPH)
				{
					case (uint32_t) TIM1:	TIM1_PWM_ReadConfiguration(pMsg->Data[0], &tmp_u32, &tmp_u8);	break;
					case (uint32_t) TIM2:	TIM2_PWM_ReadConfiguration(pMsg->Data[0], &tmp_u32, &tmp_u8);	break;
					case (uint32_t) TIM4:	TIM4_PWM_ReadConfiguration(pMsg->Data[0], &tmp_u32, &tmp_u8);	break;
					case (uint32_t) TIM8:	TIM8_PWM_ReadConfiguration(pMsg->Data[0], &tmp_u32, &tmp_u8);	break;
					case (uint32_t) TIM12:	TIM12_PWM_ReadConfiguration(pMsg->Data[0], &tmp_u32, &tmp_u8);	break;
					case (uint32_t) TIM14:	TIM14_PWM_ReadConfiguration(pMsg->Data[0], &tmp_u32, &tmp_u8);	break;
					default:
						Status = Status_KO;
						break;
				}

				Conv_u32_to_pu8(tmp_u32, &pMsg->Data[1], MSB_First);
				pMsg->Data[5] = tmp_u8;
				pMsg->DLC = 6;
			}
			else
			{
				// Execution de la fonction
				tmp_u32 = Conv_pu8_to_u32(&pMsg->Data[1], MSB_First);
				Status = Status_OK;

				switch (PinConfig.PERIPH)
				{
					case (uint32_t) TIM1:	TIM1_Init(tmp_u32);		TIM1_PWM_Configurer(pMsg->Data[0], Etat_ACTIF, pMsg->Data[5]);	break;
					case (uint32_t) TIM2:	TIM2_Init(tmp_u32);		TIM2_PWM_Configurer(pMsg->Data[0], Etat_ACTIF, pMsg->Data[5]);	break;
					case (uint32_t) TIM4:	TIM4_Init(tmp_u32);		TIM4_PWM_Configurer(pMsg->Data[0], Etat_ACTIF, pMsg->Data[5]);	break;
					case (uint32_t) TIM8:	TIM8_Init(tmp_u32);		TIM8_PWM_Configurer(pMsg->Data[0], Etat_ACTIF, pMsg->Data[5]);	break;
					case (uint32_t) TIM12:	TIM12_Init(tmp_u32);	TIM12_PWM_Configurer(pMsg->Data[0], Etat_ACTIF, pMsg->Data[5]);	break;
					case (uint32_t) TIM14:	TIM14_Init(tmp_u32);	TIM14_PWM_Configurer(pMsg->Data[0], Etat_ACTIF, pMsg->Data[5]);	break;
					default:
						Status = Status_KO;
						break;
				}
			}

			//Status = Status_OK;
			break;

		//--------------------------------------------------------------
		case Test_Fct_MemoryConfig:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 1))
				break;

			// Verification des parametres
			if (pMsg->Data[0] >= Test_MemType_NB)
				break;

			Test.Mem_Type = pMsg->Data[0];

			Status = Status_OK;
			break;

		//--------------------------------------------------------------
		case Test_Fct_MemoryRead:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 4))
				break;

			// Verification memoire configuree
			if (Test.Mem_Type == Test_MemType_Inconnu)
				break;

			// Decodage de l'adresse
			MemAddr = Conv_pu8_to_u32(&pMsg->Data[0], MSB_First);

			// Effacement partie Data de la trame reponse
			for (tmp_u8=4; tmp_u8<8; tmp_u8++)
				pMsg->Data[tmp_u8] = 0;

			pMsg->DLC = 8;

			// Lecture Memoire
			Status = TEST_MemoireRead(MemAddr, &pMsg->Data[4]);

			break;

		//--------------------------------------------------------------
		case Test_Fct_MemoryWrite:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 8))
				break;

			// Verification memoire configuree
			if (Test.Mem_Type == Test_MemType_Inconnu)
				break;

			// Decodage de l'adresse
			MemAddr = Conv_pu8_to_u32(&pMsg->Data[0], MSB_First);

			// Ecriture Memoire
			Status = TEST_MemoireWrite(MemAddr, &pMsg->Data[4]);

			break;

		//--------------------------------------------------------------
		case Test_Fct_Rtc:

			// Lecture
			if (pMsg->RTR_Type == RTR_Type_Remote)
			{
				RTC_Lire(&tmp_RTC);

				pMsg->Data[5]	= tmp_RTC.Annee - 2000;
				pMsg->Data[4]	= tmp_RTC.Mois ;
				pMsg->Data[3]	= tmp_RTC.Jour;
				pMsg->Data[2]	= tmp_RTC.Heure;
				pMsg->Data[1]	= tmp_RTC.Minute;
				pMsg->Data[0]	= tmp_RTC.Seconde;

				pMsg->RTR_Type = RTR_Type_Data;
				pMsg->DLC = 6;

				Status = Status_OK;
			}

			// Ecriture
			else
			{
				// Verification des parametres
				if (pMsg->DLC != 6)
					break;

				Status = RTC_ReglerDateHeure(
						1,						// Jour de la semaine
						pMsg->Data[3],			// Jour
						pMsg->Data[4],			// Mois
						2000 + pMsg->Data[5],	// Annee
						pMsg->Data[2],			// Heure
						pMsg->Data[1],			// Minute
						pMsg->Data[0],			// Seconde
						TRUE					// Mode 24h
				);

				Status = Status_OK;
			}
			break;

		//--------------------------------------------------------------
		case Test_Fct_UartRead:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 1)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			// Lecture de la configuration de la pin
			PinConfig = GPIO_GetPinConfiguration(pMsg->Data[0]);

			// Preparation de la trame reponse
			for (tmp_u8=2; tmp_u8<8;tmp_u8++)
				pMsg->Data[tmp_u8] = 0;

			Status = Status_OK;

			switch (PinConfig.PERIPH)
			{
				case (uint32_t) USART1:	tmp_u16 = USART1_Read(&pMsg->Data[2], 6);	break;
				case (uint32_t) USART2:	tmp_u16 = USART2_Read(&pMsg->Data[2], 6);	break;
				case (uint32_t) USART3:	tmp_u16 = USART3_Read(&pMsg->Data[2], 6);	break;
				case (uint32_t) UART4:	tmp_u16 = UART4_Read(&pMsg->Data[2], 6);	break;
				case (uint32_t) UART5:	tmp_u16 = UART5_Read(&pMsg->Data[2], 6);	break;
				case (uint32_t) USART6:	tmp_u16 = USART6_Read(&pMsg->Data[2], 6);	break;
				default:
					Status = Status_KO;
					tmp_u16 = 0;
					break;
			}

			// Nombre de donnees lues
			pMsg->Data[1] = (uint8_t) tmp_u16;
			pMsg->DLC = 8;

			break;

		//--------------------------------------------------------------
		case Test_Fct_UartWrite:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->DLC != 8)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			if (pMsg->Data[1] > 6)
				pMsg->Data[1] = 6;

			// Lecture de la configuration de la pin
			PinConfig = GPIO_GetPinConfiguration(pMsg->Data[0]);

			Status = Status_OK;

			switch (PinConfig.PERIPH)
			{
				case (uint32_t) USART1:	USART1_Write(&pMsg->Data[2], pMsg->Data[1]);	break;
				case (uint32_t) USART2:	USART2_Write(&pMsg->Data[2], pMsg->Data[1]);	break;
				case (uint32_t) USART3:	USART3_Write(&pMsg->Data[2], pMsg->Data[1]);	break;
				case (uint32_t) UART4:	UART4_Write(&pMsg->Data[2], pMsg->Data[1]);		break;
				case (uint32_t) UART5:	UART5_Write(&pMsg->Data[2], pMsg->Data[1]);		break;
				case (uint32_t) USART6:	USART6_Write(&pMsg->Data[2], pMsg->Data[1]);	break;
				default:
					Status = Status_KO;
					break;
			}

			break;

		//--------------------------------------------------------------
		case Test_Fct_CanConfig:

			// Verification des parametres
			if ((pMsg->RTR_Type == RTR_Type_Remote)
			||	(pMsg->Data[0] >= NB_PORT_GPIO))
				break;

			// Lecture de la configuration de la pin
			PinConfig = GPIO_GetPinConfiguration(pMsg->Data[0]);

			switch (PinConfig.PERIPH)
			{
				case (uint32_t) CAN1:
				case (uint32_t) CAN2:
					Test.CAN_Periph = PinConfig.PERIPH;
					Test.CAN_TestType = (pMsg->DLC == 1) ? 1 : 0;
					Status = Status_OK;
					break;

				default:
					Test.CAN_TestType = 0xFF;
					Status = Status_KO;
					break;
			}

			switch (Test.CAN_TestType)
			{
				//--------------------------
				// LECTURE
				case 0:

					//Lecture du message
					if (Test.CAN_Periph == (uint32_t) CAN1)
						tmp_u8 = CAN1_Read(&Test.CAN_Msg);
					else
						tmp_u8 = CAN2_Read(&Test.CAN_Msg);

					// Copie dans le buffer si messsge lu
					if (tmp_u8 == TRUE)
					{
						// ID
						Conv_u32_to_pu8(Test.CAN_Msg.ID, &pMsg->Data[1], MSB_First);
						// Flags
						pMsg->Data[1] |= ((Test.CAN_Msg.RTR_Type << 5) & 0x20);
						pMsg->Data[1] |= ((Test.CAN_Msg.ID_Type << 6) & 0x40);
						// DLC
						pMsg->Data[5] = Test.CAN_Msg.DLC;
						// Data (sauvegarde pour lecture ulterieure)
						for (tmp_u8=0; tmp_u8<8; tmp_u8++)
							Test.CAN_MsgCpy.Data[tmp_u8] = Test.CAN_Msg.Data[tmp_u8];

						pMsg->DLC = 6;
					}
					else
					{
						Test.CAN_TestType = 0xFF;

						for (tmp_u8=1; tmp_u8<6; tmp_u8++)
							pMsg->Data[tmp_u8] = 0;
					}
					break;

				//--------------------------
				// ECRITURE
				case 1:

					if (pMsg->DLC != 6)
						break;

					tmp_u8 = pMsg->Data[0];

					// Sauvegarde pour ecriture ulterieure
					TEST_buffer_to_CAN_Msg(pMsg->Data, &Test.CAN_MsgCpy);
					pMsg->Data[0] = tmp_u8;

					break;

				default:
					break;
			}

			break;


		//--------------------------------------------------------------
		case Test_Fct_CanData:

			switch (Test.CAN_TestType)
			{
				//--------------------------
				// LECTURE
				case 0:
					if (pMsg->RTR_Type == RTR_Type_Remote)
					{
						for (tmp_u8=0; tmp_u8<8; tmp_u8++)
							pMsg->Data[tmp_u8] = Test.CAN_MsgCpy.Data[tmp_u8];

						pMsg->DLC = 8;
						pMsg->RTR_Type = RTR_Type_Data;

						Status = Status_OK;
					}
					break;

				//--------------------------
				// ECRITURE
				case 1:

					if ((pMsg->DLC == 8)
					&&	(pMsg->RTR_Type == RTR_Type_Data))
					{
						for (tmp_u8=0; tmp_u8<8; tmp_u8++)
							Test.CAN_MsgCpy.Data[tmp_u8] = pMsg->Data[tmp_u8];
						if (Test.CAN_Periph == (uint32_t) CAN1)
							CAN1_Write(Test.CAN_MsgCpy);
						else
							CAN2_Write(Test.CAN_MsgCpy);
						Status = Status_OK;
					}
					break;

				default:
					Status = Status_KO;
					break;
			}
			break;

		//--------------------------------------------------------------
		default:
			break;
	}

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Conversion d'une trame CAN en buffer.
 *
 * @param[in]	pMsg	Pointeur vers le Message Can source.
 * @param[out]	Buffer	Buffer cible.
 *
 */
void TEST_CAN_Msg_to_buffer(CAN_Msg_t* pMsg, uint8_t* Buffer) {

	// SOH (Start of Heading)
	Buffer[0] = 1;

	// ID
	Conv_u32_to_pu8(pMsg->ID, &Buffer[1], MSB_First);

	// Flags
	Buffer[1] |= ((pMsg->RTR_Type << 5) & 0x20);
	Buffer[1] |= ((pMsg->ID_Type << 6) & 0x40);

	// DLC
	Buffer[5] = pMsg->DLC;

	// Data
	for (int i = 0; i < 8; i++)
		Buffer[6 + i] = pMsg->Data[i];

	// EOT (End of Transmission)
	Buffer[14] = 4;
}

/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un buffer en trame CAN.
 *
 * @param[out]	Buffer	Buffer source.
 * @param[in]	pMsg	Pointeur vers le Message Can cible.
 *
 */
void TEST_buffer_to_CAN_Msg(uint8_t* Buffer, CAN_Msg_t* pMsg) {

	// ID
	pMsg->ID = Conv_pu8_to_u32(&Buffer[1], MSB_First);
	pMsg->ID &= 0x1FFFFFFF;

	// Flags
	pMsg->RTR_Type = (Buffer[1] >> 5) & 0x01;
	pMsg->ID_Type = (Buffer[1] >> 6) & 0x01;

	// DLC
	pMsg->DLC = Buffer[5];

	// Data
	for (int i = 0; i < 8; i++)
		pMsg->Data[i] = Buffer[6 + i];
}

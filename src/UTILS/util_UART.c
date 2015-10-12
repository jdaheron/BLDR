/**
 ***************************************************************************************************
 * @file	util_UART.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	16 juil. 2014
 * @brief   Gestion d'Uarts.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "util_UART.h"


/* External Variables *****************************************************************************/


/** @addtogroup util_UART
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
 *
 */
#define VALIDATION_UTIL_UART_ENABLE 0

#if VALIDATION_UTIL_UART_ENABLE

UartHandle_s TestUart;
uint8_t TestUart_RxBuffer[100];

void TestUart_Init()
{
	UartConfig_s UartConfig = {
		.pInitFct		= (pFunction) HAL_Console_Init,
		.Baudrate		= 115200,

    	.pRxFct			= (pFunction) HAL_Console_Read,
		.pRxBuffer		= TestUart_RxBuffer,
		.RxBufferSize	= 100,
		.RxSOFcar		= 0,
		.RxEOFstr		= "$\n",
		.RxEOFsize		= 2,
		.RxTimeout_ms	= 10,

		.pTxFct			= (pFunction) HAL_Console_Write,
	};

	Uart_Init(&TestUart, &UartConfig);
}

void VALIDATION_UTIL_UART()
{
	uint16_t RxSize;
	uint8_t* pRxBuffer;

	TestUart_Init();

	while (1)
	{
		Uart_GererReception(&TestUart);

		if (Uart_IsTrameRecue(&TestUart))
		{
			Uart_GetTrameRecue(&TestUart, &pRxBuffer, &RxSize);

			Uart_EnvoyerTrame(&TestUart, "Rx:", 3);
			Uart_EnvoyerTrame(&TestUart, pRxBuffer, RxSize);

			Uart_PreparerReception(&TestUart);
		}
	}
}
#endif


/**
 ***************************************************************************************************
 * Validation OK - JD 16/07/2014
 */
void
Uart_Init(
		UartHandle_s*	hUart,			/**<[in] Handle Uart.*/
		UartConfig_s*	pUartConfig		/**<[in] Pointeur vers la structure de configuration.*/
)
{
	// Copie de la configuration de reception
	hUart->Rx.pFct			= pUartConfig->pRxFct;
	hUart->Rx.pBuffer		= pUartConfig->pRxBuffer;
	hUart->Rx.BufferSize	= pUartConfig->RxBufferSize;
	hUart->Rx.SOFcar		= pUartConfig->RxSOFcar;
	strncpy((char*) hUart->Rx.EOFstr, (char*) pUartConfig->RxEOFstr, EOF_MAX_SIZE);
	if (pUartConfig->RxEOFsize > EOF_MAX_SIZE)
		pUartConfig->RxEOFsize = EOF_MAX_SIZE;
	hUart->Rx.EOFsize		= pUartConfig->RxEOFsize;
	hUart->Rx.Timeout_ms	= pUartConfig->RxTimeout_ms;

	// Copie de la configuration d'emission
	hUart->Tx.pFct			= pUartConfig->pTxFct;

	// Preparation de la reception
	Uart_PreparerReception(hUart);

	// Initalisation de l'UART
	if (pUartConfig->pInitFct != NULL)
		pUartConfig->pInitFct((uint32_t*) pUartConfig->Baudrate);
}

/**
 ***************************************************************************************************
 * Validation OK - JD 16/07/2014
 */
void
Uart_PreparerReception(
		UartHandle_s*	hUart			/**<[in] Handle Uart.*/
)
{
	// Initialisation de la reception
	hUart->Rx.RxSize		= 0;
	hUart->Rx.State			= UART_ATTENTE_RECEPTION;

	TSW_Stop(&hUart->Rx.Timer);
}

/**
 ***************************************************************************************************
 * Validation OK - JD 16/07/2014
 */
void
Uart_GererReception(
		UartHandle_s*	hUart			/**<[in] Handle Uart.*/
)
{
	uint8_t DataRx;

	if (hUart->Rx.pFct == NULL)
		return;

	switch (hUart->Rx.State)
	{
		//--------------------------------------------------------------
		case UART_ATTENTE_RECEPTION:

			if (hUart->Rx.pFct((uint8_t*) &DataRx, (uint16_t) 1) != 0) {

				// SOF recu ou SOF inactif
				if ((DataRx == hUart->Rx.SOFcar) || (hUart->Rx.SOFcar == 0))
				{
					// Effacement buffer
					memset(hUart->Rx.pBuffer, 0, hUart->Rx.BufferSize);

					// Lecture donnee
					hUart->Rx.RxSize = 0;
					hUart->Rx.pBuffer[hUart->Rx.RxSize] = DataRx;
					hUart->Rx.RxSize++;

					// Lancement Timeout si initialise
					if (hUart->Rx.Timeout_ms != 0)
						TSW_Start(&hUart->Rx.Timer, hUart->Rx.Timeout_ms);

					// MAJ Etat
					hUart->Rx.State = UART_RECEPTION_EN_COURS;
				}
			}

			break;

		//--------------------------------------------------------------
		case UART_RECEPTION_EN_COURS:

			// Lecture des données
			while ( (hUart->Rx.pFct((uint8_t*) &DataRx, (uint16_t) 1) != 0)		// Lecture donnee
			&&		(hUart->Rx.RxSize < hUart->Rx.BufferSize)					// Verification overflow
			&&		(hUart->Rx.State == UART_RECEPTION_EN_COURS))				// Reception non terminee
			{
				// Lecture donnee
				hUart->Rx.pBuffer[hUart->Rx.RxSize] = DataRx;
				hUart->Rx.RxSize++;

				// Verification overflow
				if (hUart->Rx.RxSize >= hUart->Rx.BufferSize - 1) {
					Uart_PreparerReception(hUart);	// On jette la trame en cas d'overflow
					break;
				}

				// Verification de la chaine de fin
				if ((hUart->Rx.EOFsize != 0) && (hUart->Rx.RxSize >= hUart->Rx.EOFsize))
				{
					if (	strncmp(	(const char*)&(hUart->Rx.pBuffer[hUart->Rx.RxSize - hUart->Rx.EOFsize]),
									 	(const char*)hUart->Rx.EOFstr,
									 	hUart->Rx.EOFsize )    == 0	)
					{
						// Ajout fin de chaine si pas d'overflow
						if (hUart->Rx.RxSize < hUart->Rx.BufferSize - 1)
						{
							hUart->Rx.pBuffer[hUart->Rx.RxSize] = '\0';
							//hUart->Rx.RxSize++;
						}
						hUart->Rx.State = UART_TRAME_RECUE;
						break;
					}
				}

				// Lancement Timeout si initialise
				if (hUart->Rx.Timeout_ms != 0)
					TSW_ReStart(&hUart->Rx.Timer);
			}

			// Verification TimeOut si initialise
			if (hUart->Rx.Timeout_ms != 0)
			{
				if (TSW_IsFinished(&hUart->Rx.Timer) == TRUE)
				{
					// Ajout fin de chaine si pas d'overflow
					if (hUart->Rx.RxSize < hUart->Rx.BufferSize - 1)
					{
						hUart->Rx.pBuffer[hUart->Rx.RxSize] = '\0';
						//hUart->Rx.RxSize++;
					}

					if (hUart->Rx.EOFsize == 0)				// Si EOF non initialise, on conserve la trame en cas de timeout
						hUart->Rx.State = UART_TRAME_RECUE;
					else
						Uart_PreparerReception(hUart);		// sinon, on jette la trame
					break;
				}
			}

			break;

		//--------------------------------------------------------------
		case UART_TRAME_RECUE:

			if (hUart->Rx.pBuffer[hUart->Rx.RxSize-1] != '\0')
			{
				// Ajout fin de chaine si pas d'overflow
				if (hUart->Rx.RxSize < hUart->Rx.BufferSize - 1)
				{
					hUart->Rx.pBuffer[hUart->Rx.RxSize] = '\0';
					//hUart->Rx.RxSize++;
				}
			}

			break;

		//--------------------------------------------------------------
		default:
			Uart_PreparerReception(hUart);
			break;
		}
}

/**
 ***************************************************************************************************
 * Validation OK - JD 16/07/2014
 */
Bool_e
Uart_IsTrameRecue(
		UartHandle_s*	hUart			/**<[in] Handle Uart.*/
)
{
	if (hUart->Rx.State == UART_TRAME_RECUE)
		return TRUE;

	return FALSE;
}

/**
 ***************************************************************************************************
 * Validation OK - JD 16/07/2014
 */
void
Uart_GetTrameRecue(
		UartHandle_s*	hUart,	/**<[in] Handle Uart.*/
		uint8_t**		pTrame,	/**<[in] Pointeur vers la trame recue.*/
		uint16_t*		pTaille	/**<[in] Pointeur vers la taille de trame recue.*/
)
{
	if (hUart->Rx.State != UART_TRAME_RECUE)
		return;

	*pTrame = hUart->Rx.pBuffer;
	*pTaille = hUart->Rx.RxSize;
}

/**
 ***************************************************************************************************
 * Validation OK - JD 16/07/2014
 */
void
Uart_EnvoyerTrame(
		UartHandle_s*	hUart,	/**<[in] Handle Uart.*/
		uint8_t*		pTrame,	/**<[in] Pointeur vers la trame a emettre.*/
		uint16_t		Taille	/**<[in] Taille de trame a emettre.*/
)
{
	if (hUart->Tx.pFct == NULL)
		return;

	hUart->Tx.pFct((uint8_t*) pTrame, (uint16_t) Taille);
}


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

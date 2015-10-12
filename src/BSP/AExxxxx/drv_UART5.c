/**
 *******************************************************************************
 * @file	drv_UART5.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'UART5.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_UART5.h"
#include "util_FIFO.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define UART5_FIFO_SIZE				64
#define UART5_TIMEOUT				100000	//10ms env.
#define UART5_USE_TX_INTERRUPT		1


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static uint8_t UART5_FifoRxBuffer[UART5_FIFO_SIZE];
static Fifo_t UART5_FifoRx;
#if UART5_USE_TX_INTERRUPT
	static uint8_t UART5_FifoTxBuffer[UART5_FIFO_SIZE];
	static Fifo_t UART5_FifoTx;
	static uint8_t TxEnCours = FALSE;
#endif


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 * @param[in]	Baudrate	Debit en bauds.
 */
void UART5_Init(uint32_t Baudrate) {

	USART_InitTypeDef xUSART_Init = {

			.USART_BaudRate				= Baudrate,
			.USART_WordLength			= USART_WordLength_8b,
			.USART_StopBits				= USART_StopBits_1,
			.USART_Parity				= USART_Parity_No,
			.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx,
			.USART_HardwareFlowControl	= USART_HardwareFlowControl_None,
	};

	// Initialisation des Fifo
	FIFO_Init(&UART5_FifoRx, UART5_FifoRxBuffer, UART5_FIFO_SIZE, sizeof(uint8_t));
	#if UART5_USE_TX_INTERRUPT
		FIFO_Init(&UART5_FifoTx, UART5_FifoTxBuffer, UART5_FIFO_SIZE, sizeof(uint8_t));
	#endif

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	// Config peripherique
	USART_Init(UART5, &xUSART_Init);

	// Configuration interruption
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);

	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(UART5_IRQn);
	#endif

	// Activation peripherique
	USART_Cmd(UART5, ENABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void UART5_DeInit() {

	NVIC_DisableIRQ(UART5_IRQn);
	USART_Cmd(UART5, DISABLE);								// Desactivation peripherique
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du buffer de reception.
 *
 * @param[out]	pBuffer	Donnees lues (NULL si vide).
 * @param[in]	Taille	Nombre de donnees a lire.
 *
 * return		NbRead	Nombre de donnees lues
 */
uint16_t UART5_Read(uint8_t* pBuffer, uint16_t Taille) {

	uint16_t NbRead=0;

	// Remplissage du buffer
	while ((FIFO_Pull(&UART5_FifoRx, &(pBuffer[NbRead++])) != FIFO_EMPTY) && (NbRead != Taille));

	return NbRead;
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un buffer.
 *
 * @param[in]	pBuffer	Donnees a ecrire.
 * @param[in]	Taille	Nombre de donnees a ecrire.
 */
void UART5_Write(uint8_t* pBuffer, uint16_t Taille) {

	uint16_t NbWrite = 0;

#if UART5_USE_TX_INTERRUPT

	// Remplissage du buffer
	while ((NbWrite != Taille) && (FIFO_Push(&UART5_FifoTx, pBuffer[NbWrite]) != FIFO_FULL)){

		NbWrite++;
	}

	// Si pas d'emission en cours, on la lance
	if (TxEnCours == FALSE)
	{
		TxEnCours = TRUE;
		USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
	}

#else

	uint32_t TO;

	while (Taille != 0){

		// Attente donnee envoyee
		TO = UART5_TIMEOUT;
		while ((USART_GetFlagStatus(UART5, USART_FLAG_TXE) != SET) && TO) TO--;
		if (!TO) return;	// On quitte en cas d'erreur

		// Ecriture donnee
		USART_SendData(UART5, pBuffer[NbWrite++]);

		Taille--;
	}

#endif

}


/**-----------------------------------------------------------------------------
  * @brief  UART5_IRQHandler
  */
void UART5_IRQHandler() {

	uint8_t data;

	// Verification donnee recue
	while (USART_GetITStatus(UART5, USART_IT_RXNE) == SET) {

		// Ajout a la fifo
		FIFO_Push(&UART5_FifoRx, (uint8_t) USART_ReceiveData(UART5));

	}

#if UART5_USE_TX_INTERRUPT
	// Verification donnee a emettre
	if (USART_GetITStatus(UART5, USART_IT_TXE) == SET)
	{
		if (FIFO_Pull(&UART5_FifoTx, &data) != FIFO_EMPTY)
		{
			USART_SendData(UART5, data);
		}
		else
		{
			USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
			TxEnCours = FALSE;
		}
	}
#endif

}






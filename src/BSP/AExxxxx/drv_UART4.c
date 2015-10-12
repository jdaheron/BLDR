/**
 *******************************************************************************
 * @file	drv_UART4.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'UART4.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_UART4.h"
#include "util_FIFO.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define UART4_FIFO_SIZE				64
#define UART4_TIMEOUT				100000	//10ms env.
#define UART4_USE_TX_INTERRUPT		1


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static uint8_t UART4_FifoRxBuffer[UART4_FIFO_SIZE];
static Fifo_t UART4_FifoRx;
#if UART4_USE_TX_INTERRUPT
	static uint8_t UART4_FifoTxBuffer[UART4_FIFO_SIZE];
	static Fifo_t UART4_FifoTx;
	static uint8_t TxEnCours = FALSE;
#endif


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 * @param[in]	Baudrate	Debit en bauds.
 */
void UART4_Init(uint32_t Baudrate) {

	USART_InitTypeDef xUSART_Init = {

			.USART_BaudRate				= Baudrate,
			.USART_WordLength			= USART_WordLength_8b,
			.USART_StopBits				= USART_StopBits_1,
			.USART_Parity				= USART_Parity_No,
			.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx,
			.USART_HardwareFlowControl	= USART_HardwareFlowControl_None,
	};

	// Initialisation des Fifo
	FIFO_Init(&UART4_FifoRx, UART4_FifoRxBuffer, UART4_FIFO_SIZE, sizeof(uint8_t));
	#if UART4_USE_TX_INTERRUPT
		FIFO_Init(&UART4_FifoTx, UART4_FifoTxBuffer, UART4_FIFO_SIZE, sizeof(uint8_t));
	#endif

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	// Config peripherique
	USART_Init(UART4, &xUSART_Init);

	// Configuration interruption
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);

	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(UART4_IRQn);
	#endif

	// Activation peripherique
	USART_Cmd(UART4, ENABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void UART4_DeInit() {

	NVIC_DisableIRQ(UART4_IRQn);
	USART_Cmd(UART4, DISABLE);								// Desactivation peripherique
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du buffer de reception.
 *
 * @param[out]	pBuffer	Donnees lues (NULL si vide).
 * @param[in]	Taille	Nombre de donnees a lire.
 *
 * return		NbRead	Nombre de donnees lues
 */
uint16_t UART4_Read(uint8_t* pBuffer, uint16_t Taille) {

	uint16_t NbRead=0;

	// Remplissage du buffer
	while ((FIFO_Pull(&UART4_FifoRx, &(pBuffer[NbRead++])) != FIFO_EMPTY) && (NbRead != Taille));

	return NbRead;
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un buffer.
 *
 * @param[in]	pBuffer	Donnees a ecrire.
 * @param[in]	Taille	Nombre de donnees a ecrire.
 */
void UART4_Write(uint8_t* pBuffer, uint16_t Taille) {

	uint16_t NbWrite = 0;

#if UART4_USE_TX_INTERRUPT

	// Remplissage du buffer
	while ((NbWrite != Taille) && (FIFO_Push(&UART4_FifoTx, pBuffer[NbWrite]) != FIFO_FULL)){

		NbWrite++;
	}

	// Si pas d'emission en cours, on la lance
	if (TxEnCours == FALSE)
	{
		TxEnCours = TRUE;
		USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
	}

#else

	uint32_t TO;

	while (Taille != 0){

		// Attente donnee envoyee
		TO = UART4_TIMEOUT;
		while ((USART_GetFlagStatus(UART4, USART_FLAG_TXE) != SET) && TO) TO--;
		if (!TO) return;	// On quitte en cas d'erreur

		// Ecriture donnee
		USART_SendData(UART4, pBuffer[NbWrite++]);

		Taille--;
	}

#endif

}


/**-----------------------------------------------------------------------------
  * @brief  UART4_IRQHandler
  */
void UART4_IRQHandler() {

	uint8_t data;

	// Verification donnee recue
	while (USART_GetITStatus(UART4, USART_IT_RXNE) == SET) {

		// Ajout a la fifo
		FIFO_Push(&UART4_FifoRx, (uint8_t) USART_ReceiveData(UART4));

	}

#if UART4_USE_TX_INTERRUPT
	// Verification donnee a emettre
	if (USART_GetITStatus(UART4, USART_IT_TXE) == SET)
	{
		if (FIFO_Pull(&UART4_FifoTx, &data) != FIFO_EMPTY)
		{
			USART_SendData(UART4, data);
		}
		else
		{
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
			TxEnCours = FALSE;
		}
	}
#endif

}






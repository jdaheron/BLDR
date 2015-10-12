/**
 *******************************************************************************
 * @file	drv_USART1.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	04 nov. 2013
 * @brief   Gestion de l'USART1.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_USART1.h"
#include "util_FIFO.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define USART1_FIFO_SIZE			1024	//TODO à adapter
#define USART1_TIMEOUT				100000	//10ms env.
#define USART1_USE_TX_INTERRUPT		0


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static uint8_t USART1_FifoRxBuffer[USART1_FIFO_SIZE];
static Fifo_t USART1_FifoRx;
#if USART1_USE_TX_INTERRUPT
	static uint8_t USART1_FifoTxBuffer[USART1_FIFO_SIZE];
	static Fifo_t USART1_FifoTx;
	static uint8_t TxEnCours = FALSE;
#endif


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 * @param[in]	Baudrate	Debit en bauds.
 */
void USART1_Init(uint32_t Baudrate) {

	USART_InitTypeDef xUSART_Init = {

			.USART_BaudRate				= Baudrate,
			.USART_WordLength			= USART_WordLength_8b,
			.USART_StopBits				= USART_StopBits_1,
			.USART_Parity				= USART_Parity_No,
			.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx,
			.USART_HardwareFlowControl	= USART_HardwareFlowControl_None,
	};

	// Initialisation des Fifo
	FIFO_Init(&USART1_FifoRx, USART1_FifoRxBuffer, USART1_FIFO_SIZE, sizeof(uint8_t));
	#if USART1_USE_TX_INTERRUPT
		FIFO_Init(&USART1_FifoTx, USART1_FifoTxBuffer, USART1_FIFO_SIZE, sizeof(uint8_t));
	#endif

	// Activation horloges
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// Config peripherique
	USART_Init(USART1, &xUSART_Init);

	// Configuration interruption
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(USART1_IRQn);
	#endif

	// Activation peripherique
	USART_Cmd(USART1, ENABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void USART1_DeInit() {

	NVIC_DisableIRQ(USART1_IRQn);
	USART_Cmd(USART1, DISABLE);								// Desactivation peripherique
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du buffer de reception.
 *
 * @param[in]	Taille	Nombre de donnees a lire.
 * @param[out]	pBuffer	Donnees lues (NULL si vide).
 *
 * return		NbRead	Nombre de donnees lues
 */
uint16_t USART1_Read(uint8_t* pBuffer, uint16_t Taille) {

	uint16_t NbRead=0;

	// Remplissage du buffer
	while ((NbRead != Taille) && (FIFO_Pull(&USART1_FifoRx, &(pBuffer[NbRead])) != FIFO_EMPTY)){

		NbRead++;
	}

	return NbRead;
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un buffer.
 *
 * @param[in]	pBuffer	Donnees a ecrire.
 * @param[in]	Taille	Nombre de donnees a ecrire.
 */
void USART1_Write(uint8_t* pBuffer, uint16_t Taille) {

	uint16_t NbWrite = 0;

#if USART1_USE_TX_INTERRUPT

	// Remplissage du buffer
	while ((NbWrite != Taille) && (FIFO_Push(&USART1_FifoTx, pBuffer[NbWrite]) != FIFO_FULL)){

		NbWrite++;
	}

	// Si pas d'emission en cours, on la lance
	if (TxEnCours == FALSE)
	{
		TxEnCours = TRUE;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}

#else

	uint32_t TO;

	while (Taille != 0){

		// Attente donnee envoyee
		TO = USART1_TIMEOUT;
		while ((USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET) && TO) TO--;
		if (!TO) return;	// On quitte en cas d'erreur

		// Ecriture donnee
		USART_SendData(USART1, pBuffer[NbWrite++]);

		Taille--;
	}

#endif

}


/**-----------------------------------------------------------------------------
  * @brief  USART1_IRQHandler
  */
void USART1_IRQHandler() {

	uint8_t data;

	// Verification donnee recue
	while (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {

		// Ajout a la fifo
		FIFO_Push(&USART1_FifoRx, (uint8_t) USART_ReceiveData(USART1));

	}

#if USART1_USE_TX_INTERRUPT
	// Verification donnee a emettre
	if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		if (FIFO_Pull(&USART1_FifoTx, &data) != FIFO_EMPTY)
		{
			USART_SendData(USART1, data);
		}
		else
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			TxEnCours = FALSE;
		}
	}
#endif

}






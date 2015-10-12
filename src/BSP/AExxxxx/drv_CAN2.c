/**
 *******************************************************************************
 * @file	drv_CAN2.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 oct. 2013
 * @brief   Gestion du CAN2.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_CAN2.h"
#include "util_FIFO.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define CAN2_TX_FIFO_SIZE		64
#define CAN2_RX_FIFO_SIZE		64
#define CAN2_USE_TX_INTERRUPT	1


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static CAN_Msg_t CAN2_FifoTxBuffer[CAN2_TX_FIFO_SIZE];
static CAN_Msg_t CAN2_FifoRxBuffer[CAN2_RX_FIFO_SIZE];
static Fifo_t CAN2_FifoTx;
static Fifo_t CAN2_FifoRx;
static uint8_t TxEnCours = FALSE;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


#define  VALIDATION_CAN2	0

#if VALIDATION_CAN2

void CAN2_Test() {

	CAN_Msg_t msg[5]={
			{.ID_Type = ID_Type_Ext,	.ID = 1,	.DLC = 8,	.Data = "01234567"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x123456,	.DLC = 8,	.Data = "12345678"},
			{.ID_Type = ID_Type_Ext,	.ID = 3,	.DLC = 8,	.Data = "23456789"},
			{.ID_Type = ID_Type_Ext,	.ID = 4,	.DLC = 8,	.Data = "34567890"},
			{.ID_Type = ID_Type_Ext,	.ID = 5,	.DLC = 8,	.Data = "45678901"},
	};
	CAN_Msg_t Rx;
	int i=0;

	CAN2_Init();

	while(1) {

		if (TimerSW_GetStatus(TIMER_Generique) == Status_Fini){
			TimerSW_Start(TIMER_Generique, 5000);

			CAN2_Write(msg[i]);

			i++;
			if (i >= 5)
				i=0;

		}

		if (TimerSW_GetStatus(TIMER_Generique2) == Status_Fini){
			TimerSW_Start(TIMER_Generique2, 50);

			//echo
			if (CAN2_Read(&Rx))
				CAN2_Write(Rx);
		}


		CAN2_GestionEnvoiTrame();
	}

}
#endif



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 */
void CAN2_Init() {

	CAN_InitTypeDef xCAN_Init = {

			.CAN_Mode		= CAN_Mode_Normal,

			// Baudrate = 500kbps
			.CAN_Prescaler	= 3,
			.CAN_SJW		= CAN_SJW_1tq,
			.CAN_BS1		= CAN_BS1_16tq,
			.CAN_BS2		= CAN_BS2_3tq,

			.CAN_TTCM = DISABLE,				// Time triggered communication mode
			.CAN_ABOM = ENABLE,				// Automatic bus-off management
			.CAN_AWUM = DISABLE,				// Automatic wakeup mode
			.CAN_NART = DISABLE,				// Automatic retransmission
			.CAN_RFLM = DISABLE,				// Receive FIFO locked mode
			.CAN_TXFP = ENABLE				// Transmit FIFO priority
	};

	// Initialisation des Fifo
	FIFO_Init(&CAN2_FifoRx, CAN2_FifoRxBuffer, CAN2_RX_FIFO_SIZE, sizeof(CAN_Msg_t));
	FIFO_Init(&CAN2_FifoTx, CAN2_FifoTxBuffer, CAN2_TX_FIFO_SIZE, sizeof(CAN_Msg_t));

	// Activation horloges CAN1 obligatioire pour utilisation CAN2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	// Activation peripherique
	CAN_Init(CAN2, &xCAN_Init);

	// Configuration interruption Rx
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(CAN2_RX0_IRQn);
	#endif

	// Configuration interruption Tx
	#if CAN2_USE_TX_INTERRUPT
		CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);

		#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
			NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init( &NVIC_InitStructure );
		#else
			NVIC_EnableIRQ(CAN2_TX_IRQn);
		#endif
	#endif

	// Configuration interruption Status
	CAN_ITConfig(CAN2, CAN_IT_LEC | CAN_IT_ERR, ENABLE);
	NVIC_EnableIRQ(CAN2_SCE_IRQn);
	// Init Filtres
	CAN_FilterInitTypeDef xCAN_FilterInit = {
			.CAN_FilterNumber			= 14,	// CAN_1: 0, CAN_2: 14
			.CAN_FilterMode				= CAN_FilterMode_IdMask,
			.CAN_FilterScale			= CAN_FilterScale_32bit,
			.CAN_FilterIdHigh			= 0x0000,
			.CAN_FilterIdLow			= 0x0000,
			.CAN_FilterMaskIdHigh		= 0x0000,
			.CAN_FilterMaskIdLow		= 0x0000,
			.CAN_FilterFIFOAssignment	= 0,
			.CAN_FilterActivation		= ENABLE
	};
	CAN_FilterInit(&xCAN_FilterInit);

	// Vidage Buffer
	CAN_FIFORelease(CAN2, CAN_FIFO0);
	CAN_FIFORelease(CAN2, CAN_FIFO1);
	FIFO_Clear(&CAN2_FifoRx);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void CAN2_DeInit() {

	CAN_DeInit(CAN2);									// Desactivation peripherique
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du buffer de reception.
 *
 * @param[out]	pTrame	Trame reçue.
 *
 * return		fTrameRecue (TRUE/FALSE)
 */
uint8_t CAN2_Read(CAN_Msg_t* pTrame) {

	uint8_t fTrameRecue;

	//NVIC_DisableIRQ(CAN2_RX0_IRQn);

	if (FIFO_Pull(&CAN2_FifoRx, pTrame) != FIFO_EMPTY)
		fTrameRecue = TRUE;
	else
		fTrameRecue = FALSE;

	//NVIC_EnableIRQ(CAN2_RX0_IRQn);

	return fTrameRecue;
}


/**-----------------------------------------------------------------------------
 * @brief	Gestion de l'envoi des trames CAN.
 *
 */
uint8_t CAN2_GestionEnvoiTrame() {

	uint8_t Trame_Envoyee=FALSE;
	CAN_Msg_t TrameTx;
	CanTxMsg TxMsg;
	int i;

	// Lecture fifo
#if CAN2_USE_TX_INTERRUPT
	if (FIFO_Pull(&CAN2_FifoTx, &TrameTx) != FIFO_EMPTY)
#else
	while (FIFO_Pull(&CAN2_FifoTx, &TrameTx) != FIFO_EMPTY)
#endif
	{
		// Copie
		if (TrameTx.ID_Type == ID_Type_Std)
		{
			TxMsg.StdId	= TrameTx.ID;
			TxMsg.ExtId	= 0x00000000;
			TxMsg.IDE	= CAN_Id_Standard;
		}
		else
		{
			TxMsg.ExtId	= TrameTx.ID;
			TxMsg.StdId	= 0x000;
			TxMsg.IDE	= CAN_Id_Extended;
		}
		if (TrameTx.RTR_Type == RTR_Type_Data)
			TxMsg.RTR = CAN_RTR_Data;
		else
			TxMsg.RTR = CAN_RTR_Remote;
		TxMsg.DLC	= TrameTx.DLC;
		for (i=0; i<8; i++)
			TxMsg.Data[i] = TrameTx.Data[i];

		// Ecriture trame
		CAN_Transmit(CAN2, &TxMsg);

		Trame_Envoyee = TRUE;
	}

	return Trame_Envoyee;
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un buffer.
 *
 * @param[in]	pTrame	Trame a emettre.
 *
 * return		fTrameEmpilee (TRUE/FALSE)
 */
uint8_t CAN2_Write(CAN_Msg_t Trame) {

	uint8_t fTrameEmpilee;

	if (FIFO_Push(&CAN2_FifoTx, Trame) != FIFO_FULL)
		fTrameEmpilee = TRUE;
	else
		fTrameEmpilee = FALSE;

	#if CAN2_USE_TX_INTERRUPT
		// Si pas d'emission en cours, on la lance
		if (TxEnCours == FALSE)
		{
			TxEnCours = TRUE;
			NVIC_EnableIRQ(CAN2_TX_IRQn);
			CAN2_GestionEnvoiTrame();
		}
	#endif

	return fTrameEmpilee;
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN2_RX0_IRQHandler
  */
void CAN2_RX0_IRQHandler(void)
{
	CAN_Msg_t TrameRx;
	CanRxMsg RxMsg;
	int i;

	if (CAN_GetFlagStatus(CAN2, CAN_FLAG_FMP0))
	{
		// Lecture de la trame reçue
		CAN_Receive(CAN2, CAN_FIFO0, &RxMsg);

		// Copie
		if (RxMsg.IDE == CAN_Id_Standard)
		{
			TrameRx.ID		= RxMsg.StdId;
			TrameRx.ID_Type	= ID_Type_Std;
		}
		else
		{
			TrameRx.ID		= RxMsg.ExtId;
			TrameRx.ID_Type	= ID_Type_Ext;
		}
		if (RxMsg.RTR == CAN_RTR_Data)
			TrameRx.RTR_Type = RTR_Type_Data;
		else
			TrameRx.RTR_Type = RTR_Type_Remote;
		TrameRx.DLC		= RxMsg.DLC;
		for (i=0; i<8; i++)
			TrameRx.Data[i] = RxMsg.Data[i];

		// Ajout a la fifo
		FIFO_Push(&CAN2_FifoRx, TrameRx);
	}
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN2_TX_IRQHandler
  * @param  None
  * @retval None
  */
void CAN2_TX_IRQHandler(void)
{
	#if CAN2_USE_TX_INTERRUPT
		if (CAN2_GestionEnvoiTrame() == FALSE)
		{
			NVIC_DisableIRQ(CAN2_TX_IRQn);
			TxEnCours = FALSE;
		}
	#endif
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN2_RX1_IRQHandler
  * @param  None
  * @retval None
  */
void CAN2_RX1_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN2_SCE_IRQHandler
  * @param  None
  * @retval None
  */
void CAN2_SCE_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN2, CAN_IT_LEC))
	{
		if (CAN_GetLastErrorCode(CAN2) == CAN_ErrorCode_ACKErr)
		{
			for( uint8_t TransmitMailbox = 0; TransmitMailbox < 3; TransmitMailbox++)
			{
				if( CAN_TransmitStatus(CAN2, TransmitMailbox) == CAN_TxStatus_Failed)
					CAN_CancelTransmit(CAN2, TransmitMailbox);
			}
		}

		CAN_ClearITPendingBit(CAN2, CAN_IT_LEC);
	}
}


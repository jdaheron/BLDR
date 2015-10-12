/**
 *******************************************************************************
 * @file	drv_CAN1.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 oct. 2013
 * @brief   Gestion du CAN1.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_CAN1.h"
#include "util_FIFO.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define CAN1_TX_FIFO_SIZE		32
#define CAN1_RX_FIFO_SIZE		16
#define CAN1_USE_TX_INTERRUPT	1


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static CAN_Msg_t CAN1_FifoTxBuffer[CAN1_TX_FIFO_SIZE];
static CAN_Msg_t CAN1_FifoRxBuffer[CAN1_RX_FIFO_SIZE];
static Fifo_t CAN1_FifoTx;
static Fifo_t CAN1_FifoRx;
static uint8_t TxEnCours = FALSE;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


#define  VALIDATION_CAN1	0

#if VALIDATION_CAN1

void CAN1_Test() {

	CAN_Msg_t msg[10]={
			{.ID_Type = ID_Type_Ext,	.ID = 0x0123,	.DLC = 8,		.Data = "01234567"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x123456,	.DLC = 8,		.Data = "12345678"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x2,		.DLC = 8,		.Data = "23456789"},
			{.ID_Type = ID_Type_Std,	.ID = 0x3,		.DLC = 8,		.Data = "34567890"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x4,		.DLC = 8,		.Data = "45678901"},
			{.ID_Type = ID_Type_Std,	.ID = 0x5,		.DLC = 8,		.Data = "56789012"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x6,		.DLC = 8,		.Data = "67890123"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x7123,	.DLC = 8,		.Data = "78901234"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x823456,	.DLC = 8,		.Data = "89012345"},
			{.ID_Type = ID_Type_Ext,	.ID = 0x97865,	.DLC = 8,		.Data = "90123456"},
	};
	CAN_Msg_t Rx;
	int i=0;

	CAN1_Init();

	while(1) {

		if (TimerSW_GetStatus(TIMER_Generique) == Status_Fini){


			for (i=0; i<10;i++)
			{
				CAN1_Write(msg[i]);
				TimerSW_Start(TIMER_Generique2, 10);
			}

			TimerSW_Start(TIMER_Generique, 100);

		}

		if (TimerSW_GetStatus(TIMER_TimeOutGenerique) == Status_Fini){

			if (CAN1_Read(&Rx)){
				if (Rx.DLC == 1)
				{
					switch (Rx.ID)
					{
						case 0x10:
							if (Rx.Data[0] == 0)	GPIO_Set(PORT_LED_MVT, Etat_INACTIF);
							else					GPIO_Set(PORT_LED_MVT, Etat_ACTIF);
							break;

						case 0x20:
							if (Rx.Data[0] == 0)	GPIO_Set(PORT_LED_DEFAUT, Etat_INACTIF);
							else					GPIO_Set(PORT_LED_DEFAUT, Etat_ACTIF);
							break;

						case 0x30:
							if (Rx.Data[0] == 0)	GPIO_Set(PORT_LED_LIFEBIT, Etat_INACTIF);
							else					GPIO_Set(PORT_LED_LIFEBIT, Etat_ACTIF);
							break;

						default:
							GPIO_Set(PORT_LED_STAT4, Etat_ACTIF);
							break;
					}
				}
				else
					GPIO_Set(PORT_LED_STAT4, Etat_ACTIF);

				if (GPIO_Get(PORT_LED_STAT4) == Etat_ACTIF)
				{
					TimerSW_Temporiser(TIMER_TimeOutGenerique, 100);
					GPIO_Set(PORT_LED_STAT4, Etat_INACTIF);
				}
			}
			TimerSW_Start(TIMER_TimeOutGenerique, 5);
		}

	}

}
#endif



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 */
void CAN1_Init() {

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
	FIFO_Init(&CAN1_FifoRx, CAN1_FifoRxBuffer, CAN1_RX_FIFO_SIZE, sizeof(CAN_Msg_t));
	FIFO_Init(&CAN1_FifoTx, CAN1_FifoTxBuffer, CAN1_TX_FIFO_SIZE, sizeof(CAN_Msg_t));

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	// Activation peripherique
	CAN_Init(CAN1, &xCAN_Init);

	// Configuration interruption Rx
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(CAN1_RX0_IRQn);
	#endif

	// Configuration interruption Tx
	#if CAN1_USE_TX_INTERRUPT
		CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);

		#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
			NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init( &NVIC_InitStructure );
		#else
			NVIC_EnableIRQ(CAN1_TX_IRQn);
		#endif
	#endif

	// Configuration interruption Status
	CAN_ITConfig(CAN1, CAN_IT_LEC | CAN_IT_ERR, ENABLE);
	NVIC_EnableIRQ(CAN2_SCE_IRQn);
	// Init Filtres
	CAN_FilterInitTypeDef xCAN_FilterInit = {
			.CAN_FilterNumber			= 0,	// CAN_1: 0, CAN_2: 14
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
	CAN_FIFORelease(CAN1, CAN_FIFO0);
	CAN_FIFORelease(CAN1, CAN_FIFO1);
	FIFO_Clear(&CAN1_FifoRx);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void CAN1_DeInit() {

	CAN_DeInit(CAN1);									// Desactivation peripherique
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, ENABLE);	// Desactivation horloge

}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du buffer de reception.
 *
 * @param[out]	pTrame	Trame reçue.
 *
 * return		fTrameRecue (TRUE/FALSE)
 */
uint8_t CAN1_Read(CAN_Msg_t* pTrame) {

	uint8_t fTrameRecue;

	NVIC_DisableIRQ(CAN1_RX0_IRQn);

	if (FIFO_Pull(&CAN1_FifoRx, pTrame) != FIFO_EMPTY)
		fTrameRecue = TRUE;
	else
		fTrameRecue = FALSE;

	NVIC_EnableIRQ(CAN1_RX0_IRQn);

	return fTrameRecue;
}


/**-----------------------------------------------------------------------------
 * @brief	Gestion de l'envoi des trames CAN.
 *
 */
uint8_t CAN1_GestionEnvoiTrame() {

	uint8_t Trame_Envoyee=FALSE;
	CAN_Msg_t TrameTx;
	CanTxMsg TxMsg;
	int i;

	// Lecture fifo
#if CAN1_USE_TX_INTERRUPT
	if (FIFO_Pull(&CAN1_FifoTx, &TrameTx) != FIFO_EMPTY)
#else
	while (FIFO_Pull(&CAN1_FifoTx, &TrameTx) != FIFO_EMPTY)
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
		CAN_Transmit(CAN1, &TxMsg);

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
uint8_t CAN1_Write(CAN_Msg_t Trame) {

	uint8_t fTrameEmpilee;

	if (FIFO_Push(&CAN1_FifoTx, Trame) != FIFO_FULL)
		fTrameEmpilee = TRUE;
	else
		fTrameEmpilee = FALSE;

	#if CAN1_USE_TX_INTERRUPT
		// Si pas d'emission en cours, on la lance
		if (TxEnCours == FALSE)
		{
			TxEnCours = TRUE;
			NVIC_EnableIRQ(CAN1_TX_IRQn);
			CAN1_GestionEnvoiTrame();
		}
	#endif

	return fTrameEmpilee;
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN1_RX0_IRQHandler
  */
void CAN1_RX0_IRQHandler(void)
{
	CAN_Msg_t TrameRx;
	CanRxMsg RxMsg;
	int i;

	if (CAN_GetFlagStatus(CAN1, CAN_FLAG_FMP0))
	{
		// Lecture de la trame reçue
		CAN_Receive(CAN1, CAN_FIFO0, &RxMsg);

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
		FIFO_Push(&CAN1_FifoRx, TrameRx);
	}
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN1_TX_IRQHandler
  * @param  None
  * @retval None
  */
void CAN1_TX_IRQHandler(void)
{
	#if CAN1_USE_TX_INTERRUPT
		if (CAN1_GestionEnvoiTrame() == FALSE)
		{
			NVIC_DisableIRQ(CAN1_TX_IRQn);
			TxEnCours = FALSE;
		}
	#endif
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN1_RX1_IRQHandler
  * @param  None
  * @retval None
  */
void CAN1_RX1_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  CAN1_SCE_IRQHandler
  * @param  None
  * @retval None
  */
void CAN1_SCE_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_LEC))
	{
		if (CAN_GetLastErrorCode(CAN1) == CAN_ErrorCode_ACKErr)
		{
			for( uint8_t TransmitMailbox = 0; TransmitMailbox < 3; TransmitMailbox++)
			{
				if( CAN_TransmitStatus(CAN1, TransmitMailbox) == CAN_TxStatus_Failed)
					CAN_CancelTransmit(CAN1, TransmitMailbox);
			}
		}

		CAN_ClearITPendingBit(CAN1, CAN_IT_LEC);
	}
}


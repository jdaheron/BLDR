/**
 *******************************************************************************
 * @file	BSP_CARTE.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	19 mars 2013
 * @brief   Board Support Package
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "BSP_CARTE.h"


/* External variables --------------------------------------------------------*/

extern uint32_t OFFSET;


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation de la carte.
 */
void BSP_Init() {

	MappingGpio_e PinId;

	/* Desactivation des interruptions */
	__disable_irq();

	/* Initialisation STM32 system (clock, PLL and Flash configuration) */
	SystemInit();
	SystemCoreClockUpdate();

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, OFFSET);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	/* Initialisation des GPIO */
	for (PinId = 0; PinId < NB_PORT_GPIO; PinId++) // Parcours de a table du mapping
		GPIO_InitPin(PinId);

	// Configuration du fonctionnement du Debug
	DBGMCU_Config(DBGMCU_STOP, ENABLE);					// Keep debugger connection during STOP mode
	DBGMCU_APB1PeriphConfig(DBGMCU_WWDG_STOP, ENABLE);	// Debug WWDG stopped when Core is halted

	// Effacement des flags d'erreur d'acces a la flash
	FLASH_ClearFlag(	FLASH_FLAG_EOP		| FLASH_FLAG_OPERR	| FLASH_FLAG_WRPERR	| FLASH_FLAG_PGAERR |
						FLASH_FLAG_PGPERR	| FLASH_FLAG_PGSERR | FLASH_FLAG_BSY);

	/* Activation des interruptions */
	__enable_irq();
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation de la carte.
 */
void BSP_DeInit() {

	// Desactivation des interruptions
	__disable_irq();

	// Arret Systick
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	// DeInitialisation des horloges

	//RCC_AHB1PeriphClockCmd(0xFFFFFFFF, DISABLE);
	RCC_AHB1PeriphResetCmd(0xFFFFFFFF, ENABLE);
	RCC_AHB1PeriphResetCmd(0xFFFFFFFF, DISABLE);

	//RCC_AHB2PeriphClockCmd(0xFFFFFFFF, DISABLE);
	RCC_AHB2PeriphResetCmd(0xFFFFFFFF, ENABLE);
	RCC_AHB2PeriphResetCmd(0xFFFFFFFF, DISABLE);

	//RCC_AHB3PeriphClockCmd(0xFFFFFFFF, DISABLE);
	RCC_AHB3PeriphResetCmd(0xFFFFFFFF, ENABLE);
	RCC_AHB3PeriphResetCmd(0xFFFFFFFF, DISABLE);

	//RCC_APB1PeriphClockCmd(0xFFFFFFFF, DISABLE);
	RCC_APB1PeriphResetCmd(0xFFFFFFFF, ENABLE);
	RCC_APB1PeriphResetCmd(0xFFFFFFFF, DISABLE);

	//RCC_APB2PeriphClockCmd(0xFFFFFFFF, DISABLE);
	RCC_APB2PeriphResetCmd(0xFFFFFFFF, ENABLE);
	RCC_APB2PeriphResetCmd(0xFFFFFFFF, DISABLE);
	RCC_DeInit();

	// ReInit Flash
	FLASH_PrefetchBufferCmd(DISABLE);
	FLASH_InstructionCacheCmd(DISABLE);
	FLASH_DataCacheCmd(DISABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Entree en mode Stop.
 */
void STOP_MODE() {

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Config IT
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);	// Connect EXTI Line0 to PA0 pin

	// Configure EXTI Line0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// Enable and set EXTI Line0 Interrupt to the lowest priority
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Config Mode Stop
	PWR_WakeUpPinCmd(ENABLE);
	PWR_BackupRegulatorCmd(DISABLE);
	PWR_FlashPowerDownCmd(ENABLE);

	/* Enable PWR and BKP clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	// Entree Mode Stop
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}


/**-----------------------------------------------------------------------------
 * @brief	Initialisation d'une Pin.
 *
 * @param[in]	PinId		Numero d'identification de la pin.
 */
void GPIO_InitPin(MappingGpio_e PinId) {

	GPIO_InitTypeDef GPIO;

	// Activation de l'horloge assiciee a la Pin
	switch ((uint32_t) MappingGpio[PinId].pGPIOx) {

		case (uint32_t) GPIOA :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			break;

		case (uint32_t) GPIOB :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOB, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			break;

		case (uint32_t) GPIOC :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOC, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			break;

		case (uint32_t) GPIOD :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOD, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			break;

		case (uint32_t) GPIOE :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
			break;

		case (uint32_t) GPIOF :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOF, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
			break;

		case (uint32_t) GPIOG :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOG, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
			break;

		case (uint32_t) GPIOH :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOH, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
			break;

		case (uint32_t) GPIOI :
			RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOI, DISABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
			break;

		default:
			break;
	}

	// Configuration de la pin
	GPIO.GPIO_Pin = (uint16_t) (1 << MappingGpio[PinId].Pin);
	GPIO.GPIO_Mode = MappingGpio[PinId].GPIO_Mode;
	GPIO.GPIO_Speed = MappingGpio[PinId].GPIO_Speed;
	GPIO.GPIO_OType = MappingGpio[PinId].GPIO_OType;
	GPIO.GPIO_PuPd = MappingGpio[PinId].GPIO_PuPd;
	GPIO_Init(MappingGpio[PinId].pGPIOx, &GPIO);

	// Initialisation de la pin si necessaire
	switch (MappingGpio[PinId].GPIO_Mode) {

		// Si sortie, ecriture de l'état initial
		case GPIO_Mode_OUT:
			GPIO_Set(PinId, MappingGpio[PinId].EtatInit);
			break;

		// Si Mode AF, connect pin to alternate function
		case GPIO_Mode_AF:
			GPIO_PinAFConfig(MappingGpio[PinId].pGPIOx, MappingGpio[PinId].Pin, MappingGpio[PinId].GPIO_AF);
			break;

		case GPIO_Mode_IN:
		case GPIO_Mode_AN:
		default:
			break;
	}

}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture d'un GPIO.
 *
 * @param[in]	PinId		Numero d'identification de la pin.
 * @param[in]	Etat		Valeur a ecrire sur la pin.
 */
void GPIO_Set(MappingGpio_e PinId, Etat_e Etat) {

	if (PinId >= NB_PORT_GPIO)
		return;

	GPIO_WriteBit(MappingGpio[PinId].pGPIOx,
			(uint16_t) (1 << MappingGpio[PinId].Pin),
			Etat ^ MappingGpio[PinId].Inverse);
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture de l'etat d'un GPIO.
 *
 * @param[in]	PinId	Numero d'identification de la pin.
 * @return 		Etat	Valeur lue sur la pin.
 */
Etat_e GPIO_Get(MappingGpio_e PinId) {

	uint8_t eEtat;

	if (PinId >= NB_PORT_GPIO)
		return Etat_INACTIF;

	// Lecture de l'etat d'une sortie
	if (MappingGpio[PinId].GPIO_Mode == GPIO_Mode_OUT) {

		eEtat = GPIO_ReadOutputDataBit(MappingGpio[PinId].pGPIOx,
				(uint16_t) (1 << MappingGpio[PinId].Pin));

	}
	// Lecture de l'etat d'une entree
	else {

		eEtat = GPIO_ReadInputDataBit(MappingGpio[PinId].pGPIOx,
				(uint16_t) (1 << MappingGpio[PinId].Pin));

	}

	// Gestion de l'inversion
	if (((eEtat == Bit_RESET) && (MappingGpio[PinId].Inverse == FALSE))
			|| ((eEtat == Bit_SET) && (MappingGpio[PinId].Inverse == TRUE))) {

		return Etat_INACTIF;
	}

	return	Etat_ACTIF;
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture de la configuration d'une Pin.
 *
 * @param[in]	PinId		Numero d'identification de la pin.
 * @return 		Config		Configuration de la Pin..
 */
MappingGpio_s GPIO_GetPinConfiguration(MappingGpio_e PinId) {

	return MappingGpio[PinId];
}

/**-----------------------------------------------------------------------------
 * @brief	Initialisation de la sortie Clock.
 *
 */
void BSP_ClockOutput_Init() {

	//RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_4);	// 120/4 = 30MHz
	RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2 );	// 120/2 = 60MHz
}


/**-----------------------------------------------------------------------------
 * @brief	Envoi d'une donnée sur la console.
 *
 * param[in]	x		Donnee a ecrire.
 */
void Console_Send(uint8_t x) {
	HAL_Console_Write(&x, 1);
}




/**-----------------------------------------------------------------------------
 * @brief	Delais approximatif...
 */
void Delay_ms(uint32_t delay_ms) {

	uint32_t i;

	while (delay_ms--) {
		for (i = 0; i < 9230; i++)
			;
	}
}



/*----------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
 *******************************************************************************
 * @file	drv_WDG.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	06 dec. 2013
 * @brief   Gestion du Watchdog.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "drv_WDG.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define WWDG_TICK_DELAI_ms	69


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static uint16_t WWDG_Reload = 0;
static uint16_t WWDG_Cnt = 0;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation du watchdog Independant.
 *
 * param[in]	Timeout_ms	Duree du timeout watchdog en ms.
 */
void WDG_InitIWDG(uint16_t Timeout_ms){

	// Verification des parametres d'entree
	if (Timeout_ms > 32768)	Timeout_ms = 32768;
	if (Timeout_ms == 0)	return;

	// Configuration du Timeout
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	if (Timeout_ms <= 4096)
		IWDG_SetPrescaler(IWDG_Prescaler_32);
	else{
		IWDG_SetPrescaler(IWDG_Prescaler_256);
		Timeout_ms /= 8;
	}
	IWDG_SetReload(Timeout_ms);
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);

	// Activation
	IWDG_Enable();
	IWDG_ReloadCounter();
}


/**-----------------------------------------------------------------------------
 * @brief	Initialisation du watchdog Windows.
 *
 * param[in]	Timeout_ms	Duree du timeout watchdog en ms.
 */
void WDG_InitWWDG(uint16_t Timeout_ms){

	RCC_ClocksTypeDef xRCC_Clocks;
	uint32_t ulClock_kHz;

	WWDG_DeInit();

	// Initialisation des variables
	WWDG_Reload = Timeout_ms;
	WWDG_Cnt = Timeout_ms;

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	// Configuration interruption
	WWDG_EnableIT();
	#ifdef configLIBRARY_KERNEL_INTERRUPT_PRIORITY
		NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
	#else
		NVIC_EnableIRQ(WWDG_IRQn);
	#endif

	// Configuration timeout
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x7F);

	// Activation
	WWDG_Enable(0x7F);
	WDG_Refresh();
}

/**-----------------------------------------------------------------------------
 * @brief	Rafraichissement du watchdog.
 *
 */
void  WDG_Refresh(){
	IWDG_ReloadCounter();

	WWDG_SetCounter(0x7F);
	WWDG_Cnt = WWDG_Reload;
}


/**-----------------------------------------------------------------------------
 * @brief	Desactivation du watchdog.
 *
 */
void WDG_Disable(){
	WWDG_DeInit();
	WDG_Refresh();
}

/**-----------------------------------------------------------------------------
 * @brief	Tick watchdog.
 *
 */
void WDG_Tick(){


	WWDG_SetCounter(0x7F);	// Evite de generer une nouvelle IT

	// Decrementation du compteur
	if (WWDG_Cnt >= WWDG_TICK_DELAI_ms) {

		WWDG_Cnt -= WWDG_TICK_DELAI_ms;
	}
	else {
		WWDG_Cnt = 0;
	}

	// Verification Zero atteint
	if (WWDG_Cnt == 0) {

		WWDG_SetCounter(0x40);
		// Attente reboot
		while(1);
	}

	// Relance watchdog
	WWDG_SetCounter(0x7F);

}


/**-----------------------------------------------------------------------------
  * @brief  WWDG_IRQHandler
  */
void WWDG_IRQHandler(void)
{
	WWDG_ClearFlag();
	WDG_Tick();
}






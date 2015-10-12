/**
  ******************************************************************************
  * @file    stm32f2xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22/07/2011
  * @brief   This file includes the interrupt handlers for the application
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "BSP.h"
#include "stm32f2xx_it.h"
#include "stm32f2xx_hal.h"
#include "util_printf.h"
#include "main.h"


/* External variables --------------------------------------------------------*/

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern void ApplicationTickHook();


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{

}

/*----------------------------------------------------------------------------*/
/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	asm("nop");

	_printf("\nHARD FAULT\n");
	Delay_ms(50);

	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  SVC_Handler
  *         This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#if (!USE_FREERTOS)
void SVC_Handler(void)
{

}
#endif

/*----------------------------------------------------------------------------*/
/**-----------------------------------------------------------------------------
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
	__NOP();
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  PendSV_Handler
  *         This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if (!USE_FREERTOS)
void PendSV_Handler(void)
{

}
#endif

/*----------------------------------------------------------------------------*/
/**
  * @brief  MemManage_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
#if (!USE_FREERTOS)
void SysTick_Handler(void)
{
	ApplicationTickHook();
	HAL_IncTick();
}
#endif


/******************************************************************************/
/* External Interrupts */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/**
  * @brief  PVD_IRQHandler
  * @param  None
  * @retval None
  */
void PVD_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TAMP_STAMP_IRQHandler
  * @param  None
  * @retval None
  */
void TAMP_STAMP_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  RCC_IRQHandler
  * @param  None
  * @retval None
  */
void RCC_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  EXTI0_IRQHandler
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{

}


/*----------------------------------------------------------------------------*/
/**
  * @brief  EXTI1_IRQHandler
  *         This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{

}

/*----------------------------------------------------------------------------*/
/**
  * @brief  EXTI2_IRQHandler
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  EXTI3_IRQHandler
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  EXTI4_IRQHandler
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream0_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream0_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream1_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream1_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream2_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream2_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream3_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream3_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream4_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream4_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream5_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream5_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream6_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream6_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  EXTI9_5_IRQHandler
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM1_BRK_TIM9_IRQHandler
  * @param  None
  * @retval None
  */
void TIM1_BRK_TIM9_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM1_UP_TIM10_IRQHandler
  * @param  None
  * @retval None
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM1_TRG_COM_TIM11_IRQHandler
  * @param  None
  * @retval None
  */
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM1_CC_IRQHandler
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM3_IRQHandler
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM4_IRQHandler
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
#if 0
	//TODO : Verif port EXTI
	if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
	{
		Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
		/* Clear interrupt pending bit */
		EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
	}
#endif
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  OTG_FS_WKUP_IRQHandler
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM8_BRK_TIM12_IRQHandler
  * @param  None
  * @retval None
  */
void TIM8_BRK_TIM12_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM8_UP_TIM13_IRQHandler
  * @param  None
  * @retval None
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM8_TRG_COM_TIM14_IRQHandler
  * @param  None
  * @retval None
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM8_CC_IRQHandler
  * @param  None
  * @retval None
  */
void TIM8_CC_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA1_Stream7_IRQHandler
  * @param  None
  * @retval None
  */
void DMA1_Stream7_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  FSMC_IRQHandler
  * @param  None
  * @retval None
  */
void FSMC_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  SDIO_IRQHandler
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM5_IRQHandler
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM6_IRQHandler
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  TIM7_IRQHandler
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream0_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream0_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream1_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream1_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream2_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream2_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream3_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream4_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream4_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  ETH_IRQHandler
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  ETH_WKUP_IRQHandler
  * @param  None
  * @retval None
  */
void ETH_WKUP_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
//	 NVIC_ClearPendingIRQ(OTG_FS_IRQn);
//	 HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream5_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream5_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream6_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  DMA2_Stream7_IRQHandler
  * @param  None
  * @retval None
  */
void DMA2_Stream7_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}


#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/*----------------------------------------------------------------------------*/
/**
  * @brief  OTG_HS_EP1_OUT_IRQHandler
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  OTG_HS_EP1_IN_IRQHandler
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}
#endif
/*----------------------------------------------------------------------------*/
/**
  * @brief  OTG_HS_WKUP_IRQHandler
  * @param  None
  * @retval None
  */
void OTG_HS_WKUP_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  OTG_HS_IRQHandler
  * @param  None
  * @retval None
  */
void OTG_HS_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}


/*----------------------------------------------------------------------------*/
/**
  * @brief  DCMI_IRQHandler
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  CRYP_IRQHandler
  * @param  None
  * @retval None
  */
void CRYP_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

/*----------------------------------------------------------------------------*/
/**
  * @brief  HASH_RNG_IRQHandler
  * @param  None
  * @retval None
  */
void HASH_RNG_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}

#if 0
/*----------------------------------------------------------------------------*/
/**
  * @brief  yyy
  * @param  None
  * @retval None
  */
void xxx(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}
#endif



/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/



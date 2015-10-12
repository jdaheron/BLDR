/**
  ******************************************************************************
  * @file    stm32f2x7_eth_bsp.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011 
  * @brief   STM32F2x7 Ethernet hardware configuration.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2x7_eth.h"
#include "stm32f2x7_eth_bsp.h"
#include "main.h"
#include "stm32f2xx_hal_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//__IO uint32_t  EthInitStatus = 0;
//__IO uint8_t EthLinkStatus = 0;

/* Private function prototypes -----------------------------------------------*/
//static void ETH_GPIO_Config(void);
//static void ETH_MACDMA_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{
#if 0
  RCC_ClocksTypeDef RCC_Clocks;
  
  /* Configure the GPIO ports for ethernet pins */
  ETH_GPIO_Config();
  
  /* Configure the Ethernet MAC/DMA */
  ETH_MACDMA_Config();

  if (EthInitStatus == 0)
  {
    _printf("Ethernet Init failed\n");
    while(1);
  }

  /* Configure the PHY to generate an interrupt on change of link status */
  //Eth_Link_PHYITConfig(DP83848_PHY_ADDRESS);
  Eth_Link_PHYITConfig(KS8721_PHY_ADDRESS);

  /* Configure the EXTI for Ethernet link status. */
  //Eth_Link_EXTIConfig();
#else

	ETH_InitTypeDef ETH_InitStructure;

	/* Enable ETHERNET clocks  */
	RCC_AHB1PeriphClockCmd(	  RCC_AHB1Periph_ETH_MAC
							| RCC_AHB1Periph_ETH_MAC_Tx
							| RCC_AHB1Periph_ETH_MAC_Rx
							| RCC_AHB1Periph_ETH_MAC_PTP, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/*Select RMII Interface*/
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

	/* Reset ETHERNET on AHB Bus */
	ETH_DeInit();

	/* Software reset */
	ETH_SoftwareReset();

	/* Wait for software reset */
	while(ETH_GetSoftwareResetStatus() == SET);

	/* ETHERNET Configuration ------------------------------------------------------*/
	/* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	ETH_StructInit(&ETH_InitStructure);

	/* Fill ETH_InitStructure parametrs */
	/*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation			= ETH_AutoNegotiation_Disable  ;
	ETH_InitStructure.ETH_Speed						= ETH_Speed_100M;
	ETH_InitStructure.ETH_LoopbackMode				= ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_Mode						= ETH_Mode_FullDuplex;
	ETH_InitStructure.ETH_RetryTransmission			= ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip		= ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll				= ETH_ReceiveAll_Enable;
	ETH_InitStructure.ETH_BroadcastFramesReception	= ETH_BroadcastFramesReception_Disable;
	ETH_InitStructure.ETH_PromiscuousMode			= ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter		= ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter		= ETH_UnicastFramesFilter_Perfect;

	unsigned int PhyAddr;

	// read the ID for match
	for(PhyAddr = 1; 32 >= PhyAddr; PhyAddr++)
	{
		if((0x0022 == ETH_ReadPHYRegister(PhyAddr,2)) && (0x1619 == (ETH_ReadPHYRegister(PhyAddr,3))))
			break;
	}

	if(32 < PhyAddr)
	{
		_printf("Ethernet Phy Not Found\n\r");
		return;// 1;
	}

	/* Configure Ethernet */
	if(0 == ETH_Init(&ETH_InitStructure, PhyAddr))
	{
		_printf("Ethernet Initialization Failed\n\r");
		return;// 1;
	}

	_printf("Check LAN LEDs\n\r");
  
#endif
}


#if 0
/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(void)
{
  ETH_InitTypeDef ETH_InitStructure;

  /* Enable ETHERNET clock  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                        RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
                        
  /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();

  /* Software reset */
  ETH_SoftwareReset();

  /* Wait for software reset */
  while (ETH_GetSoftwareResetStatus() == SET);

  /* ETHERNET Configuration --------------------------------------------------*/
  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
  ETH_StructInit(&ETH_InitStructure);

  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
  //ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable; 
  //  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
  //  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;   

  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

  /*------------------------   DMA   -----------------------------------*/  
  
  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;     
 
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

  /* Configure Ethernet */
  //EthInitStatus = ETH_Init(&ETH_InitStructure, DP83848_PHY_ADDRESS);
  EthInitStatus = ETH_Init(&ETH_InitStructure, KS8721_PHY_ADDRESS);
}

/**
  * @brief  Configure the PHY to generate an interrupt on change of link status.
  * @param PHYAddress: external PHY address  
  * @retval None
  */
uint32_t Eth_Link_PHYITConfig(uint16_t PHYAddress)
{
	uint32_t tmpreg = 0;

	/* Read MICR register */
	tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MICR);

	/* Enable output interrupt events to signal via the INT pin */
	tmpreg |= (uint32_t) PHY_MICR_INT_EN | PHY_MICR_INT_OE;
	if (!(ETH_WritePHYRegister(PHYAddress, PHY_MICR, tmpreg)))
	{
		/* Return ERROR in case of write timeout */
		return ETH_ERROR;
	}

	/* Read MISR register */
	tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MISR);

	/* Enable Interrupt on change of link status */
	tmpreg |= (uint32_t) PHY_MISR_LINK_INT_EN;
	if (!(ETH_WritePHYRegister(PHYAddress, PHY_MISR, tmpreg)))
	{
		/* Return ERROR in case of write timeout */
		return ETH_ERROR;
	}
	/* Return SUCCESS */
	return ETH_SUCCESS;
}

/**
  * @brief  EXTI configuration for Ethernet link status.
  * @param PHYAddress: external PHY address  
  * @retval None
  */
void Eth_Link_EXTIConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //TODO PA3 ?


  /* Enable the INT (PB14) Clock */
  RCC_AHB1PeriphClockCmd(ETH_LINK_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure INT pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = ETH_LINK_PIN;
  GPIO_Init(ETH_LINK_GPIO_PORT, &GPIO_InitStructure);

  /* Connect EXTI Line to INT Pin */
  SYSCFG_EXTILineConfig(ETH_LINK_EXTI_PORT_SOURCE, ETH_LINK_EXTI_PIN_SOURCE);

  /* Configure EXTI line */
  EXTI_InitStructure.EXTI_Line = ETH_LINK_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set the EXTI interrupt to the highest priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles Ethernet link status.
  * @param  None
  * @retval None
  */
void Eth_Link_ITHandler(uint16_t PHYAddress)
{
  /* Check whether the link interrupt has occurred or not */
  if(((ETH_ReadPHYRegister(PHYAddress, PHY_MISR)) & PHY_LINK_STATUS) != 0)
  {
    EthLinkStatus = ~EthLinkStatus;

#ifdef USE_LCD
    /* Set the LCD Text Color */
    LCD_SetTextColor(Red);

    if(EthLinkStatus != 0)
    {
      /* Display message on the LCD */
      LCD_DisplayStringLine(Line5, (uint8_t*)"  Network Cable is  ");
      LCD_DisplayStringLine(Line6, (uint8_t*)"     unplugged      ");
    }
    else
    {
      /* Display message on the LCD */
      LCD_DisplayStringLine(Line5, (uint8_t*)"  Network Cable is  ");
      LCD_DisplayStringLine(Line6, (uint8_t*)"   now connected    ");
    }
#endif
  }
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

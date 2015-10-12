/**
 *******************************************************************************
 * @file	startup_stm32f20x.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	13 mars 2013
 * @brief	STM32F2xx Devices startup file.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the initial PC == Reset_Handler,
 *                - Set the vector table entries with the exceptions ISR
 *                  address.
 *                - Configure the clock system
 *                - Branches to main in the C library (which eventually
 *                  calls main()).
 *            After Reset the Cortex-M3 processor is in Thread mode,
 *            priority is Privileged, and the Stack is set to Main.
 ******************************************************************************
 */

#define WEAK __attribute__ ((weak))

/* Includes ----------------------------------------------------------------------*/
#include "stm32f2xx.h"

void WEAK Reset_Handler(void);
void WEAK NMI_Handler(void);
void WEAK HardFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK BusFault_Handler(void);
void WEAK UsageFault_Handler(void);
void WEAK SVC_Handler(void);
void WEAK DebugMon_Handler(void);
void WEAK PendSV_Handler(void);
void WEAK SysTick_Handler(void);

/* External Interrupts */
void WEAK WWDG_IRQHandler(void);
void WEAK PVD_IRQHandler(void);
void WEAK TAMP_STAMP_IRQHandler(void);
void WEAK RTC_WKUP_IRQHandler(void);
void WEAK FLASH_IRQHandler(void);
void WEAK RCC_IRQHandler(void);
void WEAK EXTI0_IRQHandler(void);
void WEAK EXTI1_IRQHandler(void);
void WEAK EXTI2_IRQHandler(void);
void WEAK EXTI3_IRQHandler(void);
void WEAK EXTI4_IRQHandler(void);
void WEAK DMA1_Stream0_IRQHandler(void);
void WEAK DMA1_Stream1_IRQHandler(void);
void WEAK DMA1_Stream2_IRQHandler(void);
void WEAK DMA1_Stream3_IRQHandler(void);
void WEAK DMA1_Stream4_IRQHandler(void);
void WEAK DMA1_Stream5_IRQHandler(void);
void WEAK DMA1_Stream6_IRQHandler(void);
void WEAK ADC_IRQHandler(void);
void WEAK CAN1_TX_IRQHandler(void);
void WEAK CAN1_RX0_IRQHandler(void);
void WEAK CAN1_RX1_IRQHandler(void);
void WEAK CAN1_SCE_IRQHandler(void);
void WEAK EXTI9_5_IRQHandler(void);
void WEAK TIM1_BRK_TIM9_IRQHandler(void);
void WEAK TIM1_UP_TIM10_IRQHandler(void);
void WEAK TIM1_TRG_COM_TIM11_IRQHandler(void);
void WEAK TIM1_CC_IRQHandler(void);
void WEAK TIM2_IRQHandler(void);
void WEAK TIM3_IRQHandler(void);
void WEAK TIM4_IRQHandler(void);
void WEAK I2C1_EV_IRQHandler(void);
void WEAK I2C1_ER_IRQHandler(void);
void WEAK I2C2_EV_IRQHandler(void);
void WEAK I2C2_ER_IRQHandler(void);
void WEAK SPI1_IRQHandler(void);
void WEAK SPI2_IRQHandler(void);
void WEAK USART1_IRQHandler(void);
void WEAK USART2_IRQHandler(void);
void WEAK USART3_IRQHandler(void);
void WEAK EXTI15_10_IRQHandler(void);
void WEAK RTC_Alarm_IRQHandler(void);
void WEAK OTG_FS_WKUP_IRQHandler(void);
void WEAK TIM8_BRK_TIM12_IRQHandler(void);
void WEAK TIM8_UP_TIM13_IRQHandler(void);
void WEAK TIM8_TRG_COM_TIM14_IRQHandler(void);
void WEAK TIM8_CC_IRQHandler(void);
void WEAK DMA1_Stream7_IRQHandler(void);
void WEAK FSMC_IRQHandler(void);
void WEAK SDIO_IRQHandler(void);
void WEAK TIM5_IRQHandler(void);
void WEAK SPI3_IRQHandler(void);
void WEAK UART4_IRQHandler(void);
void WEAK UART5_IRQHandler(void);
void WEAK TIM6_IRQHandler(void);
void WEAK TIM7_IRQHandler(void);
void WEAK DMA2_Stream0_IRQHandler(void);
void WEAK DMA2_Stream1_IRQHandler(void);
void WEAK DMA2_Stream2_IRQHandler(void);
void WEAK DMA2_Stream3_IRQHandler(void);
void WEAK DMA2_Stream4_IRQHandler(void);
void WEAK ETH_IRQHandler(void);
void WEAK ETH_WKUP_IRQHandler(void);
void WEAK CAN2_TX_IRQHandler(void);
void WEAK CAN2_RX0_IRQHandler(void);
void WEAK CAN2_RX1_IRQHandler(void);
void WEAK CAN2_SCE_IRQHandler(void);
void WEAK OTG_FS_IRQHandler(void);
void WEAK DMA2_Stream5_IRQHandler(void);
void WEAK DMA2_Stream6_IRQHandler(void);
void WEAK DMA2_Stream7_IRQHandler(void);
void WEAK USART6_IRQHandler(void);
void WEAK I2C3_EV_IRQHandler(void);
void WEAK I2C3_ER_IRQHandler(void);
void WEAK OTG_HS_EP1_OUT_IRQHandler(void);
void WEAK OTG_HS_EP1_IN_IRQHandler(void);
void WEAK OTG_HS_WKUP_IRQHandler(void);
void WEAK OTG_HS_IRQHandler(void);
void WEAK DCMI_IRQHandler(void);
void WEAK CRYP_IRQHandler(void);
void WEAK HASH_RNG_IRQHandler(void);


/* Exported types --------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/

extern unsigned long _flash_data;
extern unsigned long _data;			/* start address for the .data section. defined in linker script */
extern unsigned long _edata;		/* end address for the .data section. defined in linker script */

extern unsigned long _bss;			/* start address for the .bss section. defined in linker script */
extern unsigned long _ebss;			/* end address for the .bss section. defined in linker script */

extern unsigned long  _estack;

extern char VERSION_SW[];


/* Private typedef -----------------------------------------------------------*/


/* function prototypes ------------------------------------------------------*/

void Reset_Handler(void) __attribute__((__interrupt__));
extern int main(void);


/******************************************************************************
 *
 * The minimal vector table for a Cortex M3.  Note that the proper constructs
 * must be placed on this to ensure that it ends up at physical address
 * 0x0000.0000.
 *
 ******************************************************************************/

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
		(void*) &_estack,				// The initial stack pointer
		Reset_Handler,					/* Reset Handler */
		NMI_Handler,					/* NMI Handler */
		HardFault_Handler,				/* Hard Fault Handler */
		MemManage_Handler,				/* MPU Fault Handler */
		BusFault_Handler,				/* Bus Fault Handler */
		UsageFault_Handler,				/* Usage Fault Handler */
		0,								/* Reserved */
		0,								/* Reserved */
		0,								/* Reserved */
		0,								/* Reserved */
		SVC_Handler,					/* SVCall Handler */
		DebugMon_Handler,				/* Debug Monitor Handler */
		0,								/* Reserved */
		PendSV_Handler,					/* PendSV Handler */
		SysTick_Handler,				/* SysTick Handler */

		/* External Interrupts */
		WWDG_IRQHandler,				/* Window Watchdog */
		PVD_IRQHandler,					/* PVD through EXTI Line detection */
		TAMP_STAMP_IRQHandler,			/* Tamper and TimeStamps through the EXTI line */
		RTC_WKUP_IRQHandler,			/* RTC Wakeup through the EXTI line */
		FLASH_IRQHandler,				/* Flash */
		RCC_IRQHandler,					/* RCC */
		EXTI0_IRQHandler,				/* EXTI Line 0 */
		EXTI1_IRQHandler,				/* EXTI Line 1 */
		EXTI2_IRQHandler,				/* EXTI Line 2 */
		EXTI3_IRQHandler,				/* EXTI Line 3 */
		EXTI4_IRQHandler,				/* EXTI Line 4 */
		DMA1_Stream0_IRQHandler,		/* DMA1 Stream 0 */
		DMA1_Stream1_IRQHandler,		/* DMA1 Stream 1 */
		DMA1_Stream2_IRQHandler,		/* DMA1 Stream 2 */
		DMA1_Stream3_IRQHandler,		/* DMA1 Stream 3 */
		DMA1_Stream4_IRQHandler,		/* DMA1 Stream 4 */
		DMA1_Stream5_IRQHandler,		/* DMA1 Stream 5 */
		DMA1_Stream6_IRQHandler,		/* DMA1 Stream 6 */
		ADC_IRQHandler,					/* ADC1, ADC2 and ADC3s */
		CAN1_TX_IRQHandler,				/* USB High Priority or CAN1 TX */
		CAN1_RX0_IRQHandler,			/* USB Low  Priority or CAN1 RX0 */
		CAN1_RX1_IRQHandler,			/* CAN1 RX1 */
		CAN1_SCE_IRQHandler,			/* CAN1 SCE */
		EXTI9_5_IRQHandler,				/* EXTI Line 9..5 */
		TIM1_BRK_TIM9_IRQHandler,		/* TIM1 Break and TIM9 */
		TIM1_UP_TIM10_IRQHandler,		/* TIM1 Update and TIM10 */
		TIM1_TRG_COM_TIM11_IRQHandler,	/* TIM1 Trigger and Commutation and TIM11 */
		TIM1_CC_IRQHandler,				/* TIM1 Capture Compare */
		TIM2_IRQHandler,				/* TIM2 */
		TIM3_IRQHandler,				/* TIM3 */
		TIM4_IRQHandler,				/* TIM4 */
		I2C1_EV_IRQHandler,				/* I2C1 Event */
		I2C1_ER_IRQHandler,				/* I2C1 Error */
		I2C2_EV_IRQHandler,				/* I2C2 Event */
		I2C2_ER_IRQHandler,				/* I2C2 Error */
		SPI1_IRQHandler,				/* SPI1 */
		SPI2_IRQHandler,				/* SPI2 */
		USART1_IRQHandler,				/* USART1 */
		USART2_IRQHandler,				/* USART2 */
		USART3_IRQHandler,				/* USART3 */
		EXTI15_10_IRQHandler,			/* EXTI Line 15..10 */
		RTC_Alarm_IRQHandler,			/* RTC Alarm (A and B) through EXTI Line */
		OTG_FS_WKUP_IRQHandler,			/* USB OTG FS Wakeup through EXTI line */
		TIM8_BRK_TIM12_IRQHandler,		/* TIM8 Break and TIM12 */
		TIM8_UP_TIM13_IRQHandler,		/* TIM8 Update and TIM13 */
		TIM8_TRG_COM_TIM14_IRQHandler,	/* TIM8 Trigger and Commutation and TIM14 */
		TIM8_CC_IRQHandler,				/* TIM8 Capture Compare */
		DMA1_Stream7_IRQHandler,		/* DMA1 Stream7 */
		FSMC_IRQHandler,				/* FSMC */
		SDIO_IRQHandler,				/* SDIO */
		TIM5_IRQHandler,				/* TIM5 */
		SPI3_IRQHandler,				/* SPI3 */
		UART4_IRQHandler,				/* UART4 */
		UART5_IRQHandler,				/* UART5 */
		TIM6_IRQHandler,				/* TIM6 */
		TIM7_IRQHandler,				/* TIM7 */
		DMA2_Stream0_IRQHandler,		/* DMA2 Stream 0 */
		DMA2_Stream1_IRQHandler,		/* DMA2 Stream 1 */
		DMA2_Stream2_IRQHandler,		/* DMA2 Stream 2 */
		DMA2_Stream3_IRQHandler,		/* DMA2 Stream 3 */
		DMA2_Stream4_IRQHandler,		/* DMA2 Stream 4 */
		ETH_IRQHandler,					/* Ethernet */
		ETH_WKUP_IRQHandler,			/* Ethernet Wakeup through EXTI line*/
		CAN2_TX_IRQHandler,				/* CAN2 TX */
		CAN2_RX0_IRQHandler,			/* CAN2 RX0 */
		CAN2_RX1_IRQHandler,			/* CAN2 RX1 */
		CAN2_SCE_IRQHandler,			/* CAN2 SCE */
		OTG_FS_IRQHandler,				/* USB OTG FS */
		DMA2_Stream5_IRQHandler,		/* DMA2 Stream 5 */
		DMA2_Stream6_IRQHandler,		/* DMA2 Stream 6 */
		DMA2_Stream7_IRQHandler,		/* DMA2 Stream 7 */
		USART6_IRQHandler,				/* USART6 */
		I2C3_EV_IRQHandler,				/* I2C3 event */
		I2C3_ER_IRQHandler,				/* I2C3 error */
		OTG_HS_EP1_OUT_IRQHandler,		/* USB OTG HS End Point 1 Out */
		OTG_HS_EP1_IN_IRQHandler,		/* USB OTG HS End Point 1 In */
		OTG_HS_WKUP_IRQHandler,			/* USB OTG HS Wakeup through EXTI */
		OTG_HS_IRQHandler,				/* USB OTG HS */
		DCMI_IRQHandler,				/* DCMI */
		CRYP_IRQHandler,				/* CRYP crypto */
		HASH_RNG_IRQHandler,			/* Hash and Rng */
		0,								
		0,								
		0,								
		(void*) VERSION_SW,				// [100]
		(void*) &_estack,				// [101] TODO JD : SW size
};

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
 */
void Reset_Handler(void) {

	unsigned long* pulSrc;
	unsigned long* pulDest;

	// Copy the data segment initializers from flash to SRAM.
	pulSrc = &_flash_data;
	for (pulDest = &_data; pulDest < &_edata;) {
		*(pulDest++) = *(pulSrc++);
	}

	// Zero fill the bss segment.
	for (pulDest = &_bss; pulDest < &_ebss;) {
		*(pulDest++) = 0;
	}

	// Call the application's entry point.
	main();
	while(1);
}

/*******************************************************************************
 *
 * Provide weak aliases for each Exception handler to the Default_Handler.
 * As they are weak aliases, any function with the same name will override
 * this definition.
 *
 *******************************************************************************/
#pragma weak NMI_Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler

#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak PVD_IRQHandler = Default_Handler
#pragma weak TAMP_STAMP_IRQHandler = Default_Handler
#pragma weak RTC_WKUP_IRQHandler = Default_Handler
#pragma weak FLASH_IRQHandler = Default_Handler
#pragma weak RCC_IRQHandler = Default_Handler
#pragma weak EXTI0_IRQHandler = Default_Handler
#pragma weak EXTI1_IRQHandler = Default_Handler
#pragma weak EXTI2_IRQHandler = Default_Handler
#pragma weak EXTI3_IRQHandler = Default_Handler
#pragma weak EXTI4_IRQHandler = Default_Handler
#pragma weak DMA1_Stream0_IRQHandler = Default_Handler
#pragma weak DMA1_Stream1_IRQHandler = Default_Handler
#pragma weak DMA1_Stream2_IRQHandler = Default_Handler
#pragma weak DMA1_Stream3_IRQHandler = Default_Handler
#pragma weak DMA1_Stream4_IRQHandler = Default_Handler
#pragma weak DMA1_Stream5_IRQHandler = Default_Handler
#pragma weak DMA1_Stream6_IRQHandler = Default_Handler
#pragma weak ADC_IRQHandler = Default_Handler
#pragma weak CAN1_TX_IRQHandler = Default_Handler
#pragma weak CAN1_RX0_IRQHandler = Default_Handler
#pragma weak CAN1_RX1_IRQHandler = Default_Handler
#pragma weak CAN1_SCE_IRQHandler = Default_Handler
#pragma weak EXTI9_5_IRQHandler = Default_Handler
#pragma weak TIM1_BRK_TIM9_IRQHandler = Default_Handler
#pragma weak TIM1_UP_TIM10_IRQHandler = Default_Handler
#pragma weak TIM1_TRG_COM_TIM11_IRQHandler = Default_Handler
#pragma weak TIM1_CC_IRQHandler = Default_Handler
#pragma weak TIM2_IRQHandler = Default_Handler
#pragma weak TIM3_IRQHandler = Default_Handler
#pragma weak TIM4_IRQHandler = Default_Handler
#pragma weak I2C1_EV_IRQHandler = Default_Handler
#pragma weak I2C1_ER_IRQHandler = Default_Handler
#pragma weak I2C2_EV_IRQHandler = Default_Handler
#pragma weak I2C2_ER_IRQHandler = Default_Handler
#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak USART1_IRQHandler = Default_Handler
#pragma weak USART2_IRQHandler = Default_Handler
#pragma weak USART3_IRQHandler = Default_Handler
#pragma weak EXTI15_10_IRQHandler = Default_Handler
#pragma weak RTC_Alarm_IRQHandler = Default_Handler
#pragma weak OTG_FS_WKUP_IRQHandler = Default_Handler
#pragma weak TIM8_BRK_TIM12_IRQHandler = Default_Handler
#pragma weak TIM8_UP_TIM13_IRQHandler = Default_Handler
#pragma weak TIM8_TRG_COM_TIM14_IRQHandler = Default_Handler
#pragma weak TIM8_CC_IRQHandler = Default_Handler
#pragma weak DMA1_Stream7_IRQHandler = Default_Handler
#pragma weak FSMC_IRQHandler = Default_Handler
#pragma weak SDIO_IRQHandler = Default_Handler
#pragma weak TIM5_IRQHandler = Default_Handler
#pragma weak SPI3_IRQHandler = Default_Handler
#pragma weak UART4_IRQHandler = Default_Handler
#pragma weak UART5_IRQHandler = Default_Handler
#pragma weak TIM6_IRQHandler = Default_Handler
#pragma weak TIM7_IRQHandler = Default_Handler
#pragma weak DMA2_Stream0_IRQHandler = Default_Handler
#pragma weak DMA2_Stream1_IRQHandler = Default_Handler
#pragma weak DMA2_Stream2_IRQHandler = Default_Handler
#pragma weak DMA2_Stream3_IRQHandler = Default_Handler
#pragma weak DMA2_Stream4_IRQHandler = Default_Handler
#pragma weak ETH_IRQHandler = Default_Handler
#pragma weak ETH_WKUP_IRQHandler = Default_Handler
#pragma weak CAN2_TX_IRQHandler = Default_Handler
#pragma weak CAN2_RX0_IRQHandler = Default_Handler
#pragma weak CAN2_RX1_IRQHandler = Default_Handler
#pragma weak CAN2_SCE_IRQHandler = Default_Handler
#pragma weak OTG_FS_IRQHandler = Default_Handler
#pragma weak DMA2_Stream5_IRQHandler = Default_Handler
#pragma weak DMA2_Stream6_IRQHandler = Default_Handler
#pragma weak DMA2_Stream7_IRQHandler = Default_Handler
#pragma weak USART6_IRQHandler = Default_Handler
#pragma weak I2C3_EV_IRQHandler = Default_Handler
#pragma weak I2C3_ER_IRQHandler = Default_Handler
#pragma weak OTG_HS_EP1_OUT_IRQHandler = Default_Handler
#pragma weak OTG_HS_EP1_IN_IRQHandler = Default_Handler
#pragma weak OTG_HS_WKUP_IRQHandler = Default_Handler
#pragma weak OTG_HS_IRQHandler = Default_Handler
#pragma weak DCMI_IRQHandler = Default_Handler
#pragma weak CRYP_IRQHandler = Default_Handler
#pragma weak HASH_RNG_IRQHandler = Default_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
 */

void Default_Handler(void) {
	/* Go into an infinite loop. */
	while (1) {
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


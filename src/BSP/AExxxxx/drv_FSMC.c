/**
 *******************************************************************************
 * @file	drv_FSMC.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	11 avr. 2013
 * @brief   Gestion du controleur de memoire externe.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_FSMC.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation du controleur de memoire externe.
 */
void vFSMC_Init() {

	FSMC_NORSRAMInitTypeDef  xFSMC_NORSRAMInit;
	FSMC_NORSRAMTimingInitTypeDef  xFSMC_NORSRAMTimingInit;

	// Initialisation de la structure de configuration
	xFSMC_NORSRAMInit.FSMC_ReadWriteTimingStruct		= &xFSMC_NORSRAMTimingInit;
	xFSMC_NORSRAMInit.FSMC_WriteTimingStruct			= &xFSMC_NORSRAMTimingInit;
	FSMC_NORSRAMStructInit(&xFSMC_NORSRAMInit);

	// Activation horloge
	RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_FSMC, DISABLE);
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);


	//----------------------------------
	// Configuration du controleur SSD1906

	// Config Timing
	xFSMC_NORSRAMTimingInit.FSMC_AddressSetupTime		= 0;
	xFSMC_NORSRAMTimingInit.FSMC_AddressHoldTime		= 0;
	xFSMC_NORSRAMTimingInit.FSMC_DataSetupTime			= 60;
	xFSMC_NORSRAMTimingInit.FSMC_AccessMode				= FSMC_AccessMode_A;

	// Config Bank
	//	- Data/Address MUX = Disable
	//	- Memory Type = SRAM
	//	- Data Width = 16bit
	//	- Write Operation = Enable
	//	- Extended Mode = Disable
	//	- Asynchronous Wait = Enable
	xFSMC_NORSRAMInit.FSMC_DataAddressMux				= FSMC_DataAddressMux_Disable;
	xFSMC_NORSRAMInit.FSMC_MemoryType					= FSMC_MemoryType_SRAM;
	xFSMC_NORSRAMInit.FSMC_MemoryDataWidth				= FSMC_MemoryDataWidth_16b;
	xFSMC_NORSRAMInit.FSMC_BurstAccessMode				= FSMC_BurstAccessMode_Disable;
	xFSMC_NORSRAMInit.FSMC_WaitSignalPolarity			= FSMC_WaitSignalPolarity_Low;
	xFSMC_NORSRAMInit.FSMC_WrapMode						= FSMC_WrapMode_Disable;
	xFSMC_NORSRAMInit.FSMC_WaitSignalActive				= FSMC_WaitSignalActive_BeforeWaitState;
	xFSMC_NORSRAMInit.FSMC_WriteOperation				= FSMC_WriteOperation_Enable;
	xFSMC_NORSRAMInit.FSMC_WaitSignal					= FSMC_WaitSignal_Enable;
	xFSMC_NORSRAMInit.FSMC_ExtendedMode					= FSMC_ExtendedMode_Disable;
	xFSMC_NORSRAMInit.FSMC_WriteBurst					= FSMC_WriteBurst_Disable;

	// Variable non initialisee sur la version precedante, creant une instabilite sur le bus FSMC.
	xFSMC_NORSRAMInit.FSMC_AsynchronousWait				= FSMC_AsynchronousWait_Disable;

	// Config Bank 1 : Buffer d'affichage
	xFSMC_NORSRAMInit.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInit(&xFSMC_NORSRAMInit);			// Initialisation des registres FSMC
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);	// Activation FSMC Bank1_NOR Bank

	// Config de la Bank 2 pour les registres
	xFSMC_NORSRAMInit.FSMC_Bank = FSMC_Bank1_NORSRAM2;
	FSMC_NORSRAMInit(&xFSMC_NORSRAMInit);			// Initialisation des registres FSMC
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);	// Activation FSMC Bank1_NOR Bank

}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation du controleur de memoire externe.
 */
void vFSMC_DeInit() {

	RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_FSMC, ENABLE);
	FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM1);
	FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM2);

}


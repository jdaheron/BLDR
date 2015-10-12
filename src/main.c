/**
 ***************************************************************************************************
 * @file	main.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	20 juin 2014
 * @brief   Fichier principal.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "fct_MemoireFAT.h"
#include "drivers/drv_SdCard_SPI.h"

#include "equ_Hygrometre.h"
#include "equ_ThermocoupleK.h"

#include "util_TSW.h"
#include "util_printf.h"
#include "util_Goto.h"
#include "util_LogFile.h"

#include "main.h"
#include "stm32f2x7_eth.h"
#include "netconf.h"
#include "httpd.h"
#include "telnet.h"
#include "tftpserver.h"

#include "app_MAJ.h"

#include "mod_Demarrage.h"
#include "mod_MAJ.h"

#include "fct_MemoireFAT.h"
#include "drv_SdCard_SPI.h"

#include "fct_SwFlash.h"
#include "fct_MemoireFAT.h"

#include "drv_SdCard_SPI.h"

#include "util_TSW.h"
#include "util_printf.h"
#include "util_Goto.h"


/* External Variables *****************************************************************************/

const char VERSION_SW[] = {"00002AA"};
// Definition de l'offset d'execution en fonction de l'option de compilation
// Modifier aussi le script du linker...
const uint32_t OFFSET = 0x00000000;


/** @addtogroup main
  * @{
  */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

typedef enum
{
	MODE_TEST = 0,
	MODE_BOOTLOADER,
	MODE_RESET,
	MODE_NORMAL,
	MODE_MAJ,
	MODE_ARRET,
	MODE_ATTENTE,
	MODE_DEMARRAGE,

} Mode_e;


/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */
 
#define MAIN_CONSOLE_ENABLE		1


/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */

#if MAIN_CONSOLE_ENABLE
	#define _MAIN_CONSOLE	_printf
#else
	#define _MAIN_CONSOLE	((void*) 0)
#endif



/**
 * @}
 */ 

  
 /** 
 ***************************************************************************************************
 * @defgroup Private_FunctionPrototypes Private FunctionPrototypes
 * @{
 */


/**
 * @}
 */ 
 
 
  /** 
 ***************************************************************************************************
 * @defgroup Private_Variables Private Variables
 * @{
 */

static Mode_e Mode = MODE_MAJ;
static Mode_e LastMode = 0xFF;
static MajInfo_s MajInfo;


/**
 * @}
 */ 
  	
	
/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 
 

/**
 * @brief	Gestion de la Led de LifeBit
 * @return 	void.
 */
void LifeBit_Main()
{
	TSW Tmr_LifeBit;
	static Etat_e Etat = Etat_INACTIF;
	static uint8_t Etape = 0;

	if (TSW_IsRunning(&Tmr_LifeBit) != TRUE)
	{
		switch (Etape)
		{
			case 0:
				Etat = Etat_ACTIF;
				TSW_Start(&Tmr_LifeBit, 100);
				Etape++;
				break;
			case 1:
				Etat = Etat_INACTIF;
				TSW_Start(&Tmr_LifeBit, 100);
				Etape++;
				break;
			case 2:
				Etat = Etat_ACTIF;
				TSW_Start(&Tmr_LifeBit, 100);
				Etape++;
				break;
			case 3:
				Etat = Etat_INACTIF;
				TSW_Start(&Tmr_LifeBit, 700);
				Etape = 0;
				break;

		}

		GPIO_Set(PORT_STAT1, Etat);
	}
}


/**
 * @}
 */ 

/**
 ***************************************************************************************************
 *
 */
int main(void)
{
	Bool_e NouveauMode;

	// ------------------------------------------------------------------------
	// --- INIT DU SYSTEME
	// --- Initialisations uC, Peripheriques, Fonctions et Interfaces
	BSP_Init();									// Init carte
	SysTick_Config(SystemCoreClock / 1000);		// Init Tick 1ms
	HAL_Console_Init(115200);
	RTC_StartInit(TRUE);						// Start Init RTC

	_printf("\n--- START BL - ALJ");
	_printf(VERSION_SW);
	_printf(" ---\n");

	//----------------------------------
	// FONCTIONNALITES
	MemoireFAT_Init((Diskio_drvTypeDef*) &SdCard_SPI_Driver);

	/* configure ethernet (GPIOs, clocks, MAC, DMA) */
	ETH_BSP_Config();

	/* Init LwIP stack */
	LwIP_Init();

	/* Init services */
	tftpd_init();
	telnet_init();

	_printf("--- StartupTime=%dms ---\n\n", TSW_GetTimestamp_ms());
	while(1)
	{
		/* check if any packet received */
		if (ETH_CheckFrameReceived())
		{
			/* process received ethernet packet */
			LwIP_Pkt_Handle();
		}

		/* handle periodic timers for LwIP */
		LwIP_Periodic_Handle(TSW_GetTimestamp_ms());

		//----------------------------------
		// PROCESSUS
		LifeBit_Main();
		MemoireFAT_Main();

		//----------------------------------
		// DETECTION CHG MODE
		if (LastMode != Mode)
		{
			LastMode = Mode;
			NouveauMode = TRUE;
		}
		else
			NouveauMode = FALSE;

		//----------------------------------
		// GESTION DES MODES
		switch (Mode)
		{
			//--------------------------------------------------------------
			// MODE MAJ
			case MODE_MAJ:

				switch (MAJ(&MajInfo))
				{
					// Fin de maj Master
					case Status_Fini:
						Mode = MODE_RESET;
						break;

					// Demarrage Master possible sans MAJ
					case Status_OK:
						Mode = MODE_DEMARRAGE;
						break;

					// Erreur de MAJ
					case Status_KO:
						_printf("Mode Attente\n");
						Mode = MODE_ATTENTE;
						break;
				}

				break;

			//--------------------------------------------------------------
			// MODE DEMARRAGE
			case MODE_DEMARRAGE:

				switch (Demarrage(SwFlash_GetAppAddress()))
				{
					case Status_OK:	// En fin de demarrage, l'applicatif doit etre lance
					case Status_KO:
						_printf("Mode Attente\n");
						Mode = MODE_ATTENTE;
						break;
				}

				break;

			//--------------------------------------------------------------
			// MODE BOOTLOADER
			case MODE_BOOTLOADER:

				_printf("Mode Bootloader\n");
				if (GOTO(0) == Status_KO)
				{
					_printf("ECHEC\n");
					Mode = MODE_ATTENTE;
				}
				break;

			//--------------------------------------------------------------
			// MODE RESET
			case MODE_RESET:

				_printf("Mode Reset\n");
				if (GOTO(0) == Status_KO)
				{
					_printf("ECHEC\n");
					Mode = MODE_ATTENTE;
				}
				break;

			//--------------------------------------------------------------
			// MODE NON UTILISE PAR L'APPLICATIF
			default:
			case MODE_TEST:
			case MODE_ATTENTE:
				break;

		}
	}

	return 0;

}


/**
 ***************************************************************************************************
 * Validation OK - JD le 08/09/2014
 */

extern void SdCard_SPI_timerproc (void);

void ApplicationTickHook (void) {

	static uint8_t ucTick_10ms=0;
	static uint8_t ucTick_100ms=0;


	/* Gestion du Tick Timer Software */
	TSW_Refresh();

	/* Tick 10ms */
	ucTick_10ms++;
	if (ucTick_10ms >= 10){
		ucTick_10ms = 0;
		ADC1_Tick();
		SdCard_SPI_timerproc();
	}

	/* Tick 100ms */
	ucTick_100ms++;
	if (ucTick_100ms >= 10){
		ucTick_100ms = 0;

	}
}


/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  uint32_t timingdelay = TSW_GetTimestamp_ms() + nCount;

  /* wait until the desired delay finish */
  while(timingdelay > TSW_GetTimestamp_ms())
  {
  }
}


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

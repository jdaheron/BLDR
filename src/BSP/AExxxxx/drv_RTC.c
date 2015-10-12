/**
 ***************************************************************************************************
 * @file	drv_RTC.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	30 juil. 2014
 * @brief   Gestion de la RTC interne.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "drv_RTC.h"
#include "util_printf.h"	// TODO : Debug


/* External Variables *****************************************************************************/


/** @addtogroup drv_RTC
  * @{
  */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */


/** Liste des etapes d'init de la RTC. */
typedef enum
{
	RTC_ETAPE_INIT	 = 0,
	RTC_ETAPE_ATTENTE_STABILISATION,
	RTC_ETAPE_ENABLE_RTC_CLOCK,
	RTC_ETAPE_ATTENTE_SYNCHRONISATION,
	RTC_ETAPE_READY

} RTC_Etape_e;


/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */
 
/**
 * @}
 */ 

 
 /** 
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */
  
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
 
static Bool_e		QuartzPresent	= FALSE;
static RTC_Etape_e	Etape			= RTC_ETAPE_INIT;

/**
 * @}
 */ 
  	
	
/** 
 ***************************************************************************************************
 * @defgroup Private_Functions Private Functions
 * @{
 */ 
 
 /**
 * @}
 */ 
 

/** 
 ***************************************************************************************************
 * @todo Validation
 */
void
RTC_StartInit(
		Bool_e IsQuartzPresent		/**<[in] Flag de presence du quartz externe.*/
)
{
	// Enable the Power Controller (PWR) APB1 interface clock
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	Etape = RTC_ETAPE_INIT;
	QuartzPresent = IsQuartzPresent;

	RTC_Main();
}

/** 
 ***************************************************************************************************
 * @todo Validation
 */
void
RTC_Main(
		void
)
{
	switch (Etape)
	{
		case RTC_ETAPE_INIT:
			//_printf("RTC INIT\n");

			PWR_BackupAccessCmd(ENABLE);

			// Select the RTC clock source
			if (QuartzPresent)
			{
				RCC_LSEConfig(RCC_LSE_ON);						// Enable LSE clock
				Etape = RTC_ETAPE_ATTENTE_STABILISATION;
			}
			else
			{
				RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div31);	// Select HSE as RTC Clock Source
				Etape = RTC_ETAPE_ENABLE_RTC_CLOCK;
			}
			//break;

		case RTC_ETAPE_ATTENTE_STABILISATION:

			if (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	// Wait till LSE is ready
				break;

			//_printf("RTC WAIT STAB OK\n");

			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				// Select LSE as RTC Clock Source
			Etape = RTC_ETAPE_ENABLE_RTC_CLOCK;
			//break;

		case RTC_ETAPE_ENABLE_RTC_CLOCK:

			//_printf("RTC_Main: RTC_ETAPE_ENABLE_RTC_CLOCK\n");

			RCC_RTCCLKCmd(ENABLE);		// Enable RTC Clock
			Etape = RTC_ETAPE_ATTENTE_SYNCHRONISATION;
			//_printf("RTC WAIT SYNC\n");

			//break;

		case RTC_ETAPE_ATTENTE_SYNCHRONISATION:

			// Wait for RTC registers synchronization
			RTC_WaitForSynchro();

			RTC_InitTypeDef RTC_InitStrture;
			RTC_StructInit(&RTC_InitStrture);

			RTC_InitStrture.RTC_AsynchPrediv = 0x7F;
			RTC_InitStrture.RTC_SynchPrediv = 0xFF;
			RTC_InitStrture.RTC_HourFormat = RTC_HourFormat_24;
			RTC_Init(&RTC_InitStrture);

			Etape = RTC_ETAPE_READY;

			//_printf("RTC READY\n");

			break;

		case RTC_ETAPE_READY:

			break;
	}

}

/**
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
RTC_Lire(
		Horodatage_s* pHorodatage
)
{
	RTC_DateTypeDef date_curr;
	RTC_TimeTypeDef time_curr;
	uint8_t mode;

	// Verification RTC prete
	if (Etape != RTC_ETAPE_READY)
		return FALSE;

	RTC_GetTime(RTC_Format_BIN, &time_curr);
	RTC_GetDate(RTC_Format_BIN, &date_curr);

	mode = time_curr.RTC_H12;

	pHorodatage->Annee				= date_curr.RTC_Year + 2000;
	pHorodatage->Mois				= date_curr.RTC_Month;
	pHorodatage->Jour				= date_curr.RTC_Date;
	pHorodatage->Heure				= time_curr.RTC_Hours;
	pHorodatage->Minute				= time_curr.RTC_Minutes;
	pHorodatage->Seconde			= time_curr.RTC_Seconds;
	if (time_curr.RTC_H12 == RTC_H12_AM)
		pHorodatage->Mode24H 		= TRUE;
	else
		pHorodatage->Mode24H 		= FALSE;
	pHorodatage->JourSemaine		= date_curr.RTC_WeekDay;

	return TRUE;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
RTC_ReglerDateHeure(
		JourSemaine_e	JourSemaine,
		uint8_t			Jour,
		Mois_e			Mois,
		uint16_t		Annee,
		uint8_t			Heure,
		uint8_t			Minute,
		uint8_t			Seconde,
		Bool_e			Mode24H
)
{
	Horodatage_s Horodatage = {
			.JourSemaine	= JourSemaine,
			.Jour			= Jour,
			.Mois			= Mois,
			.Annee			= Annee,
			.Heure			= Heure,
			.Minute			= Minute,
			.Seconde		= Seconde,
			.Mode24H		= Mode24H,
	};

	return RTC_Regler(&Horodatage);
}



/**
 ***************************************************************************************************
 * @todo Validation
 */
Bool_e
RTC_Regler(
		Horodatage_s* pHorodatage
)
{
	RTC_DateTypeDef date_curr;
	RTC_TimeTypeDef time_curr;
	ErrorStatus eErr;
	uint8_t mode;

	// Verification RTC prete
	if (Etape != RTC_ETAPE_READY)
		return FALSE;

	if (pHorodatage->Mode24H == TRUE)
		mode = RTC_H12_AM;
	else
		mode = RTC_H12_PM;

	if (pHorodatage->Mode24H == RTC_HourFormat_12)
	{
		if (IS_RTC_HOUR12(pHorodatage->Heure) == FALSE)
			return FALSE;
	}
	else
	{
		if (IS_RTC_HOUR24(pHorodatage->Heure) == FALSE)
			return FALSE;
	}

	if (IS_RTC_MINUTES(pHorodatage->Minute) == FALSE)
		return FALSE;

	if (IS_RTC_SECONDS(pHorodatage->Seconde) == FALSE)
		return FALSE;

	if (IS_RTC_YEAR(pHorodatage->Annee - 2000) == FALSE)
		return FALSE;

	if (IS_RTC_MONTH(pHorodatage->Mois) == FALSE)
		return FALSE;

	if (IS_RTC_DATE(pHorodatage->Jour) == FALSE)
		return FALSE;

	if (IS_RTC_WEEKDAY(pHorodatage->JourSemaine) == FALSE)
		return FALSE;

	date_curr.RTC_Date		= pHorodatage->Jour;
	date_curr.RTC_Year		= pHorodatage->Annee - 2000;
	date_curr.RTC_Month		= pHorodatage->Mois;
	date_curr.RTC_WeekDay	= pHorodatage->JourSemaine;
	time_curr.RTC_H12		= mode;
	time_curr.RTC_Hours		= pHorodatage->Heure;
	time_curr.RTC_Minutes	= pHorodatage->Minute;
	time_curr.RTC_Seconds	= pHorodatage->Seconde;

	if (RTC_SetTime(RTC_Format_BIN, &time_curr) == ERROR)
		return FALSE;

	if (RTC_SetDate(RTC_Format_BIN, &date_curr) == ERROR)
		return FALSE;

	return TRUE;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
uint32_t RTC_GetTimestampFromHorodatage(
		Horodatage_s* pHorodatage
)
{
	uint32_t TS = 0;
	uint32_t NbAnnees;
	uint32_t NbJours;

	const uint32_t JoursDesMoisPrecedents[12] = {
			0,										// Janvier
			31,										// Fevrier
			31+28,									// Mars
			31+28+31,								// Avril
			31+28+31+30,							// Mai
			31+28+31+30+31,							// Juin
			31+28+31+30+31+30,						// Juillet
			31+28+31+30+31+30+31,					// Aout
			31+28+31+30+31+30+31+31,				// Septembre
			31+28+31+30+31+30+31+31+30,				// Octobre
			31+28+31+30+31+30+31+31+30+31,			// Novembre
			31+28+31+30+31+30+31+31+30+31+30,		// Decembre
	};

	if (pHorodatage->Annee < 1970)
		return 0;

	NbAnnees = pHorodatage->Annee - 1970;

	NbJours  = NbAnnees * 365;
	NbJours += (NbAnnees-2) / 4;	// Annees bisextiles (commencant en 1972)
	NbJours += JoursDesMoisPrecedents[pHorodatage->Mois-1];
	NbJours += pHorodatage->Jour;

	TS  = NbJours * 24 * 3600;
	TS += (pHorodatage->Heure-2) * 3600;	// Heure d'ete (UTC+2)
	//TS += (pHorodatage->Heure-1) * 3600;	// Heure d'hiver (UTC+1)
	TS += pHorodatage->Minute * 60;
	TS += pHorodatage->Seconde;

	return TS;
}



/**
 ***************************************************************************************************
 * @todo Validation
 */
Horodatage_s RTC_GetHorodatageFromTimestamp(
		uint32_t Timestamp
)
{
	Horodatage_s Horodatage;

	//TODO JD...implementation

	return Horodatage;
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
uint32_t RTC_GetTimestamp(
		void
)
{
	Horodatage_s Horodatage;

	RTC_Lire(&Horodatage);
	return RTC_GetTimestampFromHorodatage(&Horodatage);
}






/**
 ***************************************************************************************************
 * @todo Validation
 */
void
RTC_BkpRegister_Write(
		uint32_t NumRegistre,
		int32_t Data
)
{
	if (NumRegistre > 19)	return;

	PWR_BackupAccessCmd(ENABLE);
	RTC_WriteBackupRegister(NumRegistre, Data);
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
uint32_t
RTC_BkpRegister_Read(
		uint32_t NumRegistre
)
{
	if (NumRegistre > 19)	return 0;

	PWR_BackupAccessCmd(ENABLE);
	return RTC_ReadBackupRegister(NumRegistre);
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
void RTC_WKUP_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}


/**
 ***************************************************************************************************
 * @todo Validation
 */
void RTC_Alarm_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}


 /**
 * @}
 */ 

/* End Of File ************************************************************************************/

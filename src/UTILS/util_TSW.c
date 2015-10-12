/**
 ***************************************************************************************************
 * @file	util_TSW.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	02 juil. 2014
 * @brief	Gestion des Timers logiciels.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "util_TSW.h"
#include "util_printf.h"


/* External Variables *****************************************************************************/


/** @addtogroup util_TSW
  * @{
  */


/**
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */

#define TICK_INTERVAL_ms	(uint32_t) 1


/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Macros Private Macros
 * @{
 */

#define TSW_CONSOLE_ENABLE	0

#if TSW_CONSOLE_ENABLE
	#define _TSW_CONSOLE	_printf
#else
	#define _TSW_CONSOLE	__NOP();
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
 *
 * @}
 */


  /**
 ***************************************************************************************************
 * @defgroup Private_Variables Private Variables
 * @{
 */

static volatile uint32_t	TS_ms = 0;
static TSW_s*				pFirstTSW = NULL;
static uint32_t				MeasureTime = 0;

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


/**-----------------------------------------------------------------------------
 */
#if 0

static TSW_s Tmr0;
static TSW_s Tmr1;
static TSW_s Tmr2;

void TSW_VALIDATION()
{
	TSW_Start(&Tmr0, 1000);
	TSW_Start(&Tmr1, 3000);
	TSW_Start(&Tmr2, 5000);
	TSW_PrintList();

	TSW_Start(&Tmr0, 1000);
	TSW_Start(&Tmr1, 5000);
	TSW_Start(&Tmr2, 3000);
	TSW_PrintList();

	TSW_Start(&Tmr0, 3000);
	TSW_Start(&Tmr1, 1000);
	TSW_Start(&Tmr2, 5000);
	TSW_PrintList();

	TSW_Start(&Tmr0, 3000);
	TSW_Start(&Tmr1, 5000);
	TSW_Start(&Tmr2, 1000);
	TSW_PrintList();

	TSW_Start(&Tmr0, 5000);
	TSW_Start(&Tmr1, 1000);
	TSW_Start(&Tmr2, 3000);
	TSW_PrintList();

	TSW_Start(&Tmr0, 10000);
	TSW_Start(&Tmr2, 1000);
	TSW_PrintList();


	int etape=0;
	_printf("\n");
	while(1)
	{
		if(TSW_IsFinished(&Tmr0) == TRUE) {
			 TSW_Stop(&Tmr0);
			 _printf("Tmr0 End");
		}

		switch (etape)
		{
			case 0:
				TSW_Start(&Tmr1, 2000);
				etape++;
				break;

			case 1:
				if (TSW_IsRunning(&Tmr1))
					break;

				 _printf("TSW_GetStatus(&Tmr0) = %d\n",			TSW_GetStatus(&Tmr0));
				 _printf("TSW_IsFinished(&Tmr0) = %d\n",		TSW_IsFinished(&Tmr0));
				 _printf("TSW_IsRunning(&Tmr0) = %d\n", 		TSW_IsRunning(&Tmr0));
				 _printf("TSW_GetElapsedTime(&Tmr0) = %d\n",	TSW_GetElapsedTime(&Tmr0));
				 _printf("TSW_GetRemainingTime(&Tmr0) = %d\n",	TSW_GetRemainingTime(&Tmr0));
				 _printf("\n");

				TSW_Start(&Tmr1, 1000);
				etape++;
				break;

			case 2:
				if (TSW_IsRunning(&Tmr1))
					break;

				_printf("TSW_GetStatus(&Tmr0) = %d\n",			TSW_GetStatus(&Tmr0));
				_printf("TSW_IsFinished(&Tmr0) = %d\n",			TSW_IsFinished(&Tmr0));
				_printf("TSW_IsRunning(&Tmr0) = %d\n", 			TSW_IsRunning(&Tmr0));
				_printf("TSW_GetElapsedTime(&Tmr0) = %d\n",		TSW_GetElapsedTime(&Tmr0));
				_printf("TSW_GetRemainingTime(&Tmr0) = %d\n",	TSW_GetRemainingTime(&Tmr0));
				_printf("\n");

				_printf("TSW_Pause(&Tmr0)\n");
				TSW_Pause(&Tmr0);

				TSW_Start(&Tmr1, 1000);
				etape++;
				break;

			case 3:
				if (TSW_IsRunning(&Tmr1))
					break;

				_printf("TSW_GetStatus(&Tmr0) = %d\n",			TSW_GetStatus(&Tmr0));
				_printf("TSW_IsFinished(&Tmr0) = %d\n",			TSW_IsFinished(&Tmr0));
				_printf("TSW_IsRunning(&Tmr0) = %d\n", 			TSW_IsRunning(&Tmr0));
				_printf("TSW_GetElapsedTime(&Tmr0) = %d\n",		TSW_GetElapsedTime(&Tmr0));
				_printf("TSW_GetRemainingTime(&Tmr0) = %d\n",	TSW_GetRemainingTime(&Tmr0));
				_printf("\n");

				_printf("TSW_Resume(&Tmr0)\n");
				TSW_Resume(&Tmr0);

				TSW_Start(&Tmr1, 2000);
				etape++;
				break;

			case 4:
				if (TSW_IsRunning(&Tmr1))
					break;

				_printf("TSW_GetStatus(&Tmr0) = %d\n",			TSW_GetStatus(&Tmr0));
				_printf("TSW_IsFinished(&Tmr0) = %d\n",			TSW_IsFinished(&Tmr0));
				_printf("TSW_IsRunning(&Tmr0) = %d\n", 			TSW_IsRunning(&Tmr0));
				_printf("TSW_GetElapsedTime(&Tmr0) = %d\n",		TSW_GetElapsedTime(&Tmr0));
				_printf("TSW_GetRemainingTime(&Tmr0) = %d\n",	TSW_GetRemainingTime(&Tmr0));
				_printf("\n");

				etape++;
				break;

			default:
				break;
		}
	}
}
#endif

/**-----------------------------------------------------------------------------
 */
void Delete(TSW_s* pElement)
{
	TSW_s* pPrec;
	TSW_s* pCherche;

	__disable_irq();

	// Aucun element dans la liste
	if (pFirstTSW == NULL)
	{
		__enable_irq();
		return;
	}

	// Si l'element recherche est le premier de la liste
	if (pFirstTSW == pElement)
	{
		pFirstTSW = pFirstTSW->pNext;
		__enable_irq();
		return;
	}
	else
	{
		pCherche = pFirstTSW;
		pPrec = pFirstTSW;
		while ((pCherche->pNext != NULL) && (pCherche != pElement))
		{
			pPrec = pCherche;
			pCherche = pCherche->pNext;
		}

		// L'element a ete trouve
		if (pCherche == pElement)
			pPrec->pNext = pCherche->pNext;
	}

	__enable_irq();
}

/**-----------------------------------------------------------------------------
 */
void Insert(TSW_s* pElement)
{
	TSW_s* pPrec;
	TSW_s* pCherche;

	__disable_irq();

	// Preparation de l'element a inserer
	pElement->pNext = NULL;

	// Aucun element dans la liste
	if (pFirstTSW == NULL)
	{
		pFirstTSW = pElement;
		__enable_irq();
		return;
	}

	// Insersion en premier element
	if (pElement->TS_End < pFirstTSW->TS_End)
	{
		pElement->pNext = pFirstTSW;
		pFirstTSW = pElement;
		__enable_irq();
		return;
	}

	// Recherche de la position d'insersion
	pPrec = pFirstTSW;
	pCherche = pFirstTSW;
	while ((pCherche->pNext != NULL) && (pCherche->TS_End <= pElement->TS_End))
	{
		pPrec = pCherche;
		pCherche = pCherche->pNext;
	}

	// Insersion avant l'element pRecherche
	if (pCherche->TS_End > pElement->TS_End)
	{
		pElement->pNext = pCherche;
		pPrec->pNext = pElement;
	}
	// Insersion en fin de lise
	else
		pCherche->pNext = pElement;

	__enable_irq();
}

/**-----------------------------------------------------------------------------
 */
void TSW_Refresh()
{

	TS_ms += TICK_INTERVAL_ms;

	while ((pFirstTSW != NULL) && (pFirstTSW->TS_End <= TS_ms))
	{
		pFirstTSW->Status = TSW_FINISHED;
		Delete(pFirstTSW);
	}
}

/**-----------------------------------------------------------------------------
 */
void TSW_Delay(uint32_t Time_ms)
{
	uint32_t End;

	End = TS_ms + Time_ms;
	while (TS_ms < End)
		;
}

/**-----------------------------------------------------------------------------
 */
uint32_t TSW_GetTimestamp_ms()
{
	return TS_ms;
}

/**-----------------------------------------------------------------------------
 */
void TSW_Start(TSW_s* pTSW, uint32_t Time_ms)
{
	// Suppression du Timer si deja present
	Delete(pTSW);

	// Sauvegarde du TS de debut et calcul du TS de fin
	pTSW->TS_Start = TS_ms;
	pTSW->TS_Pause = TS_ms;
	pTSW->TS_End = TS_ms + Time_ms;

	// Ajout du timer dans la liste
	Insert(pTSW);
	pTSW->Status = TSW_RUNNING;
}

/**-----------------------------------------------------------------------------
 */
void TSW_ReStart(TSW_s* pTSW)
{
	uint32_t Time_ms;

	// Suppression du Timer si deja present
	Delete(pTSW);

	// Sauvegarde du TS de debut et calcul du TS de fin
	Time_ms = pTSW->TS_End - pTSW->TS_Start;
	pTSW->TS_Start = TS_ms;
	pTSW->TS_Pause = TS_ms;
	pTSW->TS_End = TS_ms + Time_ms;

	// Ajout du timer dans la liste
	Insert(pTSW);
	pTSW->Status = TSW_RUNNING;
}

/**-----------------------------------------------------------------------------
 */
void TSW_Stop(TSW_s* pTSW)
{
	if (pTSW->Status == TSW_STOPPED)
		return;

	Delete(pTSW);
	pTSW->Status = TSW_STOPPED;
}

/**-----------------------------------------------------------------------------
 */
void TSW_Pause(TSW_s* pTSW)
{
	if (pTSW->Status == TSW_PAUSED)
		return;

	Delete(pTSW);
	pTSW->TS_Pause = TS_ms;
	pTSW->Status = TSW_PAUSED;
}

/**-----------------------------------------------------------------------------
 */
void TSW_Resume(TSW_s* pTSW)
{
	if (pTSW->Status != TSW_PAUSED)
		return;

	// Recalcul du TS de debut et calcul du TS de fin
	pTSW->TS_Start = TS_ms - pTSW->TS_Pause + pTSW->TS_Start;
	pTSW->TS_End = TS_ms - pTSW->TS_Pause + pTSW->TS_End;

	// Ajout du timer dans la liste
	Insert(pTSW);
	pTSW->Status = TSW_RUNNING;
}

/**-----------------------------------------------------------------------------
 */
TSW_Status_e TSW_GetStatus(TSW_s* pTSW)
{
	return pTSW->Status;
}

/**-----------------------------------------------------------------------------
 */
Bool_e TSW_IsFinished(TSW_s* pTSW)
{
	if (pTSW->Status == TSW_FINISHED)
		return TRUE;

	return FALSE;
}

/**-----------------------------------------------------------------------------
 */
Bool_e TSW_IsRunning(TSW_s* pTSW)
{
	if (pTSW->Status == TSW_RUNNING)
		return TRUE;

	return FALSE;
}

/**-----------------------------------------------------------------------------
 */
uint32_t TSW_GetElapsedTime(TSW_s* pTSW)
{
	if (pTSW->TS_Start > TS_ms)
	{
		return 0;
	}
	else
	{
		if (pTSW->Status == TSW_PAUSED)
			return (pTSW->TS_Pause - pTSW->TS_Start);

		return (TS_ms - pTSW->TS_Start);
	}
}

/**-----------------------------------------------------------------------------
 */
uint32_t TSW_GetRemainingTime(TSW_s* pTSW)
{
	if (pTSW->TS_End < TS_ms)
	{
		return 0;
	}
	else
	{
		if (pTSW->Status == TSW_PAUSED)
			return (pTSW->TS_End - pTSW->TS_Pause);

		return (pTSW->TS_End - TS_ms);
	}
}

/**-----------------------------------------------------------------------------
 */
void TSW_MeasureTime_Start()
{
	MeasureTime = TS_ms;
}

/**-----------------------------------------------------------------------------
 */
uint32_t TSW_MeasureTime_Get()
{
	return (TS_ms - MeasureTime);
}

/**-----------------------------------------------------------------------------
 */
void TSW_PrintList()
{
	uint32_t i = 0;
	TSW_s* pCherche;

	_printf("Liste:\n");

	if (pFirstTSW == NULL)
	{
		_printf("  vide!\n");
		return;
	}

	pCherche = pFirstTSW;
	do
	{
		_printf("  %d:0x%08X Start=%08d End=%08d Status=%d\n", i, pCherche,
				pCherche->TS_Start, pCherche->TS_End, pCherche->Status);
		i++;
		pCherche = pCherche->pNext;
	}
	while (pCherche != NULL);
}

/**
 * @}
 */

/* End Of File ************************************************************************************/


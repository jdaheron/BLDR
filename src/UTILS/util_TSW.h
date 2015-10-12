/**
 ***************************************************************************************************
 * @file	util_TSW.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	02 juil. 2014
 * @brief	Gestion des Timers logiciels.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef UTIL_TSW_H_
#define UTIL_TSW_H_


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"


/** @defgroup util_TSW util_TSW
 * @brief 	Gestion des Timers logiciels.
 * @{
 */

/**
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */

 /**
 * @}
 */



/**
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */

/** Definition des etats de la FIFO. */
typedef enum{

	TSW_STOPPED = 0,
	TSW_FINISHED,
	TSW_PAUSED,
	TSW_RUNNING,

} TSW_Status_e;

/** Definition de l'objet TimerSW. */
typedef struct {

	uint32_t TS_Start;
	uint32_t TS_Pause;
	uint32_t TS_End;
	TSW_Status_e Status;
	void* pNext;

} TSW_s;


#define TSW static TSW_s

/**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Exported_Macros Exported Macros
 * @{
 */

 /**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Exported_Variables Exported Variables
 * @{
 */

 /**
 * @}
 */


/**
 ***************************************************************************************************
 * @defgroup Exported_Functions Exported Functions
 * @{
 */
void	TSW_VALIDATION();

void	TSW_Refresh();
void 	TSW_Delay(uint32_t Time_ms);
uint32_t TSW_GetTimestamp_ms();

void TSW_Start(TSW_s* pTSW, uint32_t Time_ms);
void TSW_ReStart(TSW_s* pTSW);
void TSW_Stop(TSW_s* pTSW);
void TSW_Pause(TSW_s* pTSW);
void TSW_Resume(TSW_s* pTSW);

TSW_Status_e TSW_GetStatus(TSW_s* pTSW);
Bool_e	TSW_IsFinished(TSW_s* pTSW);
Bool_e	TSW_IsRunning(TSW_s* pTSW);
uint32_t TSW_GetElapsedTime(TSW_s* pTSW);
uint32_t TSW_GetRemainingTime(TSW_s* pTSW);

void TSW_MeasureTime_Start();
uint32_t TSW_MeasureTime_Get();

void TSW_PrintList();
 /**
 * @}
 */


 /**
 * @}
 */

#endif /* UTIL_TSW_H_ */





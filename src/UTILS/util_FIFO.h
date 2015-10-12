/**
 *******************************************************************************
 * @file	util_FIFO.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 oct. 2013
 * @brief   Gestion de FIFO.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef UTIL_FIFO_H_
#define UTIL_FIFO_H_


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"



/* Exported types ------------------------------------------------------------*/

/** Definition des etats de la FIFO. */
typedef enum{

	FIFO_EMPTY = 0,		/// Fifo vide
	FIFO_LAST_DATA,		/// On vient de lire la dernier donnee disponible
	FIFO_DATA_DISPO,	/// Des donnees sont disponibles
	FIFO_FULL			/// Fifo pleine

} FifoState_e;

/** Definition de l'objet FIFO. */
typedef struct {

	volatile uint32_t IndexIn;
	volatile uint32_t IndexOut;
	uint32_t FifoSize;
	uint32_t DataSize;
	volatile uint8_t* pFifo;
	FifoState_e State;

} Fifo_t;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void FIFO_Init(Fifo_t* pFIFO, void* pBuffer, uint32_t FifoSize, uint32_t DataSize);
void FIFO_Clear(Fifo_t* pFIFO);
FifoState_e FIFO_Push(Fifo_t* pFIFO, ...);
FifoState_e FIFO_Pull(Fifo_t* pFIFO, void* pData);

#endif /* UTIL_FIFO_H_ */





/**
 *******************************************************************************
 * @file	util_FIFO.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	17 oct. 2013
 * @brief   Gestion de FIFO.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "util_FIFO.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**-----------------------------------------------------------------------------
 * @brief	Initialisation d'une FIFO.
 *
 * @param[in]	pFIFO		Pointeur vers la fifo
 * @param[in]	pBuffer		Pointeur vers le buffer
 * @param[in]	FifoSize	Nombre d'elements de la fifo
 * @param[in]	DataSize	Taille d'un element, en octet
 *
 */
void FIFO_Init(Fifo_t* pFIFO, void* pBuffer, uint32_t FifoSize, uint32_t DataSize) {

	if (pFIFO == NULL) return;
	if (pBuffer == NULL) return;

	pFIFO->IndexIn	= 0;
	pFIFO->IndexOut	= 0;
	pFIFO->FifoSize	= FifoSize;
	pFIFO->DataSize	= DataSize;
	pFIFO->pFifo	= pBuffer;
	pFIFO->State	= FIFO_EMPTY;
}

/*
//TODO : Creation dynamique ?
void FIFO_Create(Fifo_t* pFIFO, uint32_t FifoSize, uint32_t DataSize) {
}
void FIFO_Delete(Fifo_t* pFIFO) {
}
*/


/**-----------------------------------------------------------------------------
 * @brief	Effacement d'une fifo.
 *
 * @param[in]	pFIFO	Pointeur vers la fifo
 */
void FIFO_Clear(Fifo_t* pFIFO) {

	pFIFO->IndexIn	= 0;
	pFIFO->IndexOut	= 0;
	pFIFO->State	= FIFO_EMPTY;

}


/**-----------------------------------------------------------------------------
 * @brief	Ajout d'une données en Fifo.
 *
 * @param[in]	pFIFO	Pointeur vers la fifo
 * @param[in]	...		Donnee a ajouter
 *
 * @return		FifoState_e
 */
FifoState_e FIFO_Push(Fifo_t* pFIFO, ...) {

	uint32_t i;

	// Récupération du pointeur vers la donnee a empiler
	register int* varg = (int*) (&pFIFO+1);
	uint8_t* pData = (uint8_t*) (varg);

	// Verification place disponible
	if (pFIFO->State == FIFO_FULL)
		return FIFO_FULL;


	// Ajout de la donnee en fifo
	for (i=0; i<pFIFO->DataSize; i++)
	{
		pFIFO->pFifo[pFIFO->IndexIn++] = (uint8_t) *((uint8_t*)pData + i);
	}

	// Incrémentation de l'index d'entree
	if (pFIFO->IndexIn >= pFIFO->FifoSize * pFIFO->DataSize)
		pFIFO->IndexIn = 0;

	// MAJ Statut
	if (pFIFO->IndexIn == pFIFO->IndexOut)
	{
		pFIFO->State = FIFO_FULL;
		return FIFO_LAST_DATA;
	}
	else
		pFIFO->State = FIFO_DATA_DISPO;

	return pFIFO->State;
}


/**-----------------------------------------------------------------------------
 * @brief	Retrait d'une donnée en Fifo.
 *
 * @param[in]	pFIFO	Pointeur vers la fifo
 * @param[in]	pData	Pointeur vers la Donnee retiree
 *
 * @return	FifoState_e
 */
FifoState_e FIFO_Pull(Fifo_t* pFIFO, void* pData) {

	uint32_t i;

	// Verification donnee disponible
	if (pFIFO->State == FIFO_EMPTY)
		return FIFO_EMPTY;

	// Retrait de la donnee en fifo
	for (i=0; i<pFIFO->DataSize; i++)
	{
		 *((uint8_t*)pData + i) = pFIFO->pFifo[pFIFO->IndexOut++];
	}

	// Incrémentation de l'index de sortie
	if (pFIFO->IndexOut >= pFIFO->FifoSize * pFIFO->DataSize)
		pFIFO->IndexOut = 0;

	// MAJ Statut
	if (pFIFO->IndexIn == pFIFO->IndexOut)
	{
		pFIFO->State = FIFO_EMPTY;
		return FIFO_LAST_DATA;
	}
	else
		pFIFO->State = FIFO_DATA_DISPO;

	return pFIFO->State;
}



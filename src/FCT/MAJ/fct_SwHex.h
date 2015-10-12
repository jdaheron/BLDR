/**
 *******************************************************************************
 * @file	fct_SwHex
 * @author	j.daheron
 * @version	1.0.0
 * @date	12 fev. 2014
 * @brief   Gestion des Logiciels Intel Hex (Fichier et Package).
 *******************************************************************************
 */



/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef FCT_SW_HEX_H
#define FCT_SW_HEX_H


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"
#include "ff.h"

#include "fct_ihex.h"

#define NB_SW_MAX			8
#define TAILLE_VERSION_MAX	16


/* Exported types ------------------------------------------------------------*/


typedef struct {

	uint32_t StartAddress;		// Adresse de debut
	uint32_t Size;				// Taille des donnees
	uint32_t Checksum;			// Checksum de la plage de donnees

} SwData_t;

typedef struct {

	char	 	Version[TAILLE_VERSION_MAX];
	uint32_t	BufferOffset;	// Position de debut dans le fichier
	uint32_t	HexSize;		// Taille du Hex (SW_Info et SW_Checksum inclus)
	uint32_t	Checksum;		// Checksum du Hex (SW_Info inclus)
	uint8_t		IsValide;		// Validite du Hex

	SwData_t	Data;			// Structure de definition des donnees

} Sw_s;

typedef struct {

	char		FileName[_MAX_LFN];
	char		Version[TAILLE_VERSION_MAX];
	uint32_t	HeaderChecksum;
	uint8_t		HeaderIsValide;
	uint32_t	Checksum;
	uint8_t		IsValide;
	uint8_t		NbSw;
	Sw_s		Sw[NB_SW_MAX];

} Swp_s;


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void VALIDATION_SEQUENCE_VALIDATION_FCT_SW_HEX();
void FONCTION_VALIDATION_READ_SW_INFO_AND_CHECK_SWP( void );

Status_e SwHex_ReadSwInfoAndCheckSwp(char* FileName, Swp_s* pSwp);

#if 0 // XXX __FONCTION_OBSOLETE__
	Status_e SwHex_CheckSwpChecksum(char* FileName);
	uint8_t SwHex_ReadSwInfo(FIL *pFile, Sw_s* pSw);
#endif

uint8_t SwHex_ReadSwpInfo(FIL *pFile, Swp_s* pSwp);
Status_e SwHex_UpdateSwInfo(const IHexRecord* pRecord, Sw_s* pSw);
void SwHex_PrintSwpInfo( Swp_s* pSwp );

#endif /* FCT_SW_HEX_H */

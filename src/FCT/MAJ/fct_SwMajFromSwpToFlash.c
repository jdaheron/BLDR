/**
 ***************************************************************************************************
 * @file	fct_SwMajFromSwpToFlash.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	12 sept. 2014
 * @brief   Gestion de la MAJ d'une carte depuis un SWP vers la flash interne.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "fct_SwMajFromSwpToFlash.h"
#include "fct_SwFlash.h"
#include "fct_SwHex.h"
#include "fct_ihex.h"
#include "fct_BL.h"

#include "util_printf.h"
#include "util_TSW.h"


/* External Variables *****************************************************************************/


/** @addtogroup fct_SwMajFromSwpToFlash
  * @{
  */


 /**
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

#define NB_SECTORS_MAX	16

typedef enum {

	MAJ_UpdateSW_Init = 0,
	MAJ_UpdateSW_GetID,
	MAJ_UpdateSW_OpenFile,
	MAJ_UpdateSW_ReadHex,
	MAJ_UpdateSW_EraseSector,
	MAJ_UpdateSW_WriteMemory,
	MAJ_UpdateSW_SendSwInfo,


} MAJ_UpdateSW_Etape_e;


/** Definition de l'Objet UpdateSW. */
typedef struct {

	FIL			HexFile;
	IHexRecord	HexRec;
	int8_t		CurrentSector;
	uint8_t		SectorErased[NB_SECTORS_MAX];
	uint32_t	StartAddress;
	uint32_t	BaseAddress;
	uint32_t	CurrentAddress;
	uint32_t	ExpectedAddress;
	uint32_t	Checksum;

} UpdateSW_s;

static UpdateSW_s UpdateSW;

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

static int NbTry;
static uint8_t			Etape = MAJ_UpdateSW_Init;
static uint16_t			Avancement_pr1000;
static SwFlash_Info_s	TmpSwInfo;


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
Status_e
SwMajFromSwpToFlash_Main(
		char* Filename			/**<[in] Nom du fichier d'entree. */
)
{
	Status_e Status = Status_EnCours;
	uint16_t tmp_u16;
	uint32_t tmp_u32;

	switch (Etape)
	{
		//--------------------------------------------------------------
		case MAJ_UpdateSW_Init :

			_printf("SwMajFromSwpToFlash_Main\n");

			// Init de la structure de MAJ
			NbTry = 5;
			Avancement_pr1000 = 0;

			for (int i=0; i<NB_SECTORS_MAX; i++)
				UpdateSW.SectorErased[i] = FALSE;
			UpdateSW.CurrentSector		= (-1);
			UpdateSW.BaseAddress		= 0;
			UpdateSW.CurrentAddress		= 0;
			UpdateSW.StartAddress		= 0xFFFFFFFF;
			UpdateSW.ExpectedAddress	= 0xFFFFFFFF;
			UpdateSW.Checksum			= 0;

			Etape = MAJ_UpdateSW_GetID;
			break;

		//--------------------------------------------------------------
		case MAJ_UpdateSW_GetID :

			switch (BL_GetID(&tmp_u16))
			{
				case Status_OK:
					_printf("GetID 0x%X OK\n", tmp_u16);
					Etape = MAJ_UpdateSW_OpenFile;
					break;

				case Status_KO:
					_printf("GetID KO\n", tmp_u16);
					if (--NbTry == 0)	Status = Status_KO;
					break;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_UpdateSW_OpenFile:

			if (f_open(&UpdateSW.HexFile, Filename, FA_READ) != FR_OK)
				Status = Status_KO;
			else
			{
				// Positionnement du pointeur au debut des donnees
				//f_lseek(&UpdateSW.HexFile, pSwp->Sw[iSwInSwp].BufferOffset);

				Etape = MAJ_UpdateSW_ReadHex;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_UpdateSW_ReadHex :

			switch (IHex_ReadRecordFile(&UpdateSW.HexRec, &UpdateSW.HexFile, &tmp_u16, NULL))
			{
				case IHEX_OK:
					NbTry = 5;

					switch (UpdateSW.HexRec.type)
					{
						// Data Record
						case IHEX_TYPE_00:

							//_printf("*");

							// Definition de l'adresse courante
							UpdateSW.CurrentAddress	= UpdateSW.HexRec.address + UpdateSW.BaseAddress;

							// Definition de l'adresse de debut du SW
							if (UpdateSW.StartAddress == 0xFFFFFFFF)
								UpdateSW.StartAddress = UpdateSW.CurrentAddress;

							// Ajout des donnees au calcul de checksum
							for (int i=0; i<UpdateSW.HexRec.dataLen; i++)
								UpdateSW.Checksum += UpdateSW.HexRec.data[i];

							// Detection des sauts d'adresses (pour calcul checksum)
							if ((UpdateSW.CurrentAddress != UpdateSW.ExpectedAddress) && (UpdateSW.ExpectedAddress != 0xFFFFFFFF))
							{
								for (uint32_t  i=0; i<UpdateSW.CurrentAddress - UpdateSW.ExpectedAddress; i++)
									UpdateSW.Checksum += 0xFF;		// On complete par des 0xFF
							}
							UpdateSW.ExpectedAddress = UpdateSW.CurrentAddress + UpdateSW.HexRec.dataLen;

							// Verification validite de l'adresse
							UpdateSW.CurrentSector	= FLASH_GetSectorNum(UpdateSW.CurrentAddress);
							if (UpdateSW.CurrentSector >= 0)
							{
								if (UpdateSW.SectorErased[UpdateSW.CurrentSector] == FALSE)	// Effacement secteur si besoin
									Etape = MAJ_UpdateSW_EraseSector;
								else
									Etape = MAJ_UpdateSW_WriteMemory;
							}

							// Calcul de l'avancement
//							if (pSwp->Sw[iSwInSwp].Data.Size > 0)
//							{
//								tmp_u32 = UpdateSW.CurrentAddress - UpdateSW.StartAddress;
//								tmp_u32 *= 1000;
//								tmp_u32 /= pSwp->Sw[iSwInSwp].Data.Size;
//								Avancement_pr1000 = (uint16_t) tmp_u32;
//							}

							break;

						// End of File Record
						case IHEX_TYPE_01:

							// Finalisation du calcul du Checksum (Complement a 2)
							UpdateSW.Checksum = ~UpdateSW.Checksum + 1;	// Fin du calcul du Checksum (Complement a 2)
							Avancement_pr1000 = 1000;

							Etape = MAJ_UpdateSW_SendSwInfo;
							break;

						// Extended Linear Address Record
						case IHEX_TYPE_04:

							UpdateSW.BaseAddress = (UpdateSW.HexRec.data[0] << 24) + (UpdateSW.HexRec.data[1] << 16);

							// Definition de l'adresse de debut du SW
							if (UpdateSW.StartAddress == 0xFFFFFFFF)
								UpdateSW.StartAddress = UpdateSW.BaseAddress;

							break;

						// Types non geres
						case IHEX_TYPE_02:		// Extended Segment Address Record
						case IHEX_TYPE_03:		// Start Segment Address Record
						case IHEX_TYPE_05:		// Start Linear Address Record
						default:
							break;
					}

					break;

				default:
					Status = Status_KO;
					break;
			}

			break;


		//--------------------------------------------------------------
		case MAJ_UpdateSW_EraseSector :

			Status = BL_Erase(UpdateSW.CurrentSector);

			switch (Status)
			{
				case Status_OK:
					_printf("MAJ_UpdateSW_EraseSector %d OK\n", UpdateSW.CurrentSector);
					UpdateSW.SectorErased[UpdateSW.CurrentSector] = TRUE;
					Status = Status_EnCours;
					Etape = MAJ_UpdateSW_WriteMemory;
					break;

				case Status_KO:
					_printf("MAJ_UpdateSW_EraseSector %d KO\n", UpdateSW.CurrentSector);
					WDG_Refresh();
					TSW_Delay(100);
					Status = Status_EnCours;
					if (--NbTry == 0)	Status = Status_KO;
					break;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_UpdateSW_WriteMemory :

			Status = BL_WriteMemory(UpdateSW.HexRec.address + UpdateSW.BaseAddress, UpdateSW.HexRec.data, UpdateSW.HexRec.dataLen);

			switch (Status)
			{
				case Status_OK:
					Status = Status_EnCours;
					Etape = MAJ_UpdateSW_ReadHex;
					break;

				case Status_KO:
					_printf("MAJ_UpdateSW_WriteMemory 0x%08X KO\n", UpdateSW.HexRec.address + UpdateSW.BaseAddress);
					Status = Status_EnCours;
					if (--NbTry == 0)	Status = Status_KO;
					break;
			}

			break;

		//--------------------------------------------------------------
		case MAJ_UpdateSW_SendSwInfo :

//			TmpSwInfo.StartAdress	= UpdateSW.StartAddress;
//			TmpSwInfo.Size			= UpdateSW.ExpectedAddress - UpdateSW.StartAddress;
//			TmpSwInfo.Checksum		= UpdateSW.Checksum;
//			TmpSwInfo.IsValide		= TRUE;
//			strncpy(TmpSwInfo.Version, pSwp->Sw[iSwInSwp].Version, VERSION_SIZE);
//
//			SwFlash_SaveAppInfo(&TmpSwInfo);

			_printf("BL_AppInfo Addr=0x%08X, Size=%d,  Checksum=0x%08X OK\n", UpdateSW.StartAddress, UpdateSW.CurrentAddress - UpdateSW.StartAddress, UpdateSW.Checksum);
			Status = Status_OK;

			break;
	}

	// Gestion de la sortie de la fonction
	if (Status != Status_EnCours)
	{
		// Fermeture du fichier
		f_close(&UpdateSW.HexFile);

		_printf("\n...MAJ CARTE TERMINEE...\n\n");

		Etape = 0;
	}

	return Status;
}

/**
 ***************************************************************************************************
 * @todo Validation
 */
uint16_t
SwMajFromSwpToFlash_GetAvancement_pr1000(
		void
)
{
	return Avancement_pr1000;
}

 /**
 * @}
 */

/* End Of File ************************************************************************************/

/**
 *******************************************************************************
 * @file	fct_SwHex
 * @author	j.daheron
 * @version	1.0.0
 * @date	12 fev. 2014
 * @brief   Gestion des Logiciels Intel Hex (Fichier et Package).
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "fct_SwHex.h"
#include "fct_MemoireFAT.h"
#include "util_Conversions.h"
#include "util_TSW.h"
#include "util_CONSOLE.h"

/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

#define TIMEOUT_CHECK_SWP_s						120
#define PREMIER_FICHIER_SW						0
#define VALIDATION_READ_SW_INFO_AND_CHECK_SWP	0

#if VALIDATION_READ_SW_INFO_AND_CHECK_SWP
	#include "app_MAJ.h"
#endif


/* Private macros ------------------------------------------------------------*/

#define LogId	"SWP"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/*
uint32_t CalculSommeAlphanumerique(IHexRecord* pRecord)
{
	uint32_t sum = 0;

	sum = pRecord->dataLen;
	sum += (uint8_t) (pRecord->address >> 8) & 0x00FF;
	sum += (uint8_t) (pRecord->address) & 0x00FF;
	sum += pRecord->type;
	for (int i=0; i<pRecord->dataLen; i++)
		sum += pRecord->data[i];
	sum += pRecord->checksum;

	return sum;
}

*/



/**-----------------------------------------------------------------------------
 * @brief	Validation des fonctions.
 *
 */
#define  VALIDATION_FCT_SW_HEX	 0

#if VALIDATION_FCT_SW_HEX

//const uint8_t FileName[] = "SWP_V12.32_FFFF8700.swp";
//const uint8_t FileName[] = "GMAX_V00.01_FE5E1000.swp";
const uint8_t FileName[] = "GMAX_V00.09_FE5E0000.swp";
//const uint8_t FileName[] = "AL00059AC.hex";



void VALIDATION_SEQUENCE_VALIDATION_FCT_SW_HEX(){

	int Status = Status_EnCours;
	uint8_t Etape=0;
	Swp_t Swp;
	FIL File;
	uint8_t FileName[32];
	int i;

/*
	uint8_t Num;
	Num = Memoire_SearchFile("", "GMAX_V*", 0, 0);
	_printf("Memoire_SearchFile : GMAX_V*  > %d\n", Num);

	Num = Memoire_SearchFile("", "GMAX_V00.09_FE5E0000.swp", 0, 0);
	_printf("Memoire_SearchFile : GMAX_V00.09_FE5E0000.swp  > %d\n", Num);

	Num = Memoire_SearchFile("", "*.swp", 0, 0);
	_printf("Memoire_SearchFile : *.swp  > %d\n", Num);
*/

	while (1)
	{
		switch (Etape)
		{
			// Verification du nombre de fichiers
			case 0:
				TimerSW_Start(TIMER_Generique, 50000);
				if (Memoire_SearchFile("", "*.swp", 1, &FileName) > 1)
				{
					_printf("Memoire_SearchFile KO : Plus d'un fichier trouve...\n");
				}
				else
				{
					_printf("Memoire_SearchFile OK : Fichier=%s\n", FileName);
				}

				_printf("Memoire_SearchFile > %dms\n", TimerSW_GetTempsPasse(TIMER_Generique));
				Etape++;
				break;

			// Lecture des informations du SWP
			case 1:

				_printf("\n");
				TimerSW_Start(TIMER_Generique, 50000);

				// Ouverture fichier
				if (f_open(&File, FileName, FA_READ) != FR_OK)
					while(1);

				switch (SwHex_ReadSwpInfo(&File, &Swp))
				{
					case Status_OK:
						_printf("SwHex_ReadSwpFileInfo OK\n");
						break;

					case Status_KO:
					default:
						_printf("SwHex_ReadSwpFileInfo KO\n");
						break;
				}
				_printf("SwHex_ReadSwpInfo > %dms\n", TimerSW_GetTempsPasse(TIMER_Generique));

				f_close(&File);

				// Affichage infos
				_printf("SWP :\n");
				_printf("-Version        = %s\n",		Swp.Version);
				_printf("-HeaderChecksum = 0x%08X\n",	Swp.HeaderChecksum);
				_printf("-HeaderIsValide = %d\n",		Swp.HeaderIsValide);
				_printf("-NbSw           = %d\n",		Swp.NbSw);
				for (i=0; i<NB_SW_MAX; i++)
				{
					_printf("-SW %d :\n",		i);
					_printf("--Version      = %s\n",		Swp.Sw[i].Version);
					_printf("--BufferOffset = %d\n",		Swp.Sw[i].BufferOffset);
					_printf("--HexSize      = %d\n",		Swp.Sw[i].HexSize);
					_printf("--Checksum     = 0x%08X\n",	Swp.Sw[i].Checksum);
					_printf("--IsValide     = %d\n",		Swp.Sw[i].IsValide);
				}

				Etape++;
				break;

			// Verification du checksum du package
			case 2:
				_printf("\n");
				TimerSW_Start(TIMER_Generique, 50000);
				if (SwHex_CheckSwpChecksum(FileName) == Status_KO)
				{
					_printf("SwHex_CheckSwpChecksum KO\n");
				}
				else
				{
					_printf("SwHex_CheckSwpChecksum OK\n");
				}

				_printf("SwHex_CheckSwpChecksum > %dms\n", TimerSW_GetTempsPasse(TIMER_Generique));
				Etape++;
				break;

			case 3:
				_printf("\n");
				for (i=0; i<Swp.NbSw; i++)
				{
					TimerSW_Start(TIMER_Generique, 50000);

					f_open(&File, FileName, FA_READ);
					SwHex_ReadSwInfo(&File, &Swp.Sw[i]);
					f_close(&File);

					_printf("SwHex_GetSwInfo (SW%d) > %dms\n", i, TimerSW_GetTempsPasse(TIMER_Generique));
					_printf("-SW Data %d :\n",		i);
					_printf("--StartAddress = 0x%08X\n",		Swp.Sw[i].Data.StartAddress);
					_printf("--Size         = 0x%08X\n",		Swp.Sw[i].Data.Size);
					_printf("--Checksum     = 0x%08X\n",		Swp.Sw[i].Data.Checksum);
				}

				Etape++;
				break;

			case 4:
				break;

		}

	}

	while (1)
	{
	}
}
#endif


/**-----------------------------------------------------------------------------
 * @brief	Calcul de checksum de donnees d'un fichier ouvert.
 *
 */
Status_e SwHex_CalculChecksum(FIL* pFile, uint32_t Debut, uint32_t Taille, uint32_t* Checksum)
{
	static int Etape = 0;
	static uint32_t CalcChecksum;
	static uint32_t TailleRestante;
	Status_e Status = Status_EnCours;
	FRESULT res;
	uint8_t buffer[512] = {0};
	uint32_t NbRead;

	switch (Etape)
	{
		// Init
		case 0:
			TailleRestante = Taille;
			CalcChecksum = 0;
			f_lseek(pFile, Debut);
			Etape++;
			break;

		case 1:
			// Parcours du fichiers
			if (TailleRestante >= 512)
				res = f_read(pFile, buffer, 512, &NbRead);
			else
				res = f_read(pFile, buffer, TailleRestante, &NbRead);

			// Verification du status de lecture
			if ((res != FR_OK) || (NbRead == 0))
				Status = Status_OK;
			else
				Status = Status_EnCours;

			// Ajout au calcul de checksums
			for (uint32_t i=0; i<NbRead; i++)
				CalcChecksum += buffer[i];

			// Maj de la taille restante a lire
			TailleRestante -= NbRead;
			if (TailleRestante == 0)
				Status = Status_OK;

			break;
	}

	if (Status != Status_EnCours)
	{
		*Checksum = (~CalcChecksum + 1);
		Etape = 0;
	}

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Verification du checksum du fichier.
 */
Status_e SwHex_ReadSwInfoAndCheckSwp(char* FileName, Swp_s* pSwp)
{
	static uint32_t TS_Start;
	static TSW_s Timeout;
	static int Etape = 0;
	static uint32_t ChecksumFichierLu = 0;
	static uint32_t ChecksumFichierCalcule = 0;
	static uint32_t ChecksumSwCalcule = 0;
	static Bool_e FlagEndSw = FALSE;
	static Bool_e FlagEndSwp = FALSE;
	static FIL File;
	static uint8_t IndiceFichier = PREMIER_FICHIER_SW;
	uint8_t buffer[10] = {0};
	Status_e Status = Status_EnCours;
	uint32_t ChecksumLine;
	IHexRecord HexRec;
	int i;


	switch (Etape)
	{
		// --- Extraction du checksum dans le nom de fichier ---------------------------------------
		case 0:
			TS_Start = TSW_GetTimestamp_ms();

			// Format du fichier :
			// ABCDEFGH[...]YZ_FE5E1000.swp

			//-----------------------------------
			for (i=0; i<8;i++)
				buffer[i] = FileName[strlen(FileName) - 12 + i];	// Le chk debute 12 caracteres avant la fin du nom

			ChecksumFichierLu  = (Conv_ASCII_to_Hexa(buffer[0], buffer[1]) << 24)	& 0xFF000000;
			ChecksumFichierLu += (Conv_ASCII_to_Hexa(buffer[2], buffer[3]) << 16)	& 0x00FF0000;
			ChecksumFichierLu += (Conv_ASCII_to_Hexa(buffer[4], buffer[5]) << 8)	& 0x0000FF00;
			ChecksumFichierLu += (Conv_ASCII_to_Hexa(buffer[6], buffer[7]) )		& 0x000000FF;

			if (f_open(&File, FileName, FA_READ) != FR_OK)
				Status = Status_KO;

			TSW_Start(&Timeout, TIMEOUT_CHECK_SWP_s * 1000);

			ChecksumSwCalcule = 0;
			FlagEndSwp = FALSE;
			FlagEndSw = FALSE;
			Etape++;
			break;

		// --- Parcours des enregistrements avec calcul du CRC de la ligne -------------------------
		case 1:

			switch ( IHex_ReadRecordFile(&HexRec, &File, 0, &ChecksumLine) )
			{
			case IHEX_OK:
				// Verification de fin suppose du fichier swp.
				if (FlagEndSw)
				{
					if ( FlagEndSwp || (HexRec.type != IHEX_TYPE_SWP_EOF) )
					{
						_CONSOLE(LogId, "**ERROR** Data read after end of file detection\n");
						Status = Status_KO;
					}
					else
						FlagEndSwp = TRUE;
				}
				else
				{
					// Mise jour des donnees SwInfo, si la ligne lue correspond au moins a des donnees
					// du premier fichier Sw.
					if (  f_tell( &File ) > pSwp->Sw[PREMIER_FICHIER_SW].BufferOffset )
					{
						switch ( SwHex_UpdateSwInfo( &HexRec, &pSwp->Sw[IndiceFichier] ) )
						{
						case Status_EnCours:
							ChecksumSwCalcule += ChecksumLine;
							break;

						case Status_OK:
							// --- Cloture checksum du Sw lu
							ChecksumSwCalcule += ChecksumLine;
							ChecksumSwCalcule = (~ChecksumSwCalcule + 1);

							if ( ChecksumSwCalcule == pSwp->Sw[IndiceFichier].Checksum )
								pSwp->Sw[IndiceFichier].IsValide = TRUE;
							else
							{
								pSwp->Sw[IndiceFichier].IsValide = FALSE;
								_CONSOLE( LogId, "**ERROR** CRC Calcule=0x%08X Attendu=0x%08X\n", ChecksumSwCalcule, pSwp->Sw[IndiceFichier].Checksum );
							}

							ChecksumSwCalcule 	= 0;
							// ---------------------------------------------------------------------

							// --- Verification si un autre Sw Hex est attendu
							if ( pSwp->NbSw - 1 > IndiceFichier )
								IndiceFichier++;
							else if ( pSwp->NbSw - 1 == IndiceFichier )
								FlagEndSw = TRUE; // Pas d'incrementation, on a lu la fin du dernier Sw dans le Swp
							else
							{
								_CONSOLE(LogId, "**ERROR** Invalide Nb of Sw in SWInfo\n");
								Status = Status_KO;
							}
							// ---------------------------------------------------------------------
							break;

						case Status_Fini:
						case Status_KO:
						default:
							_CONSOLE(LogId, "**ERROR** Invalide Status\n");
							break;
						}
					}
					Status = Status_EnCours;
				}

				// Mise a jour du Checksum avec celui de la ligne lue.
				ChecksumFichierCalcule	+= ChecksumLine;
				break;

			case IHEX_ERROR_EOF:
				// --- Fin de lecture:
				// Cacul du cheksum final et comparaison.
				ChecksumFichierCalcule = (~ChecksumFichierCalcule + 1);
				Etape = 0;

				if (ChecksumFichierCalcule != ChecksumFichierLu)
				{
					_CONSOLE( LogId, "**ERROR** Cheksum error Calcule=0x%08X Attendu=0x%08X\n", ChecksumFichierCalcule, ChecksumFichierLu);
					Status = Status_KO;
				}
				else
				{
					pSwp->Checksum = ChecksumFichierCalcule;
					SwHex_PrintSwpInfo( pSwp );
					Status = Status_OK;
				}
				break;

			default:
				// Nothing
				break;
			}

			// Verif Timeout
			if (TSW_IsRunning(&Timeout) == FALSE)
			{
				_CONSOLE(LogId, "**ERROR** Timeout SwHex_ReadSwInfoAndCheckSwp \n");
				Status = Status_KO;
			}
			break;

		default:
			break;
	}

	if (Status != Status_EnCours)
	{
		_CONSOLE(LogId, "SwHex_ReadSwInfoAndCheckSwp = %dms\n", TSW_GetTimestamp_ms() - TS_Start);
		ChecksumSwCalcule = 0;
		FlagEndSwp = FALSE;
		FlagEndSw = FALSE;
		f_close(&File);
		Etape = 0;
	}

	return Status;
}

#if 0 // XXX __FONCTION_OBSOLETE__
/**-----------------------------------------------------------------------------
 * @brief	Verification du checksum du fichier.
 */
Status_e SwHex_CheckSwpChecksum(char* FileName)
{
	static TSW_s Timeout;
	static int Etape = 0;
	static uint32_t ChecksumFichier = 0;
	static FIL File;
	Status_e Status = Status_EnCours;
	uint8_t buffer[10] = {0};
	uint32_t ChecksumCalcule = 0;
	int i;
	static uint32_t TS_Start;

	switch (Etape)
	{
		// Extraction du checksum dans le nom de fichier
		case 0:
			TS_Start = TSW_GetTimestamp_ms();

			// Format du fichier :
			// ABCDEFGH[...]YZ_FE5E1000.swp

			//-----------------------------------
			for (i=0; i<8;i++)
				buffer[i] = FileName[strlen(FileName) - 12 + i];	// Le chk debute 12 caracteres avant la fin du nom

			ChecksumFichier  = (Conv_ASCII_to_Hexa(buffer[0], buffer[1]) << 24) & 0xFF000000;
			ChecksumFichier += (Conv_ASCII_to_Hexa(buffer[2], buffer[3]) << 16)	& 0x00FF0000;
			ChecksumFichier += (Conv_ASCII_to_Hexa(buffer[4], buffer[5]) << 8)	& 0x0000FF00;
			ChecksumFichier += (Conv_ASCII_to_Hexa(buffer[6], buffer[7]) )		& 0x000000FF;

			if (f_open(&File, FileName, FA_READ) != FR_OK)
				Status = Status_KO;

			TSW_Start(&Timeout, TIMEOUT_CHECK_SWP_s * 1000);

			Etape++;
			break;

		// Calcul du checksum du fichier
		case 1:

			// Verif Timeout
			if (TSW_IsRunning(&Timeout) == FALSE)
				Status = Status_KO;

			if (SwHex_CalculChecksum(&File, 0, f_size(&File), &ChecksumCalcule) == Status_EnCours)
				break;

			if (ChecksumCalcule != ChecksumFichier)
				Status = Status_KO;
			else
				Status = Status_OK;

			break;
	}

	if (Status != Status_EnCours)
	{
		_CONSOLE(LogId, "CheckSwpChecksum = %dms (0x%08X)\n", TSW_GetTimestamp_ms() - TS_Start, ChecksumCalcule);
		f_close(&File);
		Etape = 0;
	}

	return Status;
}
#endif


/**-----------------------------------------------------------------------------
 * @brief	Lecture des informations du Package.
 *
 * 			Parcours de l'entete du fichier swp afin de recuperer les
 * 			informations sur les SW contenus dans le package (cf. spec SWP).
 */
uint8_t SwHex_ReadSwpInfo(FIL *pFile, Swp_s* pSwp)
{
	static uint8_t TypeAttendu = IHEX_TYPE_SWP_INFO;
	uint8_t Status = Status_EnCours;

	uint32_t i, iSw;
	IHexRecord HexRec;
	uint16_t NbRead;
	Sw_s* pSw = NULL;

	// Verification des parametres
	if ((pFile == NULL) || (pSwp == NULL))
		return Status_KO;

	TSW_MeasureTime_Start();

	// Parcours de l'entete de fichier
	while (Status == Status_EnCours)
	{
		// Parcours des enregistrements
		if (IHex_ReadRecordFile(&HexRec, pFile, &NbRead, NULL) == IHEX_OK)
		{
			switch (HexRec.type)
			{
				//--------------------------------------
				case IHEX_TYPE_SWP_INFO:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SWP_INFO)
					{
						Status = Status_KO;
						break;
					}

					// Initialisation de l'objet Swp
					for (i=0; i<TAILLE_VERSION_MAX; i++)
						pSwp->Version[i] = 0;

					pSwp->HeaderChecksum	= 0;
					pSwp->HeaderIsValide	= FALSE;
					pSwp->Checksum			= 0;
					pSwp->IsValide			= FALSE;
					pSwp->NbSw				= 0;

					// Initialisation des objets Sw
					for (iSw=0; iSw<NB_SW_MAX; iSw++)
					{
						for (i=0; i<TAILLE_VERSION_MAX; i++)
							pSwp->Sw[iSw].Version[i] = 0;
						pSwp->Sw[iSw].BufferOffset		= 0;
						pSwp->Sw[iSw].HexSize			= 0;
						pSwp->Sw[iSw].Checksum			= 0;
						pSwp->Sw[iSw].IsValide			= FALSE;

						// TODO : Voir si necessaire d'initialiser les data ici...
						pSwp->Sw[iSw].Data.StartAddress	= 0xFFFFFFFF;
						pSwp->Sw[iSw].Data.Size			= 0;
						pSwp->Sw[iSw].Data.Checksum		= 0;
					}

					pSw = NULL;

					// Copie de la version lue dans le fichier
					HexRec.data[HexRec.dataLen] = 0;
					strncpy(pSwp->Version, HexRec.data, TAILLE_VERSION_MAX);

					TypeAttendu = IHEX_TYPE_SWP_NB_SW;
					break;

				//--------------------------------------
				case IHEX_TYPE_SWP_NB_SW:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SWP_NB_SW)
					{
						Status = Status_KO;
						break;
					}

					if (HexRec.data[0] > NB_SW_MAX)
					{
						Status = Status_KO;
						break;
					}

					iSw = 0;
					pSwp->NbSw = HexRec.data[0];

					TypeAttendu = IHEX_TYPE_SW_INFO;
					break;


				//--------------------------------------
				case IHEX_TYPE_SW_INFO:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SW_INFO)
					{
						Status = Status_KO;
						break;
					}

					iSw++;
					pSw = &(pSwp->Sw[iSw-1]);

					HexRec.data[HexRec.dataLen] = 0;
					strncpy(pSw->Version, HexRec.data, TAILLE_VERSION_MAX);

					TypeAttendu = IHEX_TYPE_SW_OFFSET;
					break;

				//--------------------------------------
				case IHEX_TYPE_SW_OFFSET:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SW_OFFSET)
					{
						Status = Status_KO;
						break;
					}

					pSw->BufferOffset = Conv_pu8_to_u32(HexRec.data, MSB_First);

					TypeAttendu = IHEX_TYPE_SW_SIZE;
					break;

				//--------------------------------------
				case IHEX_TYPE_SW_SIZE:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SW_SIZE)
					{
						Status = Status_KO;
						break;
					}

					pSw->HexSize = Conv_pu8_to_u32(HexRec.data, MSB_First);

					TypeAttendu = IHEX_TYPE_SW_CHECKSUM;
					break;

				//--------------------------------------
				case IHEX_TYPE_SW_CHECKSUM:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SW_CHECKSUM)
					{
						Status = Status_KO;
						break;
					}

					pSw->Checksum = Conv_pu8_to_u32(HexRec.data, MSB_First);

					if (iSw >= pSwp->NbSw)
						TypeAttendu = IHEX_TYPE_SWP_HEADERCHECKSUM;
					else
						TypeAttendu = IHEX_TYPE_SW_INFO;

					break;

				//--------------------------------------
				case IHEX_TYPE_SWP_HEADERCHECKSUM:

					// On verifie qu'on a lu le type attendu
					if (TypeAttendu != IHEX_TYPE_SWP_HEADERCHECKSUM)
					{
						Status = Status_KO;
						break;
					}

					// On recupere la position de debut de l'enregistrement
					NbRead = f_tell(pFile) - NbRead - 2;

					TSW_MeasureTime_Start();
					// Calcul du checksum
					while (SwHex_CalculChecksum(pFile, 0, NbRead, &pSwp->HeaderChecksum) == Status_EnCours);
					_CONSOLE(LogId, "IHEX_TYPE_SWP_HEADERCHECKSUM SwHex_CalculChecksum = %dms\n", TSW_MeasureTime_Get());

					if (pSwp->HeaderChecksum == Conv_pu8_to_u32(HexRec.data, MSB_First))
						pSwp->HeaderIsValide = TRUE;
					else
						pSwp->HeaderIsValide = FALSE;

					TypeAttendu = IHEX_TYPE_SWP_INFO;	// Pour une eventuelle relecture...
					Status = Status_OK;
					break;

				//--------------------------------------
				case IHEX_TYPE_DATA:
				case IHEX_TYPE_EOF:
				case IHEX_TYPE_EXT_SEG_ADDR:
				case IHEX_TYPE_START_SEG_ADDR:
				case IHEX_TYPE_EXT_LIN_ADDR:
				case IHEX_TYPE_START_LIN_ADDR:
					// On ne doit jamais atteindre le debut des données
				default:
					Status = Status_KO;
					break;
			}
		}
	}

	_CONSOLE(LogId, "ReadSwpInfo = %dms\n", TSW_MeasureTime_Get());

	return Status;
}

#if 0 // XXX __FONCTION_OBSOLETE__
/**-----------------------------------------------------------------------------
 * @brief	Lecture des informations du Fichier.
 *
 * 			Note : Le fichier est parcouru dans son integralité.
 * 			Le temps de lecture depends du support et de la taille du fichier.
 */
uint8_t SwHex_ReadSwInfo(FIL *pFile, Sw_s* pSw)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t EndAddress=0;
	uint32_t BaseAddress=0;
	uint32_t ExpectedAddress=0xFFFFFFFF;
	uint32_t CurrentAddress=0;

	// Initialisation des donness
	pSw->Data.StartAddress	= 0xFFFFFFFF;
	pSw->Data.Size			= 0;
	pSw->Data.Checksum		= 0;

	// Positionnement au pointeur
	f_lseek(pFile, pSw->BufferOffset);

	TSW_MeasureTime_Start();

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecordFile(&HexRec, pFile, 0, NULL) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_DATA:

					CurrentAddress = BaseAddress + HexRec.address;

					// Determination des adresses de debut et fin
					if (pSw->Data.StartAddress == 0xFFFFFFFF)
						pSw->Data.StartAddress =  CurrentAddress;
					EndAddress = BaseAddress + HexRec.address + HexRec.dataLen;

					// Calcul du checksum
					for (int i=0; i<HexRec.dataLen; i++)
						pSw->Data.Checksum += HexRec.data[i];

					if ((CurrentAddress != ExpectedAddress) && (ExpectedAddress != 0xFFFFFFFF))	// Verifiaction des sauts d'adresses
					{
						for (int i=0; i<CurrentAddress-ExpectedAddress; i++)
							pSw->Data.Checksum += 0xFF;		// On complete par des 0xFF
					}
					ExpectedAddress = CurrentAddress + HexRec.dataLen;

					break;

				case IHEX_TYPE_EXT_SEG_ADDR:
					BaseAddress  = (HexRec.data[0] << 8) + HexRec.data[1];
					BaseAddress *= 16;
					BaseAddress &= 0xFFFFFFF0;
					break;

				case IHEX_TYPE_EXT_LIN_ADDR:
					BaseAddress  = (HexRec.data[0] << 8) + HexRec.data[1];
					BaseAddress <<= 16;
					BaseAddress &= 0xFFFF0000;
					break;

				case IHEX_TYPE_EOF:
					pSw->Data.Checksum = ~pSw->Data.Checksum + 1;			// Fin du calcul du Checksum (Complement a 2)
					pSw->Data.Size = EndAddress - pSw->Data.StartAddress;	// Calcul de la taille
					Status = Status_OK;
					break;

			}
		}
		else
			Status = Status_KO;
	}

	_CONSOLE(LogId, "ReadSwInfo = %dms\n", TSW_MeasureTime_Get());

	return Status;
}
#endif

/**-----------------------------------------------------------------------------
 * @brief	Mise a jour des donnees des Sw dans la structure Sw_s a partir
 * 			du Hex Record lu.
 *
 */
Status_e SwHex_UpdateSwInfo( const IHexRecord* pRecord, Sw_s* pSw )
{
	static uint32_t EndAddress=0;
	static uint32_t BaseAddress=0;
	static uint32_t ExpectedAddress=0xFFFFFFFF;
	static uint32_t CurrentAddress=0;
	Status_e Statut = Status_EnCours;

	TSW_MeasureTime_Start();

	switch (pRecord->type)
	{
		// Data Record
		case IHEX_TYPE_DATA:

			CurrentAddress = BaseAddress + pRecord->address;

			// Determination des adresses de debut et fin
			if (pSw->Data.StartAddress == 0xFFFFFFFF)
				pSw->Data.StartAddress =  CurrentAddress;
			EndAddress = BaseAddress + pRecord->address + pRecord->dataLen;

			// Calcul du checksum
			for (int i=0; i<pRecord->dataLen; i++)
				pSw->Data.Checksum += pRecord->data[i];

			if ((CurrentAddress != ExpectedAddress) && (ExpectedAddress != 0xFFFFFFFF))	// Verifiaction des sauts d'adresses
			{
				for (int i=0; i<CurrentAddress-ExpectedAddress; i++)
					pSw->Data.Checksum += 0xFF;		// On complete par des 0xFF
			}
			ExpectedAddress = CurrentAddress + pRecord->dataLen;
			break;

		case IHEX_TYPE_EXT_SEG_ADDR:
			BaseAddress  = (pRecord->data[0] << 8) + pRecord->data[1];
			BaseAddress *= 16;
			BaseAddress &= 0xFFFFFFF0;
			break;

		case IHEX_TYPE_EXT_LIN_ADDR:
			BaseAddress  = (pRecord->data[0] << 8) + pRecord->data[1];
			BaseAddress <<= 16;
			BaseAddress &= 0xFFFF0000;
			break;

		case IHEX_TYPE_EOF:
			pSw->Data.Checksum = ~pSw->Data.Checksum + 1;			// Fin du calcul du Checksum (Complement a 2)
			pSw->Data.Size = EndAddress - pSw->Data.StartAddress;	// Calcul de la taille

			// Fin de fichier: mise a jour des variables.
			EndAddress=0;
			BaseAddress=0;
			ExpectedAddress=0xFFFFFFFF;
			CurrentAddress=0;

			Statut = Status_OK;
			break;

	}

	return (Statut);
}


/**-----------------------------------------------------------------------------
 * @brief	Affichage des informations du Swp
 *
 */
void SwHex_PrintSwpInfo( Swp_s* pSwp )
{

	_CONSOLE( LogId, "-------------------------------------------------------\n");
	_CONSOLE( LogId, "----------------------- SWP INFO ----------------------\n");
	_CONSOLE( LogId, "pSwp->FileName			= %s\n", pSwp->FileName );
	_CONSOLE( LogId, "pSwp->Version				= %s\n", pSwp->Version );
	_CONSOLE( LogId, "pSwp->Checksum			= 0x%08X\n", pSwp->Checksum );
	_CONSOLE( LogId, "pSwp->IsValide			= %d\n", pSwp->IsValide );
	_CONSOLE( LogId, "pSwp->HeaderChecksum		= 0x%08X\n", pSwp->HeaderChecksum );
	_CONSOLE( LogId, "pSwp->HeaderIsValide		= %d\n", pSwp->HeaderIsValide );
	_CONSOLE( LogId, "pSwp->NbSw				= %d\n", pSwp->NbSw );
	_CONSOLE( LogId, "\n");

	for (uint8_t i=0; i<pSwp->NbSw; i++)
	{
		_CONSOLE( LogId, "pSwp->Sw[%d].BufferOffset		= 0x%08X\n", i, pSwp->Sw[i].BufferOffset );
		_CONSOLE( LogId, "pSwp->Sw[%d].Checksum			= 0x%08X\n", i, pSwp->Sw[i].Checksum );
		_CONSOLE( LogId, "pSwp->Sw[%d].HexSize			= %d\n", i, pSwp->Sw[i].HexSize );
		_CONSOLE( LogId, "pSwp->Sw[%d].IsValide			= %d\n", i, pSwp->Sw[i].IsValide );
		_CONSOLE( LogId, "pSwp->Sw[%d].Version			= %s\n", i, pSwp->Sw[i].Version );
		_CONSOLE( LogId, "pSwp->Sw[%d].Data.StartAddress	= 0x%08X\n", i, pSwp->Sw[i].Data.StartAddress );
		_CONSOLE( LogId, "pSwp->Sw[%d].Data.Size			= %d\n", i, pSwp->Sw[i].Data.Size );
		_CONSOLE( LogId, "pSwp->Sw[%d].Data.Checksum		= 0x%08X\n", i, pSwp->Sw[i].Data.Checksum );
		_CONSOLE( LogId, "\n");
	}
	_CONSOLE( LogId, "-------------------------------------------------------\n");
}

/**
 ***************************************************************************************************
 */
void FONCTION_VALIDATION_READ_SW_INFO_AND_CHECK_SWP( void )
{
#if VALIDATION_READ_SW_INFO_AND_CHECK_SWP

	static MajInfo_s MajInfo = {0};
	Status_e Statut = Status_EnCours;

	appMAJ_Init(&MajInfo);

	appMAJ_CheckSWPInfo(&MajInfo, "GMAX_");

	TSW_MeasureTime_Start();

	while ( Statut == Status_EnCours )
	{
		Statut = SwHex_ReadSwInfoAndCheckSwp(MajInfo.Swp.FileName, &MajInfo.Swp );
		switch (Statut)
		{
			case Status_OK:
				_CONSOLE( LogId, "SwHex_ReadSwInfoAndCheckSwp OK\n");
				MajInfo.Swp.IsValide = TRUE;
				MajInfo.SwpCheckTS = RTC_GetTimestamp();
				break;

			case Status_KO:
				_CONSOLE( LogId, "**ERROR** SwHex_ReadSwInfoAndCheckSwp KO\n");
				MajInfo.Swp.IsValide = FALSE;
				MajInfo.SwpCheckTS = RTC_GetTimestamp();
				break;

			default:
				break;
		}
	}

	_CONSOLE( LogId, "MajInfo.Swp.FileName			= %s\n", MajInfo.Swp.FileName );
	_CONSOLE( LogId, "MajInfo.Swp.Version			= %s\n", MajInfo.Swp.Version );
	_CONSOLE( LogId, "MajInfo.Swp.Checksum			= 0x%08X\n", MajInfo.Swp.Checksum );
	_CONSOLE( LogId, "MajInfo.Swp.IsValide			= %d\n", MajInfo.Swp.IsValide );
	_CONSOLE( LogId, "MajInfo.Swp.HeaderChecksum		= 0x%08X\n", MajInfo.Swp.HeaderChecksum );
	_CONSOLE( LogId, "MajInfo.Swp.HeaderIsValide		= %d\n", MajInfo.Swp.HeaderIsValide );
	_CONSOLE( LogId, "MajInfo.Swp.NbSw				= %d\n", MajInfo.Swp.NbSw );
	_CONSOLE( LogId, "\n");

	for (uint8_t i=0; i<MajInfo.Swp.NbSw; i++)
	{
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].BufferOffset		= 0x%08X\n", i, MajInfo.Swp.Sw[i].BufferOffset );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].Checksum			= 0x%08X\n", i, MajInfo.Swp.Sw[i].Checksum );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].HexSize			= %d\n", i, MajInfo.Swp.Sw[i].HexSize );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].IsValide			= %d\n", i, MajInfo.Swp.Sw[i].IsValide );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].Version			= %s\n", i, MajInfo.Swp.Sw[i].Version );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].Data.StartAddress	= 0x%08X\n", i, MajInfo.Swp.Sw[i].Data.StartAddress );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].Data.Size			= %d\n", i, MajInfo.Swp.Sw[i].Data.Size );
		_CONSOLE( LogId, "MajInfo.Swp.Sw[%d].Data.Checksum		= 0x%08X\n", i, MajInfo.Swp.Sw[i].Data.Checksum );
		_CONSOLE( LogId, "\n");
	}
#endif
}

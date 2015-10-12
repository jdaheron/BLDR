/**
 *******************************************************************************
 * @file	drv_OBS4xx.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	27 nov. 2013
 * @brief   Gestion du module BT OBS4xx.
 *******************************************************************************
 */



/* Includes ------------------------------------------------------------------*/

#include "drv_OBS4xx.h"
#include "util_TSW.h"
#include "util_printf.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

__IN_FLASH const char* OBS4xx_ConfigClient[] = {				"AGCM=1,0",	"AGPM=2,0",	"AGSM=2,0",	"AMMP=255,0",	"AMDS=2,1,0",	"\0"};
__IN_FLASH const char* OBS4xx_ConfigServeur[] = {	"AGDM=1,0",	"AGCM=2,0",	"AGPM=2,0",	"AGSM=3,0",	"AMMP=255,0",	"AMDS=2,1,0",	"AGUB=ffffffffffff",	"\0"};


#define OBS4xx_CONSOLE_ENABLE			0
#define OBS4xx__CONSOLE_RX_TX			0

#define OBS4xx_DEFAULT_BAUDRATE			57600
#define OBS4xx_DEFAULT_ESCAPE_TIME_ms	1100
#define OBS4xx_INIT_ESCAPE_TIME_ms		200
#define OBS4xx_STARTUP_TIME_ms			2000

// Definition du debit precedent pour restauration de la config du module
#define OBS4xx_PRECEDANT_BAUDRATE		19200


/* Private macros ------------------------------------------------------------*/

#if OBS4xx_CONSOLE_ENABLE
	#define _OBS4xx_CONSOLE	_printf
#else
	#define _OBS4xx_CONSOLE
#endif


/* Private variables ---------------------------------------------------------*/

static OBS4xx_t OBS4xx;

static TSW_s TIMER_OBS4xx;
static TSW_s TIMER_OBS4xx_TimeoutRx;
static TSW_s TIMER_OBS4xx_TimeoutReponse;


/* Private constantes --------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


uint8_t OBS4xx_GestionReception();
uint8_t OBS4xx_WaitForACK();
uint8_t OBS4xx_EnvoiTrame_WaitACK(char* Trame);

uint8_t OBS4xx_EscapeSequence(uint16_t DelayBeforeAndAfter);
uint8_t OBS4xx_ModuleType_Verify();
uint8_t OBS4xx_ModuleName_Read();
uint8_t OBS4xx_ModuleName_Write();
uint8_t OBS4xx_ModuleAddress_Read();
uint8_t OBS4xx_Pin_Write();
uint8_t OBS4xx_EnvoiCommandesConfig();
uint8_t OBS4xx_Connect(char* Address);



/**-----------------------------------------------------------------------------
 * @brief	Validation des fonctions.
 *
 */
#define  VALIDATION_DRV_OBS4xx 0

#if VALIDATION_DRV_OBS4xx
void VALIDATION_SEQUENCE_DRV_OBS4xx(){

}
#endif


/**-----------------------------------------------------------------------------
 * @brief	Gestion du Module BT OBS4xx.
 *
 */
void OBS4xx_Init(uint32_t Baudrate, OBS4xx_Mode_e Mode)
{
	int i;

	_OBS4xx_CONSOLE("OBS4xx_Initialize\n");

	// Initaialisation de la liaison
	HAL_BT_Init(Baudrate);
	_OBS4xx_CONSOLE("OBS4xx_Init COM %d\n", Baudrate);

	// Initialisation des variables
	for (i=0; i<OBS4xx_ModuleName_SIZE; i++)
		OBS4xx.ModuleName[i] = 0;
	for (i=0; i<OBS4xx_ModuleAddress_SIZE; i++)
		OBS4xx.ModuleAddress[i] = 0;
	for (i=0; i<OBS4xx_PIN_SIZE; i++)
		OBS4xx.PIN[i] = 0;


	switch (Baudrate)
	{
		case 300 : 		OBS4xx.TargetBaudrateEnum = 1;	break;
		case 1200 : 	OBS4xx.TargetBaudrateEnum = 2;	break;
		case 2400 : 	OBS4xx.TargetBaudrateEnum = 3;	break;
		case 4800 : 	OBS4xx.TargetBaudrateEnum = 4;	break;
		case 9600 : 	OBS4xx.TargetBaudrateEnum = 5;	break;
		case 19200 : 	OBS4xx.TargetBaudrateEnum = 6;	break;
		case 38400 : 	OBS4xx.TargetBaudrateEnum = 7;	break;
		case 115200 : 	OBS4xx.TargetBaudrateEnum = 9;	break;
		case 230400 : 	OBS4xx.TargetBaudrateEnum = 10;	break;
		case 460800 : 	OBS4xx.TargetBaudrateEnum = 11;	break;
		case 921600 : 	OBS4xx.TargetBaudrateEnum = 12;	break;
		case 57600:
		default: 		OBS4xx.TargetBaudrateEnum = 8;	break;
	}
	OBS4xx.TargetBaudrate				= Baudrate;
	OBS4xx.InitTargetBaudrateRequise	= FALSE;
	OBS4xx.NbTentative					= 0;
	OBS4xx.EtatReception				= OBS4xx_AttenteReception;
	OBS4xx.ConfigMode					= OBS4xx_ConfigMode_AUCUN;
	OBS4xx.ConfigLastStatus				= Status_OK;
	OBS4xx.ModuleType					= OBS4xx_TypeInconnu;
	OBS4xx.Mode							= Mode;
}


/**-----------------------------------------------------------------------------
 * @brief	Envoi de trames sur la liaison OBS4xx.
 *
 */
void OBS4xx_Write(char* Buffer, uint16_t Taille) {

	#if OBS4xx__CONSOLE_RX_TX
		_OBS4xx_CONSOLE("OBS4xx Tx: \"");
		_OBS4xx_CONSOLE(Buffer);
		_OBS4xx_CONSOLE("\"\n");
	#endif

	HAL_BT_Write(Buffer, Taille);
}


/**-----------------------------------------------------------------------------
 * @brief	Envoi de trames sur la liaison OBS4xx.
 *
 */
uint16_t OBS4xx_Read(char* Buffer, uint16_t Taille) {

	return HAL_BT_Read(Buffer, Taille);
}

/**-----------------------------------------------------------------------------
 * @brief	Deconnexion.
 *
 */
void OBS4xx_Deconnecter()
{
	if (OBS4xx_GetConfigMode() != OBS4xx_ConfigMode_AUCUN)
		return;

	OBS4xx_Configure(OBS4xx_ConfigMode_DISCONNECT);
}

/**-----------------------------------------------------------------------------
 * @brief	Module visible.
 *
 */
void OBS4xx_Discoverable()
{
	if (OBS4xx_GetConfigMode() != OBS4xx_ConfigMode_AUCUN)
		return;

	OBS4xx_Configure(OBS4xx_ConfigMode_DISCOVERABLE);
}

/**-----------------------------------------------------------------------------
 * @brief	Module invisible.
 *
 */
void OBS4xx_Undiscoverable()
{
	if (OBS4xx_GetConfigMode() != OBS4xx_ConfigMode_AUCUN)
		return;

	OBS4xx_Configure(OBS4xx_ConfigMode_UNDISCOVERABLE);
}

/**-----------------------------------------------------------------------------
 * @brief	Definition du code PIN.
 *
 */
void OBS4xx_Set_Pin(char* Pin)
{
	if (OBS4xx_GetConfigMode() != OBS4xx_ConfigMode_AUCUN)
		return;

	// Effacement PIN actuel
	for (int i=0; i<OBS4xx_PIN_SIZE; i++)
		OBS4xx.PIN[i] = 0;

	// Copie nouveau PIN
	strncpy(OBS4xx.PIN, Pin, OBS4xx_PIN_SIZE);
	OBS4xx.PIN[OBS4xx_PIN_SIZE] = 0;

	OBS4xx_Configure(OBS4xx_ConfigMode_SET_PIN);
}

/**-----------------------------------------------------------------------------
 * @brief	Definition du Nom du Module.
 *
 */
void OBS4xx_Set_Name(char* Name)
{
	if (OBS4xx_GetConfigMode() != OBS4xx_ConfigMode_AUCUN)
		return;

	// Effacement Nom actuel
	for (int i=0; i<OBS4xx_ModuleName_SIZE; i++)
		OBS4xx.ModuleName[i] = 0;

	// Copie nouveau Nom
	strncpy(OBS4xx.ModuleName, Name, OBS4xx_ModuleName_SIZE);

	OBS4xx_Configure(OBS4xx_ConfigMode_SET_LOCAL_NAME);
}

/**-----------------------------------------------------------------------------
 * @brief	Definition de l'addresse du module cible.
 *
 */
void OBS4xx_Set_TargetAdress(char* TargetAdress)
{
	if (OBS4xx_GetConfigMode() != OBS4xx_ConfigMode_AUCUN)
		return;

	// Effacement Nom actuel
	for (int i=0; i<OBS4xx_ModuleName_SIZE; i++)
		OBS4xx.TargetModuleAddress[i] = 0;

	// Copie nouveau Nom
	strncpy(OBS4xx.TargetModuleAddress, TargetAdress, OBS4xx_ModuleName_SIZE);

	OBS4xx_Configure(OBS4xx_ConfigMode_CONNECT);
}

/**-----------------------------------------------------------------------------
 * @brief	Recuperation du code PIN actuel.
 *
 */
char* OBS4xx_Get_Pin()
{
	return OBS4xx.PIN;
}

/**-----------------------------------------------------------------------------
 * @brief	Recuperation du nom actuel.
 *
 */
char* OBS4xx_Get_Name()
{
	return OBS4xx.ModuleName;
}

/**-----------------------------------------------------------------------------
 * @brief	Recuperation de l'adresse du module.
 *
 */
char* OBS4xx_Get_Address()
{
	return OBS4xx.ModuleAddress;
}

#if 0
void OBS4xx_RestaureDefaultBaudrate(uint32_t BaudratePrecedant)
{
	_OBS4xx_CONSOLE("OBS4xx_RestaureDefaultBaudrate\n");
	HAL_BT_Init(BaudratePrecedant);
	TSW_Delay(100);
	HAL_BT_Write("///", sizeof("///"));
	TSW_Delay(100);
	HAL_BT_Write("AT*AMRS=8,1,1,1,2,1,1\r", sizeof("AT*AMRS=8,1,1,1,2,1,1\r"));
	TSW_Delay(10);
	HAL_BT_Init(OBS4xx_DEFAULT_BAUDRATE);
}
#endif


/**-----------------------------------------------------------------------------
 * @brief	Configuration du Module.
 *
 */
uint8_t OBS4xx_Configure(uint8_t ConfigMode) {

	static uint8_t Etape = OBS4xx_ConfigEtape_Init;
	static uint16_t Delai_EscapeSequence;
	uint8_t Status = Status_EnCours;
	char buff[64] = {0};

	switch(Etape) {

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_Init:

			if (ConfigMode == OBS4xx_ConfigMode_AUCUN)
				return Status_Fini;

			_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Init. Mode %d\n", ConfigMode);

			OBS4xx.InitTargetBaudrateRequise	= FALSE;
			OBS4xx.NbTentative					= 0;
			OBS4xx.ConfigMode					= ConfigMode;

			TSW_Stop(&TIMER_OBS4xx);
			Delai_EscapeSequence = OBS4xx_INIT_ESCAPE_TIME_ms;

			// Tempo de demarrage
			if (OBS4xx.ConfigMode == OBS4xx_ConfigMode_INIT_MODULE)
				TSW_Start(&TIMER_OBS4xx, OBS4xx_STARTUP_TIME_ms);

			Etape = OBS4xx_ConfigEtape_StartupDelay;
			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_StartupDelay:

			if (TSW_IsRunning(&TIMER_OBS4xx))
				break;

			Etape = OBS4xx_ConfigEtape_EscapeSequence;
			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_EscapeSequence:

			if (OBS4xx_EscapeSequence(Delai_EscapeSequence) == Status_OK)
				Etape = OBS4xx_ConfigEtape_EchoOff;

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_EchoOff:

			switch (OBS4xx_EnvoiTrame_WaitACK("ATE0\r"))
			{
				case Status_OK:

					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_EchoOff = OK\n");

					switch (OBS4xx.ConfigMode)
					{
						case OBS4xx_ConfigMode_INIT_MODULE:
							Etape = OBS4xx_ConfigEtape_LectureModuleName;
							break;

						case OBS4xx_ConfigMode_CONNECT:
							Etape = OBS4xx_ConfigEtape_Connect;
							break;

						case OBS4xx_ConfigMode_DISCONNECT:
							Etape = OBS4xx_ConfigEtape_Disconnect;
							break;

						case OBS4xx_ConfigMode_SET_PIN:
							Etape = OBS4xx_ConfigEtape_SetPin;
							break;

						case OBS4xx_ConfigMode_SET_LOCAL_NAME:
							Etape = OBS4xx_ConfigEtape_SetLocalName;
							break;

						case OBS4xx_ConfigMode_DISCOVERABLE:
							Etape = OBS4xx_ConfigEtape_Discoverable;
							break;

						case OBS4xx_ConfigMode_UNDISCOVERABLE:
							Etape = OBS4xx_ConfigEtape_Undiscoverable;
							break;

						case OBS4xx_ConfigMode_CLEAR_BONDING_LIST:
							Etape = OBS4xx_ConfigEtape_ClearBondingList;
							break;

						default:
							Etape = OBS4xx_ConfigEtape_Error;
							break;
					}

					break;

				case Status_KO:

					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_EchoOff = KO\n");

					OBS4xx.NbTentative++;
					switch (OBS4xx.NbTentative)
					{
						// Essai au debit par defaut
						case 1:
							OBS4xx.InitTargetBaudrateRequise = TRUE;	// Init du module BT au debit cible necessaire
							HAL_BT_Init(OBS4xx_DEFAULT_BAUDRATE);		// Config en baudrate par defaut
							_OBS4xx_CONSOLE("OBS4xx_Init COM %d\n", OBS4xx_DEFAULT_BAUDRATE);
							Etape = OBS4xx_ConfigEtape_EscapeSequence;
							Delai_EscapeSequence = OBS4xx_DEFAULT_ESCAPE_TIME_ms;
							break;

						// Essai a un autre debit preconfigure
						case 2:
							OBS4xx.InitTargetBaudrateRequise = TRUE;	// Init du module BT au debit cible necessaire
							HAL_BT_Init(OBS4xx_PRECEDANT_BAUDRATE);		// Config en baudrate par defaut
							_OBS4xx_CONSOLE("OBS4xx_Init COM %d\n", OBS4xx_PRECEDANT_BAUDRATE);
							Etape = OBS4xx_ConfigEtape_EscapeSequence;
							Delai_EscapeSequence = OBS4xx_DEFAULT_ESCAPE_TIME_ms;
							break;

						default :
							Status = Status_KO;
							break;
					}
					break;
			}

			break;


		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_LectureModuleName:

			switch (OBS4xx_ModuleName_Read())
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_LectureModuleName = OK\n");
					_OBS4xx_CONSOLE("ModuleName = %s\n", OBS4xx.ModuleName);

					Etape = OBS4xx_ConfigEtape_LectureModuleAddress;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_LectureModuleName = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;


		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_LectureModuleAddress:

			switch (OBS4xx_ModuleAddress_Read())
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_LectureModuleAddress = OK\n");
					_OBS4xx_CONSOLE("ModuleAddress = %s\n", OBS4xx.ModuleAddress);

					if(OBS4xx.InitTargetBaudrateRequise == TRUE)
						Etape = OBS4xx_ConfigEtape_ConfigEscapeSeq;
					else
						Etape = OBS4xx_ConfigEtape_VerificationModuleType;

					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_LectureModuleAddress = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;


		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_VerificationModuleType:

			switch (OBS4xx_ModuleType_Verify())
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_LectureModuleType = OK\n");

					Etape = OBS4xx_ConfigEtape_EnvoiCommandesConfig;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_LectureModuleType = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_ConfigEscapeSeq:

			switch (OBS4xx_EnvoiTrame_WaitACK("AT*AMET=50,50,1\r"))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_ConfigEscapeSeq = OK\n");

					Etape = OBS4xx_ConfigEtape_ConfigTargetBaudrate;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_ConfigEscapeSeq = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_ConfigTargetBaudrate:

			_sprintf(buff, "AT*AMRS=%d,1,1,1,2,1,1\r", OBS4xx.TargetBaudrateEnum);

			switch (OBS4xx_EnvoiTrame_WaitACK(buff))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_ConfigTargetBaudrate = OK\n");

					HAL_BT_Init(OBS4xx.TargetBaudrate);
					_OBS4xx_CONSOLE("OBS4xx_Init COM %d\n", OBS4xx.TargetBaudrate);
					OBS4xx.InitTargetBaudrateRequise = FALSE;

					Etape = OBS4xx_ConfigEtape_EscapeSequence;	// Redemarrage de la configuration
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_ConfigTargetBaudrate = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_EnvoiCommandesConfig:

			switch (OBS4xx_EnvoiCommandesConfig())
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_EnvoiCommandesConfig = OK\n");

					Etape = OBS4xx_ConfigEtape_EntreeDataMode;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_EnvoiCommandesConfig = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_Disconnect:

			_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Disconnect\n");

			//OBS4xx_Send("AT*ADCC=0", CR);	// pas nécessaire
			TSW_Start(&TIMER_OBS4xx, 100);

			Etape = OBS4xx_ConfigEtape_EntreeDataMode;
			break;


		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_SetPin:

			switch (OBS4xx_Pin_Write())
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_SetPin = OK\n");

					// Efface la bonding list après le chgt de code PIN sauf si mode maintenance
					Etape = OBS4xx_ConfigEtape_ClearBondingList;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_SetPin = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_SetLocalName:

			switch (OBS4xx_ModuleName_Write())
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_SetLocalName = OK\n");

					Etape = OBS4xx_ConfigEtape_EntreeDataMode;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_SetLocalName = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_Discoverable:

			switch (OBS4xx_EnvoiTrame_WaitACK("AT*AGDM=3,0\r"))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Discoverable = OK\n");

					Etape = OBS4xx_ConfigEtape_EntreeDataMode;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Discoverable = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_Undiscoverable:

			switch (OBS4xx_EnvoiTrame_WaitACK("AT*AGDM=1,0\r"))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Undiscoverable = OK\n");

					Etape = OBS4xx_ConfigEtape_EntreeDataMode;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Undiscoverable = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_ClearBondingList:

			switch (OBS4xx_EnvoiTrame_WaitACK("AT*AGUB=FFFFFFFFFFFF\r"))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_ClearBondingList = OK\n");

					Etape = OBS4xx_ConfigEtape_EntreeDataMode;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_ClearBondingList = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_Connect:

			switch (OBS4xx_Connect(OBS4xx.TargetModuleAddress))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Connect = OK\n");

					//TODO Etape = OBS4xx_ConfigEtape_OK;
					Etape = OBS4xx_ConfigEtape_EntreeDataMode;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Connect = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;

			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_EntreeDataMode:

			// Attente fin tempo
			if (TSW_IsRunning(&TIMER_OBS4xx))
				break;

			switch (OBS4xx_EnvoiTrame_WaitACK("AT*ADDM\r"))
			{
				case Status_OK:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_EntreeDataMode = OK\n");

					Etape = OBS4xx_ConfigEtape_OK;
					break;

				case Status_KO:
					_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_EntreeDataMode = KO\n");

					Etape = OBS4xx_ConfigEtape_Error;
					break;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_OK:

			_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_OK\n");

			Status = Status_OK;

			break;

		//--------------------------------------------------------------
		case OBS4xx_ConfigEtape_Error:

			OBS4xx.NbTentative++;
//			if(OBS4xx.NbTentative < 2)
//			{
//				_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Error 1\n");
//				Etape = OBS4xx_ConfigEtape_EscapeSequence;
//			}
//			else
//			{
				_OBS4xx_CONSOLE("OBS4xx_ConfigEtape_Error\n");
				Status = Status_KO;
//			}
			break;

		//--------------------------------------------------------------
		default:
			break;

	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
	{
		OBS4xx.InitTargetBaudrateRequise	= FALSE;
		OBS4xx.ConfigMode					= OBS4xx_ConfigMode_AUCUN;
		OBS4xx.ConfigLastStatus				= Status;
		Etape								= OBS4xx_ConfigEtape_Init;
	}

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture du mode de configuration en cours.
 *
 */
uint8_t OBS4xx_GetConfigMode()
{
	return OBS4xx.ConfigMode;
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture du mode de configuration en cours.
 *
 */
uint8_t OBS4xx_GetLastConfigStatus()
{
	return OBS4xx.ConfigLastStatus;
}

/**-----------------------------------------------------------------------------
 * @brief	Reception des donnees issues du module BT OBS4xx.
 *
 */
uint8_t OBS4xx_GestionReception() {

	uint8_t data;

	switch (OBS4xx.EtatReception) {

		//--------------------------------------------------------------
		case OBS4xx_AttenteReception:

			if (OBS4xx_Read(&data, 1) != 0) {

				// Effacement buffer
				for (int i=0; i<OBS4xx_BUFFER_SIZE; i++)
					OBS4xx.Buffer[i] = 0;

				// Lecture donnee
				OBS4xx.iBuffer = 0;
				OBS4xx.Buffer[OBS4xx.iBuffer++] = data;

				// Lancement Timeout
				TSW_Start(&TIMER_OBS4xx_TimeoutRx, OBS4xx_TIMEOUT_RX_ms);

				// MAJ Etat
				OBS4xx.EtatReception = OBS4xx_ReceptionEnCours;
			}

			break;

		//--------------------------------------------------------------
		case OBS4xx_ReceptionEnCours:

			// Lecture des données
			while ((OBS4xx_Read(&data, 1) != 0)
			&& (OBS4xx.iBuffer < OBS4xx_BUFFER_SIZE))
			{
				// Lecture donnee
				OBS4xx.Buffer[OBS4xx.iBuffer++] = data;

				// Verification overflow
				if (OBS4xx.iBuffer >= OBS4xx_BUFFER_SIZE-1){
					OBS4xx.EtatReception = OBS4xx_ReceptionTerminee;
				}

				// Relance Timeout
				TSW_Start(&TIMER_OBS4xx_TimeoutRx, OBS4xx_TIMEOUT_RX_ms);
			}

			// TimeOut
			if (TSW_IsRunning(&TIMER_OBS4xx_TimeoutRx) == FALSE)
				OBS4xx.EtatReception = OBS4xx_ReceptionTerminee;

			break;

		//--------------------------------------------------------------
		case OBS4xx_ReceptionTerminee:

			#if OBS4xx__CONSOLE_RX_TX
				_OBS4xx_CONSOLE("OBS4xx Rx: \"");
				_OBS4xx_CONSOLE((char*) OBS4xx.Buffer);
				_OBS4xx_CONSOLE("\"\n");
			#endif

			OBS4xx.EtatReception = OBS4xx_AttenteReception;
			OBS4xx.Buffer[OBS4xx.iBuffer++] = '\0';
			return OBS4xx_FinReception;

			break;

		//--------------------------------------------------------------
		default:
			OBS4xx.EtatReception = OBS4xx_AttenteReception;
			break;
	}

	return OBS4xx.EtatReception;
}


/**-----------------------------------------------------------------------------
 * @brief	Attente de reception d'un ACK ("OK"), ou Timeout.
 *
 */
uint8_t OBS4xx_WaitForACK()
{
	uint8_t Status = Status_EnCours;

	// Attente reponse
	if (OBS4xx_GestionReception() == OBS4xx_FinReception)
	{
		if(strstr((char*) OBS4xx.Buffer, "\r\nOK\r\n") != 0)
			Status = Status_OK;
		else
			Status = Status_KO;
	}

	// Timeout
	if (TSW_IsRunning(&TIMER_OBS4xx_TimeoutReponse) == FALSE)
		Status = Status_KO;

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Envoi d'une commande vers le Module OBS4xx, et attente d'un "OK".
 *
 */
uint8_t OBS4xx_EnvoiTrame_WaitACK(char* Trame)
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;

	switch (Etape) {

		// Envoi Requete
		case 0:

			OBS4xx_Write(Trame, strlen(Trame));
			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);

			Etape++;
			break;

		// Attente Reponse
		case 1:
			Status = OBS4xx_WaitForACK();
			break;

		default:
			Etape = 0;
			break;

	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
		Etape = 0;

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Envoi de la sequence d'echappement (///).
 *
 */
uint8_t OBS4xx_EscapeSequence(uint16_t DelayBeforeAndAfter)
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;

	switch (Etape)
	{
		// Delai avant escape
		case 0:

			_OBS4xx_CONSOLE("OBS4xx_EscapeSequence %dms\n", DelayBeforeAndAfter);

			// Demarrage Tempo
			TSW_Start(&TIMER_OBS4xx, DelayBeforeAndAfter);

			Etape++;
			break;

		// Attente fin tempo et envoi sequence
		case 1:
			if (TSW_IsRunning(&TIMER_OBS4xx))
				break;

			// Envoi Séquence d'échappement
			OBS4xx_Write("///", 3);

			// Demarrage Tempo
			TSW_Start(&TIMER_OBS4xx, DelayBeforeAndAfter);

			Etape++;
			break;


		// Delai apres escape
		case 2:
			if (TSW_IsRunning(&TIMER_OBS4xx))
				break;

			Status = Status_OK;
			break;
	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
		Etape = 0;

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Verificaton du type de module BT (AT*AILTI?).
 *
 */
uint8_t OBS4xx_ModuleType_Verify()
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;

	switch (Etape) {

		// Init + Requete
		case 0:

			// Envoi requete
			OBS4xx_Write("AT*AILTI?\r", 10);

			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
			Etape++;
			break;

		// Reponse
		case 1:

			// Attente reponse
			if (OBS4xx_WaitForACK() == Status_OK)
			{
				Status = Status_KO;
				if (OBS4xx.iBuffer >= 9)
				{
					if (strncmp(&OBS4xx.Buffer[9], "1,8\r", 4) == 0)
					{
						_OBS4xx_CONSOLE("OBS4xx_ModuleType = OBS410\n");
						OBS4xx.ModuleType = OBS4xx_TypeOBS410;
						Status = Status_OK;
					}
					else if (strncmp(&OBS4xx.Buffer[9], "1,13", 4) == 0)
					{
						_OBS4xx_CONSOLE("OBS4xx_ModuleType = OBS418\n");
						OBS4xx.ModuleType = OBS4xx_TypeOBS418;
						Status = Status_OK;
					}
				}
			}

			// Timeout
			if (TSW_IsRunning(&TIMER_OBS4xx_TimeoutReponse) == FALSE)
				Status = Status_KO;

			break;

		default:
			Etape = 0;
			break;

	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
		Etape = 0;

	return Status;
}



/**-----------------------------------------------------------------------------
 * @brief	Lecture du nom du module OBS4xx (AT*AGLN?).
 *
 */
uint8_t OBS4xx_ModuleName_Read()
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;
	int i;

	switch (Etape) {

		// Init + Requete
		case 0:

			// Effacement donnees precedentes
			for (i=0; i<OBS4xx_ModuleName_SIZE; i++)
				OBS4xx.ModuleName[i] = 0;

			// Envoi requete
			OBS4xx_Write("AT*AGLN?\r", 9);

			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
			Etape++;
			break;

		// Reponse
		case 1:

			// Attente reponse
			if (OBS4xx_WaitForACK() == Status_OK)
			{
				if (OBS4xx.iBuffer >= 9)
				{
					i = 9;		// la réponse à AGLN commence à index=9
					while ((OBS4xx.Buffer[i] != '\"')
					&& (OBS4xx.Buffer[i] != '\r')
					&& (OBS4xx.Buffer[i] != '\0')
					&& (i-9 < OBS4xx_ModuleName_SIZE))
					{
						OBS4xx.ModuleName[i-9] = OBS4xx.Buffer[i];
						i++;
					}
					OBS4xx.ModuleName[i] = 0;
					OBS4xx.ModuleName[OBS4xx_ModuleName_SIZE] = 0;

					Status = Status_OK;
				}
				else
					Status = Status_KO;
			}

			// Timeout
			if (TSW_IsRunning(&TIMER_OBS4xx_TimeoutReponse) == FALSE)
				Status = Status_KO;

			break;

		default:
			Etape = 0;
			break;

	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
		Etape = 0;

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Ecriture du nom du module OBS4xx (AT*AGLN=).
 *
 */
uint8_t OBS4xx_ModuleName_Write()
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;

	switch (Etape) {

		// Init + Requete
		case 0:

			// Preparation requete
			OBS4xx.Buffer[0] = 0;
			strcat((char*) OBS4xx.Buffer, "AT*AGLN=\"");			// Ecrit le Nom de la Table
			strncat((char*) OBS4xx.Buffer, (char*) OBS4xx.ModuleName, OBS4xx_ModuleName_SIZE);
			strcat((char*) OBS4xx.Buffer, "\",1\r");

			// Envoi requete
			OBS4xx_Write(OBS4xx.Buffer, strlen(OBS4xx.Buffer));

			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
			Etape++;
			break;

		// Reponse
		case 1:
			// Attente reponse, ou Timeout
			Status = OBS4xx_WaitForACK();
			break;

		default:
			Etape = 0;
			break;
	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
		Etape = 0;

	return Status;
}

/**-----------------------------------------------------------------------------
 * @brief	Lecture de l'adresse du module OBS4xx (AT*AILBA?).
 *
 */
uint8_t OBS4xx_ModuleAddress_Read()
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;
	int i;

	switch (Etape) {

		// Init + Requete
		case 0:

			// Effacement donnees precedentes
			for (i=0; i<OBS4xx_ModuleAddress_SIZE; i++)
				OBS4xx.ModuleAddress[i] = 0;

			// Envoi requete
			OBS4xx_Write("AT*AILBA?\r", 10);

			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
			Etape++;
			break;

		// Reponse
		case 1:

			// Attente reponse
			if (OBS4xx_WaitForACK() == Status_OK)
			{
				if (OBS4xx.iBuffer >= 9)
				{
					i = 9;		// la réponse à AILBA commence à index=9
					while ((OBS4xx.Buffer[i] != '\r')
					&& (i-9 < OBS4xx_ModuleAddress_SIZE))
					{
						OBS4xx.ModuleAddress[i-9] = OBS4xx.Buffer[i];
						i++;
					}

					OBS4xx.ModuleAddress[OBS4xx_ModuleAddress_SIZE] = 0;

					Status = Status_OK;
				}
				else
					Status = Status_KO;
			}

			// Timeout
			if (TSW_IsRunning(&TIMER_OBS4xx_TimeoutReponse) == FALSE)
				Status = Status_KO;

			break;

		default:
			Etape = 0;
			break;

	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
		Etape = 0;

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Definition du code PIN (AT*AGFP).
 *
 */
uint8_t OBS4xx_Pin_Write()
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;

	switch (Etape) {

		// Init + Requete AGFP
		case 0:

			// Preparation trame
			OBS4xx.Buffer[0] = 0;
			strcat((char*) OBS4xx.Buffer, "AT*AGFP=\"");	// Ecrit un code PIN dans le module
			strcat((char*) OBS4xx.Buffer, (char*) OBS4xx.PIN);
			strcat((char*) OBS4xx.Buffer, "\",1\r");

			// Envoi requete
			OBS4xx_Write(OBS4xx.Buffer, strlen((char*) OBS4xx.Buffer));

			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
			Etape++;
			break;

		// Reponse AGFP
		case 1:

			// Attente reponse, ou Timeout
			if (OBS4xx_WaitForACK() == Status_OK)
			{
				//OBS4xx.Trame_TCDE = 4;						// phase de SET code PIN terminée
				Etape++;
				Status = Status_EnCours;

				TSW_Start(&TIMER_OBS4xx, 100);
			}

			// Timeout
			if (TSW_IsRunning(&TIMER_OBS4xx_TimeoutReponse) == FALSE)
				Status = Status_KO;

			break;

		// Requete AGFP2
		case 2:

			// Attente fin tempo
			if (TSW_IsRunning(&TIMER_OBS4xx))
				break;

			OBS4xx.Buffer[0] = 0;
#if 0
			if(OBS4xx.UsePinMaintenance == FALSE)
			{
				strcat(OBS4xx.Buffer, "AT*AGFP2=");
				Passkey = 0;
				for (int i=0;i<6;i++)				// du code pin de la tcde TFT
				{
					Passkey += DI_ConvAscii2Int(OBS4xx.PIN[i+6]);
					Passkey <<= 4;
				}
				Passkey &= 0x7FFFF;		// masque sur 19bits pour être entre 0 et 999999
				strcat(OBS4xx.Buffer, DI_ConvNbre2Chaine(Passkey));
				strcat(OBS4xx.Buffer, ",1");

				/*
				// Preparation trame
				for (int i=0; i<OBS4xx_PASSKEY_SIZE; i++)	// 6 derniers caractères du code PIN
					PIN2[i] = OBS4xx.PIN[OBS4xx_PINCODE_SIZE + i] = 0;
				PIN2[OBS4xx_PASSKEY_SIZE] = 0;

				_sprintf((char*) OBS4xx.Buffer, "AT*AGFP2=%d,", strtoul((char*) PIN2, NULL, 0));
				strcat((char*) OBS4xx.Buffer, "1\r");
				*/

			}
			else
			{
#endif
				// Forcage sécurité a 2 si en mode maintenance pour ne pas avoir a configurer AGFP2
				// le code pin est alors le même avec le module 310 ou 410.
				strcat((char*) OBS4xx.Buffer, "AT*AGSM=2,0\r");
			//}

			// Envoi requete
			OBS4xx_Write(OBS4xx.Buffer, strlen(OBS4xx.Buffer));

			TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
			Etape++;
			break;

		// Reponse AGFP2
		case 3:
			// Attente reponse, ou Timeout
			Status = OBS4xx_WaitForACK();
			break;

		default:
			Etape = 0;
			break;
	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
	{
		TSW_Stop(&TIMER_OBS4xx);
		Etape = 0;
	}

	return Status;
}


/**-----------------------------------------------------------------------------
 * @brief	Envoi des commandes de configuration du Module BT.
 *
 */
uint8_t OBS4xx_EnvoiCommandesConfig()
{
	static uint8_t Etape = 0;
	static uint8_t NumTrame = 0;
	static char* pTrame = NULL;
	uint8_t Status = Status_EnCours;

	switch (Etape) {

		// Init
		case 0:
			if (OBS4xx.Mode == OBS4xx_ModeClient)
				pTrame = (char*) OBS4xx_ConfigClient[0];
			else
				pTrame = (char*) OBS4xx_ConfigServeur[0];

			NumTrame = 0;
			Etape++;
			break;

		// Creation Requetes
		case 1:

			// Attente fin tempo
			if (TSW_IsRunning(&TIMER_OBS4xx))
				break;

			// Creation requete
			OBS4xx.Buffer[0] = 0;
			strcat((char*) OBS4xx.Buffer, "AT*");
			strcat((char*) OBS4xx.Buffer, pTrame);
			strcat((char*) OBS4xx.Buffer, "\r");

			Etape++;
			break;

		// Envoi Requetes et attente reponse
		case 2:

			// Envoi requete
			Status = OBS4xx_EnvoiTrame_WaitACK(OBS4xx.Buffer);

			// Passage a la trame suivante si status Ok
			if (Status == Status_OK)
			{
				NumTrame++;

				if (OBS4xx.Mode == OBS4xx_ModeClient)
					pTrame = (char*) OBS4xx_ConfigClient[NumTrame];
				else
					pTrame = (char*) OBS4xx_ConfigServeur[NumTrame];

				if (pTrame[0] == '\0')
					Status = Status_OK;
				else
				{
					TSW_Start(&TIMER_OBS4xx, 200);
					Etape = 1;
					Status = Status_EnCours;
				}
			}

			break;

		default:
			Etape = 0;
			break;

	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
	{
		Etape = 0;
	}

	return Status;
}

/**-----------------------------------------------------------------------------
 * @brief	Envoi de la sequence d'echappement (///).
 *
 */
uint8_t OBS4xx_Connect(char* Address)
{
	static uint8_t Etape = 0;
	uint8_t Status = Status_EnCours;
	char buff[64] = {0};

	switch (Etape)
	{
		case 0:
			_OBS4xx_CONSOLE("OBS4xx_Connect...\n");

			// Demarrage Tempo
			TSW_Start(&TIMER_OBS4xx, OBS4xx_TIMEOUT_CONNEXION_ms);

			// Envoi demande de connexion
			_sprintf(buff, "AT*ADCP=%s,0,0,0\r", Address);
			OBS4xx_Write(buff, strlen(buff));

			Etape++;
			break;

		case 1:

			// Attente reponse
			switch (OBS4xx_WaitForACK())
			{
				case Status_OK:
					Status = Status_OK;
					break;

				case Status_KO:
					// Relance systematique du timeout Rx general
					TSW_Start(&TIMER_OBS4xx_TimeoutReponse, OBS4xx_TIMEOUT_REPONSE_ms);
					break;
			}

			// Timeout
			if (TSW_IsRunning(&TIMER_OBS4xx) == FALSE)
				Status = Status_KO;

			break;
	}

	// Gestion de la fin de fonction
	if ((Status == Status_OK) || (Status == Status_KO))
	{
		TSW_Stop(&TIMER_OBS4xx);
		Etape = 0;
	}

	return Status;
}




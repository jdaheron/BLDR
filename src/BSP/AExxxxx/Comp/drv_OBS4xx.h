/**
 *******************************************************************************
 * @file	drv_OBS4xx.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	27 nov. 2013
 * @brief   Gestion du module BT OBS4xx.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef DRV_OBS4xx_H_
#define DRV_OBS4xx_H_


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"


/* Exported constants --------------------------------------------------------*/

#define OBS4xx_TIMEOUT_RX_ms		20
#define OBS4xx_TIMEOUT_REPONSE_ms	500
#define OBS4xx_TIMEOUT_CONNEXION_ms	5000
#define OBS4xx_BUFFER_SIZE			256

#define OBS4xx_ModuleName_SIZE		32//240
#define OBS4xx_ModuleAddress_SIZE	12
#define OBS4xx_PIN_SIZE				12


/* Exported types ------------------------------------------------------------*/

/** Mode de fonctionnement du Module OBS4xx. */
typedef enum {

	OBS4xx_ModeClient = 0,
	OBS4xx_ModeServeur,

} OBS4xx_Mode_e;


/** Etat du Module OBS4xx. */
typedef enum {

	OBS4xx_Initialize = 0,
	OBS4xx_Configuring,
	OBS4xx_DataMode,
	OBS4xx_Error,

} OBS4xx_Etat_e;

/** Etapes de configuration du Module BT. */
typedef enum {

	OBS4xx_ConfigEtape_Init = 0,
	OBS4xx_ConfigEtape_StartupDelay,

	OBS4xx_ConfigEtape_EscapeSequence,
	OBS4xx_ConfigEtape_EchoOff,

	OBS4xx_ConfigEtape_LectureModuleName,
	OBS4xx_ConfigEtape_LectureModuleAddress,
	OBS4xx_ConfigEtape_VerificationModuleType,
	OBS4xx_ConfigEtape_ConfigEscapeSeq,
	OBS4xx_ConfigEtape_ConfigTargetBaudrate,
	OBS4xx_ConfigEtape_EnvoiCommandesConfig,

	OBS4xx_ConfigEtape_Disconnect,
	OBS4xx_ConfigEtape_SetPin,
	OBS4xx_ConfigEtape_SetLocalName,
	OBS4xx_ConfigEtape_Discoverable,
	OBS4xx_ConfigEtape_Undiscoverable,
	OBS4xx_ConfigEtape_ClearBondingList,

	OBS4xx_ConfigEtape_Connect,
	OBS4xx_ConfigEtape_EntreeDataMode,

	OBS4xx_ConfigEtape_OK,
	OBS4xx_ConfigEtape_Error,

} OBS4xx_ConfigEtape_e;



/** Modes. */
typedef enum {

	OBS4xx_ConfigMode_AUCUN = 0,
	OBS4xx_ConfigMode_INIT_MODULE,
	OBS4xx_ConfigMode_CONNECT,
	OBS4xx_ConfigMode_DISCONNECT,
	OBS4xx_ConfigMode_SET_PIN,
	OBS4xx_ConfigMode_SET_LOCAL_NAME,
	OBS4xx_ConfigMode_DISCOVERABLE,
	OBS4xx_ConfigMode_UNDISCOVERABLE,
	OBS4xx_ConfigMode_CLEAR_BONDING_LIST,

} OBS4xx_ConfigMode_e;


/** Etat de la reception BT. */
typedef enum {

	OBS4xx_AttenteReception,
	OBS4xx_ReceptionEnCours,
	OBS4xx_ReceptionTerminee,
	OBS4xx_FinReception

} OBS4xx_EtatRx_e;

/** Type de module. */
typedef enum {

	OBS4xx_TypeInconnu = 0,
	OBS4xx_TypeOBS410,
	OBS4xx_TypeOBS418,

} OBS4xx_Type_e;

/** Definition de l'objet Template. */
typedef struct {

	OBS4xx_ConfigMode_e	ConfigMode;			/**< Mode de configuration du Module. */
	uint8_t				ConfigLastStatus;
	OBS4xx_Mode_e		Mode;

	uint32_t			TargetBaudrate;
	uint8_t				TargetBaudrateEnum;
	Bool_e				InitTargetBaudrateRequise;
	uint8_t				NbTentative;

	char				TargetModuleAddress[OBS4xx_ModuleAddress_SIZE+1];

	OBS4xx_Type_e		ModuleType;
	char				ModuleName[OBS4xx_ModuleName_SIZE+1];
	char				ModuleAddress[OBS4xx_ModuleAddress_SIZE+1];
	char				PIN[OBS4xx_PIN_SIZE+1];

	OBS4xx_EtatRx_e		EtatReception;
	char				Buffer[OBS4xx_BUFFER_SIZE];
	uint16_t			iBuffer;

} OBS4xx_t;


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void VALIDATION_SEQUENCE_IF_BT();

void OBS4xx_Init(uint32_t Baudrate, OBS4xx_Mode_e Mode);
void OBS4xx_Write(char* Buffer, uint16_t Taille);
uint16_t OBS4xx_Read(char* Buffer, uint16_t Taille) ;
uint8_t OBS4xx_Configure(uint8_t ConfigMode);
uint8_t OBS4xx_GetConfigMode();
uint8_t OBS4xx_GetLastConfigStatus();
void OBS4xx_Deconnecter();
void OBS4xx_Discoverable();
void OBS4xx_Undiscoverable();
void OBS4xx_Set_Pin(char* Pin);
void OBS4xx_Set_Name(char* Name);
void OBS4xx_Set_TargetAdress(char* TargetAdress);

char* OBS4xx_Get_Pin();
char* OBS4xx_Get_Name();
char* OBS4xx_Get_Address();

//void OBS4xx_RestaureDefaultBaudrate(uint32_t BaudratePrecedant);

#endif /* DRV_OBS4xx_H_ */





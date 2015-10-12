/**
 ***************************************************************************************************
 * @file	util_UART.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	16 juil. 2014
 * @brief   Gestion d'Uarts.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef UTIL_UART_H_
#define UTIL_UART_H_


/* Includes ***************************************************************************************/

#include "BSP.h"
#include "util_TSW.h"


/** @defgroup util_UART util_UART
  * @brief Gestion d'Uarts.
  * @{
  */ 

/** 
 ***************************************************************************************************
 * @defgroup Exported_Defines Exported Defines
 * @{
 */
 
#define EOF_MAX_SIZE	7
 /**
 * @}
 */ 
 
 
/** 
 ***************************************************************************************************
 * @defgroup Exported_TypesDefinitions Exported TypesDefinitions
 * @{
 */


/** Etat de la reception d'un UART. */
typedef enum
{
	UART_ATTENTE_RECEPTION = 0,		/**< Attente de reception.*/
	UART_RECEPTION_EN_COURS,		/**< Reception en cours.*/
	UART_TRAME_RECUE,				/**< Une trame est disponible.*/

} Uart_RxState_e;

/** Configuration de la reception UART. */
typedef struct
{
	// Config
	pFunction		pFct;					/**< Pointeur vers la fonction de lecture.*/
	uint8_t*		pBuffer;				/**< Pointeur vers le buffer de reception.*/
	uint16_t		BufferSize;				/**< Taille du buffer de reception.*/
	uint8_t			SOFcar;					/**< Caractere de debut de trame.*/
	uint8_t			EOFstr[EOF_MAX_SIZE];	/**< Chaine de fin de trame.*/
	uint8_t			EOFsize;				/**< Taille de la chaine.*/
	uint32_t		Timeout_ms;				/**< Duree de timeout reception (0 si inactif).*/

	// Etat
	uint16_t		RxSize;					/**< Taille des donnes recues.*/
	Uart_RxState_e	State;					/**< Etat de la reception.*/
	TSW_s			Timer;					/**< Timer de gestion de reception.*/

} UartRx_s;

/** Configuration de l'emission UART. */
typedef struct
{
	// Config
	pFunction		pFct; 					/**< Pointeur vers la fonction d'ecriture.*/

} UartTx_s;

/** Gestion de UART. */
typedef struct
{
	UartRx_s	Rx;	/**< Gestion des receptions.*/
	UartTx_s	Tx;	/**< Gestion des emissions.*/

} UartHandle_s;

/** Configuration de la reception UART. */
typedef struct
{
	// Initialisations
	pFunction		pInitFct;					/**< Pointeur vers la fonction d'initialisation.*/
	uint32_t		Baudrate;					/**< Debit en bauds.*/

	// Gestion des receptions
	pFunction		pRxFct;						/**< Pointeur vers la fonction de lecture.*/
	uint8_t*		pRxBuffer;					/**< Pointeur vers le buffer de reception.*/
	uint16_t		RxBufferSize;				/**< Taille du buffer de reception.*/
	uint8_t			RxSOFcar;					/**< Caractere de debut de trame.*/
	uint8_t			RxEOFstr[EOF_MAX_SIZE+1];	/**< Chaine de fin de trame.*/
	uint8_t			RxEOFsize;					/**< Taille de la chaine.*/
	uint32_t		RxTimeout_ms;				/**< Duree de timeout reception (0 si inactif).*/

	// Gestion des emissions
	pFunction		pTxFct;						/**< Pointeur vers la fonction d'ecriture.*/

} UartConfig_s;


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

/**
 * @brief	Fonction de validation du composant.
 * @return 	void
 */
void VALIDATION_UTIL_UART();

/**
 * @brief	Initialisation de l'UART.
 * @return 	void
 */
void
Uart_Init(
		UartHandle_s*	hUart,			/**<[in] Handle Uart.*/
		UartConfig_s*	pUartConfig		/**<[in] Pointeur vers la structure de configuration.*/
);

/**
 * @brief	Preparation a le reception d'une nouvelle trame.
 * @return 	void
 */
void
Uart_PreparerReception(
		UartHandle_s*	hUart	/**<[in] Handle Uart.*/
);

/**
 * @brief	Gestion de la reception.
 * @return 	void
 */
void
Uart_GererReception(
		UartHandle_s*	hUart	/**<[in] Handle Uart.*/
);

/**
 * @brief	Verification de trame recue.
 * @return 	void
 */
Bool_e
Uart_IsTrameRecue(
		UartHandle_s*	hUart	/**<[in] Handle Uart.*/
);

/**
 * @brief	Lecture de la trame recue.
 * @return 	void
 */
void
Uart_GetTrameRecue(
		UartHandle_s*	hUart,	/**<[in] Handle Uart.*/
		uint8_t**		pTrame,	/**<[in] Pointeur vers la trame recue.*/
		uint16_t*		pTaille	/**<[in] Pointeur vers la taille de trame recue.*/
);

/**
 * @brief	Envoi d'une trame.
 * @return 	void
 */
void
Uart_EnvoyerTrame(
		UartHandle_s*	hUart,	/**<[in] Handle Uart.*/
		uint8_t*		pTrame,	/**<[in] Pointeur vers la trame a emettre.*/
		uint16_t		Taille	/**<[in] Taille de trame a emettre.*/
);




 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* UTIL_UART_H_ */

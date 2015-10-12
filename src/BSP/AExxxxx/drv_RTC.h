/**
 ***************************************************************************************************
 * @file	drv_RTC.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	30 juil. 2014
 * @brief    Gestion de la RTC interne.
 ***************************************************************************************************
 */


/* Define to prevent recursive inclusion **********************************************************/

#ifndef DRV_RTC_H_
#define DRV_RTC_H_


/* Includes ***************************************************************************************/

#include "BSP.h"



/** @defgroup drv_RTC drv_RTC
  * @brief  Gestion de la RTC interne.
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

/** Liste des jours de la semaine. */
typedef enum
{
	LUNDI	 = 1,
	MARDI,
	MERCREDI,
	JEUDI,
	VENDREDI,
	SAMEDI,
	DIMANCHE,

} JourSemaine_e;

/** Liste des mois. */
typedef enum
{
	JANVIER	 = 1,
	FEVRIER,
	MARS,
	AVRIL,
	MAI,
	JUIN,
	JUILLET,
	AOUT,
	SEPTEMBRE,
	OCTOBRE,
	NOVEMBRE,
	DECEMBRE

} Mois_e;



/** Definition breve de la structure. */
typedef struct {

	Bool_e			Mode24H;
	JourSemaine_e	JourSemaine;
	uint16_t		Annee;
	Mois_e			Mois;
	uint8_t			Jour;
	uint8_t			Heure;
	uint8_t			Minute;
	uint8_t			Seconde;

} Horodatage_s;



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
 * @brief	Demarrage de l'initialisation de la RTC.
 * @return 	void
 */
void
RTC_StartInit(
		Bool_e IsQuartzPresent		/**<[in] Flag de presence du quartz externe.*/
);


/**
 * @brief	Gestion de la RTC.
 * @return 	void
 */
void
RTC_Main(
		void
);

/**
 * @brief	Lecture de la valeur actuelle de la RTC.
 * @return 	FALSE=RTC non disponible.
 */
Bool_e
RTC_Lire(
		Horodatage_s* pHorodatage
);

/**
 * @brief	Ecriture de la valeur de la RTC.
 * @return 	FALSE=RTC non disponible.
 */
Bool_e
RTC_ReglerDateHeure(
		JourSemaine_e	JourSemaine,
		uint8_t			Jour,
		Mois_e			Mois,
		uint16_t		Annee,
		uint8_t			Heure,
		uint8_t			Minute,
		uint8_t			Seconde,
		Bool_e			Mode24H
);

/**
 * @brief	Ecriture de la valeur de la RTC.
 * @return 	FALSE=RTC non disponible.
 */
Bool_e
RTC_Regler(
		Horodatage_s* pHorodatage
);

/**
 * @brief	Conversion d'un horodatage en Timestamp.
 * @return 	Timestamp.
 */
uint32_t RTC_GetTimestampFromHorodatage(
		Horodatage_s* pHorodatage
);

/**
 * @brief	Conversion d'un Timestamp en horodatage.
 * @return 	Horodatage.
 */
Horodatage_s RTC_GetHorodatageFromTimestamp(
		uint32_t Timestamp
);

/**
 * @brief	Lecture du Timestamp actuel.
 * @return 	Timestamp.
 */
uint32_t RTC_GetTimestamp(
		void
);

/**
 * @brief	Ecriture dans un registre du domaine de Backup RTC.
 * @return 	void
 */
void
RTC_BkpRegister_Write(
		uint32_t NumRegistre,
		int32_t Data
);

/**
 * @brief	Lecture d'un registre du domaine de Backup RTC.
 * @return	Valeur du registre lu.
 */
uint32_t
RTC_BkpRegister_Read(
		uint32_t NumRegistre
);

	 
 /**
 * @}
 */ 
 
 
 /**
 * @}
 */ 


#endif /* DRV_RTC_H_ */

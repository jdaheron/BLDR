/**
 ***************************************************************************************************
 * @file	util_CONSOLE.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	26 sept. 2014
 * @brief   Gestion de la Console.
 ***************************************************************************************************
 */


/* Includes ***************************************************************************************/

#include "util_CONSOLE.h"
#include "util_printf.h"
#include "util_TSW.h"


/* External Variables *****************************************************************************/


/** @addtogroup util_CONSOLE
  * @{
  */


 /**
 ***************************************************************************************************
 * @defgroup Private_TypesDefinitions Private TypesDefinitions
 * @{
 */

/**
 * @}
 */


 /**
 ***************************************************************************************************
 * @defgroup Private_Defines Private Defines
 * @{
 */

#define TAB_SIZE	10

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
 *
 */
void
_CONSOLE(
		const char*	Name,			/**<[in] Nom du Log (0=Pas de nom; (-1)=Log desactive).*/
		const char* Chaine,			/**<[in] Chaine de log.*/
		...							/**<[in] Parametres pour la chaine.*/
)
{
	va_list args;

	if (Name == NO_LOG)
		return;

	if (Name != NO_NAME)
	{
		// Troncation nom a "TAB_SIZE" caracteres
		char buff[TAB_SIZE+1];
		char NameOk[TAB_SIZE];

		// Formatage du nom
		if (strlen(Name) >= TAB_SIZE-3)
		{
			strncpy(NameOk, Name, TAB_SIZE-3);
			NameOk[TAB_SIZE-3] = 0;
		}
		else
			strcpy(NameOk, Name);

		_printf("[%07d]", TSW_GetTimestamp_ms());

		_sprintf(buff, "[%s]", NameOk);

		while (strlen(buff) < TAB_SIZE)
			strcat(buff, ".");


		// Envoi nom
		_printf("%s", buff);
	}

	// Envoi chaine
	va_start( args, Chaine);
	print( 0, Chaine, args);
}




 /**
 * @}
 */

/* End Of File ************************************************************************************/

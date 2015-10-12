/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "main.h"

#include "util_Goto.h"

#include <string.h>
#include <stdlib.h>

tSSIHandler ADC_Page_SSI_Handler;
uint32_t ADC_not_configured=1;
uint32_t ADC_IdChannel=0;

/* we will use character "t" as tag for CGI */
char const* TAGCHAR="t";
char const** TAGS=&TAGCHAR;

/* CGI handler for LED control */ 
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* StartUsb_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/main.cgi" will start MAIN_CGI_Handler */
const tCGI LEDS_CGI={"/Leds.cgi", LEDS_CGI_Handler};
const tCGI StartUsb_CGI={"/StartUsb.cgi", StartUsb_CGI_Handler};

/* Cgi call table, only one CGI used */
#define NB_CGI_HANDLER	2

tCGI CGI_TAB[NB_CGI_HANDLER];


/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
static void ADC_Configuration(void)
{
	MappingGpio_s PinConfig;

	// Lecture de la configuration de la pin
	PinConfig = GPIO_GetPinConfiguration(PORT_TRIM);

	// Execution de la fonction
	ADC1_ChannelConf_t ADC_Channel = {
			.ADCx				= (ADC_TypeDef*) PinConfig.PERIPH,
			.ADC_Channel		= PinConfig.PARAM,
			.pStoreValue_mV		= NULL,
			.pFct_CallbackEOC	= NULL, };

	ADC_IdChannel = ADC1_ChannelConfigure(ADC_Channel);
	ADC1_ConversionContinue_Activer(ADC_IdChannel);
}

/**
  * @brief  ADC_Handler : SSI handler for ADC page 
  */
u16_t ADC_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
	/* We have only one SSI handler iIndex = 0 */
	if (iIndex == 0)
	{
		char Digit1 = 0, Digit2 = 0, Digit3 = 0, Digit4 = 0;
		uint32_t ADCVal = 0;

		/* configure ADC if not yet configured */
		if (ADC_not_configured == 1)
		{
			ADC_Configuration();
			ADC_not_configured = 0;
		}

		/* get ADC conversion value */
		ADCVal = ADC1_Get_mV(ADC_IdChannel);

		/* get digits to display */

		Digit1 = ADCVal / 1000;
		Digit2 = (ADCVal - (Digit1 * 1000)) / 100;
		Digit3 = (ADCVal - ((Digit1 * 1000) + (Digit2 * 100))) / 10;
		Digit4 = ADCVal - ((Digit1 * 1000) + (Digit2 * 100) + (Digit3 * 10));

		/* prepare data to be inserted in html */
		*pcInsert = (char) (Digit1 + 0x30);
		*(pcInsert + 1) = (char) (Digit2 + 0x30);
		*(pcInsert + 2) = (char) (Digit3 + 0x30);
		*(pcInsert + 3) = (char) (Digit4 + 0x30);

		/* 4 characters need to be inserted in html*/
		return 4;
	}
	return 0;
}

/**
  * @brief  CGI handler for LEDs control 
  */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	uint32_t i = 0;

	if (iIndex == 0)
	{
		/* All leds off */
		GPIO_Set(PORT_STAT1, Etat_INACTIF);
		GPIO_Set(PORT_STAT2, Etat_INACTIF);
		GPIO_Set(PORT_STAT4, Etat_INACTIF);

		/* Check cgi parameter : example GET /leds.cgi?led=2&led=4 */
		for (i = 0; i < iNumParams; i++)
		{
			/* check parameter "led" */
			if (strcmp(pcParam[i], "led") == 0)
			{

				/* switch led1 ON if 1 */
				if (strcmp(pcValue[i], "1") == 0)
					GPIO_Set(PORT_STAT1, Etat_ACTIF);

				/* switch led2 ON if 2 */
				else if (strcmp(pcValue[i], "2") == 0)
					GPIO_Set(PORT_STAT2, Etat_ACTIF);

				/* switch led3 ON if 3 */
				else if (strcmp(pcValue[i], "3") == 0)
					__NOP();

				/* switch led4 ON if 4 */
				else if (strcmp(pcValue[i], "4") == 0)
					GPIO_Set(PORT_STAT4, Etat_ACTIF);
			}
		}
	}

	/* uri to send after cgi call*/
	return "/index.shtml";
}

/**
  * @brief  CGI handler for Start USB Bouton
  */
const char * StartUsb_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	uint32_t i = 0;

	if (iIndex == 1)
	{
		RTC_BkpRegister_Write(0, 1);
		GOTO(0);
	}

	/* uri to send after cgi call*/
	return "/index.shtml";
}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{  
  /* configure SSI handlers (ADC page SSI) */
  http_set_ssi_handler(ADC_Handler, (char const **)TAGS, 1);
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{ 
	/* configure CGI handlers (LEDs control CGI) */
	CGI_TAB[0] = LEDS_CGI;
	CGI_TAB[1] = StartUsb_CGI;
	http_set_cgi_handlers(CGI_TAB, NB_CGI_HANDLER);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

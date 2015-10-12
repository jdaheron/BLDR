/**
 *******************************************************************************
 * @file	BSP_CARTE.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	19 mars 2013
 * @brief   Board Support Package
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_CARTE_H_
#define BSP_CARTE_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "stm32f2xx.h"
#include "stm32f2xx_conf.h"


/* Exported constants --------------------------------------------------------*/

//TODO : Choix de la cible
#define USE_EVAL_BOARD_OLIMEX	0
#define USE_TARGET_BOARD		1

// Adresse de base pour les fonctionnalites de test
#define TEST_BASE_ADDRESS		0x60
#define TEST_BASE_RANGE			0x20
#define FAT_BASE_ADDRESS		0x80

// Adresse de l'ISP
#define ISP_ADDRESS				(uint32_t) 0x1FFF0000


/* Exported types ------------------------------------------------------------*/

/** Definition d'une pin (STM32F2xx type). */
typedef struct {

	GPIO_TypeDef*		pGPIOx;			// Pointeur vers le port
	uint16_t			Pin;			// Numero de pin
	uint8_t				Inverse;		// Inversion de l'entree ou de la sortie
	GPIOMode_TypeDef	GPIO_Mode;		// Mode
	uint32_t			GPIO_AF;		// Alternate function, NumADC
	GPIOSpeed_TypeDef	GPIO_Speed;		// Vitesse
	GPIOOType_TypeDef	GPIO_OType;		// (Si Mode OUT) Type de sortie
	GPIOPuPd_TypeDef	GPIO_PuPd;		// Activation PullUp ou PullDown
	Etat_e				EtatInit;		// Etat a l'initialisation
	uint32_t			PERIPH;			// Peripherique utilise (NumUART, NumAdc,...)
	uint32_t			PARAM;			// Parametres du peripherique (ADC_Channel, Tim_Channel, ...)s

} MappingGpio_s;


/* Mapping uC ----------------------------------------------------------------*/

#if USE_EVAL_BOARD_OLIMEX

/** Index du Mapping uC. */
typedef enum{

	PORT_WKUP						= 0	,


	/* FIN DU MAPPING */
	NB_PORT_GPIO,	// Doit etre conserve en fin d'enum pour connaitre la taille de la table de mapping uC.


	PORT_ETH_RMII_REF_CLK				,
	PORT_ETH_RMII_MDIO					,
	PORT_ETH_RMII_MDINT					,
	PORT_DCMI_HSYNC						,
	PORT_SPI1_SCK						,
	PORT_DCMI_PIXCLK					,
	PORT_ETH_RMII_CRS_DV				,
	PORT_MCO1							,
	PORT_OTG_FS_VBUS					,
	PORT_DCMI_D1						,
	PORT_OTG_FS_DM						,
	PORT_OTG_FS_DP						,
	PORT_TMS							,
	PORT_TCK							,
	PORT_TDI = PORT_I2S3_WS					,

	PORT_LCD_BL							,
	PORT_BUZ							,
	PORT_CAM_ENB						,
	PORT_TDO = PORT_I2S3_CK					,
	PORT_TRST = PORT_SPI1_MISO				,
	PORT_I2S3_SD						,
	PORT_DCMI_D5						,
	PORT_DCMI_VSYNC						,
	PORT_CAN1_RX						,
	PORT_CAN1_TX						,
	PORT_USB_FS_FAULT					,
	PORT_ETH_RMII_TX_EN					,
	PORT_OTH_HS_ID						,
	PORT_OTH_HS_VBUS					,
	PORT_OTH_HS_DM						,
	PORT_OTH_HS_DP						,

	PORT_TRIM							,
	PORT_ETH_RMII_MDC					,
	PORT_USB_FS_VBUSON					,
	PORT_SOFT_SPI_MOSI					,
	PORT_ETH_RMII_RXD0					,
	PORT_ETH_RMII_RXD1					,
	PORT_DCMI_D0 = PORT_USART6_TX			,
	PORT_I2S3_MCK						,
	PORT_SD_D0 = PORT_DCMI_D2				,
	PORT_SD_D1 = PORT_DCMI_D3				,
	PORT_SD_D2 = PORT_USART3_TX = PORT_SPI3_SCK	,
	PORT_SD_D3 = PORT_USART3_RX = PORT_SPI3_MISO,
	PORT_SD_CLK = PORT_SPI3_MOSI			,
	PORT_TAMPER							,

	PORT_D2								,
	PORT_D3								,
	PORT_SD_CMD							,
	PORT_LCD_RST						,
	PORT_nOE							,
	PORT_nWE							,
	PORT_LCD_CS							,
	PORT_NE1							,
	PORT_D13 = PORT_USART3_TX				,
	PORT_D14 = PORT_USART3_RX				,
	PORT_D15							,
	PORT_A16 = PORT_USART3_CTS				,
	PORT_A17 = PORT_USART3_RTS				,
	PORT_USB_HS_FAULT					,
	PORT_D0								,
	PORT_D1								,

	PORT_nBLE							,
	PORT_nBHE							,
	PORT_TEMP_ALERT						,
	PORT_USB_HS_VBUSON					,
	PORT_DCMI_D4						,
	PORT_DCMI_D6						,
	PORT_DCMI_D7						,
	PORT_D4								,
	PORT_D5								,
	PORT_D6								,
	PORT_D7								,
	PORT_D8								,
	PORT_D9								,
	PORT_D10							,
    PORT_D11							,
	PORT_D12							,

	PORT_A0								,
    PORT_A1								,
	PORT_A2								,
	PORT_A3								,
	PORT_A4								,
	PORT_A5								,
	PORT_STAT1							,
	PORT_STAT2 = PORT_CAN_CTRL				,
	PORT_STAT3 = PORT_CS_UEXT				,
	PORT_STAT4 = PORT_CAM_PWR				,
	PORT_ETH_RXER						,
	PORT_CAM_RST						,
	PORT_A6								,
	PORT_A7								,
	PORT_A8								,
	PORT_A9								,

	PORT_A10							,
	PORT_A11							,
	PORT_A12							,
	PORT_A13							,
	PORT_A14							,
	PORT_A15							,
	PORT_RIGHT							,
	PORT_UP								,
	PORT_DOWN							,
	PORT_USART6_RX						,
	PORT_SOFT_SCL						,
	PORT_LEFT							,
	PORT_SOFT_SDA						,
	PORT_ETH_RMII_TXD0					,
	PORT_ETH_RMII_TXD1					,
	PORT_CENT							,


} MappingGpio_e;


/** Mapping uC. */
static const MappingGpio_s MappingGpio[] = {
// DEBUT_MAPPING (Balise a conserver en debut de tableau)


/*									PORT	PIN		INVERSE		MODE			ALTERN. FCT		SPEED				OUTPUT TYPE		Pu-Pd				INIT			PERIPH				PARAM				*/
/*																																															- ADC_Channel		*/
/*																																															- TIM_Channel		*/
/* XX ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FIN DES PIN INITIALISEES ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


/* PORT_WKUP				*/	{	GPIOA,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_ETH_RMII_REF_CLK	*/	{	GPIOA,	 1	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_MDIO		*/	{	GPIOA,	 2	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_MDINT		*/	{	GPIOA,	 3	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_DCMI_HSYNC			*/	{	GPIOA,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SPI1_SCK			*/	{	GPIOA,	 5	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DCMI_PIXCLK			*/	{	GPIOA,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_ETH_RMII_CRS_DV		*/	{	GPIOA,	 7	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_MCO1				*/	{	GPIOA,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_OTG_FS_VBUS			*/	{	GPIOA,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_DOWN		, Etat_INACTIF	, NULL				, NULL				},
/* PORT_DCMI_D1				*/	{	GPIOA,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_OTG_FS_DM			*/	{	GPIOA,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_OTG_FS,	GPIO_Speed_100MHz,	NULL,			GPIO_PuPd_NOPULL	, NULL			, NULL				, NULL				},
/* PORT_OTG_FS_DP			*/	{	GPIOA,	12	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_OTG_FS,	GPIO_Speed_100MHz,	NULL,			GPIO_PuPd_NOPULL	, NULL			, NULL				, NULL				},
/* PORT_TMS					*/	{	GPIOA,	13	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_TCK					*/	{	GPIOA,	14	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_TDI					*/	{	GPIOA,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_I2S3_WS				*/	{	GPIOA,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO

/* PORT_LCD_BL				*/	{	GPIOB,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_BUZ					*/	{	GPIOB,	 1	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_CAM_ENB				*/	{	GPIOB,	 2	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_TDO					*/	{	GPIOB,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_I2S3_CK				*/	{	GPIOB,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_TRST				*/	{	GPIOB,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_SPI1_MISO			*/	{	GPIOB,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_I2S3_SD				*/	{	GPIOB,	 5	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DCMI_D5				*/	{	GPIOB,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DCMI_VSYNC			*/	{	GPIOB,	 7	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_CAN1_RX				*/	{	GPIOB,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_CAN1_TX				*/	{	GPIOB,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_USB_FS_FAULT		*/	{	GPIOB,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_ETH_RMII_TX_EN		*/	{	GPIOB,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_OTH_HS_ID			*/	{	GPIOB,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_OTH_HS_VBUS			*/	{	GPIOB,	13	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_OTH_HS_DM			*/	{	GPIOB,	14	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_OTH_HS_DP			*/	{	GPIOB,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO

/* PORT_TRIM				*/	{	GPIOC,	 0	,	FALSE	,	GPIO_Mode_AN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ADC1	, ADC_Channel_10	},/* PORT_ETH_RMII_MDC		*/	{	GPIOC,	 1	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_ETH_RMII_MDC		*/	{	GPIOC,	 1	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_USB_FS_VBUSON		*/	{	GPIOC,	 2	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SOFT_SPI_MOSI		*/	{	GPIOC,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_ETH_RMII_RXD0		*/	{	GPIOC,	 4	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_RXD1		*/	{	GPIOC,	 5	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_DCMI_D0				*/	{	GPIOC,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART6_TX			*/	{	GPIOC,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_I2S3_MCK			*/	{	GPIOC,	 7	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SD_D0				*/	{	GPIOC,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_DCMI_D2				*/	{	GPIOC,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SD_D1				*/	{	GPIOC,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_DCMI_D3				*/	{	GPIOC,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SD_D2				*/	{	GPIOC,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART3_TX			*/	{	GPIOC,	10	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) USART3	, NULL				},
or
/* PORT_SPI3_SCK			*/	{	GPIOC,	10	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_SPI3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) SPI3	, NULL				},
/* PORT_SD_D3				*/	{	GPIOC,	11	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART3_RX			*/	{	GPIOC,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) USART3	, NULL				},
or
/* PORT_SPI3_MISO			*/	{	GPIOC,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_SPI3,	GPIO_Speed_25MHz,	NULL,			GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) SPI3	, NULL				},
/* PORT_SD_CLK				*/	{	GPIOC,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_SPI3_MOSI			*/	{	GPIOC,	12	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_SPI3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) SPI3	, NULL				},
/* PORT_TAMPER				*/	{	GPIOC,	13	,	TRUE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},

/* PORT_D2					*/	{	GPIOD,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D3					*/	{	GPIOD,	 1	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SD_CMD				*/	{	GPIOD,	 2	,	TRUE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, NULL				, NULL				},
/* PORT_LCD_RST				*/	{	GPIOD,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_nOE					*/	{	GPIOD,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_nWE					*/	{	GPIOD,	 5	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_LCD_CS				*/	{	GPIOD,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_NE1					*/	{	GPIOD,	 7	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D13					*/	{	GPIOD,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART3_TX			*/	{	GPIOD,	 8	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) USART3	, NULL				},
/* PORT_D14					*/	{	GPIOD,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART3_RX			*/	{	GPIOD,	 9	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) USART3	, NULL				},
/* PORT_D15					*/	{	GPIOD,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A16					*/	{	GPIOD,	11	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART3_CTS			*/	{	GPIOD,	11	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A17					*/	{	GPIOD,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
or
/* PORT_USART3_RTS			*/	{	GPIOD,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_USB_HS_FAULT		*/	{	GPIOD,	13	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D0					*/	{	GPIOD,	14	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D1					*/	{	GPIOD,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO

/* PORT_nBLE				*/	{	GPIOE,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_nBHE				*/	{	GPIOE,	 1	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_TEMP_ALERT			*/	{	GPIOE,	 2	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_USB_HS_VBUSON		*/	{	GPIOE,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DCMI_D4				*/	{	GPIOE,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DCMI_D6				*/	{	GPIOE,	 5	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DCMI_D7				*/	{	GPIOE,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D4					*/	{	GPIOE,	 7	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D5					*/	{	GPIOE,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D6					*/	{	GPIOE,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D7					*/	{	GPIOE,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D8					*/	{	GPIOE,	11	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D9					*/	{	GPIOE,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D10					*/	{	GPIOE,	13	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D11					*/	{	GPIOE,	14	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_D12					*/	{	GPIOE,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO

/* PORT_A0					*/	{	GPIOF,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A1					*/	{	GPIOF,	 1	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A2					*/	{	GPIOF,	 2	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A3					*/	{	GPIOF,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A4					*/	{	GPIOF,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A5					*/	{	GPIOF,	 5	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_STAT1				*/	{	GPIOF,	 6	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_STAT2				*/	{	GPIOF,	 7	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
or
/* PORT_CAN_CTRL			*/	{	GPIOF,	 7	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_STAT3				*/	{	GPIOF,	 8	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
or
/* PORT_CS_UEXT				*/	{	GPIOF,	 8	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, NULL				, NULL				},
/* PORT_STAT4				*/	{	GPIOF,	 9	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
or
/* PORT_CAM_PWR				*/	{	GPIOF,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_ETH_RXER			*/	{	GPIOF,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_CAM_RST				*/	{	GPIOF,	11	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A6					*/	{	GPIOF,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A7					*/	{	GPIOF,	13	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A8					*/	{	GPIOF,	14	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A9					*/	{	GPIOF,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO

/* PORT_A10					*/	{	GPIOG,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A11					*/	{	GPIOG,	 1	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A12					*/	{	GPIOG,	 2	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A13					*/	{	GPIOG,	 3	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A14					*/	{	GPIOG,	 4	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_A15					*/	{	GPIOG,	 5	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_RIGHT				*/	{	GPIOG,	 6	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_UP					*/	{	GPIOG,	 7	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_DOWN				*/	{	GPIOG,	 8	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_USART6_RX			*/	{	GPIOG,	 9	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SOFT_SCL			*/	{	GPIOG,	10	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_LEFT				*/	{	GPIOG,	11	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_SOFT_SDA			*/	{	GPIOG,	12	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO
/* PORT_ETH_RMII_TXD0		*/	{	GPIOG,	13	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_TXD1		*/	{	GPIOG,	14	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_CENT				*/	{	GPIOG,	15	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},	//TODO

// FIN_MAPPING (Balise a conserver en fin de tableau)


};

#elif USE_TARGET_BOARD


/** Index du Mapping uC. */
typedef enum{

	PORT_CONSOLE_TX = 0,
	PORT_CONSOLE_RX,

	PORT_WKUP,
	PORT_TAMPER,

	PORT_USART3_TX,
	PORT_USART3_RX,

	PORT_STAT1,
	PORT_STAT2,
	PORT_STAT4,

	PORT_TRIM,

	PORT_SPI3_SCK,
	PORT_SPI3_MISO,
	PORT_SPI3_MOSI,
	PORT_SD_CMD,
	PORT_CS_UEXT,

	PORT_REL1_VENTILLATION,
	PORT_REL2_CHAUFFAGE,
	PORT_REL3,
	PORT_REL4,

	PORT_ETH_RMII_REF_CLK,
	PORT_ETH_RMII_MDIO,
	PORT_ETH_RMII_MDINT,
	PORT_ETH_RMII_CRS_DV,
	PORT_ETH_RMII_TX_EN,
	PORT_ETH_RMII_MDC,
	PORT_ETH_RMII_RXD0,
	PORT_ETH_RMII_RXD1,
    PORT_ETH_RMII_TXD0,
	PORT_ETH_RMII_TXD1,

	PORT_OTG_FS_DM,
	PORT_OTG_FS_DP,


	/* FIN DU MAPPING */
	NB_PORT_GPIO,	// Doit etre conserve en fin d'enum pour connaitre la taille de la table de mapping uC.


} MappingGpio_e;


/** Mapping uC. */
static const MappingGpio_s MappingGpio[] = {

/*									PORT	PIN		INVERSE		MODE			ALTERN. FCT		SPEED				OUTPUT TYPE		Pu-Pd				INIT			PERIPH				PARAM				*/
/*																																															- ADC_Channel		*/
/*																																															- TIM_Channel		*/
/* XX ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* PORT_CONSOLE_TX			*/	{	GPIOA,	 9	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART1,	GPIO_Speed_50MHz,	NULL,			GPIO_PuPd_NOPULL	, NULL			, (uint32_t) USART1	, NULL				},
/* PORT_CONSOLE_RX			*/	{	GPIOA,	 10	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART1,	GPIO_Speed_50MHz,	NULL,			GPIO_PuPd_NOPULL	, NULL			, (uint32_t) USART1	, NULL				},

/* PORT_WKUP				*/	{	GPIOA,	 0	,	FALSE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_TAMPER				*/	{	GPIOC,	13	,	TRUE	,	GPIO_Mode_IN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},

/* PORT_USART3_TX			*/	{	GPIOD,	 8	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) USART3	, NULL				},
/* PORT_USART3_RX			*/	{	GPIOD,	 9	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_USART3,	GPIO_Speed_25MHz,	GPIO_OType_PP,	GPIO_PuPd_UP		, Etat_INACTIF	, (uint32_t) USART3	, NULL				},

/* PORT_STAT1				*/	{	GPIOF,	 6	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_STAT2				*/	{	GPIOF,	 7	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_STAT4				*/	{	GPIOF,	 9	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},

/* PORT_TRIM				*/	{	GPIOC,	 0	,	FALSE	,	GPIO_Mode_AN,	NULL,			GPIO_Speed_2MHz,	NULL,			GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ADC1	, ADC_Channel_10	},

/* PORT_SPI3_SCK			*/	{	GPIOC,	10	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_SPI3,	GPIO_Speed_50MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, NULL			, (uint32_t) SPI3	, NULL				},
/* PORT_SPI3_MISO			*/	{	GPIOC,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_SPI3,	GPIO_Speed_50MHz,	NULL,			GPIO_PuPd_UP		, NULL			, (uint32_t) SPI3	, NULL				},
/* PORT_SPI3_MOSI			*/	{	GPIOC,	12	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_SPI3,	GPIO_Speed_50MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, NULL			, (uint32_t) SPI3	, NULL				},
/* PORT_SD_CMD				*/	{	GPIOD,	 2	,	TRUE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_50MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, NULL			, NULL				, NULL				},
/* PORT_CS_UEXT				*/	{	GPIOF,	 8	,	TRUE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_50MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},

/* PORT_REL1_VENTILLATION	*/	{	GPIOE,	 0	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_REL2_CHAUFFAGE		*/	{	GPIOE,	 1	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_REL3				*/	{	GPIOE,	 2	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},
/* PORT_REL4				*/	{	GPIOE,	 3	,	FALSE	,	GPIO_Mode_OUT,	NULL,			GPIO_Speed_2MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, NULL				, NULL				},



/* PORT_ETH_RMII_REF_CLK	*/	{	GPIOA,	 1	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_MDIO		*/	{	GPIOA,	 2	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_MDINT		*/	{	GPIOA,	 3	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_CRS_DV		*/	{	GPIOA,	 7	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},

/* PORT_ETH_RMII_TX_EN		*/	{	GPIOB,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},

/* PORT_ETH_RMII_MDC		*/	{	GPIOC,	 1	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_RXD0		*/	{	GPIOC,	 4	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_RXD1		*/	{	GPIOC,	 5	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},

/* PORT_ETH_RMII_TXD0		*/	{	GPIOG,	13	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},
/* PORT_ETH_RMII_TXD1		*/	{	GPIOG,	14	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_ETH,	GPIO_Speed_100MHz,	GPIO_OType_PP,	GPIO_PuPd_NOPULL	, Etat_INACTIF	, (uint32_t) ETH	, NULL				},

/* PORT_OTG_FS_DM			*/	{	GPIOA,	11	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_OTG_FS,	GPIO_Speed_100MHz,	NULL,			GPIO_PuPd_NOPULL	, NULL			, NULL				, NULL				},
/* PORT_OTG_FS_DP			*/	{	GPIOA,	12	,	FALSE	,	GPIO_Mode_AF,	GPIO_AF_OTG_FS,	GPIO_Speed_100MHz,	NULL,			GPIO_PuPd_NOPULL	, NULL			, NULL				, NULL				},

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FIN DES PIN INITIALISEES ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

};

#endif

//#define PORT_CONSOLE_TX		USART1_TX
//#define PORT_CONSOLE_RX		USART1_RX


/* Inclusion des peripheriques -----------------------------------------------*/

// Peripheriques internes
#include "drv_ADC1.h"
#include "drv_CAN.h"
#include "drv_CAN1.h"
#include "drv_CAN2.h"
#include "drv_FLASH.h"
#include "drv_FSMC.h"
#include "drv_I2C1.h"
#include "drv_RTC.h"
#include "drv_SPI1.h"
#include "drv_SPI2.h"
#include "drv_SPI3.h"
#include "drv_TEST.h"
#include "drv_TIM1.h"
#include "drv_TIM2.h"
#include "drv_TIM4.h"
#include "drv_TIM8.h"
#include "drv_TIM12.h"
#include "drv_TIM14.h"
#include "drv_USART1.h"
#include "drv_USART2.h"
#include "drv_USART3.h"
#include "drv_UART4.h"
#include "drv_UART5.h"
#include "drv_USART6.h"
#include "drv_WDG.h"

// Peripheriques externes
//#include "drv_SdCard_SPI.h"


/* Exported macro ------------------------------------------------------------*/

// CONSOLE
#define HAL_Console_Init			USART3_Init
#define HAL_Console_Read			USART3_Read
#define HAL_Console_Write			USART3_Write

// COMMUNICATION PC
#define HAL_PC_Init					USART6_Init
#define HAL_PC_DeInit				USART6_DeInit
#define HAL_PC_Read					USART6_Read
#define HAL_PC_Write				USART6_Write

// CARTE SD SUR SPI3
#define HAL_SdSpi_Init(x)			SPI3_Init(x)
#define HAL_SdSpi_Deinit			SPI3_DeInit
#define HAL_SdSpi_Read				SPI3_Read
#define HAL_SdSpi_Write(x) 			SPI3_Write(x)
#define HAL_SdSpi_Select()			GPIO_Set(PORT_SD_CMD, Etat_ACTIF)
#define HAL_SdSpi_Deselect()		GPIO_Set(PORT_SD_CMD, Etat_INACTIF)

// TEMPERATURE (MAX6675) SUR SPI3
#define HAL_MAX6675_Init(x)			SPI3_Init(x)
#define HAL_MAX6675_Deinit			SPI3_DeInit
#define HAL_MAX6675_Read			SPI3_Read
#define HAL_MAX6675_Write(x)		SPI3_Write(x)
#define HAL_MAX6675_Select()		GPIO_Set(PORT_CS_UEXT, Etat_ACTIF)
#define HAL_MAX6675_Deselect()		GPIO_Set(PORT_CS_UEXT, Etat_INACTIF)

/* Exported functions ------------------------------------------------------- */

// GENERAL
void BSP_Init();
void BSP_DeInit();
void STOP_MODE();
void BSP_ClockOutput_Init();

// GPIO
void GPIO_InitPin(MappingGpio_e PinId);
void GPIO_Set(MappingGpio_e PinId, Etat_e Etat);
Etat_e GPIO_Get(MappingGpio_e PinId);
MappingGpio_s GPIO_GetPinConfiguration(MappingGpio_e PinId);

// TICK
void Delay_ms(uint32_t delay_ms);


#endif /* BSP_CARTE_H_ */





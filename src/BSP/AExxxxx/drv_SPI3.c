/**
 *******************************************************************************
 * @file	drv_SPI3.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 oct. 2013
 * @brief   Gestion du SPI3.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_SPI3.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Initialisation.
 *
 */
void SPI3_Init(uint32_t Vitesse_kHz) {

	SPI_InitTypeDef  xSPI_Init = {

				.SPI_Direction			= SPI_Direction_2Lines_FullDuplex,
				.SPI_Mode				= SPI_Mode_Master,
				.SPI_DataSize			= SPI_DataSize_8b,
				.SPI_CPOL				= SPI_CPOL_Low,
				.SPI_CPHA				= SPI_CPHA_1Edge,
				.SPI_NSS				= SPI_NSS_Soft,
				.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_4,
				.SPI_FirstBit			= SPI_FirstBit_MSB,
				.SPI_CRCPolynomial		= 7,
	};

	// Lecture horloge Bus
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	// Calcul Prescaler
	uint32_t Calcul_Prescaler = RCC_Clocks.PCLK1_Frequency / 1000;
	Calcul_Prescaler /= Vitesse_kHz;

	if (Calcul_Prescaler <= 2)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	else if (Calcul_Prescaler <= 4)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	else if (Calcul_Prescaler <= 8)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	else if (Calcul_Prescaler <= 16)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	else if (Calcul_Prescaler <= 32)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	else if (Calcul_Prescaler <= 64)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	else if (Calcul_Prescaler <= 128)
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	else
		xSPI_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;

	// Activation horloges
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	SPI_Init(SPI3, &xSPI_Init);
	SPI_CalculateCRC(SPI3, DISABLE);
	SPI_Cmd(SPI3, ENABLE);

	/* Drain SPI */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI3);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void SPI3_DeInit(){

	SPI_I2S_DeInit(SPI3);
	SPI_Cmd(SPI3, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, DISABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * return	Data	Donnee lue pendant l'ecriture.
 */
uint8_t SPI3_Write(uint8_t Data) {

	/* Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI peripheral */
	SPI_I2S_SendData(SPI3, Data);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * return	Data	Donnee lue.
 */
uint8_t SPI3_Read() {

	return SPI3_Write(0xff);

}


/*----------------------------------------------------------------------------*/
/**
  * @brief  SPI3_IRQHandler
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}






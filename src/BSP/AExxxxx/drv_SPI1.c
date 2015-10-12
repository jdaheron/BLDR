/**
 *******************************************************************************
 * @file	drv_SPI1.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 oct. 2013
 * @brief   Gestion du SPI1.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_SPI1.h"


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
void SPI1_Init(uint32_t Vitesse_kHz) {

	SPI_InitTypeDef  xSPI_Init = {

				.SPI_Direction			= SPI_Direction_2Lines_FullDuplex,
				.SPI_Mode				= SPI_Mode_Master,
				.SPI_DataSize			= SPI_DataSize_8b,
				.SPI_CPOL				= SPI_CPOL_Low,
				.SPI_CPHA				= SPI_CPHA_1Edge,
				.SPI_NSS				= SPI_NSS_Soft,
				.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2,
				.SPI_FirstBit			= SPI_FirstBit_MSB,
				.SPI_CRCPolynomial		= 7,
	};

	// Lecture horloge Bus
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	// Calcul Prescaler
	uint32_t Calcul_Prescaler = RCC_Clocks.PCLK2_Frequency / 1000;
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
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_Init(SPI1, &xSPI_Init);
	SPI_CalculateCRC(SPI1, DISABLE);
	SPI_Cmd(SPI1, ENABLE);

	/* Drain SPI */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI1);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void SPI1_DeInit(){

	SPI_I2S_DeInit(SPI1);
	SPI_Cmd(SPI1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * return	Data	Donnee lue pendant l'ecriture.
 */
uint8_t SPI1_Write(uint8_t Data) {

	/* Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI peripheral */
	SPI_I2S_SendData(SPI1, Data);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * return	Data	Donnee lue.
 */
uint8_t SPI1_Read() {

	return SPI1_Write(0xff);

}


/*----------------------------------------------------------------------------*/
/**
  * @brief  SPI1_IRQHandler
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}






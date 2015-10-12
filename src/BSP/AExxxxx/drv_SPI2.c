/**
 *******************************************************************************
 * @file	drv_SPI2.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 oct. 2013
 * @brief   Gestion du SPI2.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"
#include "drv_SPI2.h"


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
void SPI2_Init(uint32_t Vitesse_kHz) {

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
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_Init(SPI2, &xSPI_Init);
	SPI_CalculateCRC(SPI2, DISABLE);
	SPI_Cmd(SPI2, ENABLE);

	/* Drain SPI */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);
}


/**-----------------------------------------------------------------------------
 * @brief	Desinitialisation.
 *
 */
void SPI2_DeInit(){

	SPI_I2S_DeInit(SPI2);
	SPI_Cmd(SPI2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * return	Data	Donnee lue pendant l'ecriture.
 */
uint8_t SPI2_Write(uint8_t Data) {

	/* Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI peripheral */
	SPI_I2S_SendData(SPI2, Data);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
}


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un octet.
 *
 * return	Data	Donnee lue.
 */
uint8_t SPI2_Read() {

	return SPI2_Write(0xff);

}


/*----------------------------------------------------------------------------*/
/**
  * @brief  SPI2_IRQHandler
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
	/* Go to infinite loop */
	while (1)
	{}
}






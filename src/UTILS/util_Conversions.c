/**
 *******************************************************************************
 * @file	util_Conversions.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 avr. 2013
 * @brief   Divers fonctions de conversions.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"
#include "util_Conversions.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Conversion ASCII-hexadecimal.
 *
 */
uint8_t Conv_ASCII_to_Hexa(uint8_t MSB, uint8_t LSB) {

	uint8_t hexa;

	// MSB
	if((MSB >= '0') && (MSB <= '9'))	hexa = MSB - 0x30;
	if((MSB >= 'A') && (MSB <= 'F'))	hexa = MSB - 0x41 + 10;
	hexa <<= 4;

	// LSB
	if((LSB >= '0') && (LSB <= '9'))	hexa += LSB - 0x30;
	if((LSB >= 'A') && (LSB <= 'F'))	hexa += LSB - 0x41 + 10;

	return hexa;
}



/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un buffer d'octets en entier 32bits.
 *
 */
uint32_t Conv_pu8_to_u32(uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First) {

	uint32_t tmp_u32;

	if (LSB_MSB_First == LSB_First)
	{
		tmp_u32  = ((pu8[3] << 24) & 0xFF000000);
		tmp_u32 += ((pu8[2] << 16) & 0x00FF0000);
		tmp_u32 += ((pu8[1] << 8 ) & 0x0000FF00);
		tmp_u32 += ( pu8[0]        & 0x000000FF);
	}
	else
	{
		tmp_u32  = ((pu8[0] << 24) & 0xFF000000);
		tmp_u32 += ((pu8[1] << 16) & 0x00FF0000);
		tmp_u32 += ((pu8[2] << 8 ) & 0x0000FF00);
		tmp_u32 += ( pu8[3]        & 0x000000FF);
	}

	return tmp_u32;
}

/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un buffer d'octets en entier 16bits.
 *
 */
uint16_t Conv_pu8_to_u16(uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First) {

	uint16_t tmp_u16;

	if (LSB_MSB_First == LSB_First)
	{
		tmp_u16  = ((pu8[1] << 8 ) & 0xFF00);
		tmp_u16 += ( pu8[0]        & 0x00FF);
	}
	else
	{
		tmp_u16  = ((pu8[0] << 8 ) & 0xFF00);
		tmp_u16 += ( pu8[1]        & 0x00FF);
	}

	return tmp_u16;
}

/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un entier 32bits en buffer d'octets.
 *
 */
void Conv_u32_to_pu8(uint32_t u32, uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First) {

	if (LSB_MSB_First == LSB_First)
	{
		pu8[3] = (uint8_t) (u32 >> 24);
		pu8[2] = (uint8_t) (u32 >> 16);
		pu8[1] = (uint8_t) (u32 >> 8);
		pu8[0] = (uint8_t) (u32);
	}
	else
	{
		pu8[0] = (uint8_t) (u32 >> 24);
		pu8[1] = (uint8_t) (u32 >> 16);
		pu8[2] = (uint8_t) (u32 >> 8);
		pu8[3] = (uint8_t) (u32);
	}
}


/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un entier 16bits en buffer d'octets.
 *
 */
void Conv_u16_to_pu8(uint16_t u16, uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First) {

	if (LSB_MSB_First == LSB_First)
	{
		pu8[1] = (uint8_t) (u16 >> 8);
		pu8[0] = (uint8_t) (u16);
	}
	else
	{
		pu8[0] = (uint8_t) (u16 >> 8);
		pu8[1] = (uint8_t) (u16);
	}
}

/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un flottant vers un entier 32bits.
 *
 */
int32_t	Conv_float_to_int32(float value)
{
	int32_t int32 = (int32_t) value;	// Recuperation de la valeur entiere

	if (value >= 0)		// Traitement des positifs et nulls
	{
		if (((value - int32) * 10) >= 5)
			int32++;
	}
	else				// Traitement des negatifs
	{
		if (((value - int32) * 10) <= (-5))
			int32--;
	}

	return int32;
}



/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un flottant vers un entier 32bits.
 *
 */
int32_t	Conv_Arrondir_int(int32_t value, int32_t diviseur)
{
	return Conv_float_to_int32(((float) value / diviseur));
}



/**-----------------------------------------------------------------------------
 * @brief	Conversion d'un flottant vers un entier 32bits.
 *
 */
float Conv_Absolu_float(float value)
{
	if(value >= 0)
		return value;
	else
		return (-value);
}


/**-----------------------------------------------------------------------------
 * @brief	Conversion d'une version Sw Inovelec vers une version client.
 *
 */
void Conv_VersionSW_to_VClient(char* VersionSw, char* VersionClient)
{
	uint8_t Major, Minor, Build;
	uint8_t iVersionSw;

	if (strncmp(VersionSw, "AL", 2) == 0)
		iVersionSw = 2;
	else
		iVersionSw = 0;

	// On se positionne en debut des lettres
	while ((VersionSw[iVersionSw] < 'A') || (VersionSw[iVersionSw] > 'Z'))
		iVersionSw++;

	Major = VersionSw[iVersionSw] - 'A' + 1;
	iVersionSw++;

	Minor = VersionSw[iVersionSw] - 'A';
	iVersionSw++;

	if (VersionSw[iVersionSw] != 0)
	{
		Build = VersionSw[iVersionSw] - 'A';
		_sprintf(VersionClient, "%d.%02d.%02d", Major, Minor, Build);
	}
	else
		_sprintf(VersionClient, "%d.%02d", Major, Minor);
}




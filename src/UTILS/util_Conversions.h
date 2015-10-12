/**
 *******************************************************************************
 * @file	util_Conversions.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	22 avr. 2013
 * @brief   Divers fonctions de conversions.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UTIL_CONVERSIONS_H_
#define UTIL_CONVERSIONS_H_


/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/

typedef enum {
	LSB_First = 0,
	MSB_First,
} LSB_MSB_First_e;


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

uint8_t		Conv_ASCII_to_Hexa(uint8_t MSB, uint8_t LSB);
uint32_t	Conv_pu8_to_u32(uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First);
uint16_t	Conv_pu8_to_u16(uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First);
void		Conv_u32_to_pu8(uint32_t u32, uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First);
void		Conv_u16_to_pu8(uint16_t u16, uint8_t* pu8, LSB_MSB_First_e LSB_MSB_First);
int32_t		Conv_float_to_int32(float value);
int32_t		Conv_Arrondir_int(int32_t value, int32_t diviseur);
float		Conv_Absolu_float(float value);
void		Conv_VersionSW_to_VClient(char* VersionSw, char* VersionClient);


#endif /* UTIL_CONVERSIONS_H_ */

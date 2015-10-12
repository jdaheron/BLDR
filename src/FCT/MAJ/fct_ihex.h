/**
 *******************************************************************************
 * @file	fct_ihex
 * @author	j.daheron
 * @version	1.0.0
 * @date	12 fev. 2014
 * @brief   Gestion des fichiers Intel Hex.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef FCT_IHEX_H
#define FCT_IHEX_H


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"
#include "ff.h"



/* Exported types ------------------------------------------------------------*/

/* General definition of the Intel HEX8 specification */
enum _IHexDefinitions {
	/* 768 should be plenty of space to read in a Intel HEX8 record */
	IHEX_RECORD_BUFF_SIZE	= 100,
	/* Offsets and lengths of various fields in an Intel HEX8 record */
	IHEX_COUNT_OFFSET		= 1,
	IHEX_COUNT_LEN			= 2,
	IHEX_ADDRESS_OFFSET		= 3,
	IHEX_ADDRESS_LEN		= 4,
	IHEX_TYPE_OFFSET		= 7,
	IHEX_TYPE_LEN			= 2,
	IHEX_DATA_OFFSET		= 9,
	IHEX_CHECKSUM_LEN		= 2,
	IHEX_MAX_DATA_LEN		= 64,
	/* Ascii hex encoded length of a single byte */
	IHEX_ASCII_HEX_BYTE_LEN	= 2,
	/* Start code offset and value */
	IHEX_START_CODE_OFFSET	= 0,
	IHEX_START_CODE			= ':',
};


/**
 * All possible error codes the Intel HEX8 record utility functions may return.
 */
enum IHexErrors {
	IHEX_OK							= 0, 	/**< Error code for success or no error. */
	IHEX_ERROR_FILE					= -1, 	/**< Error code for error while reading from or writing to a file. You may check errno for the exact error if this error code is encountered. */
	IHEX_ERROR_EOF					= -2, 	/**< Error code for encountering end-of-file when reading from a file. */
	IHEX_ERROR_INVALID_RECORD		= -3, 	/**< Error code for error if an invalid record was read. */
	IHEX_ERROR_INVALID_ARGUMENTS	= -4, 	/**< Error code for error from invalid arguments passed to function. */
	IHEX_ERROR_NEWLINE				= -5, 	/**< Error code for encountering a newline with no record when reading from a file. */
};

/**
 * Intel HEX8 Record Types 00-05
 */
enum IHexRecordTypes {
	IHEX_TYPE_00 = 0,	/**< Data Record */
	IHEX_TYPE_01,		/**< End of File Record */
	IHEX_TYPE_02,		/**< Extended Segment Address Record */
	IHEX_TYPE_03,		/**< Start Segment Address Record */
	IHEX_TYPE_04,		/**< Extended Linear Address Record */
	IHEX_TYPE_05,		/**< Start Linear Address Record */
};

enum IHexRecordCustomTypes {
	IHEX_TYPE_DATA					= 0x00,		/**< Data Record */
	IHEX_TYPE_EOF					= 0x01,		/**< End of File Record */
	IHEX_TYPE_EXT_SEG_ADDR			= 0x02,		/**< Extended Segment Address Record */
	IHEX_TYPE_START_SEG_ADDR		= 0x03,		/**< Start Segment Address Record */
	IHEX_TYPE_EXT_LIN_ADDR			= 0x04,		/**< Extended Linear Address Record */
	IHEX_TYPE_START_LIN_ADDR		= 0x05,		/**< Start Linear Address Record */

	IHEX_TYPE_SWP_INFO				= 0x10,		/**< SwPackage Information */
	IHEX_TYPE_SWP_NB_SW				= 0x11,		/**< SwPackage Nombre de SW */
	IHEX_TYPE_SW_INFO				= 0x12,		/**< Sw Information */
	IHEX_TYPE_SW_OFFSET				= 0x13,		/**< Sw Offset (dans le fichier) */
	IHEX_TYPE_SW_SIZE				= 0x14,		/**< Sw Size */
	IHEX_TYPE_SW_CHECKSUM			= 0x15,		/**< Sw Checksum */
	IHEX_TYPE_SWP_HEADERCHECKSUM	= 0x16,		/**< Checksum de l'entete */
	IHEX_TYPE_SWP_EOF				= 0x17,		/**< SwPackage Information */
};


/**
 * Structure to hold the fields of an Intel HEX8 record.
 */
typedef struct {
	uint16_t	address; 					/**< The 16-bit address field. */
	uint8_t		data[IHEX_MAX_DATA_LEN/2];	/**< The 8-bit array data field, which has a maximum size of 256 bytes. */
	int			dataLen;					/**< The number of bytes of data stored in this record. */
	int			type;						/**< The Intel HEX8 record type of this record. */
	uint8_t		checksum;					/**< The checksum of this record. */
} IHexRecord;


/* Exported constants --------------------------------------------------------*/

#define IHEX_LIGHT


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void VALIDATION_SEQUENCE_VALIDATION_FCT_IHEX();

int		IHex_ReadRecordFile(IHexRecord* pRecord, FIL* pFile, uint16_t* br, uint32_t* pChecksumLine );
int		IHex_ReadRecord(IHexRecord* pRecord, char* Buffer, uint16_t BufferSize, uint16_t* br);
uint8_t	IHex_CalculRecordChecksum(const IHexRecord *ihexRecord);
uint8_t IHex_GetFileInfo(FIL *pFile, uint32_t* pStartAddress, uint32_t* pEndAddress, uint32_t* pDataChecksum);
void	IHex_PrintRecord(const IHexRecord *ihexRecord);

#ifndef IHEX_LIGHT
int	New_IHexRecord(int type, uint16_t address, const uint8_t *data, int dataLen, IHexRecord *ihexRecord);
int	Write_IHexRecord(const IHexRecord *ihexRecord, FIL *out);
#endif


#endif /* FCT_IHEX_H */

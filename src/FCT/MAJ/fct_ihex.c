/**
 *******************************************************************************
 * @file	fct_ihex
 * @author	j.daheron
 * @version	1.0.0
 * @date	12 fev. 2014
 * @brief   Gestion des fichiers Intel Hex.
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "fct_ihex.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/


/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

static char recordBuff[IHEX_RECORD_BUFF_SIZE];


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**-----------------------------------------------------------------------------
 * @brief	Validation des fonctions.
 *
 */
#define  VALIDATION_FCT_IHEX	 0

#if VALIDATION_FCT_IHEX


void VALIDATION_SEQUENCE_VALIDATION_FCT_IHEX(){

	IHexRecord Record;
	FIL File;
	int Status;
	uint8_t buffer[2000]={0};
	uint32_t size;
	uint32_t iBuffer;
	uint16_t NbRead;

	if (f_open(&File, "SWP_V12.32_FFFF8700.swp", FA_READ) != FR_OK)
	//if (f_open(&File, "AL00059AC.hex", FA_READ) != FR_OK)
		while (1);

	size = f_size(&File);
	f_read(&File, buffer, f_size(&File), &size);

	TimerSW_Start(TIMER_Generique, 5000);

	_printf("Lecture fichier : \n");
	f_lseek(&File, 0);
	while ((Status = IHex_ReadRecordFile(&Record, &File, 0)) == IHEX_OK)
	{
		IHex_PrintRecord(&Record);
	}
	if (Status != IHEX_ERROR_EOF)
		_printf("Fin de fichier non atteinte\n");
	_printf("Temps=%dms\n", TimerSW_GetTempsPasse(TIMER_Generique));

	TimerSW_Start(TIMER_Generique, 5000);

	_printf("Lecture buffer : \n");
	iBuffer = 0;
	while (iBuffer < size)
	{
		Status = IHex_ReadRecord(&Record, &buffer[iBuffer], sizeof(buffer) - iBuffer, &NbRead);
		if (Status == IHEX_OK)
			IHex_PrintRecord(&Record);

		if (NbRead == 0)
			NbRead = 1;

		iBuffer += NbRead;
	}
	_printf("Temps=%dms\n", TimerSW_GetTempsPasse(TIMER_Generique));


//	int		IHex_ReadRecord(IHexRecord *ihexRecord, FIL *in, /*uint16_t *nbr*/);


	f_close(&File);


	free(buffer);

	while (1)
	{

		//if (GestionIF_MsgRecu(IF_Can_Normal, &Msg, 0, 0))
		//{
		//	if (GestionIF_TraiterMsg(&Msg) ==  Status_OK)
		//		GestionIF_EnvoiMsg(IF_Can_Normal, &Msg, 0, 0);
		//}


	}
}
#endif


/**-----------------------------------------------------------------------------
 * @brief	Lecture d'un enregistrement a partir d'un fichier.
 *
 * @retval	eStatus		Statut de la fonction.
 */
int IHex_ReadRecordFile(IHexRecord* pRecord, FIL* pFile, uint16_t *br, uint32_t *pChecksumLine ) {

	uint32_t StartPointerFile = f_tell(pFile);
	uint32_t EndPointerFile;

	// Verification des parametres d'entree */
	if (pRecord == NULL || pFile == NULL)
		return IHEX_ERROR_INVALID_ARGUMENTS;

	// Lecture du fichier
	if (f_gets(recordBuff, IHEX_RECORD_BUFF_SIZE, pFile) == NULL) {
		/* In case we hit EOF, don't report a file error */
		if (f_eof(pFile) != 0)
			return IHEX_ERROR_EOF;
		else
			return IHEX_ERROR_FILE;
	}

	EndPointerFile = f_tell(pFile);

	// Calcul CR et check de Nb car lu et dif de f_tell si pointeur de CRC non null
	if ( pChecksumLine != NULL )
	{
		uint8_t LineLength = strlen(recordBuff);
		uint8_t NbCharRead = (uint8_t)(EndPointerFile - StartPointerFile);

		*pChecksumLine = 0;
		for ( uint8_t i=0; i<LineLength; i++ )
			*pChecksumLine += recordBuff[i];

		switch ( NbCharRead - LineLength )
		{
		// Pas de '\r' et '\n' dans la ligne (fin de fichier)
		case 0:
			break;

		// Pas de '\r', seulement '\n' dans la ligne lue.
		case 1:
			*pChecksumLine += '\r';
			break;

		// '\r' et '\n' dans la ligne lue.
		case 2:
			*pChecksumLine += '\r';
			*pChecksumLine += '\n';
			break;

		default:
			break;
		}
	}

	// Lecture d'un enrgistrement a partir du buffer
	return IHex_ReadRecord(pRecord, recordBuff, IHEX_RECORD_BUFF_SIZE, br);
}


/**-----------------------------------------------------------------------------
 * Reads an Intel HEX8 record from an opened file.
 * \param ihexRecord A pointer to the Intel HEX8 record structure that will store the read record.
 * \param in A file pointer to an opened file that can be read.
 * \param return bytes read through pointer : Modif TEUTSCH F.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer or file pointer is NULL.
 * \retval IHEX_ERROR_EOF if end-of-file has been reached.
 * \retval IHEX_ERROR_FILE if a file reading error has occured.
 * \retval IHEX_INVALID_RECORD if the record read is invalid (record did not match specifications or record checksum was invalid).
*/
int IHex_ReadRecord(IHexRecord* pRecord, char* Buffer, uint16_t BufferSize, uint16_t *br) {

	uint16_t save_nbr = 0;
	/* A temporary buffer to hold ASCII hex encoded data, set to the maximum length we would ever need */
	char hexBuff[IHEX_ADDRESS_LEN+1];
	int dataCount, i;

	if (br)
		*br = 0;

	// Copie du buffer en local
	if (Buffer != recordBuff)
	{
		if (BufferSize > IHEX_RECORD_BUFF_SIZE)
			BufferSize = IHEX_RECORD_BUFF_SIZE;
		strncpy(recordBuff, (const char*)Buffer, BufferSize);
	}

	/* Null-terminate the string at the first sign of a \r or \n */
	for (i = 0; i < BufferSize; i++) {
		if (recordBuff[i] == '\r' || recordBuff[i] == '\n') {
			recordBuff[i] = 0;
			break;
		}
	}

	save_nbr = i;

	/* Check if we hit a newline */
	if (strlen(recordBuff) == 0)
		return IHEX_ERROR_NEWLINE;
	
	/* Size check for start code, count, address, and type fields */
	if (strlen(recordBuff) < (unsigned int)(1+IHEX_COUNT_LEN+IHEX_ADDRESS_LEN+IHEX_TYPE_LEN))
		return IHEX_ERROR_INVALID_RECORD;
		
	/* Check the for colon start code */
	if (recordBuff[IHEX_START_CODE_OFFSET] != IHEX_START_CODE)
		return IHEX_ERROR_INVALID_RECORD;
	
	/* Copy the ASCII hex encoding of the count field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_COUNT_OFFSET, IHEX_COUNT_LEN);
	hexBuff[IHEX_COUNT_LEN] = 0;
	dataCount = strtol(hexBuff, (char **)NULL, 16);
	
	/* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_ADDRESS_OFFSET, IHEX_ADDRESS_LEN);
	hexBuff[IHEX_ADDRESS_LEN] = 0;
	pRecord->address = strtol(hexBuff, (char **)NULL, 16);
	
	/* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_TYPE_OFFSET, IHEX_TYPE_LEN);
	hexBuff[IHEX_TYPE_LEN] = 0;
	pRecord->type = strtol(hexBuff, (char **)NULL, 16);
	
	/* Size check for start code, count, address, type, data and checksum fields */
	if (strlen(recordBuff) < (unsigned int)(1+IHEX_COUNT_LEN+IHEX_ADDRESS_LEN+IHEX_TYPE_LEN+dataCount*2+IHEX_CHECKSUM_LEN))
		return IHEX_ERROR_INVALID_RECORD;
	
	/* Loop through each ASCII hex byte of the data field, pull it out into hexBuff,
	 * convert it and store the result in the data buffer of the Intel HEX8 record */
	for (i = 0; i < dataCount; i++) {
		/* Times two i because every byte is represented by two ASCII hex characters */
		strncpy(hexBuff, recordBuff+IHEX_DATA_OFFSET+2*i, IHEX_ASCII_HEX_BYTE_LEN);
		hexBuff[IHEX_ASCII_HEX_BYTE_LEN] = 0;
		pRecord->data[i] = strtol(hexBuff, (char **)NULL, 16);
	}
	pRecord->dataLen = dataCount;

	// Retourne le nombre d'octets lus
	if (br)
		*br = save_nbr;
	
	/* Copy the ASCII hex encoding of the checksum field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_DATA_OFFSET+dataCount*2, IHEX_CHECKSUM_LEN);
	hexBuff[IHEX_CHECKSUM_LEN] = 0;
	pRecord->checksum = strtol(hexBuff, (char **)NULL, 16);

	if (pRecord->checksum != IHex_CalculRecordChecksum(pRecord))
		return IHEX_ERROR_INVALID_RECORD;
	
	return IHEX_OK;
}


/**-----------------------------------------------------------------------------
 * Calculates the checksum of an Intel HEX8 IHexRecord structure.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \return The 8-bit checksum.
*/
uint8_t IHex_CalculRecordChecksum(const IHexRecord *ihexRecord) {
	uint8_t checksum;
	int i;

	/* Add the data count, type, address, and data bytes together */
	checksum = ihexRecord->dataLen;
	checksum += ihexRecord->type;
	checksum += (uint8_t)ihexRecord->address;
	checksum += (uint8_t)((ihexRecord->address & 0xFF00)>>8);
	for (i = 0; i < ihexRecord->dataLen; i++)
		checksum += ihexRecord->data[i];
	
	/* Two's complement on checksum */
	checksum = ~checksum + 1;

	return checksum;
}

#if 0
/**-----------------------------------------------------------------------------
 * Calcul du checksum de toutes les donnees contenues dans le fichier.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return The 32-bit checksum.
*/
uint32_t IHex_CalculDataChecksum(FIL *pFile)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t Checksum = 0;
	uint32_t ExpectedAddress=0xFFFFFFFF;
	uint32_t CurrentAddress=0;
	uint32_t BaseAddress=0;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:
					CurrentAddress = BaseAddress + HexRec.address;

					for (int i=0; i<HexRec.dataLen; i++)
						Checksum += HexRec.data[i];

					// Verifiaction des sauts d'adresses
					if ((CurrentAddress != ExpectedAddress) && (ExpectedAddress != 0xFFFFFFFF))
					{
						for (int i=0; i<CurrentAddress-ExpectedAddress; i++)
							Checksum += 0xFF;		// On complete par des 0xFF
					}
					ExpectedAddress = CurrentAddress + HexRec.dataLen;
					break;

				// End of File Record
				case IHEX_TYPE_01:
					Checksum = ~Checksum + 1;	// Fin du calcul du Checksum (Complement a 2)
					Status = Status_OK;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return Checksum;
}

/**-----------------------------------------------------------------------------
 * Lecture de l'adresse de debut des données.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return StartAddress.
*/
uint32_t IHex_GetStartAddress(FIL *pFile)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t StartAddress=0xFFFFFFFF;
	uint32_t BaseAddress;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:
					StartAddress = BaseAddress + HexRec.address;
					Status = Status_OK;
					break;

				// End of File Record
				case IHEX_TYPE_01:
					Status = Status_KO;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return StartAddress;
}

/**-----------------------------------------------------------------------------
 * Lecture de l'adresse de fin des données.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return EndAddress.
*/
uint32_t IHex_GetEndAddress(FIL *pFile)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t EndAddress;
	uint32_t BaseAddress;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:
					EndAddress = BaseAddress + HexRec.address + HexRec.dataLen;
					break;

				// End of File Record
				case IHEX_TYPE_01:
					Status = Status_KO;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return EndAddress;
}
#endif


#if 0
/**-----------------------------------------------------------------------------
 * Lecture des informations du fichier.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return EndAddress.
*/
uint8_t IHex_GetSwFileInfo(FIL *pFile, uint32_t* pStartAddress, uint32_t* pEndAddress, uint32_t* pDataChecksum)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t StartAddress=0xFFFFFFFF;
	uint32_t EndAddress=0;
	uint32_t BaseAddress=0;

	uint32_t ExpectedAddress=0xFFFFFFFF;
	uint32_t CurrentAddress=0;
	uint32_t Checksum=0;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecordFromFile(&HexRec, pFile) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:

					CurrentAddress = BaseAddress + HexRec.address;

					// Determination des adresses de debut et fin
					if (StartAddress == 0xFFFFFFFF)
						StartAddress =  CurrentAddress;
					EndAddress = BaseAddress + HexRec.address + HexRec.dataLen;

					// Calcul du checksum
					for (int i=0; i<HexRec.dataLen; i++)
						Checksum += HexRec.data[i];

					if ((CurrentAddress != ExpectedAddress) && (ExpectedAddress != 0xFFFFFFFF))	// Verifiaction des sauts d'adresses
					{
						for (int i=0; i<CurrentAddress-ExpectedAddress; i++)
							Checksum += 0xFF;		// On complete par des 0xFF
					}
					ExpectedAddress = CurrentAddress + HexRec.dataLen;

					break;

				// End of File Record
				case IHEX_TYPE_01:
					Checksum = ~Checksum + 1;	// Fin du calcul du Checksum (Complement a 2)

					if (pStartAddress)
						*pStartAddress = StartAddress;
					if (pEndAddress)
						*pEndAddress = EndAddress;
					if (pDataChecksum)
						*pDataChecksum = Checksum;

					Status = Status_KO;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return Status;
}
#endif


/**-----------------------------------------------------------------------------
 * Prints the contents of an Intel HEX8 record structure to stdout.
 * The record dump consists of the type, address, entire data array, and checksum fields of the record.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \return Always returns IHEX_OK (success).
 * \retval IHEX_OK on success.
*/
void IHex_PrintRecord(const IHexRecord *ihexRecord) {
	int i;
	_printf("IHexRec: Type=0x%02X,Add=0x%04X,Data={", ihexRecord->type, ihexRecord->address);
	for (i = 0; i < ihexRecord->dataLen; i++) {
		if (i+1 < ihexRecord->dataLen)
			_printf("0x%02X,", ihexRecord->data[i]);
		else
			_printf("0x%02X", ihexRecord->data[i]);
	}
	_printf("}");
	_printf(",Crc=0x%02X\n", ihexRecord->checksum);
}


#ifndef IHEX_LIGHT

/**-----------------------------------------------------------------------------
 *	Initializes a new IHexRecord structure that the paramater ihexRecord points to with the passed
 *	record type, 16-bit integer address, 8-bit data array, and size of 8-bit data array.
 * \param type The Intel HEX8 record type (integer value of 0 through 5).
 * \param address The 16-bit address of the data.
 * \param data A point to the 8-bit array of data.
 * \param dataLen The size of the 8-bit data array.
 * \param ihexRecord A pointer to the target Intel HEX8 record structure where these fields will be set.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer is NULL, or if the length of the 8-bit data array is out of range (less than zero or greater than the maximum data length allowed by record specifications, see IHexRecord.data).
*/
int IHex_NewRecord(int type, uint16_t address, const uint8_t *data, int dataLen, IHexRecord *ihexRecord) {
	/* Data length size check, assertion of ihexRecord pointer */
	if (dataLen < 0 || dataLen > IHEX_MAX_DATA_LEN/2 || ihexRecord == NULL)
		return IHEX_ERROR_INVALID_ARGUMENTS;

	ihexRecord->type = type;
	ihexRecord->address = address;
	memcpy(ihexRecord->data, data, dataLen);
	ihexRecord->dataLen = dataLen;
	ihexRecord->checksum = Checksum_IHexRecord(ihexRecord);

	return IHEX_OK;
}


/**-----------------------------------------------------------------------------
 * Writes an Intel HEX8 record to an opened file.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \param out A file pointer to an opened file that can be written to.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer or file pointer is NULL.
 * \retval IHEX_ERROR_INVALID_RECORD if the record's data length is out of range (greater than the maximum data length allowed by record specifications, see IHexRecord.data).
 * \retval IHEX_ERROR_FILE if a file writing error has occured.
 */
int IHex_WriteRecord(const IHexRecord *ihexRecord, FIL *out) {
	int i;

	/* Check our record pointer and file pointer */
	if (ihexRecord == NULL || out == NULL)
		return IHEX_ERROR_INVALID_ARGUMENTS;

	/* Check that the data length is in range */
	if (ihexRecord->dataLen > IHEX_MAX_DATA_LEN/2)
		return IHEX_ERROR_INVALID_RECORD;

	/* Write the start code, data count, address, and type fields */
	if (fprintf(out, "%c%2.2X%2.4X%2.2X", IHEX_START_CODE, ihexRecord->dataLen, ihexRecord->address, ihexRecord->type) < 0)
		return IHEX_ERROR_FILE;

	/* Write the data bytes */
	for (i = 0; i < ihexRecord->dataLen; i++) {
		if (fprintf(out, "%2.2X", ihexRecord->data[i]) < 0)
			return IHEX_ERROR_FILE;
	}

	/* Calculate and write the checksum field */
	if (fprintf(out, "%2.2X\r\n", Checksum_IHexRecord(ihexRecord)) < 0)
		return IHEX_ERROR_FILE;

	return IHEX_OK;
}

#endif



#if 0
/*
 *  ihex.c
 *  Utility functions to create, read, write, and print Intel HEX8 binary records.
 *
 *  Written by Vanya A. Sergeev <vsergeev@gmail.com>
 *  Version 1.0.5 - February 2011
 *
 */

#include "fct_ihex.h"


/**-----------------------------------------------------------------------------
 * Reads an Intel HEX8 record from an opened file.
 * \param ihexRecord A pointer to the Intel HEX8 record structure that will store the read record.
 * \param in A file pointer to an opened file that can be read.
 * \param return bytes read through pointer : Modif TEUTSCH F.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer or file pointer is NULL.
 * \retval IHEX_ERROR_EOF if end-of-file has been reached.
 * \retval IHEX_ERROR_FILE if a file reading error has occured.
 * \retval IHEX_INVALID_RECORD if the record read is invalid (record did not match specifications or record checksum was invalid).
*/
int IHex_ReadRecord(IHexRecord *ihexRecord, FIL *in, uint16_t *nbr) {

	uint16_t save_nbr=0;
	char recordBuff[IHEX_RECORD_BUFF_SIZE];
	/* A temporary buffer to hold ASCII hex encoded data, set to the maximum length we would ever need */
	char hexBuff[IHEX_ADDRESS_LEN+1];
	int dataCount, i;

	/* Check our record pointer and file pointer */
	if (ihexRecord == NULL || in == NULL)
		return IHEX_ERROR_INVALID_ARGUMENTS;

	if (f_gets(recordBuff, IHEX_RECORD_BUFF_SIZE, in) == NULL) {
			/* In case we hit EOF, don't report a file error */
			if (f_eof(in) != 0)
				return IHEX_ERROR_EOF;
			else
				return IHEX_ERROR_FILE;
	}
	/* Null-terminate the string at the first sign of a \r or \n */
	for (i = 0; i < (int)strlen(recordBuff); i++) {
		if (recordBuff[i] == '\r' || recordBuff[i] == '\n') {
			recordBuff[i] = 0;
			break;
		}
	}

	save_nbr = i;

	/* Check if we hit a newline */
	if (strlen(recordBuff) == 0)
		return IHEX_ERROR_NEWLINE;

	/* Size check for start code, count, address, and type fields */
	if (strlen(recordBuff) < (unsigned int)(1+IHEX_COUNT_LEN+IHEX_ADDRESS_LEN+IHEX_TYPE_LEN))
		return IHEX_ERROR_INVALID_RECORD;

	/* Check the for colon start code */
	if (recordBuff[IHEX_START_CODE_OFFSET] != IHEX_START_CODE)
		return IHEX_ERROR_INVALID_RECORD;

	/* Copy the ASCII hex encoding of the count field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_COUNT_OFFSET, IHEX_COUNT_LEN);
	hexBuff[IHEX_COUNT_LEN] = 0;
	dataCount = strtol(hexBuff, (char **)NULL, 16);

	/* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_ADDRESS_OFFSET, IHEX_ADDRESS_LEN);
	hexBuff[IHEX_ADDRESS_LEN] = 0;
	ihexRecord->address = strtol(hexBuff, (char **)NULL, 16);

	/* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_TYPE_OFFSET, IHEX_TYPE_LEN);
	hexBuff[IHEX_TYPE_LEN] = 0;
	ihexRecord->type = strtol(hexBuff, (char **)NULL, 16);

	/* Size check for start code, count, address, type, data and checksum fields */
	if (strlen(recordBuff) < (unsigned int)(1+IHEX_COUNT_LEN+IHEX_ADDRESS_LEN+IHEX_TYPE_LEN+dataCount*2+IHEX_CHECKSUM_LEN))
		return IHEX_ERROR_INVALID_RECORD;

	/* Loop through each ASCII hex byte of the data field, pull it out into hexBuff,
	 * convert it and store the result in the data buffer of the Intel HEX8 record */
	for (i = 0; i < dataCount; i++) {
		/* Times two i because every byte is represented by two ASCII hex characters */
		strncpy(hexBuff, recordBuff+IHEX_DATA_OFFSET+2*i, IHEX_ASCII_HEX_BYTE_LEN);
		hexBuff[IHEX_ASCII_HEX_BYTE_LEN] = 0;
		ihexRecord->data[i] = strtol(hexBuff, (char **)NULL, 16);
	}
	ihexRecord->dataLen = dataCount;
	if (nbr)
		*nbr = save_nbr;

	/* Copy the ASCII hex encoding of the checksum field into hexBuff, convert it to a usable integer */
	strncpy(hexBuff, recordBuff+IHEX_DATA_OFFSET+dataCount*2, IHEX_CHECKSUM_LEN);
	hexBuff[IHEX_CHECKSUM_LEN] = 0;
	ihexRecord->checksum = strtol(hexBuff, (char **)NULL, 16);

	if (ihexRecord->checksum != IHex_CalculRecordChecksum(ihexRecord))
		return IHEX_ERROR_INVALID_RECORD;

	return IHEX_OK;
}


/**-----------------------------------------------------------------------------
 * Calculates the checksum of an Intel HEX8 IHexRecord structure.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \return The 8-bit checksum.
*/
uint8_t IHex_CalculRecordChecksum(const IHexRecord *ihexRecord) {
	uint8_t checksum;
	int i;

	/* Add the data count, type, address, and data bytes together */
	checksum = ihexRecord->dataLen;
	checksum += ihexRecord->type;
	checksum += (uint8_t)ihexRecord->address;
	checksum += (uint8_t)((ihexRecord->address & 0xFF00)>>8);
	for (i = 0; i < ihexRecord->dataLen; i++)
		checksum += ihexRecord->data[i];

	/* Two's complement on checksum */
	checksum = ~checksum + 1;

	return checksum;
}

#if 0
/**-----------------------------------------------------------------------------
 * Calcul du checksum de toutes les donnees contenues dans le fichier.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return The 32-bit checksum.
*/
uint32_t IHex_CalculDataChecksum(FIL *pFile)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t Checksum = 0;
	uint32_t ExpectedAddress=0xFFFFFFFF;
	uint32_t CurrentAddress=0;
	uint32_t BaseAddress=0;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:
					CurrentAddress = BaseAddress + HexRec.address;

					for (int i=0; i<HexRec.dataLen; i++)
						Checksum += HexRec.data[i];

					// Verifiaction des sauts d'adresses
					if ((CurrentAddress != ExpectedAddress) && (ExpectedAddress != 0xFFFFFFFF))
					{
						for (int i=0; i<CurrentAddress-ExpectedAddress; i++)
							Checksum += 0xFF;		// On complete par des 0xFF
					}
					ExpectedAddress = CurrentAddress + HexRec.dataLen;
					break;

				// End of File Record
				case IHEX_TYPE_01:
					Checksum = ~Checksum + 1;	// Fin du calcul du Checksum (Complement a 2)
					Status = Status_OK;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return Checksum;
}

/**-----------------------------------------------------------------------------
 * Lecture de l'adresse de debut des données.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return StartAddress.
*/
uint32_t IHex_GetStartAddress(FIL *pFile)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t StartAddress=0xFFFFFFFF;
	uint32_t BaseAddress;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:
					StartAddress = BaseAddress + HexRec.address;
					Status = Status_OK;
					break;

				// End of File Record
				case IHEX_TYPE_01:
					Status = Status_KO;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return StartAddress;
}

/**-----------------------------------------------------------------------------
 * Lecture de l'adresse de fin des données.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return EndAddress.
*/
uint32_t IHex_GetEndAddress(FIL *pFile)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t EndAddress;
	uint32_t BaseAddress;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:
					EndAddress = BaseAddress + HexRec.address + HexRec.dataLen;
					break;

				// End of File Record
				case IHEX_TYPE_01:
					Status = Status_KO;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return EndAddress;
}
#endif


/**-----------------------------------------------------------------------------
 * Lecture des informations du fichier.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param in A file pointer to an opened file that can be read.
 * \return EndAddress.
*/
uint8_t IHex_GetFileInfo(FIL *pFile, uint32_t* pStartAddress, uint32_t* pEndAddress, uint32_t* pDataChecksum)
{
	uint8_t Status = Status_EnCours;
	IHexRecord HexRec;
	uint32_t StartAddress=0xFFFFFFFF;
	uint32_t EndAddress=0;
	uint32_t BaseAddress=0;

	uint32_t ExpectedAddress=0xFFFFFFFF;
	uint32_t CurrentAddress=0;
	uint32_t Checksum=0;

	while (Status == Status_EnCours)
	{
		if (IHex_ReadRecord(&HexRec, pFile, 0) == IHEX_OK)
		{
			//Print_IHexRecord(&UpdateSW.HexRec);
			switch (HexRec.type)
			{
				// Data Record
				case IHEX_TYPE_00:

					CurrentAddress = BaseAddress + HexRec.address;

					// Determination des adresses de debut et fin
					if (StartAddress == 0xFFFFFFFF)
						StartAddress =  CurrentAddress;
					EndAddress = BaseAddress + HexRec.address + HexRec.dataLen;

					// Calcul du checksum
					for (int i=0; i<HexRec.dataLen; i++)
						Checksum += HexRec.data[i];

					if ((CurrentAddress != ExpectedAddress) && (ExpectedAddress != 0xFFFFFFFF))	// Verifiaction des sauts d'adresses
					{
						for (int i=0; i<CurrentAddress-ExpectedAddress; i++)
							Checksum += 0xFF;		// On complete par des 0xFF
					}
					ExpectedAddress = CurrentAddress + HexRec.dataLen;

					break;

				// End of File Record
				case IHEX_TYPE_01:
					Checksum = ~Checksum + 1;	// Fin du calcul du Checksum (Complement a 2)

					if (pStartAddress)
						*pStartAddress = StartAddress;
					if (pEndAddress)
						*pEndAddress = EndAddress;
					if (pDataChecksum)
						*pDataChecksum = Checksum;

					Status = Status_KO;
					break;

				// Extended Linear Address Record
				case IHEX_TYPE_04:
					BaseAddress  = (HexRec.data[0] << 24) + (HexRec.data[1] << 16);
					BaseAddress &= 0xFFFF0000;
					break;
			}
		}
		else
			Status = Status_KO;
	}

	return Status;
}



/**-----------------------------------------------------------------------------
 * Prints the contents of an Intel HEX8 record structure to stdout.
 * The record dump consists of the type, address, entire data array, and checksum fields of the record.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \return Always returns IHEX_OK (success).
 * \retval IHEX_OK on success.
*/
void IHex_PrintRecord(const IHexRecord *ihexRecord) {
	int i;
	_printf("IHexRec: Type=%d,Add=0x%04X,Data={", ihexRecord->type, ihexRecord->address);
	for (i = 0; i < ihexRecord->dataLen; i++) {
		if (i+1 < ihexRecord->dataLen)
			_printf("0x%02X,", ihexRecord->data[i]);
		else
			_printf("0x%02X", ihexRecord->data[i]);
	}
	_printf("}");
	_printf(",Crc=0x%02X\n", ihexRecord->checksum);
}


#ifndef IHEX_LIGHT

/**-----------------------------------------------------------------------------
 *	Initializes a new IHexRecord structure that the paramater ihexRecord points to with the passed
 *	record type, 16-bit integer address, 8-bit data array, and size of 8-bit data array.
 * \param type The Intel HEX8 record type (integer value of 0 through 5).
 * \param address The 16-bit address of the data.
 * \param data A point to the 8-bit array of data.
 * \param dataLen The size of the 8-bit data array.
 * \param ihexRecord A pointer to the target Intel HEX8 record structure where these fields will be set.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer is NULL, or if the length of the 8-bit data array is out of range (less than zero or greater than the maximum data length allowed by record specifications, see IHexRecord.data).
*/
int IHex_NewRecord(int type, uint16_t address, const uint8_t *data, int dataLen, IHexRecord *ihexRecord) {
	/* Data length size check, assertion of ihexRecord pointer */
	if (dataLen < 0 || dataLen > IHEX_MAX_DATA_LEN/2 || ihexRecord == NULL)
		return IHEX_ERROR_INVALID_ARGUMENTS;

	ihexRecord->type = type;
	ihexRecord->address = address;
	memcpy(ihexRecord->data, data, dataLen);
	ihexRecord->dataLen = dataLen;
	ihexRecord->checksum = Checksum_IHexRecord(ihexRecord);

	return IHEX_OK;
}


/**-----------------------------------------------------------------------------
 * Writes an Intel HEX8 record to an opened file.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \param out A file pointer to an opened file that can be written to.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer or file pointer is NULL.
 * \retval IHEX_ERROR_INVALID_RECORD if the record's data length is out of range (greater than the maximum data length allowed by record specifications, see IHexRecord.data).
 * \retval IHEX_ERROR_FILE if a file writing error has occured.
 */
int IHex_WriteRecord(const IHexRecord *ihexRecord, FIL *out) {
	int i;

	/* Check our record pointer and file pointer */
	if (ihexRecord == NULL || out == NULL)
		return IHEX_ERROR_INVALID_ARGUMENTS;

	/* Check that the data length is in range */
	if (ihexRecord->dataLen > IHEX_MAX_DATA_LEN/2)
		return IHEX_ERROR_INVALID_RECORD;

	/* Write the start code, data count, address, and type fields */
	if (fprintf(out, "%c%2.2X%2.4X%2.2X", IHEX_START_CODE, ihexRecord->dataLen, ihexRecord->address, ihexRecord->type) < 0)
		return IHEX_ERROR_FILE;

	/* Write the data bytes */
	for (i = 0; i < ihexRecord->dataLen; i++) {
		if (fprintf(out, "%2.2X", ihexRecord->data[i]) < 0)
			return IHEX_ERROR_FILE;
	}

	/* Calculate and write the checksum field */
	if (fprintf(out, "%2.2X\r\n", Checksum_IHexRecord(ihexRecord)) < 0)
		return IHEX_ERROR_FILE;

	return IHEX_OK;
}

#endif

#endif

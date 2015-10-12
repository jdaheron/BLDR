/**
 *******************************************************************************
 * @file	drv_SdCard_SPI.c
 * @author	j.daheron
 * @version	1.0.0
 * @date	18 dec. 2013
 * @brief   Gestion de Carte SD par SPI.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "BSP.h"

#include <string.h>
#include "ff_gen_drv.h"


/* External variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/

/* Block Size in Bytes */
#define BLOCK_SIZE                512

/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	// GO_IDLE_STATE
#define CMD1	(0x40+1)	// SEND_OP_COND (MMC)
#define ACMD41	(0xC0+41)	// SEND_OP_COND (SDC)
#define CMD8	(0x40+8)	// SEND_IF_COND
#define CMD9	(0x40+9)	// SEND_CSD
#define CMD10	(0x40+10)	// SEND_CID
#define CMD12	(0x40+12)	// STOP_TRANSMISSION
#define ACMD13	(0xC0+13)	// SD_STATUS (SDC)
#define CMD16	(0x40+16)	// SET_BLOCKLEN
#define CMD17	(0x40+17)	// READ_SINGLE_BLOCK
#define CMD18	(0x40+18)	// READ_MULTIPLE_BLOCK
#define CMD23	(0x40+23)	// SET_BLOCK_COUNT (MMC)
#define ACMD23	(0xC0+23)	// SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24	(0x40+24)	// WRITE_BLOCK
#define CMD25	(0x40+25)	// WRITE_MULTIPLE_BLOCK
#define CMD55	(0x40+55)	// APP_CMD
#define CMD58	(0x40+58)	// READ_OCR


#define SLOW_CLOCK_kHz	100				// Identification (Clk < 400kHz)
#define FAST_CLOCK_kHz	4000			// Data (Clk < 25MHz)


/* Private macros ------------------------------------------------------------*/

#if USE_EVAL_BOARD_OLIMEX
	#define SWITCH_CP		0		/* Presence switch Carte Presence */
	#define SWITCH_WP		0		/* Presence switch Write Protect */
#else
	#define SWITCH_CP		0		/* Presence switch Carte Presence */
	#define SWITCH_WP		0		/* Presence switch Write Protect */
#endif



/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

DSTATUS SdCard_SPI_initialize(void);
DSTATUS SdCard_SPI_status(void);
DRESULT SdCard_SPI_ioctl (BYTE ctrl, void *buff);
DRESULT SdCard_SPI_read(BYTE* buff, DWORD sector, BYTE count);
DRESULT SdCard_SPI_write(const BYTE* buff, DWORD sector, BYTE count);


Diskio_drvTypeDef SdCard_SPI_Driver =
{
	SdCard_SPI_initialize,
	SdCard_SPI_status,
	SdCard_SPI_read,
	#if  _USE_WRITE == 1
		SdCard_SPI_write,
	#endif /* _USE_WRITE == 1 */

	#if  _USE_IOCTL == 1
		SdCard_SPI_ioctl,
	#endif /* _USE_IOCTL == 1 */
};


extern Diskio_drvTypeDef  SdCard_SPI_Driver;



/* Private functions ---------------------------------------------------------*/


/*--------------------------------------------------------------------------

   Module Private Functions and Variables

---------------------------------------------------------------------------*/


#define STATE_MASK_CARTE_PRESENTE	0x01
#define STATE_MASK_WRITE_PROTECT	0x02

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */
static volatile DWORD Timer1, Timer2;		/* 100Hz decrement timers */
static BYTE CardType;						/* Card type flags */


/* Socket's Card-Present Pin */
static inline DWORD socket_is_empty(void)
{
	#if SWITCH_CP
		if (GPIO_Get(PORT_SD_DETECT) == Etat_ACTIF)
			return STATE_MASK_CARTE_PRESENTE;

		return 0;

	#else
		return STATE_MASK_CARTE_PRESENTE;
	#endif
}

/* Write Protect Pin */
static inline DWORD socket_is_write_protected(void)
{
	#if SWITCH_WP
		if (GPIO_Get(PORT_SD_WP) == Etat_ACTIF)
			return STATE_MASK_WRITE_PROTECT;

		return 0;

	#else
		return 0;
	#endif
}


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static BYTE wait_ready (void)
{
	BYTE res;

	Timer2 = 50;	/* Wait for ready in timeout of 500ms */
	HAL_SdSpi_Read();
	do
		res = HAL_SdSpi_Read();
	while ((res != 0xFF) && Timer2);

	return res;
}



/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/
static void release_spi (void)
{
	HAL_SdSpi_Deselect();
	HAL_SdSpi_Read();
}


/*-----------------------------------------------------------------------*/
/* Power Control and interface-initialization (Platform dependent)       */
/*-----------------------------------------------------------------------*/

static void power_on (void)
{
	/* De-select the Card: Chip Select high */
	HAL_SdSpi_Deselect();

	HAL_SdSpi_Init(SLOW_CLOCK_kHz);

	/* drain SPI */
//	while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_TXE) == RESET) { ; }
	HAL_SdSpi_Read();

}


static void power_off (void)
{
	Stat |= STA_NOINIT;		/* Set STA_NOINIT */
}


/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/
static uint8_t rcvr_datablock (BYTE *buff, UINT btr)
{
	BYTE token;

	Timer1 = 10;
	do {							/* Wait for data packet in timeout of 100ms */
		token = HAL_SdSpi_Read();
	} while ((token == 0xFF) && Timer1);

	if(token != 0xFE)
		return FALSE;	/* If not valid data token, return with error */

	do {				/* Receive the data block into buffer */
		*(buff++) = HAL_SdSpi_Read();
		*(buff++) = HAL_SdSpi_Read();
		*(buff++) = HAL_SdSpi_Read();
		*(buff++) = HAL_SdSpi_Read();
	} while (btr -= 4);

	HAL_SdSpi_Read();				/* Discard CRC */
	HAL_SdSpi_Read();

	return TRUE;					/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/
static uint8_t xmit_datablock (const BYTE *buff, BYTE token)
{
	BYTE resp;
	BYTE wc;

	if (wait_ready() != 0xFF) return FALSE;

	HAL_SdSpi_Write(token);					/* transmit data token */
	if (token != 0xFD) {	/* Is data token */

		wc = 0;
		do {							/* transmit the 512 byte data block to MMC */
			HAL_SdSpi_Write(*buff++);
			HAL_SdSpi_Write(*buff++);
		} while (--wc);

		HAL_SdSpi_Write(0xFF);					/* CRC (Dummy) */
		HAL_SdSpi_Write(0xFF);
		resp = HAL_SdSpi_Read();				/* Receive data response */
		if ((resp & 0x1F) != 0x05)		/* If not accepted, return with error */
			return FALSE;
	}

	return TRUE;
}


/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/
static BYTE send_cmd (BYTE cmd, DWORD arg)
{
	BYTE n, res;

	if (cmd & 0x80) {	/* ACMD<n> is the command sequence of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready */
	HAL_SdSpi_Deselect();
	HAL_SdSpi_Select();
	if (wait_ready() != 0xFF) {
		return 0xFF;
	}

	/* Send command packet */
	HAL_SdSpi_Write(cmd);						/* Start + Command index */
	HAL_SdSpi_Write((BYTE)(arg >> 24));		/* Argument[31..24] */
	HAL_SdSpi_Write((BYTE)(arg >> 16));		/* Argument[23..16] */
	HAL_SdSpi_Write((BYTE)(arg >> 8));			/* Argument[15..8] */
	HAL_SdSpi_Write((BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	HAL_SdSpi_Write(n);

	/* Receive command response */
	if (cmd == CMD12) HAL_SdSpi_Read();		/* Skip a stuff byte when stop reading */

	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do
		res = HAL_SdSpi_Read();
	while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/
DSTATUS SdCard_SPI_initialize()
{
	BYTE n, cmd, ty, ocr[4];

	if (Stat & STA_NODISK) return Stat;	/* No card in the socket */

	power_on();							/* Force socket power on and initialize interface */

	for (n = 10; n; n--) HAL_SdSpi_Read();	/* 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* Enter Idle state */
		Timer1 = 100;						/* Initialization timeout of 1000 milliseconds */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDHC */
			for (n = 0; n < 4; n++) ocr[n] = HAL_SdSpi_Read();		/* Get trailing return value of R7 response */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* The card can work at VDD range of 2.7-3.6V */
				while (Timer1 && send_cmd(ACMD41, 1UL << 30));	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (Timer1 && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = HAL_SdSpi_Read();
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
				}
			}
		} else {							/* SDSC or MMC */
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = CT_SD1; cmd = ACMD41;	/* SDSC */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMC */
			}
			while (Timer1 && send_cmd(cmd, 0));			/* Wait for leaving idle state */
			if (!Timer1 || send_cmd(CMD16, 512) != 0)	/* Set R/W block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	release_spi();

	if (ty)
	{			/* Initialization succeeded */
		Stat &= ~STA_NOINIT;		/* Clear STA_NOINIT */
		HAL_SdSpi_Init(FAST_CLOCK_kHz);
	}
	else
	{			/* Initialization failed */
		power_off();
	}

	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS SdCard_SPI_status ()
{
	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT SdCard_SPI_read (BYTE *buff, DWORD sector, BYTE count)
{
	if (!count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) {	/* Single block read */
		if (send_cmd(CMD17, sector) == 0)	{ /* READ_SINGLE_BLOCK */
			if (rcvr_datablock(buff, 512)) {
				count = 0;
			}
		}
	}
	else {				/* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) {
					break;
				}
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	release_spi();

	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT SdCard_SPI_write (const BYTE *buff, DWORD sector, BYTE count)
{
	if (!count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (Stat & STA_PROTECT) return RES_WRPRT;

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) {	/* Single block write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE))
			count = 0;
	}
	else {				/* Multiple block write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
				count = 1;
		}
	}
	release_spi();

	return count ? RES_ERROR : RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT SdCard_SPI_ioctl (BYTE ctrl, void *buff)
{
	DRESULT res;
	BYTE n, csd[16], *ptr = buff;
	WORD csize;

	res = RES_ERROR;

	if (ctrl == CTRL_POWER) {
		switch (*ptr) {
			case 0:		/* Sub control code == 0 (POWER_OFF) */
				power_off();		/* Power off */
				res = RES_OK;
				break;
			case 1:		/* Sub control code == 1 (POWER_ON) */
				power_on();				/* Power on */
				res = RES_OK;
				break;
			case 2:		/* Sub control code == 2 (POWER_GET) */
				*(ptr+1) = (BYTE) 1;
				res = RES_OK;
				break;
			default :
				res = RES_PARERR;
		}
	}
	else {
		if (Stat & STA_NOINIT) return RES_NOTRDY;

		switch (ctrl) {
			case CTRL_SYNC :		/* Make sure that no pending write process */
				HAL_SdSpi_Select();
				if (wait_ready() == 0xFF)
					res = RES_OK;
				break;

			case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
				if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
					if ((csd[0] >> 6) == 1) {	/* SDC version 2.00 */
						csize = csd[9] + ((WORD)csd[8] << 8) + 1;
						*(DWORD*)buff = (DWORD)csize << 10;
					} else {					/* SDC version 1.XX or MMC*/
						n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
						csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
						*(DWORD*)buff = (DWORD)csize << (n - 9);
					}
					res = RES_OK;
				}
				break;

			case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
				*(WORD*)buff = BLOCK_SIZE;
				res = RES_OK;
				break;

			case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
				if (CardType & CT_SD2) {	/* SDC version 2.00 */
					if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
						HAL_SdSpi_Read();
						if (rcvr_datablock(csd, 16)) {				/* Read partial block */
							for (n = 64 - 16; n; n--) HAL_SdSpi_Read();	/* Purge trailing data */
							*(DWORD*)buff = 16UL << (csd[10] >> 4);
							res = RES_OK;
						}
					}
				} else {					/* SDC version 1.XX or MMC */
					if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
						if (CardType & CT_SD1) {	/* SDC version 1.XX */
							*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
						} else {					/* MMC */
							*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
						}
						res = RES_OK;
					}
				}
				break;

			case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
				*ptr = CardType;
				res = RES_OK;
				break;

			case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
				if (send_cmd(CMD9, 0) == 0		/* READ_CSD */
					&& rcvr_datablock(ptr, 16))
					res = RES_OK;
				break;

			case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
				if (send_cmd(CMD10, 0) == 0		/* READ_CID */
					&& rcvr_datablock(ptr, 16))
					res = RES_OK;
				break;

			case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
				if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
					for (n = 4; n; n--) *ptr++ = HAL_SdSpi_Read();
					res = RES_OK;
				}
				break;

			case MMC_GET_SDSTAT :	/* Receive SD status as a data block (64 bytes) */
				if (send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
					HAL_SdSpi_Read();
					if (rcvr_datablock(ptr, 64))
						res = RES_OK;
				}
				break;

			default:
				res = RES_PARERR;
		}

		release_spi();
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */
void SdCard_SPI_timerproc (void)
{
	static DWORD pv;
	DWORD ns;
	BYTE n, s;


	n = Timer1;                /* 100Hz decrement timers */
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;

	ns = pv;
	pv = socket_is_empty() | socket_is_write_protected();	/* Sample socket switch */

	if (ns == pv) {								/* Have contacts stabled? */
		s = Stat;

		if (pv & STATE_MASK_WRITE_PROTECT)		/* Write protected ? */
			s |= STA_PROTECT;
		else
			s &= ~STA_PROTECT;

		if (pv & STATE_MASK_CARTE_PRESENTE)		/* Carte Presente ? */
			s &= ~STA_NODISK;
		else
			s |= (STA_NODISK | STA_NOINIT);

		Stat = s;
	}
}


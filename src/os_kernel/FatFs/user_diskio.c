/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: user_diskio.c
 * in this file :
 *============================================================================*/


#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/* 
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future. 
 * Kept to ensure backward compatibility with previous CubeMx versions when 
 * migrating projects. 
 * User code previously added there should be copied in the new user sections before 
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "sd.h"
#include "console_serial_trace.h"
#include "micro_sd_spi.h"

/* Private variables ---------------------------------------------------------*/
extern sd_info_ptr sdinfo;

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/

DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
		USER_initialize,
		USER_status,
		USER_read,
#if  _USE_WRITE
		USER_write,
#endif  /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
		USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes a Drive
 * @param  pdrv: Physical drive nmuber to identify the drive
 * @retval DSTATUS: Operation status
 */
DSTATUS USER_initialize (BYTE pdrv)
{
	__ENTER__

	SD_PowerOn();
	if(sd_ini() == 0)
	{
		Stat &= ~STA_NOINIT;
	}

	console_serial_print_log("USER_initialize status : %d", Stat);

	__LEAVE__

	return Stat;
}

/**
 * @brief  Gets Disk Status
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS USER_status (BYTE pdrv)
{
	if (pdrv) return STA_NOINIT;
	return Stat;
}

/**
 * @brief  Reads Sector(s)
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data buffer to store read data
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to read (1..128)
 * @retval DRESULT: Operation result
 */
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
	__ENTER__

	console_serial_print_log("\t> USER_read sector: %lu; count: %d ",sector, count);

	if (pdrv || !count)
	{
		return RES_PARERR;
	}

	if (Stat & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	if (!(sdinfo.type & 4))
	{
		sector *= 512; /* Convert to byte address if needed */
	}

	if (count == 1) /* Single block read */
	{
		console_serial_print_log("\t> USER_readSingle block");

		SD_Read_Block(buff,sector); //������� ���� � �����

		count = 0;
	}
	else /* Multiple block read */
	{
	}

	SPI_Release();

	__LEAVE__

	return (count ? RES_ERROR : RES_OK);
}

/**
 * @brief  Writes Sector(s)
 * @param  pdrv: Physical drive nmuber to identify the drive
 * @param  *buff: Data to be written
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to write (1..128)
 * @retval DRESULT: Operation result
 */
#if _USE_WRITE == 1
DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count )
{
	__ENTER__

	console_serial_print_log("\t> USER_write sector: %lu ",sector);

	if (pdrv || !count)
	{
		return RES_PARERR;
	}

	if (Stat & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	if (Stat & STA_PROTECT)
	{
		return RES_WRPRT;
	}

	if (!(sdinfo.type & 4))
	{
		sector *= 512; /* Convert to byte address if needed */
	}

	if (count == 1) /* Single block read */
	{
		SD_Write_Block((BYTE*)buff,sector); //������� ���� � �����
		count = 0;
	}
	else /* Multiple block read */
	{
	}

	SPI_Release();

	__LEAVE__

	return count ? RES_ERROR : RES_OK;

}
#endif /* _USE_WRITE == 1 */

/**
 * @brief  I/O control operation
 * @param  pdrv: Physical drive number (0..)
 * @param  cmd: Control code
 * @param  *buff: Buffer to send/receive control data
 * @retval DRESULT: Operation result
 */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff )
{
	DRESULT res;

	console_serial_print_log("USER_ioctl cmd: %d",cmd);
	if (pdrv)
	{
		return RES_PARERR;
	}

	if (Stat & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	res = RES_ERROR;

	switch (cmd)
	{
	case CTRL_SYNC : /* Flush dirty buffer if present */
		SS_SD_SELECT();
		if (SPI_wait_ready() == 0xFF)
		{
			res = RES_OK;
		}
		break;

	case GET_SECTOR_SIZE : /* Get sectors on the disk (WORD) */
		*(WORD*)buff = 512;
		res = RES_OK;
		break;

	default:
		res = RES_PARERR;
	}

	SPI_Release();

	return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

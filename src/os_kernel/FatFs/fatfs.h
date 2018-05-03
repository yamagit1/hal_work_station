/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: fatfs.h
 * in this file :
 *============================================================================*/

#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "user_diskio.h"


 extern uint8_t gStatusSD;    		// Return value for USER
 extern char 	gDrivePath[4];   	// USER logical drive path
 extern FATFS 	gFlieSysObj;    	// File system object for USER logical drive
 extern FIL 	gFileObj;       	// File object for USER


void FS_FATFS_Init(void);
void FS_scan_list_file_to_html(__uint8 *p_buff, __uint32 *p_offset, __uint8 *path);

#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

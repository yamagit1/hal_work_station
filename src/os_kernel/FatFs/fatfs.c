

#include "fatfs.h"
#include "console_serial_trace.h"

uint8_t gStatusSD;    		// Return value for USER
char 	gDrivePath[4];   	// USER logical drive path
FATFS 	gFlieSysObj;    	// File system object for USER logical drive
FIL 	gFileObj;       	// File object for USER


void FS_FATFS_Init(void)
{
	// Set up driver for fatfs
	gStatusSD = FATFS_LinkDriver(&USER_Driver, gDrivePath);

	// init disk
	disk_initialize(gFlieSysObj.drv);
}



void FS_scan_list_file_to_html(__uint8 *p_buff, __uint32 *p_offset, __uint8 *path)
{
	DIR dirs;
	FILINFO finfo;
	__uint8 strCache[BUFF_SIZE_LOW];

	// add tag block html
	snprintf(p_buff, 24, "<div><table border=\"1\">");
	*p_offset += 23;

	// get all file and dir in current dir
	if(f_mount(&gFlieSysObj, (TCHAR const*)gDrivePath, 0) == FR_OK)
	{
		if (f_opendir(&dirs, path) == FR_OK)
		{
			while (f_readdir(&dirs, &finfo) == FR_OK && finfo.fname[0])
			{
				if (finfo.fattrib & AM_DIR)
				{
					console_serial_print_infor("DIR INFOR : %s  | %d  |%d  |%s  |%d  |%d  ", "", finfo.fattrib, finfo.fdate, finfo.fname, finfo.fsize, finfo.ftime);

					sprintf(strCache,"<tr><td>%s/%s/</td><td>dir</td></tr>", path, finfo.fname);
					snprintf((p_buff + *p_offset), strlen(strCache) + 1, strCache);

					*p_offset += strlen(strCache);
				}
				else
				{
					console_serial_print_infor("FIL INFOR : %s  | %d  |%d  |%s  |%d  |%d  ", "", finfo.fattrib, finfo.fdate, finfo.fname, finfo.fsize, finfo.ftime);

					sprintf(strCache,"<tr><td>%s/%s</td><td>%d byte</td></tr>", path, finfo.fname, (finfo.fsize));
					snprintf((p_buff + *p_offset), strlen(strCache) + 1, strCache);

					*p_offset += strlen(strCache);
				}
			}
		}
		else
		{
			console_serial_print_log("\t> FS_get_list_file_dir :  open dir fail %s", path);
		}

	}
	else
	{
		console_serial_print_log("\t> FS_get_list_file_dir : mount SD card fail");
	}

	// add tag close html
	snprintf((p_buff + *p_offset), 15, "</table></div>");
	*p_offset += 14;

}


DWORD get_fattime(void)
{
	return 0;
}

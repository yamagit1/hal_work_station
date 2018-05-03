/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: perform_manage.c
 * in this file :
 *============================================================================*/

#include <micro_sd.h>
#include <perform_manage.h>
#include "fatfs.h"
#include "console_serial_trace.h"
#include "html_temple.h"
#include "driver_led.h"

osThreadId gListPID[MAX_SIZE_LIST_PID];
__S_TIME gTimeCount;


void PM_updateTimeCount()
{
	if (gTimeCount.section < 59)
	{
		gTimeCount.section++;
	}
	else if (gTimeCount.miuter < 59)
	{
		gTimeCount.section = 0;
		gTimeCount.miuter++;
	}
	else if (gTimeCount.hour <24)
	{
		gTimeCount.section = 0;
		gTimeCount.miuter = 0;
		gTimeCount.hour++;
	}
	else
	{
		gTimeCount.section = 0;
		gTimeCount.miuter = 0;
		gTimeCount.hour = 0;
	}
}


void PM_update_home_page_html()//void * argument)
{
	__uint8 buff[BUFF_SIZE_LOW];
	__uint32 currLine = 0;
	__uint32 numByteWrite;
	__uint8 listFile[BUFF_SIZE_VERY_LARGE * 2];
	char path[5]={"/"};
	__uint32 offset = 0;

	__uint32 sdTotalSize = 0;
	__uint32 sdFree = 0;
	//get list file
	FS_scan_list_file_to_html(listFile, &offset, path);

	//
	if(f_mount(&gFlieSysObj, (TCHAR const*)gDrivePath, 0)!=FR_OK)
	{
		console_serial_print_log("\t> PM_update_home_page_html : mount SD card fail");
	}
	else
	{
		console_serial_print_log("\t> PM_update_home_page_html : mount successfully ");

		if(f_open(&gFileObj,"homepage.html",  FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
		{
			console_serial_print_log("\t> PM_update_home_page_html : open fail");		}
		else
		{
			console_serial_print_log("\t> PM_update_home_page_html : open successfully ");

			f_write(&gFileObj, home_page_temple[0], strlen(home_page_temple[0]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[1], strlen(home_page_temple[1]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[2], strlen(home_page_temple[2]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[3], strlen(home_page_temple[3]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[4], strlen(home_page_temple[4]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[5], strlen(home_page_temple[5]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[6], strlen(home_page_temple[6]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[7], strlen(home_page_temple[7]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[8], strlen(home_page_temple[8]), (void*)&numByteWrite);

			//	{"<tr><td>LED_RED</td><td>ON</td></tr>"},
			if (gLedStatus[LED_RED] == eTRUE)
			{
				sprintf(buff, home_page_temple[9], "ON");
			}
			else
			{
				sprintf(buff, home_page_temple[9], "OFF");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	{"<tr><td>LED_BLUE</td><td>OFF</td></tr>"},
			if (gLedStatus[LED_BLUE] == eTRUE)
			{
				sprintf(buff, home_page_temple[10], "ON");
			}
			else
			{
				sprintf(buff, home_page_temple[10], "OFF");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	{"<tr><td>LED_GREEN</td><td>%3s</td></tr>"},
			if (gLedStatus[LED_GREEN] == eTRUE)
			{
				sprintf(buff, home_page_temple[11], "ON");
			}
			else
			{
				sprintf(buff, home_page_temple[11], "OFF");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	{"<tr><td>LED_YELLOW</td><td>%3s</td></tr>"},
			if (gLedStatus[LED_YELLOW] == eTRUE)
			{
				sprintf(buff, home_page_temple[12], "ON");
			}
			else
			{
				sprintf(buff, home_page_temple[12], "OFF");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			f_write(&gFileObj, home_page_temple[13], strlen(home_page_temple[13]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[14], strlen(home_page_temple[14]), (void*)&numByteWrite);

			//	gListPID[0]
			if (gListPID[0] == 0)
			{
				sprintf(buff, home_page_temple[15], gListPID[0], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[15], gListPID[0], pcTaskGetName(gListPID[0]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[1]
			if (gListPID[1] == 0)
			{
				sprintf(buff, home_page_temple[16], gListPID[1], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[16], gListPID[1], pcTaskGetName(gListPID[1]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[2]
			if (gListPID[2] == 0)
			{
				sprintf(buff, home_page_temple[17], gListPID[2], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[17], gListPID[2], pcTaskGetName(gListPID[2]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[3]
			if (gListPID[3] == 0)
			{
				sprintf(buff, home_page_temple[18], gListPID[3], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[18], gListPID[3], pcTaskGetName(gListPID[3]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[4]
			if (gListPID[4] == 0)
			{
				sprintf(buff, home_page_temple[19], gListPID[4], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[19], gListPID[4], pcTaskGetName(gListPID[4]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[5]
			if (gListPID[5] == 0)
			{
				sprintf(buff, home_page_temple[20], gListPID[5], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[20], gListPID[5], pcTaskGetName(gListPID[5]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[6]
			if (gListPID[6] == 0)
			{
				sprintf(buff, home_page_temple[21], gListPID[6], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[21], gListPID[6], pcTaskGetName(gListPID[6]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[7]
			if (gListPID[7] == 0)
			{
				sprintf(buff, home_page_temple[22], gListPID[7], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[22], gListPID[7], pcTaskGetName(gListPID[7]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[8]
			if (gListPID[8] == 0)
			{
				sprintf(buff, home_page_temple[23], gListPID[8], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[23], gListPID[8], pcTaskGetName(gListPID[8]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[9]
			if (gListPID[9] == 0)
			{
				sprintf(buff, home_page_temple[24], gListPID[9], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[24], gListPID[9], pcTaskGetName(gListPID[9]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[10]
			if (gListPID[10] == 0)
			{
				sprintf(buff, home_page_temple[25], gListPID[10], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[25], gListPID[10], pcTaskGetName(gListPID[10]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[11]
			if (gListPID[11] == 0)
			{
				sprintf(buff, home_page_temple[26], gListPID[11], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[26], gListPID[11], pcTaskGetName(gListPID[11]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[12]
			if (gListPID[12] == 0)
			{
				sprintf(buff, home_page_temple[27], gListPID[12], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[27], gListPID[12], pcTaskGetName(gListPID[12]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[13]
			if (gListPID[13] == 0)
			{
				sprintf(buff, home_page_temple[28], gListPID[13], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[28], gListPID[13], pcTaskGetName(gListPID[13]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[14]
			if (gListPID[14] == 0)
			{
				sprintf(buff, home_page_temple[29], gListPID[14], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[29], gListPID[14], pcTaskGetName(gListPID[14]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[15]
			if (gListPID[15] == 0)
			{
				sprintf(buff, home_page_temple[30], gListPID[15], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[30], gListPID[15], pcTaskGetName(gListPID[15]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[16]
			if (gListPID[16] == 0)
			{
				sprintf(buff, home_page_temple[31], gListPID[16], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[31], gListPID[16], pcTaskGetName(gListPID[15]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);


			//	gListPID[17]
			if (gListPID[17] == 0)
			{
				sprintf(buff, home_page_temple[32], gListPID[17], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[32], gListPID[17], pcTaskGetName(gListPID[16]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			//	gListPID[18]
			if (gListPID[18] == 0)
			{
				sprintf(buff, home_page_temple[33], gListPID[18], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[33], gListPID[18], pcTaskGetName(gListPID[17]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			//	gListPID[19]
			if (gListPID[19] == 0)
			{
				sprintf(buff, home_page_temple[34], gListPID[19], "N/A", "N/A");
			}
			else
			{
				sprintf(buff, home_page_temple[34], gListPID[19], pcTaskGetName(gListPID[18]), "RUNNING");
			}
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			f_write(&gFileObj, home_page_temple[35], strlen(home_page_temple[35]), (void*)&numByteWrite);
			f_write(&gFileObj, home_page_temple[36], strlen(home_page_temple[36]), (void*)&numByteWrite);

			sprintf(buff, home_page_temple[37], 185344, 11264, 196608);
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			sprintf(buff, home_page_temple[38], 171156, 877420, 1048576);
			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			sdTotalSize = (gFlieSysObj.n_fatent * gFlieSysObj.csize) / 2;
			sdFree = (gFlieSysObj.free_clst * gFlieSysObj.csize) / 2;

			sprintf(buff, home_page_temple[39], sdTotalSize - sdFree, sdFree, sdTotalSize);

			f_write(&gFileObj, buff, strlen(buff), (void*)&numByteWrite);

			// get list file

			if (offset > 1024)
			{
				f_write(&gFileObj, listFile, 1024, (void*)&numByteWrite);
				f_write(&gFileObj, listFile + 1024, offset - 1024, (void*)&numByteWrite);
			}
			else
			{
				f_write(&gFileObj, listFile, 1024, (void*)&numByteWrite);
			}

			f_write(&gFileObj, home_page_temple[40], strlen(home_page_temple[40]), (void*)&numByteWrite);

			console_serial_print_log("\t> My file infor :cltbl:%d clust:%d dir_sect:%d err:%d flag:%d fptr:%d sect:%d", gFileObj.cltbl, gFileObj.clust, gFileObj.dir_sect, gFileObj.err, gFileObj.flag, gFileObj.fptr, gFileObj.sect);
			console_serial_print_log("\t> My obj  file infor :attr:%d id:%d lockid:%d objsize:%d sclust:%d stat:%d", gFileObj.obj.attr, gFileObj.obj.id, gFileObj.obj.lockid, gFileObj.obj.objsize, gFileObj.obj.sclust, gFileObj.obj.stat);

			f_close(&gFileObj);
		}
	}

}


void PM_get_list_file(__uint8 *buff)
{
	char path[50]={"/UCDownloads"};
	DIR dirs;
	FILINFO finfo;

	//===========================================================================
	// in here
	//===========================================================================
	if(f_mount(&gFlieSysObj, (TCHAR const*)gDrivePath, 0)!=FR_OK)
	{
		console_serial_print_log("\t> PM_get_list_file : mount SD card fail");
	}
	else
	{
		console_serial_print_log("\t> PM_get_list_file : mount SD card successfully");

		if (f_opendir(&dirs, path) != FR_OK)
		{
			console_serial_print_log("\t> PM_get_list_file :  open dir successfully");

		}
		else
		{
			console_serial_print_log("\t> PM_get_list_file :  open dir fail");
			while (f_readdir(&dirs, &finfo) == FR_OK && finfo.fname[0])
			{
				if (finfo.fattrib & AM_DIR)
				{
					console_serial_print_infor("DIR INFOR : %s  | %d  |%d  |%s  |%d  |%d  ", "", finfo.fattrib, finfo.fdate, finfo.fname, finfo.fsize, finfo.ftime);
				}
				else
				{
					console_serial_print_infor("FIL INFOR : %s  | %d  |%d  |%s  |%d  |%d  ", "", finfo.fattrib, finfo.fdate, finfo.fname, finfo.fsize, finfo.ftime);
				}
			}

			f_closedir(&dirs);

		}

	}

}


void PM_task_update_html(void * argument)
{

	for(;;)
	{
		if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_MEDIUM) == osOK)
		{
			console_serial_print_infor("-----------------------update homepage.html-------------------------");
			PM_update_home_page_html();
			console_serial_print_infor("-----------------------update finish-------------------------");

			osSemaphoreRelease(microSDSemaphoreID);
		}

		osDelay(60000);
	}
}


void PM_init()
{
	// create task httpd
		console_serial_print_log("Create task html update");
		osThreadDef(PM_html_update, PM_task_update_html, osPriorityNormal, 0, 1300);

		gListPID[INDEX_PM] = osThreadCreate(osThread(PM_html_update), NULL);
}


#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
//#include "lwip.h"


#include "console_serial_trace.h"
#include "lcd1202.h"
#include "driver_led.h"
#include "peripheral_init.h"
#include "enc28j60.h"

#include "lcd_logo.h"


//#include "ethernetif.h"
//#include "init.h"
#include "enc28j60_spi.h"
#include "micro_sd_spi.h"
#include "net.h"

FATFS SDFatFs;
FIL MyFile;
uint8_t wtext[]="<html><tile>testweb</tile><body><h1>Content web server</h1></body></html>";
__uint32 byteswritten, bytesread;
uint8_t sect[512];
__uint8 buff[100];


FRESULT ReadLongFile(void)
{
	uint16_t i=0, i1=0;
	uint32_t ind=0;
	uint32_t f_size = MyFile.obj.objsize;

	console_serial_print_log("fsize: %lu\r\n",(unsigned long)f_size);
	ind=0;

	do
	{
		if(f_size < 512)
		{
			i1 = f_size;
		}
		else
		{
			i1 = 512;
		}

		f_size-=i1;

		f_lseek(&MyFile, ind);

		console_serial_print_log("\t> Start read");

		f_read(&MyFile, sect, i1, (UINT *)&bytesread);

		console_serial_print_log("\t> Start finish : %d", bytesread);

		console_serial_print_log("%s", sect);

		ind+=i1;

	}

	while(f_size>0);

	return FR_OK;
}

int main(void)
{
	//	struct netif netif;
	int i = 0;
	FILINFO fileInfo;

	//------------------Initialize Hardware and mcu -----------------------------

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	//------------------Initialize all configured peripherals -----------------------------
	MX_GPIO_Init();

	// console trace initialize
	console_serial_init();
	console_serial_print_log("Initialize console trace ..... complete");

	//initialize lcd1202
	lcd_1202_initialize();
	console_serial_print_log("Initialize lcd1202 .....complete");
	// view logo test
	lcd_1202_view_image_bitmap(gLogo);
	//------------------Initialize os system -----------------------------
	//------------------Initialize os system -----------------------------

	//============================================================================================

	//	console_serial_print_log("Initialize spi 2");
	//	//	ENC28J60_SPI_Init();
	microSD_SPI_Init();
	MX_FATFS_Init();
	//	sd_ini();
	disk_initialize(SDFatFs.drv);

	//	if(f_mount(&SDFatFs,(TCHAR const*)USERPath,0)!=FR_OK)
	//	{
	//		console_serial_print_log("> mount fail");
	//
	//	}
	//	else
	//	{
	//		console_serial_print_log("> mount successfully ");
	//
	//		if(f_open(&MyFile,"index.html",  FA_READ) != FR_OK)
	//		{
	//			console_serial_print_log("> open fail");		}
	//		else
	//		{
	//			console_serial_print_log("> open successfully ");
	//
	//
	////			f_write(&MyFile, wtext, sizeof(wtext), (void*)&byteswritten);
	//
	//			ReadLongFile();
	//			//			for (i = 0; i< 5; i++)
	//			//			{
	//			//				console_serial_print_log("%c", f_gets(buff,1, &MyFile ));
	//			//			}
	//
	//
	//
	//			console_serial_print_log("\t> My file infor :cltbl:%d clust:%d dir_sect:%d err:%d flag:%d fptr:%d sect:%d", MyFile.cltbl, MyFile.clust, MyFile.dir_sect, MyFile.err, MyFile.flag, MyFile.fptr, MyFile.sect);
	//			console_serial_print_log("\t> My obj  file infor :attr:%d id:%d lockid:%d objsize:%d sclust:%d stat:%d", MyFile.obj.attr, MyFile.obj.id, MyFile.obj.lockid, MyFile.obj.objsize, MyFile.obj.sclust, MyFile.obj.stat);
	//
	//			f_close(&MyFile);
	//		}
	//	}

	//=========================================================================
	net_ini();


	while (1)
	{
		net_poll();
	}


}



#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "lwip.h"


#include "console_serial_trace.h"
#include "lcd1202.h"
#include "driver_led.h"
#include "peripheral_init.h"
#include "enc28j60.h"

#include "lcd_logo.h"


#include "ethernetif.h"
#include "init.h"
#include "enc28j60_spi.h"
#include "micro_sd_spi.h"

FATFS SDFatFs;

int main(void)
{
	struct netif netif;

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

	//------------------Initialize os system -----------------------------
	//------------------Initialize os system -----------------------------

	//============================================================================================

	console_serial_print_log("Initialize spi 2");
//	ENC28J60_SPI_Init();
	microSD_SPI_Init();
	MX_FATFS_Init();
//	sd_ini();
	disk_initialize(SDFatFs.drv);


	while (1)
	{

	}


}


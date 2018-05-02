
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "console_serial_trace.h"
#include "lcd1202.h"
#include "driver_led.h"
#include "peripheral_init.h"
#include "enc28j60.h"

#include "lcd_logo.h"

#include "enc28j60_spi.h"
#include "micro_sd_spi.h"
#include "net.h"
#include "monitor.h"
#include "perform_manage.h"
#include "httpd.h"
#include "fptserver.h"


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

	console_serial_print_log("Initialize microSD card");
	FS_FATFS_Init();


	//------------------Initialize os system -----------------------------
	dled_initialize();
	MNT_initialize();
	net_ini();
	httpd_init();
	PM_init();
	fptd_init();

	console_serial_print_log("Start scheduler");
	osKernelStart();
	//	PM_update_home_page_html();

	while (1)
	{
	}


}


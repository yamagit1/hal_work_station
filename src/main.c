/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: main.c
 * in this file :
 *============================================================================*/

#include "main.h"
#include "console_serial_trace.h"
#include "driver_led.h"
#include "peripheral_init.h"
#include "net.h"
#include "monitor.h"
#include "perform_manage.h"
#include "httpd.h"
#include "fptserver.h"


int main(void)
{
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

	while (1)
	{
	}


}



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
#include "monitor.h"
#include "perform_manage.h"
#include "httpd.h"

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

#define SIG_TEST 200
#define SIG_TEST_2 201
osThreadId idTask1, idTask2, idTask3;


void handle_task_3(void * argument)
{
	for (;;)
	{
		//		console_serial_print_log(">>handle_task_3-------pool");
		net_poll();
	}
}

void handle_task_1(void * argument)
{
	int i = 0;

	for (;;)
	{
		dled_setting_led_status(LED_RED, TURN_ON);
		osDelay(200);
		dled_setting_led_status(LED_BLUE, TURN_ON);
		osDelay(200);
		dled_setting_led_status(LED_YELLOW, TURN_ON);
		osDelay(200);
		dled_setting_led_status(LED_GREEN, TURN_ON);
		osDelay(200);

		dled_setting_led_status(LED_RED, TURN_OFF);
		osDelay(200);
		dled_setting_led_status(LED_BLUE, TURN_OFF);
		osDelay(200);
		dled_setting_led_status(LED_YELLOW, TURN_OFF);
		osDelay(200);
		dled_setting_led_status(LED_GREEN, TURN_OFF);
		osDelay(200);
	}
}


void handle_task_2(void * argument)
{
	int currCount = 0;
	osEvent sig1;

	for (;;)
	{
		console_serial_print_log(">>handle_task_2 wait : %d", currCount++);
		sig1 = osSignalWait (0xffffffff, 10000);

		if (sig1.status == osEventSignal)
		{
			if (sig1.value.signals == SIG_TEST)
			{
				console_serial_print_log(">>handle_task_2 recever signal SIG_TEST");
			}
			else if (sig1.value.signals == SIG_TEST_2)
			{
				console_serial_print_log(">>handle_task_2 recever signal  SIG_TEST_2");

			}
			else
			{
				console_serial_print_log(">>handle_task_2 recever signal  NONE");

			}

		}
		else if (sig1.status == osEventTimeout)
		{
			console_serial_print_log(">>handle_task_2 recever signal time out");

		}
		//		osDelay(200);

	}

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


	console_serial_print_log("Initialize microSD card");
	FS_FATFS_Init();


	dled_initialize();
	console_serial_print_log("Initialize driver led .....complete");
	//------------------Initialize os system -----------------------------
	//------------------Initialize os system -----------------------------


	//=========================================================================

	// init task 1
	console_serial_print_log("Create task 1\n\t\t>name : %s,\n\t\t>priority : %d,\n\t\t>stacksz : %d,", "task_1", 1, 1000);
	osThreadDef(task_1_mos, handle_task_1, osPriorityNormal, 0, 500);
	idTask1 = osThreadCreate(osThread(task_1_mos), NULL);

	MNT_Render();
	//===================================================================

	net_ini();
	httpd_init();
	PM_init();

//	console_serial_print_log("Create task 3\n\t\t>name : %s,%d,\n\t\t>priority : %d,\n\t\t>stacksz : %d,", "task_2", 1, 1);
//	osThreadDef(task_3, handle_task_3, osPriorityNormal, 0, 500);
//	idTask3 = osThreadCreate(osThread(task_3), NULL);

	console_serial_print_log("Start scheduler");
	osKernelStart();
	//	PM_update_home_page_html();

	while (1)
	{
	}


}



#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "libjpeg.h"
#include "lwip.h"
#include "sdio.h"

#include "console_serial_trace.h"
#include "lcd1202.h"
#include "driver_led.h"
#include "peripheral_init.h"
#include "enc28j60.h"

#include "ethernetif.h"
#include "init.h"

//void MX_FREERTOS_Init(void);?S

void handle_led_blink(void const * argument)
{
	int i = 0;

	for(;;)
	{
		console_serial_print_log("Start blink :%d", i);

		dled_setting_led_status(LED_RED, TURN_ON);
		//		osDelay(200);
		dled_setting_led_status(LED_BLUE, TURN_ON);
		//		osDelay(200);
		dled_setting_led_status(LED_YELLOW, TURN_ON);
		//		osDelay(200);
		dled_setting_led_status(LED_GREEN, TURN_ON);
		osDelay(200);

		dled_setting_led_status(LED_RED, TURN_OFF);
		//		osDelay(200);
		dled_setting_led_status(LED_BLUE, TURN_OFF);
		//		osDelay(200);
		dled_setting_led_status(LED_YELLOW, TURN_OFF);
		//		osDelay(200);
		dled_setting_led_status(LED_GREEN, TURN_OFF);
		osDelay(200);
		console_serial_print_log("en loop :%d", i++);
	}
}

void handle_task_1(void * argument)
{
	int currCount = 0;



	for (;;)
	{
		console_serial_print_log(">>handle_task_1 count : %d", currCount++);
		dled_setting_led_status(LED_BLUE, TURN_ON);
		dled_setting_led_status(LED_RED, TURN_OFF);
		osDelay(200);
	}
}

void handle_task_2(void * argument)
{
	int currCount = 0;

	for (;;)
	{
		dled_setting_led_status(LED_RED, TURN_ON);
		dled_setting_led_status(LED_BLUE, TURN_OFF);

		console_serial_print_log(">>handle_task_2 count : %d", currCount++);
		osDelay(200);

	}

}

int main(void)
{
	struct netif netif;

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	//	MX_DMA_Init();
	//	MX_SDIO_SD_Init();

	/* Call initialize function for freertos objects (in freertos.c) */
	//	  MX_FREERTOS_Init();

	// console trace initialize
	console_serial_init();

	console_serial_print_log("Initialize console trace ..... complete");

	//	//initialize lcd1202
	//	lcd_1202_initialize();
	//	console_serial_print_log("Initialize lcd1202 .....complete");
	//
	//	//initialize led
		dled_initialize();
	//	console_serial_print_log("Initialize driver led .....complete");
	//
	//	//initialize lwip
	////	MX_LWIP_Init();
	//	console_serial_print_log("Initialize lwip .....");
	////	  lwip_init();
	////	ethernetif_init(&netif);
	//
	////
	////	MX_FREERTOS_Init();
	////	osThreadDef(blink, handle_led_blink, 1, 0, 128);
	////	osThreadId blinkTaskHandle = osThreadCreate(osThread(blink), NULL);

//	MX_FREERTOS_Init();
//	// init task 1
//	console_serial_print_log("Create task 1\n\t\tp>name : %s,\n\t\t>priority : %d,\n\t\t>stacksz : %d,",\
////			"task_1", 1, 128);
//		osThreadId task_1_Handle;
//	  osThreadDef(task_1_mos, handle_task_1, osPriorityNormal, 0, 128);
//	  task_1_Handle = osThreadCreate(osThread(task_1_mos), NULL);
//
//	  if (task_1_Handle == NULL)
//	  {
//			dled_setting_led_status(LED_GREEN, TURN_ON);
//	  }
//	  else
//	  {
//
//	  }
//
//	// init task 2
//	console_serial_print_log("Create task 1\t\tp>name : %s,%d,\n\t\t>priority : %d,\n\t\t>stacksz : %d,",\
//			"task_2", 1, 128);
//	osThreadDef(task_2, handle_task_2, osPriorityNormal, 0, 128);
//	osThreadId task_2_Handle = osThreadCreate(osThread(task_2), NULL);
////
//	/* Start scheduler */
//	console_serial_print_log("Start scheduler");
//	osKernelStart();
	TaskHandle_t task1, task2;

	xTaskCreate(handle_task_1, "task_1", 1000, NULL, osPriorityNormal, &task1);
	xTaskCreate(handle_task_2, "task_2", 1000, NULL, osPriorityNormal, &task2);

//	xTaskCreate(CM_taskMonitorHandler, "MonitorHandler", 1000, NULL, 2, &gPIDTask[1]);
//	xTaskCreate(CM_taskNetwork, "network", 3000, NULL, 2, &gPIDTask[2]);
//	xTaskCreate(CM_taskUpdateTemperature, "UpdateTemperature", 1000, NULL, 2, &gPIDTask[3]);
//	xTaskCreate(CM_taksTime, "Timer", 1000, NULL, 3, &gPIDTask[4]);

		vTaskStartScheduler();


	//	__uint8 net_buf[ENC28J60_MAXFRAME];
	__uint32 len = 0;
	__uint8 net_send[100] = \
			{
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
			};

	while (1)
	{

		//		if ((len=enc28j60_packetReceive(net_buf,sizeof(net_buf)))>0)
		//		{
		//			console_serial_print_log("network reciever data len : %d", len);
		//			enc28j60_packetSend(net_send, 100);
		//		}

	}


}



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

void MX_FREERTOS_Init(void);


__uint8 macaddr[]={0x01,0x02,0x03,0x04,0x05,0x06};

int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SDIO_SD_Init();

	/* Call initialize function for freertos objects (in freertos.c) */
//	  MX_FREERTOS_Init();

	// console trace initialize
	console_serial_init();

	console_serial_print_log("Initialize console trace ..... complete");

	//initialize lcd1202
	lcd_1202_initialize();
	console_serial_print_log("Initialize lcd1202 .....complete");

	//initialize led
	dled_initialize();
	console_serial_print_log("Initialize driver led .....complete");

	//initialize lwip
	MX_LWIP_Init();
	console_serial_print_log("Initialize lwip .....complete");

	/* Start scheduler */
	console_serial_print_log("Start scheduler");
//	osKernelStart();

	int i = 0;

	while (1)
	{
		dled_setting_led_status(LED_RED, TURN_ON);
		HAL_Delay(200);
		dled_setting_led_status(LED_BLUE, TURN_ON);
		HAL_Delay(200);
		dled_setting_led_status(LED_YELLOW, TURN_ON);
		HAL_Delay(200);
		dled_setting_led_status(LED_GREEN, TURN_ON);
		HAL_Delay(200);

		dled_setting_led_status(LED_RED, TURN_OFF);
		HAL_Delay(200);
		dled_setting_led_status(LED_BLUE, TURN_OFF);
		HAL_Delay(200);
		dled_setting_led_status(LED_YELLOW, TURN_OFF);
		HAL_Delay(200);
		dled_setting_led_status(LED_GREEN, TURN_OFF);
		HAL_Delay(200);

		console_serial_print_line("test :%d", i++);
		HAL_Delay(200);

	}


}


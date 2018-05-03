/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: driver_led.c
 * in this file :
 *============================================================================*/

#include <driver_led.h>
#include "cmsis_os.h"
#include "perform_manage.h"
#include "console_serial_trace.h"

__E_Bollen gLedStatus[4];

void dled_task_handle(void * argument)
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

void dled_initialize()
{
	//	__ENTER__

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	// init led red
	GPIO_InitStruct.Pin = PIN_LED_RED;
	HAL_GPIO_Init(COM_LED_RED, &GPIO_InitStruct);

	// init led blue
	GPIO_InitStruct.Pin = PIN_LED_BLUE;
	HAL_GPIO_Init(COM_LED_BLUE, &GPIO_InitStruct);

	// init led yellow
	GPIO_InitStruct.Pin = PIN_LED_YELLOW;
	HAL_GPIO_Init(COM_LED_YELLOW, &GPIO_InitStruct);

	// init led green
	GPIO_InitStruct.Pin = PIN_LED_GREEN;
	HAL_GPIO_Init(COM_LED_GREEN, &GPIO_InitStruct);

	//	__LEAVE__

	console_serial_print_log("Create task 1\n\t\t>name : %s,\n\t\t>priority : %d,\n\t\t>stacksz : %d,", "task_1", 1, 1000);
	osThreadDef(driver_led, dled_task_handle, osPriorityNormal, 0, 50);
	gListPID[INDEX_DRIVER_LED] = osThreadCreate(osThread(driver_led), NULL);
}

/**
 *
 */

void dled_setting_led_status(__uint32 led, __E_Led_Status ledStatus)
{
	GPIO_TypeDef* p_ledCom = NULL;
	uint16_t ledPin;

	// select pin and com for led
	if (led == LED_RED)
	{
		ledPin = PIN_LED_RED;
		p_ledCom = COM_LED_RED;
	}
	else if (led == LED_BLUE)
	{
		ledPin = PIN_LED_BLUE;
		p_ledCom = COM_LED_BLUE;
	}
	else if (led == LED_YELLOW)
	{
		ledPin = PIN_LED_YELLOW;
		p_ledCom = COM_LED_YELLOW;
	}
	else if (led == LED_GREEN)
	{
		ledPin = PIN_LED_GREEN;
		p_ledCom = COM_LED_GREEN;
	}
	else
	{
		return;
	}

	// setting led status
	HAL_GPIO_WritePin(p_ledCom, ledPin, ledStatus);

	// set global var stores status led
	gLedStatus[led] = ledStatus;

}

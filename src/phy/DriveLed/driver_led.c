/*==============================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *============================================================================*/

#include <driver_led.h>

__E_Bollen gLedStatus[4];

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

/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: driver_led.h
 * in this file :
 *============================================================================*/


#ifndef __DRIVER_LED_H__
#define __DRIVER_LED_H__

#include "header.h"

/*==============================================================================
 *	Led is use
 *============================================================================*/
#define LED_RED							0
#define LED_BLUE						1
#define LED_YELLOW						2
#define LED_GREEN						3

/*==============================================================================
 *	config with device stm32f407
 *============================================================================*/


#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG

// pin configure
#define	PIN_LED_RED						GPIO_PIN_12
#define	PIN_LED_BLUE					GPIO_PIN_13
#define	PIN_LED_YELLOW					GPIO_PIN_14
#define	PIN_LED_GREEN					GPIO_PIN_15

// com configure
#define COM_LED_RED						GPIOD
#define COM_LED_BLUE					GPIOD
#define COM_LED_YELLOW					GPIOD
#define COM_LED_GREEN					GPIOD

#endif // CONFIG_FLATFORM == FLATFORM_STM32_F407VG

typedef enum
{
	TURN_ON = GPIO_PIN_RESET,
	TURN_OFF = GPIO_PIN_SET
}__E_Led_Status;


extern __E_Bollen gLedStatus[4];

/*==============================================================================
 *	function
 *============================================================================*/

void dled_initialize();
void dled_setting_led_status(__uint32 led, __E_Led_Status ledStatus);

#endif /* __DRIVER_LED_H__ */

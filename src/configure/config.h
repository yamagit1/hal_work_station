/*==================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *==================================================================*/

/*==================================================================
 * In this file:
 * 		- config device is use
 *==================================================================*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <header.h>

// platform build
#define CONFIG_FLATFORM FLATFORM_STM32_F407VG

// target build
#define BUILD_TARGET BUILD_TARGET_DEBUG

// lcd display information in kit
#define MONITOR_USE		MONITOR_LCD_1202

// os compile
#define OS_BUILD OS_LIN64_UBUNTU_16_4

// baud rate for serial rs232
#define CONSOLE_SERIAL_SPEED 115200
#endif /*__CONFIG_H__*/

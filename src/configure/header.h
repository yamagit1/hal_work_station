/*==============================================================================
 *  Author  : YAMA															   *
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file		: header.h
 * in this file :
 * 		- define device can use
 * 		- typedef basic data
 * 		- include library
 *============================================================================*/


#ifndef __HEADER_H__
#define __HEADER_H__

/*==============================================================================
 * id for flatfrom:  x yyyy zzz
 * 					 ^ ^^^^ ^^^
 * 					 a 	b	 c
 * 	- a: company
 * 		 1-ARM  2-ATMEGA ...
 * 	- b: seri
 * 	- c: reserver (ex: version)
 *
 *============================================================================*/

#define FLATFORM_STM32_F407VG 	10407000
#define FLATFORM_ATMEGA_ARDUNO	20000000

/*==============================================================================
 * enviement build:  xx yy zzzzz
 * 					 ^^ ^^ ^^^^
 * 					 a 	 b 	 c
 * 	- a: company
 * 		 1-windows  2-Linix ...
 * 	- b: seri : 32/64 bit
 * 	- c: reserver (ex: ubuntu, red hat, win7, winxp, ...)
 *
 *============================================================================*/

#define OS_WIN32_7					10320070
#define OS_WIN64_7					10640070

#define	OS_LIN32_UBUNTU_16_4		20321016
#define OS_LIN64_UBUNTU_16_4		20641016

/*==============================================================================
 * LCD view:  x y zzzzzz
 * 			  ^ ^ ^^^^
 * 			  a b  c
 * 	- a: type
 * 		 1-mono  2-color ...
 * 	- b: type
 * 		1- small	2- medium	3- larger 4- QHD 5- HD,...
 * 	- c: ID
 *
 *============================================================================*/

#define MONITOR_LCD_1202			11001202


/*==============================================================================
 * build target
 *============================================================================*/

#define BUILD_TARGET_RELEASE		10000000
#define BUILD_TARGET_DEBUG			20000000

/*==============================================================================
 * buffer size
 *============================================================================*/
#define UART_TIME_OUT 			10
#define BUFF_SIZE_VERY_LOW 		64
#define BUFF_SIZE_LOW 			128
#define BUFF_SIZE_MEDIUM		256
#define BUFF_SIZE_LARGE			512
#define BUFF_SIZE_VERY_LARGE	1024
/*==============================================================================
 * time wait
 *============================================================================*/
#define TIME_WAIT_SHORT			10
#define TIME_WAIT_MEDIUM		100
#define TIME_WAIT_LONG			1000

/*==============================================================================
 * log level
 *============================================================================*/

#define LOG_PHY						1
#define LOG_OS						2
#define LOG_MID						3
#define LOG_APP						4

#include<config.h>

/*==============================================================================
 * typedef basic type data
 *============================================================================*/

#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG

typedef		signed		char		__int8;
typedef		unsigned	char		__uint8;
typedef		signed		short		__int16;
typedef		unsigned 	short		__uint16;
typedef		signed		int			__int32;
typedef 	unsigned	int			__uint32;
typedef		signed		long long	__int64;
typedef		unsigned  	long long	__uint64;

#endif

typedef enum
{
	eFALSE = 0,
	eTRUE  = !eFALSE
}__E_Bollen;

typedef enum
{
	eDISABLE = 0,
	eENABLE = !eDISABLE
}__E_Status;

#define TRUE			1
#define FALSE			0
/*==============================================================================
 * include library
 *============================================================================*/

#include "FreeRTOS.h"
#include"string.h"
#include "stdio.h"
#include "stdarg.h"
#include <stdlib.h>


#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG
#include "stm32f4xx_hal.h"
#endif

//#include "lcd1202.h"
#include<macros.h>

#endif

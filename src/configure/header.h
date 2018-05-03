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
 * signal key
 *============================================================================*/
#define SIG_HTTPD_REV			200
#define SIG_FPTD_REV			201

/*==============================================================================
 * log level
 *============================================================================*/

#define LOG_PHY						1
#define LOG_OS						2
#define LOG_MID						3
#define LOG_APP						4

/*==============================================================================
 * server port
 *============================================================================*/
#define FPTD_TCP_PORT 	8888
#define HTTPD_TCP_PORT 	8080

/*==============================================================================
 * HTTPT SERVER
 *============================================================================*/

// file access
#define HTTPD_FILE_HTML  0
#define HTTPD_FILE_JPEG  1

/*==============================================================================
 * FPT SERVER
 *============================================================================*/
#define FPT_CMD_KEY_VALUE		"FPT_COMMAND"
#define FPT_CMD_KEY_SIZE		11

#define FPT_DATA_KEY_VALUE		"FPT_DATA"
#define FPT_DATA_KEY_SIZE		8

#define FPT_USE_LEN				30
#define FPT_PASS_LEN			30
#define FPT_CMD_LEN				128

// list command

#define FPT_CMD_LOGIN			"LOGIN"
#define FPT_CMD_LOGOUT			"LOGOUT"
#define FPT_CMD_REBOOT			"REBOOT"
#define FPT_CMD_IPCFG			"IPCFG"
#define FPT_CMD_UPLOAD			"UPLOAD"
#define FPT_CMD_DOWLOAD			"DOWLOAD"
#define FPT_CMD_MKDIR			"MKDIR"
#define FPT_CMD_RMDIR			"RMDIR"
#define FPT_CMD_TOUCH			"TOUCH"
#define FPT_CMD_RMFIL			"RMFIL"
#define FPT_CMD_STATUS			"STATUS"

// list notify

#define FPT_NOTI_PERMIS_DENIED	"PERMISSION_DENIED"
#define FPT_NOTI_PERMIS_OK		"PERMISSION_OK"
#define FPT_NOTI_LOGIN_OK		"LOGIN_OK"
#define FPT_NOTI_LOGIN_FAIL		"LOGIN_FAIL"
#define FPT_NOTI_LOGOUT_OK		"LOGOUT_OK"
#define FPT_NOTI_LOGOUT_FAIL	"LOGOUT_FAIL"

#define FPT_NOTI_REBOOT_OK		"REBOOT_ACCEPT"
#define FPT_NOTI_REBOOT_REFUSE	"REBOOT_REFUSE"

#define FPT_NOTI_RMFIL_OK		"RMFIL_OK"
#define FPT_NOTI_RMFIL_FAIL		"RMFIL_FAIL"

#define FPT_NOTI_TOUCH_OK		"TOUCH_OK"
#define FPT_NOTI_TOUCH_FAIL		"TOUCH_FAIL"

#define FPT_NOTI_RMDIR_OK		"RMDIR_OK"
#define FPT_NOTI_RMDIR_FAIL		"RMDIR_FAIL"

#define FPT_NOTI_MKDIR_OK		"MKDIR_OK"
#define FPT_NOTI_MKDIR_FAIL		"MKDIR_FAIL"

#define FPT_NOTI_DOWLOAD_ACCEPT	"DOWLOAD_ACCEPT"
#define FPT_NOTI_DOWLOAD_REFUSE	"DOWLOAD_REFUSE"

#define FPT_NOTI_UPLOAD_ACCEPT	"UPLOAD_ACCEPT"
#define FPT_NOTI_UPLOAD_REFUSE	"UPLOAD_REFUSE"
#define FPT_NOTI_OPLOAD_RECV_OK		"RECV_OK"
#define FPT_NOTI_OPLOAD_RECV_FAIL	"RECV_FAIL"

// status

#define FPT_UPLOAD_CONNECT			0
#define FPT_UPLOAD_DISCONNECT		1

/*==============================================================================
 * reset system
 *============================================================================*/

#define NVIC_AIRCR_VECTKEY    (0x5FA << 16)   /*!< AIRCR Key for write access   */
#define NVIC_SYSRESETREQ            2         /*!< System Reset Request         */

/*==============================================================================
 * log level
 *============================================================================*/



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

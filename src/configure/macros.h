/*==============================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *============================================================================*/

/*==================================================================
 * In this file:
 * 		- define some macro usually use
 * 		- define macro print log to LCD
 *==================================================================*/

#ifndef __MACROS_H__
#define __MACROS_H__

#include <header.h>
//#include "console_serial_trace.h"

//#include "ConsoleLog.h"

#if BUILD_TARGET == BUILD_TARGET_DEBUG

#define PRINT_LOG(tofile, lvLog, content) do{} while(0)

#define __ENTER__	do { console_serial_print_enter("%s%50s ", __FUNCTION__, __FILE__); } while (0);

#define __LEAVE__	do { console_serial_print_leave("%s%50s ", __FUNCTION__, __FILE__); } while (0);

#else

#define __ENTER__			do{}while(0);
#define __LEAVE__			do{}while(0);

#endif

//#define 	HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000); 	Console_Log_Print(str1);

#endif

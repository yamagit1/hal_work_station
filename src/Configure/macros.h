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

//#include "lcd1202.h"

#include <header.h>
//#include"LCD1202.h"
//#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG
//
//#define HIGHT(X)
//#define LOW(X)			GPIO_ResetBits(LCD_COM,(X))
//
//#else
//
//#define HIGHT(X)
//#define LOW(X)
//
//#endif
////
//#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG
//#define	LogT(x)			LCD1202_PrintT("[L]");LCD1202_PrintT(x);LCD1202_EndLine()
//#define	LogN(x)			LCD1202_PrintT("[L]");LCD1202_PrintN(x);LCD1202_EndLine()
//#define	LogError(x)		LCD1202_PrintT("[E]");LCD1202_PrintT(x);LCD1202_EndLine()
//#else
//#define LogT(x)
//#define LogN(x)
//#define LogError(X)
//#endif

#if BUILD_TARGET == BUILD_TARGET_DEBUG

#define PRINT_LOG(tofile, lvLog, content) do{								\
						} while(0)

#define __LOG__(str)  do{ 													\
							LCD1202_printText(str, &gLcd1202CurrentOffset); \
							LCD1202_endLine();								\
							LCD1202_flush();								\
						}while(0)

#define __ENTER__(func)	do{ 													\
							LCD1202_printText(func, &gLcd1202CurrentOffset); 	\
							LCD1202_printText("--", &gLcd1202CurrentOffset); 	\
							LCD1202_flush();									\
						}while(0)

#define __LEAVE__()		do{ 												\
							LCD1202_printText("ok", &gLcd1202CurrentOffset); \
							LCD1202_endLine();								\
							LCD1202_flush();								\
						}while(0)

#else
#define PRINT_LOG(tofile, lvLog, content) do{								\
						} while(0)

#define __ENTER__(func)		do{}while(0)
#define __LEAVE__()			do{}while(0)

#define _Error_Handler(__FILE__, __LINE__) do {}while(1)
#endif

#endif

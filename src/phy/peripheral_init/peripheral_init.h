/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: peripheral_init.h
 * in this file :
 *============================================================================*/

#ifndef __PERIPHERAL_INIT_H__
#define __PERIPHERAL_INIT_H__

#include "header.h"

#ifdef __cplusplus
 extern "C" {
#endif
//
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);


void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#ifdef __cplusplus
}
#endif


#endif /* __PERIPHERAL_INIT_H__ */

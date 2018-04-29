/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: micro_sd_spi.c
 * in this file :
 *============================================================================*/

#ifndef __MICRO_SD_SPI_H__
#define __MICRO_SD_SPI_H__

#include "header.h"
#include "cmsis_os.h"

extern SPI_HandleTypeDef hspi1;

#define SS_SD_SELECT() 			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET)
#define SS_SD_DESELECT() 		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET)

__uint8 microSD_Send_And_Receive_Byte(__uint8 data);
void microSD_SPI_Init(void);

#endif /* __MICRO_SD_SPI_H__ */

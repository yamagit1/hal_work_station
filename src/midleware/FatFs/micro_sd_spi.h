/*
 * micro_sd_spi.h
 *
 *  Created on: Apr 25, 2018
 *      Author: yama
 */

#ifndef __MICRO_SD_SPI_H__
#define __MICRO_SD_SPI_H__

#include "header.h"

extern SPI_HandleTypeDef hspi1;

#define SS_SD_SELECT() 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)
#define SS_SD_DESELECT() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)

__uint8 microSD_Send_And_Receive_Byte(__uint8 data);
void microSD_SPI_Init(void);

#endif /* __MICRO_SD_SPI_H__ */

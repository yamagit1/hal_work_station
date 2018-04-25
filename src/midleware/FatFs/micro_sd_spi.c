/*
 * micro_sd_spi.c
 *
 *  Created on: Apr 25, 2018
 *      Author: yama
 */


#include "micro_sd_spi.h"
#include "peripheral_init.h"

SPI_HandleTypeDef hspi1;

__uint8 microSD_Send_And_Receive_Byte(__uint8 data)
{
	// while transfer complete
	uint8_t receivedbyte = 0;
	if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &data,(uint8_t*) &receivedbyte, 1, 0x1000) != HAL_OK)
	{
	}

	return receivedbyte;
}


void microSD_SPI_Init(void)
{
	__ENTER__

	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;

	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		console_serial_print_error("SPI1 init fail");
		//	    _Error_Handler(__FILE__, __LINE__);
	}

	__LEAVE__
}

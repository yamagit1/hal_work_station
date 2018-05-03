/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: micro_sd_spi.c
 * in this file :
 *============================================================================*/


#include "micro_sd_spi.h"
#include "peripheral_init.h"
#include "console_serial_trace.h"

SPI_HandleTypeDef hspi1;

__uint8 microSD_Send_And_Receive_Byte(__uint8 data)
{
	// while transfer complete
	uint8_t receivedbyte = 0;

	if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &data,(uint8_t*) &receivedbyte, 1, 0x1000) != HAL_OK)
	{
		console_serial_print_error("HAL_SPI_TransmitReceive : transmission  fail");
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

	// Register SPI1 for microSD casrd
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		console_serial_print_error("SPI1 init fail");
		//	    _Error_Handler(__FILE__, __LINE__);
	}

	__LEAVE__
}

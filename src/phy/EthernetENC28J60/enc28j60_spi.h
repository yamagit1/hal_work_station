/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: enc28j60_spi.h
 * in this file :
 *============================================================================*/

#ifndef __ENC28J60_SPI_H__
#define __ENC28J60_SPI_H__

#include "header.h"

extern SPI_HandleTypeDef hspi2;

extern void _Error_Handler(char *, int);

void MX_SPI2_Init(void);


#define enc28j60_Spi_Receive() ENC28J60_Send_And_Receive_Byte(0xff)
#define enc28j60_Spi_Send(data) ENC28J60_Send_And_Receive_Byte(data)

#define enc28j60_Enable() 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define enc28j60_Disable() 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

__uint8 ENC28J60_Send_And_Receive_Byte(__uint8 data);
void ENC28J60_SPI_Init(void);

#endif /* __ENC28J60_SPI_H__ */

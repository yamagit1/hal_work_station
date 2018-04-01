/*==============================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *============================================================================*/

#include "console_serial_traffic.h"

#define UART_TIME_OUT 			10
#define BUFF_SIZE_LOW 			64
#define BUFF_SIZE_MEDIUM		256
#define BUFF_SIZE_LARGE			512
#define BUFF_SIZE_VERY_LARGE	1024


UART_HandleTypeDef g_HandleUART;

void console_serial_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Peripheral clock enable
	__HAL_RCC_USART1_CLK_ENABLE();

	//USART1 GPIO Configuration
	//	    PB6     ------> USART1_TX
	//	    PB7     ------> USART1_RX

	GPIO_InitStruct.Pin 		= GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull 		= GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USART1 interrupt Init */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	// USART1 init
	g_HandleUART.Instance 		= USART1;
	g_HandleUART.Init.BaudRate 	= 115200;
	g_HandleUART.Init.WordLength = UART_WORDLENGTH_8B;
	g_HandleUART.Init.StopBits 	= UART_STOPBITS_1;
	g_HandleUART.Init.Parity 	= UART_PARITY_NONE;
	g_HandleUART.Init.Mode 		= UART_MODE_TX_RX;
	g_HandleUART.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
	g_HandleUART.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&g_HandleUART) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}


void console_serial_tx(char *p_str)
{
	// wait for ready
	while(HAL_UART_GetState(&g_HandleUART) != HAL_UART_STATE_BUSY_TX );

	// transmission data
	HAL_UART_Transmit(&g_HandleUART, (uint8_t*)p_str, sizeof(*p_str), UART_TIME_OUT);

}


void console_serial_rx()
{
	// TODO: declare function
}


void console_serial_print_char(char *p_ch)
{
	while(HAL_UART_GetState(&g_HandleUART) != HAL_UART_STATE_BUSY_TX );
	HAL_UART_Transmit(&g_HandleUART, (uint8_t*)p_ch, 1, UART_TIME_OUT);
}


void console_serial_print_string(char *p_str)
{

	while(*p_str)
	{
		console_serial_print_char(p_str);
		p_str++;
	}
}


void console_serial_print_line(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get list arg with format
	va_start (vaArgs, p_format);

	// Print to the local buffer
	vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

	va_end (vaArgs);

	// transmission data
	console_serial_print_string(buffer);
	console_serial_print_string("\n\r");
}


void console_serial_print_log(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get list arg with format
	va_start (vaArgs, p_format);

	// Print to the local buffer
	vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

	va_end (vaArgs);

	// transmission data
	console_serial_tx(buffer);

	// print with format : [__log__]:...
	console_serial_print_string("[___LOG___]: ");
	console_serial_print_line(buffer);
}


void console_serial_print_warring(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get list arg with format
	va_start(vaArgs, p_format);

	// Print to the local buffer
	vsnprintf(buffer, sizeof(buffer), p_format, vaArgs);

	va_end(vaArgs);

	// transmission data
	console_serial_tx(buffer);

	// print with format : [_WARRING_]:...
	console_serial_print_string("[_WARRING_]: ");
	console_serial_print_line(buffer);
}


void console_serial_print_error(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get list arg with format
	va_start (vaArgs, p_format);

	// Print to the local buffer
	vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

	va_end(vaArgs);

	// transmission data
	console_serial_tx(buffer);

	// print with format : [__ERROR__]:...
	console_serial_print_string("[__ERROR__]: ");
	console_serial_print_line(buffer);
}


void console_serial_print_infor()
{
	// TODO: declare function
}


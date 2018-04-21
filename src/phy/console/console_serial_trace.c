/*==============================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *============================================================================*/

#include "console_serial_trace.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

osSemaphoreId consoleSemaphoreID;


void console_serial_init()
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_8;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		//      _Error_Handler(__FILE__, __LINE__);
	}
	// init semaphore for console rs232 device
	osSemaphoreDef(console_log);
	consoleSemaphoreID = osSemaphoreCreate(osSemaphore(console_log) , 1 );

	// Release console rs232 device
	osSemaphoreRelease(consoleSemaphoreID);
}


void console_serial_tx(char *p_str)
{
	// wait for ready transmission
	while( !(USART1->SR & 0x00000040) );

	// transmission data
	HAL_UART_Transmit(&huart1, (uint8_t*)p_str, 1, 0x1000);

	//	USART_SendData(USART1, p_str);

}


void console_serial_rx()
{
	// TODO: declare function
}


void console_serial_print_char(const char *p_ch)
{
	console_serial_tx((char*)p_ch);
}


void console_serial_print_string(volatile char *p_str)
{
	while (*p_str)
	{
		// wait until data register is empty
		while( !(USART1->SR & 0x00000040) );

		console_serial_tx((char*)p_str);

		p_str++;
	}
}


void console_serial_print_line(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// start get list arg with format
	va_start (vaArgs, p_format);

	// Print to the local buffer
	vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

	// end get list arg
	va_end (vaArgs);

	// transmission data
	console_serial_print_string(buffer);
	console_serial_print_string("\n\r");
}


void console_serial_print_log(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get device uart
	if (osSemaphoreWait (consoleSemaphoreID, CONSOLE_WAIT_TIME) == osOK)
	{
		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// print with format : [__log__]:...
		console_serial_print_string("[___LOG___]: ");
		console_serial_print_line(buffer);

		// release device uart
		osSemaphoreRelease(consoleSemaphoreID);
	}
	else
	{
		console_serial_print_string("[__ERROR__]: semaphore for device uart not release\r\n");
	}
}


void console_serial_print_warring(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get device uart
	if (osSemaphoreWait (consoleSemaphoreID, CONSOLE_WAIT_TIME) == osOK)
	{

		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// print with format : [_WARRING_]:...
		console_serial_print_string("[_WARRING_]: ");
		console_serial_print_line(buffer);

		// release device uart
		osSemaphoreRelease(consoleSemaphoreID);
	}
	else
	{
		console_serial_print_string("[__ERROR__]: semaphore for device uart not release\r\n");
	}
}


void console_serial_print_error(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get device uart
	if (osSemaphoreWait (consoleSemaphoreID, CONSOLE_WAIT_TIME) == osOK)
	{

		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// print with format : [__ERROR__]:...
		console_serial_print_string("[__ERROR__]: ");
		console_serial_print_line(buffer);

		// release device uart
		osSemaphoreRelease(consoleSemaphoreID);
	}
	else
	{
		console_serial_print_string("[__ERROR__]: semaphore for device uart not release\r\n");
	}

}


void console_serial_print_infor(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get device uart
	if (osSemaphoreWait (consoleSemaphoreID, CONSOLE_WAIT_TIME) == osOK)
	{
		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// print with format : [__INFOR__]:...
		console_serial_print_string("[__INFOR__]: ");
		console_serial_print_line(buffer);

		// release device uart
		osSemaphoreRelease(consoleSemaphoreID);
	}
	else
	{
		console_serial_print_string("[__ERROR__]: semaphore for device uart not release\r\n");
	}
}


void console_serial_print_enter(const char* p_format, ...)
{
	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get device uart
	if (osSemaphoreWait (consoleSemaphoreID, CONSOLE_WAIT_TIME) == osOK)
	{

		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// print with format : [__ENTER__]:...
		console_serial_print_string("[__ENTER__]: ");
		console_serial_print_line(buffer);

		// release device uart
		osSemaphoreRelease(consoleSemaphoreID);
	}
	else
	{
		console_serial_print_string("[__ERROR__]: semaphore for device uart not release\r\n");
	}
}


void console_serial_print_leave(const char* p_format, ...)
{

	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	// get device uart
	if (osSemaphoreWait (consoleSemaphoreID, CONSOLE_WAIT_TIME) == osOK)
	{

		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// print with format : [__LEAVE__]:...
		console_serial_print_string("[__LEAVE__]: ");
		console_serial_print_line(buffer);

		// release device uart
		osSemaphoreRelease(consoleSemaphoreID);
	}
	else
	{
		console_serial_print_string("[__ERROR__]: semaphore for device uart not release\r\n");
	}
}


void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(uartHandle->Instance==USART1)
	{
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		/**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USART1 DMA Init */
		/* USART1_RX Init */
		hdma_usart1_rx.Instance = DMA2_Stream2;
		hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_rx.Init.Mode = DMA_NORMAL;
		hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
		{
			//      _Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

		/* USART1_TX Init */
		hdma_usart1_tx.Instance = DMA2_Stream7;
		hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_tx.Init.Mode = DMA_NORMAL;
		hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
		{
			//      _Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	}
}


void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

	if(uartHandle->Instance==USART1)
	{
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

		/* USART1 DMA DeInit */
		HAL_DMA_DeInit(uartHandle->hdmarx);
		HAL_DMA_DeInit(uartHandle->hdmatx);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}
}

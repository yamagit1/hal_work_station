/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: lcd1202.c
 * in this file :
 *============================================================================*/

#include "lcd1202.h"
#include "lcd_font.h"
#include "lcd_fontresource.h"

__uint8 gLcd1202Ram[LCD_1202_BUFFER_SIZE];
__uint32 gLcd1202CurrentOffset;
osSemaphoreId lcd1202SemaphoreID;

#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG

void lcd_1202_delay(volatile __uint32 timeCount)
{
	//HAL_Delay(timeCount);
	__uint32 currCount = timeCount;

	while(currCount--);
}

/**
 *
 */

void lcd_1202_init_pin(__uint32 pinNumber)
{
	GPIO_InitTypeDef pinConfig;
	GPIO_TypeDef *comConfig;

	// setting pin
	pinConfig.Pin			= 	pinNumber;
	pinConfig.Mode			=	GPIO_MODE_OUTPUT_PP;
	pinConfig.Pull			=	GPIO_NOPULL;
	pinConfig.Speed			=	GPIO_SPEED_FREQ_HIGH;

	// select com gpio
	if (pinNumber == LCD_1202_PIN_VCC)
	{
		comConfig = LCD_1202_COM_VCC;
	}
	else if (pinNumber == LCD_1202_PIN_RST)
	{
		comConfig = LCD_1202_COM_RST;
	}
	else if (pinNumber == LCD_1202_PIN_CS)
	{
		comConfig = LCD_1202_COM_CS;
	}
	else if (pinNumber == LCD_1202_PIN_GND)
	{
		comConfig = LCD_1202_COM_GND;
	}
	else if (pinNumber == LCD_1202_PIN_SDA)
	{
		comConfig = LCD_1202_COM_SDA;
	}
	else if (pinNumber == LCD_1202_PIN_SCL)
	{
		comConfig = LCD_1202_COM_SCL;
	}
	else if (pinNumber == LCD_1202_PIN_LIG1)
	{
		comConfig = LCD_1202_COM_LIG1;
	}
	else if (pinNumber == LCD_1202_PIN_LIG2)
	{
		comConfig = LCD_1202_COM_LIG2;
	}

	// init GPIO pin
	HAL_GPIO_Init(comConfig, &pinConfig);
}

/**
 *
 */

void lcd_1202_hight_power_pin(__uint32 pinNumber)
{
	GPIO_TypeDef *comConfig;

	// select com gpio
	if (pinNumber == LCD_1202_PIN_VCC)
	{
		comConfig = LCD_1202_COM_VCC;
	}
	else if (pinNumber == LCD_1202_PIN_RST)
	{
		comConfig = LCD_1202_COM_RST;
	}
	else if (pinNumber == LCD_1202_PIN_CS)
	{
		comConfig = LCD_1202_COM_CS;
	}
	else if (pinNumber == LCD_1202_PIN_GND)
	{
		comConfig = LCD_1202_COM_GND;
	}
	else if (pinNumber == LCD_1202_PIN_SDA)
	{
		comConfig = LCD_1202_COM_SDA;
	}
	else if (pinNumber == LCD_1202_PIN_SCL)
	{
		comConfig = LCD_1202_COM_SCL;
	}
	else if (pinNumber == LCD_1202_PIN_LIG1)
	{
		comConfig = LCD_1202_COM_LIG1;
	}
	else if (pinNumber == LCD_1202_PIN_LIG2)
	{
		comConfig = LCD_1202_COM_LIG2;
	}

	// set pin
	HAL_GPIO_WritePin(comConfig, pinNumber, GPIO_PIN_SET);
}

/**
 *
 */

void lcd_1202_low_power_pin(__uint32 pinNumber)
{
	GPIO_TypeDef *comConfig;

	// select com gpio
	if (pinNumber == LCD_1202_PIN_VCC)
	{
		comConfig = LCD_1202_COM_VCC;
	}
	else if (pinNumber == LCD_1202_PIN_RST)
	{
		comConfig = LCD_1202_COM_RST;
	}
	else if (pinNumber == LCD_1202_PIN_CS)
	{
		comConfig = LCD_1202_COM_CS;
	}
	else if (pinNumber == LCD_1202_PIN_GND)
	{
		comConfig = LCD_1202_COM_GND;
	}
	else if (pinNumber == LCD_1202_PIN_SDA)
	{
		comConfig = LCD_1202_COM_SDA;
	}
	else if (pinNumber == LCD_1202_PIN_SCL)
	{
		comConfig = LCD_1202_COM_SCL;
	}
	else if (pinNumber == LCD_1202_PIN_LIG1)
	{
		comConfig = LCD_1202_COM_LIG1;

	}
	else if (pinNumber == LCD_1202_PIN_LIG2)
	{
		comConfig = LCD_1202_COM_LIG2;
	}

	// reset pin
	HAL_GPIO_WritePin(comConfig, pinNumber, GPIO_PIN_RESET);
}

/**
 *
 */

void lcd_1202_transmission(__E_Lcd_Mode_Send modeSend, __uint8 data)
{
	__uint32 i;

	// setup mode transmission
	if (modeSend == EMS_DATA)
	{
		lcd_1202_hight_power_pin(LCD_1202_PIN_SDA);
	}
	else
	{
		lcd_1202_low_power_pin(LCD_1202_PIN_SDA);
	}

	//
	lcd_1202_low_power_pin(LCD_1202_PIN_SCL);
	lcd_1202_hight_power_pin(LCD_1202_PIN_SCL);

	for (i = 0; i < 8; i++)
	{
		if ((data<<i) & 0x80)
		{
			lcd_1202_hight_power_pin(LCD_1202_PIN_SDA);
		}
		else
		{
			lcd_1202_low_power_pin(LCD_1202_PIN_SDA);
		}

		lcd_1202_low_power_pin(LCD_1202_PIN_SCL);
		lcd_1202_hight_power_pin(LCD_1202_PIN_SCL);
	}

}

/**
 *
 */

void lcd_1202_send_data(__uint8 data)
{
	lcd_1202_low_power_pin(LCD_1202_PIN_CS);
	lcd_1202_transmission(EMS_DATA, data);
	lcd_1202_hight_power_pin(LCD_1202_PIN_CS);

}

/**
 *
 */

void lcd_1202_send_command(__uint8 command)
{
	lcd_1202_low_power_pin(LCD_1202_PIN_CS);
	lcd_1202_transmission(EMS_COMMAND, command);
	lcd_1202_hight_power_pin(LCD_1202_PIN_CS);

}

/**
 *
 */

void lcd_1202_initialize(void)
{
	__ENTER__

	// setting pin connection
	lcd_1202_init_pin(LCD_1202_PIN_VCC);
	lcd_1202_init_pin(LCD_1202_PIN_RST);
	lcd_1202_init_pin(LCD_1202_PIN_CS);
	lcd_1202_init_pin(LCD_1202_PIN_GND);
	lcd_1202_init_pin(LCD_1202_PIN_SDA);
	lcd_1202_init_pin(LCD_1202_PIN_SCL);
	lcd_1202_init_pin(LCD_1202_PIN_LIG1);
	lcd_1202_init_pin(LCD_1202_PIN_LIG2);

	// Power ON
	lcd_1202_hight_power_pin(LCD_1202_PIN_VCC);
	lcd_1202_low_power_pin(LCD_1202_PIN_GND);
	lcd_1202_low_power_pin(LCD_1202_PIN_LIG1);
	lcd_1202_hight_power_pin(LCD_1202_PIN_LIG2);

	//  set RST + CE + DC + SCK + DIN
	lcd_1202_hight_power_pin(LCD_1202_PIN_RST);
	lcd_1202_hight_power_pin(LCD_1202_PIN_CS);
	lcd_1202_hight_power_pin(LCD_1202_PIN_SCL);
	lcd_1202_hight_power_pin(LCD_1202_PIN_SDA);

	//1. reset LCD
	lcd_1202_low_power_pin(LCD_1202_PIN_CS);
	lcd_1202_low_power_pin(LCD_1202_PIN_RST);
	lcd_1202_hight_power_pin(LCD_1202_PIN_RST);
	//
	lcd_1202_send_command(LCD_1202_VO_VOLTAGE_RANGE_SET_0);
	lcd_1202_send_command(0x90); //LCD_1202_ELECTRONIC_VOLUME_15_step
	lcd_1202_send_command(LCD_1202_DISPLAY_ALL_POINT_OFF);
	lcd_1202_send_command(LCD_1202_POWER_CONTROL_SET_ON);
	//
	lcd_1202_send_command(LCD_1202_PAGE_ADDRESS_SET_0);
	lcd_1202_send_command(LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT);
	lcd_1202_send_command(LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT);
	//
	lcd_1202_send_command(LCD_1202_DISPLAY_ON);
	//
	lcd_1202_send_command(0xA7);
	lcd_1202_send_command(0xA6);

	lcd_1202_clear_screen();

	LF_setFontIsUse(gFontFullYama);

	// create semaphore for lcd1202 device
	osSemaphoreDef(lcd1202);
	lcd1202SemaphoreID = osSemaphoreCreate(osSemaphore(lcd1202) , 1 );

	// Release enc28j60 device
	osSemaphoreRelease(lcd1202SemaphoreID);

	__LEAVE__
}

/**
 *
 */

void lcd_1202_clear_screen(void)
{
	__uint32 i;

	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{
		// clear lcd cache
		for (i = 0; i < LCD_1202_BUFFER_SIZE; i++)
		{
			gLcd1202Ram[i] = 0x00;
		}

		// reset cache offset
		gLcd1202CurrentOffset = 0;

		// reset offset in lcd ram
		lcd_1202_send_command(LCD_1202_PAGE_ADDRESS_SET_0);
		lcd_1202_send_command(LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT);
		lcd_1202_send_command(LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT);

		lcd_1202_flush();

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}

}

/**
 *
 */

void lcd_1202_view_image_bitmap(__uint8 *p_img)
{
	__int32 index;

	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{
		for (index = 0; index < LCD_1202_BUFFER_SIZE; index++)
		{
			lcd_1202_send_data(p_img[index]);
		}

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}

}


/**
 *
 */

void lcd_1202_flush(void)
{
	__int32 index;

	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{
		lcd_1202_send_command(LCD_1202_PAGE_ADDRESS_SET_0);
		lcd_1202_send_command(LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT);
		lcd_1202_send_command(LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT);

		for (index = 0; index < LCD_1202_BUFFER_SIZE; index++)
		{
			lcd_1202_send_data(gLcd1202Ram[index]);
		}

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}

}

/**
 *
 */

void lcd_1202_setting_led_background(__int32 status)
{
	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{
		if (status)
		{
			lcd_1202_low_power_pin(LCD_1202_PIN_LIG2);
		}
		else
		{
			lcd_1202_hight_power_pin(LCD_1202_PIN_LIG2);
		}

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}
}

/**
 *
 */
void lcd_1202_set_position(__uint32 position)
{
	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{
		gLcd1202CurrentOffset = position;

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}
}



void lcd_1202_print_line(const char* p_format, ...)
{
	__uint32 length;
	__uint32 index;

	char buffer[BUFF_SIZE_MEDIUM];
	va_list vaArgs;

	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{
		// start get list arg with format
		va_start (vaArgs, p_format);

		// Print to the local buffer
		vsnprintf (buffer, sizeof(buffer), p_format, vaArgs);

		// end get list arg
		va_end (vaArgs);

		// transmission data
		length = strlen(buffer);
		for (index = 0; index < length; index++)
		{
			LF_printCharToLcdRam(buffer[index], &gLcd1202CurrentOffset);
		}

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}
}

/**
 *
 */

void lcd_1202_endLine(void)
{
	__int32 currentLine;
	__int32 i;

	if (osSemaphoreWait(lcd1202SemaphoreID, TIME_WAIT_MEDIUM) == osOK)
	{

		currentLine = (__int32)(gLcd1202CurrentOffset / LCD_1202_SCREEN_WIDTH);

		if (currentLine == LCD_1202_TOTAL_LINE_TEXT)
		{
			for ( i = 0; i < LCD_1202_SCREEN_WIDTH; i++)
			{
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_0 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_1 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_1 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_2 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_2 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_3 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_3 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_4 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_4 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_5 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_5 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_6 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_6 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_7 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_7 + i] = gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_8 + i];
				gLcd1202Ram[LCD_1202_ADDRESS_LINE_TEXT_8 + i] = 0x00;
			}

			gLcd1202CurrentOffset = LCD_1202_ADDRESS_LINE_TEXT_8;
		}
		else
		{
			gLcd1202CurrentOffset = (currentLine + 1) * LCD_1202_SCREEN_WIDTH;
		}

		// Release enc28j60 device
		osSemaphoreRelease(lcd1202SemaphoreID);
	}
}

#endif
//=================================================

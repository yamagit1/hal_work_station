/*==================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *==================================================================*/

/*==================================================================
 * In this file:
 *==================================================================*/

#include "lcd1202.h"
#include "lcd_font.h"
#include "lcd_fontresource.h"
#include "string.h"

__uint8 gLcd1202Ram[LCD_1202_BUFFER_SIZE];
__uint32 gLcd1202CurrentOffset;

#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG

/**
 *
 */

void LCD1202_delay(volatile __uint32 timeCount)
{

	while (timeCount != 0)
	{
		timeCount--;
	}

}

/**
 *
 */

void LCD1202_configurePin(GPIO_InitTypeDef *p_pinConfig, __uint32 pinNumber)
{

	(*p_pinConfig).Pin			= 	pinNumber;
	(*p_pinConfig).Mode			=	GPIO_MODE_OUTPUT_PP;
	(*p_pinConfig).Pull	=	GPIO_NOPULL;
	(*p_pinConfig).Speed	=	GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(LCD_1202_COM, p_pinConfig);
}

/**
 *
 */

void LCD1202_hightPowerPin(__uint32 pinSetup)
{
	HAL_GPIO_WritePin(LCD_1202_COM, pinSetup, GPIO_PIN_SET);
}

/**
 *
 */

void LCD1202_lowerPowerPin(__uint32 pinSetup)
{
	HAL_GPIO_WritePin(LCD_1202_COM, pinSetup, GPIO_PIN_RESET);
}

/**
 *
 */

void LCD1202_settingPinConnection(void)
{

	GPIO_InitTypeDef pinConfig;

	LCD1202_configurePin(&pinConfig, LCD_1202_VCC);
	LCD1202_configurePin(&pinConfig, LCD_1202_RST);
	LCD1202_configurePin(&pinConfig, LCD_1202_CS);
	LCD1202_configurePin(&pinConfig, LCD_1202_GND);
	LCD1202_configurePin(&pinConfig, LCD_1202_SDA);
	LCD1202_configurePin(&pinConfig, LCD_1202_SCL);
	LCD1202_configurePin(&pinConfig, LCD_1202_LIG1);
	LCD1202_configurePin(&pinConfig, LCD_1202_LIG2);
	//
	// cap nguan--Pow ON
	HAL_GPIO_WritePin(LCD_1202_COM, LCD_1202_VCC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_1202_COM, LCD_1202_GND, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_1202_COM, LCD_1202_LIG1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_1202_COM, LCD_1202_LIG2, GPIO_PIN_SET);

}

/**
 *
 */

void LCD1202_transficData(__E_Lcd_Mode_Send modeSend, __uint8 data)
{

	__uint32 i;

	if (modeSend == EMS_DATA)
	{
		LCD1202_hightPowerPin(LCD_1202_SDA);
	}
	else
	{
		LCD1202_lowerPowerPin(LCD_1202_SDA);
	}

	LCD1202_lowerPowerPin(LCD_1202_SCL);
	LCD1202_hightPowerPin(LCD_1202_SCL);

	for (i = 0; i < 8; i++)
	{
		if ((data<<i) & 0x80)
		{
			LCD1202_hightPowerPin(LCD_1202_SDA);
		}
		else
		{
			LCD1202_lowerPowerPin(LCD_1202_SDA);
		}

		LCD1202_lowerPowerPin(LCD_1202_SCL);
		LCD1202_hightPowerPin(LCD_1202_SCL);

//		if (modeSend == EMS_COMMAND)
//		{
//			LCD1202_delay(200);
//		}
	}

}

/**
 *
 */

void LCD1202_sendData(__uint8 data)
{

	LCD1202_lowerPowerPin(LCD_1202_CS);
	LCD1202_transficData(EMS_DATA, data);
	LCD1202_hightPowerPin(LCD_1202_CS);

}

/**
 *
 */

void LCD1202_sendCommand(__uint8 command)
{

	LCD1202_lowerPowerPin(LCD_1202_CS);
	LCD1202_transficData(EMS_COMMAND, command);
	LCD1202_hightPowerPin(LCD_1202_CS);

}

/**
 *
 */

void LCD1202_initialize(void)
{

	__ENTER__

	LCD1202_settingPinConnection();

	LCD1202_hightPowerPin(LCD_1202_RST);								//  set RST + CE + DC + SCK + DIN
	LCD1202_hightPowerPin(LCD_1202_CS);
	LCD1202_hightPowerPin(LCD_1202_SCL);
	LCD1202_hightPowerPin(LCD_1202_SDA);

	//1. reset LCD
	LCD1202_lowerPowerPin(LCD_1202_CS);
	LCD1202_lowerPowerPin(LCD_1202_RST);
	LCD1202_hightPowerPin(LCD_1202_RST);
	//
	LCD1202_sendCommand(LCD_1202_VO_VOLTAGE_RANGE_SET_0);
	LCD1202_sendCommand(0x90); //LCD_1202_ELECTRONIC_VOLUME_15_step
	LCD1202_sendCommand(LCD_1202_DISPLAY_ALL_POINT_OFF);
	LCD1202_sendCommand(LCD_1202_POWER_CONTROL_SET_ON);
	//
	LCD1202_sendCommand(LCD_1202_PAGE_ADDRESS_SET_0);
	LCD1202_sendCommand(LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT);
	LCD1202_sendCommand(LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT);
	//
	LCD1202_sendCommand(LCD_1202_DISPLAY_ON);
	//
	LCD1202_sendCommand(0xA7);
	LCD1202_sendCommand(0xA6);

	LCD1202_clearScreen();

	LF_setFontIsUse(gFontFullYama);

	__LEAVE__
}

/**
 *
 */

void LCD1202_clearScreen(void)
{

	__uint32 i;

	for (i = 0; i < LCD_1202_BUFFER_SIZE; i++)
	{
		gLcd1202Ram[i] = 0x00;
	}

	gLcd1202CurrentOffset = 0;

	//
	LCD1202_sendCommand(LCD_1202_PAGE_ADDRESS_SET_0);
	LCD1202_sendCommand(LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT);
	LCD1202_sendCommand(LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT);

	LCD1202_flush();

}

/**
 *
 */

void LCD1202_viewImageBitmap(__uint8 *p_img)
{

	__int32 index;

	for (index = 0; index < LCD_1202_BUFFER_SIZE; index++)
	{
		LCD1202_sendData(p_img[index]);
	}

}


/**
 *
 */

void LCD1202_flush(void)
{

	__int32 index;

	LCD1202_sendCommand(LCD_1202_PAGE_ADDRESS_SET_0);
	LCD1202_sendCommand(LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT);
	LCD1202_sendCommand(LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT);

	for (index = 0; index < LCD_1202_BUFFER_SIZE; index++)
	{
		LCD1202_sendData(gLcd1202Ram[index]);
	}

}

/**
 *
 */

void LCD1202_turnOffLedBackground(__int32 status)
{

	if (status)
	{
		LCD1202_lowerPowerPin(LCD_1202_LIG2);
	}
	else
	{
		LCD1202_hightPowerPin(LCD_1202_LIG2);
	}

}

/**
 *
 */

void LCD1202_printText(const char *str, __uint32 *p_position)
{
	__uint32 length;
	__uint32 index;

	length = strlen(str);

	for (index = 0; index < length; index++)
	{
		if ((index % (LCD_1202_SCREEN_WIDTH / gCurrentFont.charWidth ) ) == 0 && index != 0)
		{
			LCD1202_endLine();
		}

		LF_printCharToLcdRam(str[index], p_position);
	}
}

/**
 *
 */

void LCD1202_printNumberInterger(__int64 numberPrint,__uint32  *p_position)
{
	__uint8 buffer[20];
	__int32 currentIndexBuffer = 0;
	__int32 mumberDigit = 0;
	__int64 mask = 1;
	__int32 i;

	if (numberPrint == 0)
	{
		LF_printCharToLcdRam('0', p_position);
		return;
	}

	if (numberPrint < 0)
	{
		numberPrint = numberPrint * -1;

		buffer[currentIndexBuffer] = '-';
		currentIndexBuffer++;
	}

	while (mask <= numberPrint)
	{
		mask *= 10;
		mumberDigit++;
	}

	mask /= 10;

	while (mumberDigit > 0)
	{
		buffer[currentIndexBuffer] = (__uint8)((numberPrint / mask) + 48);
		currentIndexBuffer++;

		numberPrint %= mask;
		mask /= 10;
		mumberDigit--;
	}

	for(i = 0; i < currentIndexBuffer; i++)
	{
		LF_printCharToLcdRam(buffer[i], p_position);
	}

//	LCD1202_Flush();
}

/**
 *
 */

void LCD1202_endLine(void)
{
	__int32 currentLine;
	__int32 i;

	currentLine = (__int32)(gLcd1202CurrentOffset / LCD_1202_SCREEN_WIDTH);

	if (currentLine == (LCD_1202_TOTAL_LINE_TEXT))
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
}

#endif
//=================================================

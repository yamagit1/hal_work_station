/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: lcd1202.c
 * in this file :
 * 		- define pin connect
 * 		- use 864 byte ram to make cache LCD
 *============================================================================*/

#ifndef __LCD1202_H__
#define __LCD1202_H__

#include <header.h>
#include "cmsis_os.h"

/*==================================================================
 * command
 *==================================================================*/

#define LCD_1202_SCREEN_WIDTH		96
#define LCD_1202_SCREEN_HIGHT		68
#define LCD_1202_BUFFER_SIZE		864 // 96 * (72/8)

#define LCD_1202_DISPLAY_ON									0xAF
#define LCD_1202_DISPLAY_OFF								0xAE

#define LCD_1202_DISPLAY_NORMAL								0xA6
#define LCD_1202_DISPLAY_REVERSE							0xA7

#define LCD_1202_DISPLAY_ALL_POINT_ON						0xA5
#define LCD_1202_DISPLAY_ALL_POINT_OFF						0xA4

#define LCD_1202_PAGE_ADDRESS_SET_0							0xB0
#define LCD_1202_PAGE_ADDRESS_SET_1							0xB1
#define LCD_1202_PAGE_ADDRESS_SET_2							0xB2
#define LCD_1202_PAGE_ADDRESS_SET_3							0xB3
#define LCD_1202_PAGE_ADDRESS_SET_4							0xB4
#define LCD_1202_PAGE_ADDRESS_SET_5							0xB5
#define LCD_1202_PAGE_ADDRESS_SET_6							0xB6
#define LCD_1202_PAGE_ADDRESS_SET_7							0xB7
#define LCD_1202_PAGE_ADDRESS_SET_8							0xB8

#define LCD_1202_COLUMN_ADDRESS_SET_UPPER_DEFAULT			0x10
#define LCD_1202_COLUMN_ADDRESS_SET_LOWER_DEFAULT			0x00

#define LCD_1202_COLUMN_ADDRESS_SET_UPPER					LCD_1202_NOP
#define LCD_1202_COLUMN_ADDRESS_SET_LOWER					LCD_1202_NOP

#define LCD_1202_DISPLAY_START_LINE_ADDRESS_SET(line)		((0x40 & 0x3F) | (line | 0x3F))

#define LCD_1202_SEGMENT_DRIVER_DIRECTION_NORMAL			0xA0
#define LCD_1202_SEGMENT_DRIVER_DIRECTION_REVERSE			0xA1

#define LCD_1202_COMMON_DRIVER_DIRECTION_SELECT_NORMAL		0xC0
#define LCD_1202_COMMON_DRIVER_DIRECTION_SELECT_REVERSE		0xC8

#define LCD_1202_SELF_TEST_DATA_READING_COMMAND_IDENTIFIER	0xDB

#define LCD_1202_POWER_CONTROL_SET_ON						0x2F
#define LCD_1202_POWER_CONTROL_SET_OFF						0x2E

#define LCD_1202_VO_VOLTAGE_RANGE_SET_0						0x20	// 3.00 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_1						0x21	// 4.28 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_2						0x22	// 5.56 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_3						0x23	// 6.84 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_4						0x24	// 8.12 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_5						0x25	// 9.40 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_6						0x26	// 10.68 V
#define LCD_1202_VO_VOLTAGE_RANGE_SET_7						0x27	// 11.96 V

#define LCD_1202_VOP_SET_COMMAND_IDENTIFIER					0xE1
#define LCD_1202_VOP_SET_DEFAULT							0x00

#define LCD_1202_ELECTRONIC_VOLUME_00_step					0x80	//	Low
#define LCD_1202_ELECTRONIC_VOLUME_05_step					0x85	//	 ^
#define LCD_1202_ELECTRONIC_VOLUME_10_step					0x8A	//	 "
#define LCD_1202_ELECTRONIC_VOLUME_15_step					0x8E	//	 "
#define LCD_1202_ELECTRONIC_VOLUME_20_step					0x95	//	 "
#define LCD_1202_ELECTRONIC_VOLUME_25_step					0x9A	//	 V
#define LCD_1202_ELECTRONIC_VOLUME_30_step					0x9E	//	Hight

#define LCD_1202_RESET										0xE2	// not change DDRAM

#define LCD_1202_NOP										0xE3


#define LCD_1202_TERMAL_COMPENSATION_COMMAND_IDENTIFIER		0x38
#define LCD_1202_TERMAL_COMPENSATION_VALUE_0PPM				0x00
#define LCD_1202_TERMAL_COMPENSATION_VALUE_300PPM			0x01
#define LCD_1202_TERMAL_COMPENSATION_VALUE_600PPM			0x02
#define LCD_1202_TERMAL_COMPENSATION_VALUE_900PPM			0x03
#define LCD_1202_TERMAL_COMPENSATION_VALUE_1070PPM			0x04
#define LCD_1202_TERMAL_COMPENSATION_VALUE_1200PPM			0x05
#define LCD_1202_TERMAL_COMPENSATION_VALUE_1500PPM			0x06
#define LCD_1202_TERMAL_COMPENSATION_VALUE_1800PPM			0x07

#define LCD_1202_CHARGE_PUMP_COMMAND_IDENTIFIER				0x3D
#define LCD_1202_CHARGE_PUMP_5x								0x00
#define LCD_1202_CHARGE_PUMP_4x								0x01
#define LCD_1202_CHARGE_PUMP_3x								0x02
#define LCD_1202_CHARGE_PUMP_DISENABLE						0x03

#define LCD_1202_REFRESH_RATE_COMMAND_IDENTIFIER			0xEF
#define LCD_1202_REFRESH_RATE_80Hz							0x00
#define LCD_1202_REFRESH_RATE_75Hz							0x01
#define LCD_1202_REFRESH_RATE_70Hz							0x02
#define LCD_1202_REFRESH_RATE_65Hz							0x03

#define LCD_1202_N_LINE_INVERSION_COMMAND_IDENTIFIER		0xAD
#define LCD_1202_N_LINE_INVERSION_ENABLE					0x01
#define LCD_1202_N_LINE_INVERSION_DISABLED					0x00
#define LCD_1202_N_LINE_INVERSION_XOR_ENABLE				0x00
#define LCD_1202_N_LINE_INVERSION_XOR_DISENABLE				0x20





/*==================================================================
 * setup Pin | 3-line 9-bit seri interface
 *==================================================================*/

#if CONFIG_FLATFORM == FLATFORM_STM32_F407VG

// pin select for interface

#define LCD_1202_PIN_VCC		GPIO_PIN_7
#define LCD_1202_PIN_RST 		GPIO_PIN_8
#define LCD_1202_PIN_CS			GPIO_PIN_9
#define LCD_1202_PIN_GND		GPIO_PIN_10
#define LCD_1202_PIN_SDA		GPIO_PIN_11
#define LCD_1202_PIN_SCL		GPIO_PIN_12
#define LCD_1202_PIN_LIG1		GPIO_PIN_13
#define LCD_1202_PIN_LIG2		GPIO_PIN_14

// port select for interface

#define LCD_1202_COM_VCC		GPIOE
#define LCD_1202_COM_RST		GPIOE
#define LCD_1202_COM_CS			GPIOE
#define LCD_1202_COM_GND		GPIOE
#define LCD_1202_COM_SDA		GPIOE
#define LCD_1202_COM_SCL		GPIOE
#define LCD_1202_COM_LIG1		GPIOE
#define LCD_1202_COM_LIG2		GPIOE

#endif // CONFIG_FLATFORM == FLATFORM_STM32_F407VG

// address for new line (height 8 pixel)

#define LCD_1202_TOTAL_LINE_TEXT				8		// line 9 has view half text
#define LCD_1202_ADDRESS_LINE_TEXT_0			0
#define LCD_1202_ADDRESS_LINE_TEXT_1			96
#define LCD_1202_ADDRESS_LINE_TEXT_2			192
#define LCD_1202_ADDRESS_LINE_TEXT_3			288
#define LCD_1202_ADDRESS_LINE_TEXT_4			384
#define LCD_1202_ADDRESS_LINE_TEXT_5			480
#define LCD_1202_ADDRESS_LINE_TEXT_6			576
#define LCD_1202_ADDRESS_LINE_TEXT_7			672
#define LCD_1202_ADDRESS_LINE_TEXT_8			768

/*==================================================================
 * define cache ram for LCD
 *==================================================================*/
// The buffer use to transmission to LCD
extern __uint8 gLcd1202Ram[LCD_1202_BUFFER_SIZE];
//extern osSemaphoreId lcd1202SemaphoreID;
// the current offset written to LCD buffer
//extern __uint32 gLcd1202CurrentOffset;


typedef enum
{
	EMS_DATA	= 1,
	EMS_COMMAND	= 0

}__E_Lcd_Mode_Send;

/*==================================================================
 * function
 *==================================================================*/
void lcd_1202_delay(volatile __uint32 timeCount);
void lcd_1202_init_pin(__uint32 pinNumber);
void lcd_1202_hight_power_pin(__uint32 pinNumber);
void lcd_1202_low_power_pin(__uint32 pinNumber);
void lcd_1202_transmission(__E_Lcd_Mode_Send modeSend, __uint8 data);
void lcd_1202_send_data(__uint8 data);
void lcd_1202_send_command(__uint8 command);
void lcd_1202_initialize(void);
void lcd_1202_clear_screen(void);
void lcd_1202_view_image_bitmap(__uint8 *p_img);
void lcd_1202_flush(void);
void lcd_1202_setting_led_background(__int32 status);


/*==================================================================
 * Pint text, value in LCD
 *==================================================================*/
void lcd_1202_set_position(__uint32 position);
void lcd_1202_print_line(const char* p_format, ...);
void lcd_1202_endLine(void);




#endif

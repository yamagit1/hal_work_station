
#include "monitor.h"
#include "lcd1202.h"
//#include "temperature.h"
#include "lcd_font.h"
#include "perform_manage.h"
#include "driver_led.h"

void MNT_initialize(void)
{
	lcd_1202_initialize();
}

void MNT_printBorder(void)
{
	// line text 0
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_0);
	lcd_1202_print_line("-PID-|--SYS INFOR--");

	//line text 1
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_1);
	lcd_1202_print_line("%5d|TIME|%2d:%2d:%2d", (__uint32)gListPID[0], \
			gTimeCount.hour,gTimeCount.miuter, gTimeCount.section);

	// line text 2
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_2);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedR|ON |    ", (__uint32)gListPID[1]);
	}
	else
	{
		lcd_1202_print_line("%5d|LedR|   |OFF ", (__uint32)gListPID[1]);
	}

	// line text 3
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_3);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedG|ON |    ", (__uint32)gListPID[2]);
	}
	else
	{
		lcd_1202_print_line("%5d|LedG|   |OFF ", (__uint32)gListPID[2]);
	}

	// line text 4
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_4);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedB|ON |    ", (__uint32)gListPID[3]);
	}
	else
	{
		lcd_1202_print_line("%5d|LedB|   |OFF ", (__uint32)gListPID[3]);
	}

	// line text 5
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_5);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedY|ON |    ", (__uint32)gListPID[4]);
	}
	else
	{
		lcd_1202_print_line("%5d|LedY|   |OFF ", (__uint32)gListPID[4]);
	}

}


void MNT_Render()
{
	MNT_printBorder();
	lcd_1202_flush();
}


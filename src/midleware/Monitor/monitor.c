/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: monitor.h
 * in this file :
 *============================================================================*/

#include "monitor.h"
#include "lcd1202.h"
#include "perform_manage.h"
#include "driver_led.h"
#include "cmsis_os.h"
#include "console_serial_trace.h"
#include "httpd.h"
#include "fptserver.h"


/*==============================================================================
 *	global variable
 *============================================================================*/
/*==============================================================================
 *	function define
 *============================================================================*/


void MNT_printBorder(void)
{
	// line text 0
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_0);
	lcd_1202_print_line("-PID-|--SYS INFOR--");

	//line text 1
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_1);
	lcd_1202_print_line("%5d|TIME|%2d:%2d:%2d", (__uint32)gListPID[0] % 100000 , \
			gTimeCount.hour,gTimeCount.miuter, gTimeCount.section);

	// line text 2
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_2);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedR|ON |    ", (__uint32)gListPID[1] % 100000);
	}
	else
	{
		lcd_1202_print_line("%5d|LedR|   |OFF ", (__uint32)gListPID[1] % 100000);
	}

	// line text 3
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_3);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedG|ON |    ", (__uint32)gListPID[2] % 100000 );
	}
	else
	{
		lcd_1202_print_line("%5d|LedG|   |OFF ", (__uint32)gListPID[2] % 100000 );
	}

	// line text 4
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_4);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedB|ON |    ", (__uint32)gListPID[3] % 100000);
	}
	else
	{
		lcd_1202_print_line("%5d|LedB|   |OFF ", (__uint32)gListPID[3] % 100000);
	}

	// line text 5
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_5);

	if (gLedStatus[LED_RED] == eTRUE)
	{
		lcd_1202_print_line("%5d|LedY|ON |    ", (__uint32)gListPID[4] % 100000);
	}
	else
	{
		lcd_1202_print_line("%5d|LedY|   |OFF ", (__uint32)gListPID[4] % 100000);
	}
	//
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_6);
	lcd_1202_print_line("HTTP :%s", gHttpdProp.fname);

	//
	lcd_1202_set_position(LCD_1202_ADDRESS_LINE_TEXT_7);

	lcd_1202_print_line("FPT  :%s", gFptProp.command);
}


void MNT_Render()
{
	lcd_1202_clear_screen();
	MNT_printBorder();
	lcd_1202_flush();
}


void MNT_task_polling(void * argument)
{

	for(;;)
	{
		MNT_Render();
		osDelay(1);
	}
}


void MNT_initialize(void)
{
	lcd_1202_initialize();

	// create task httpd
	console_serial_print_log("Create task monitor poolling");
	osThreadDef(monitor_pool, MNT_task_polling, osPriorityNormal, 0, 300);

	gListPID[INDEX_MONITOR] = osThreadCreate(osThread(monitor_pool), NULL);

}

/*==============================================================================
 *  Author  : YAMA team
 *  email   : yamateamhaui@gmail.com
 *  address : Ha Noi University
 *  ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)
 *============================================================================*/

#ifndef __CONSOLE_SERIAL_TRAFFIC_H__
#define __CONSOLE_SERIAL_TRAFFIC_H__

#include "header.h"

extern UART_HandleTypeDef g_HandleUART;

void console_serial_init();
void console_serial_tx(char *p_str);
void console_serial_rx();

void console_serial_print_char(char *p_ch);
void console_serial_print_string(char *p_str);

void console_serial_print_line(const char* p_format, ...);
void console_serial_print_log(const char* p_format, ...);
void console_serial_print_warring(const char* p_format, ...);
void console_serial_print_error(const char* p_format, ...);
void console_serial_print_infor();

#endif /* __CONSOLE_SERIAL_TRAFFIC_H__ */

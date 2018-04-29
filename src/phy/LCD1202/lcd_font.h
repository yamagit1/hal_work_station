/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: lcd_font.h
 * in this file :
 * 		- declare struct Font
 * 		- declare function initialize font ,..
 *============================================================================*/

#ifndef __FONT_H__
#define __FONT_H__

#include <header.h>


typedef enum{
	EF_LCD1202 = 1
}__E_Lcd_Font;

typedef struct {
	 __int32 charWidth;
	 __int32 charHight;
	 __int32 fontIsFull;
	 __uint8 *Resource;
}__S_Font_Header ;

extern __S_Font_Header gCurrentFont;


void LF_setFontIsUse(__uint8 *p_fontSwith);

void LF_printCharToLcdRam(__uint8 ch, __uint32 *p_position);

#endif

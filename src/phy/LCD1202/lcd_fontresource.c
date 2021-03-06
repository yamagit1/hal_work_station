/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: lcd_fontresource.c
 * in this file :
 *============================================================================*/

#include "lcd_fontresource.h"
__uint8 gFontFullYama[] = {
		//===============================================
		//  FONT INFOR
		//===============================================
		0x05,		// width char
		0x08,		// hight char
		0x01,		// full
		//===============================================
		// FONT DATA
		//===============================================
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x5E, 0x00, 0x00, 0x00,
		0x00, 0x06, 0x00, 0x06, 0x00,
		0x28, 0x7C, 0x28, 0x7C, 0x00,
		0x48, 0x54, 0x7E, 0x34, 0x00,
		0x66, 0x16, 0x68, 0x66, 0x00,
		0x3C, 0x4C, 0x50, 0x20, 0x00,
		0x00, 0x08, 0x06, 0x00, 0x00,
		0x1C, 0x22, 0x41, 0x00, 0x00,
		0x00, 0x41, 0x22, 0x1C, 0x00,
		0x28, 0x10, 0x28, 0x00, 0x00,
		0x00, 0x10, 0x38, 0x10, 0x00,
		0x00, 0x06, 0x08, 0x00, 0x00,
		0x00, 0x10, 0x10, 0x10, 0x00,
		0x00, 0x00, 0x40, 0x00, 0x00,
		0x40, 0x30, 0x0C, 0x03, 0x00,
		0x3C, 0x4A, 0x46, 0x3C, 0x00,
		0x00, 0x04, 0x7E, 0x00, 0x00,
		0x64, 0x52, 0x52, 0x4C, 0x00,
		0x24, 0x42, 0x4A, 0x34, 0x00,
		0x1E, 0x20, 0x20, 0x7C, 0x00,
		0x4E, 0x4A, 0x4A, 0x32, 0x00,
		0x3C, 0x4A, 0x4A, 0x32, 0x00,
		0x02, 0x62, 0x1A, 0x04, 0x00,
		0x34, 0x4A, 0x4A, 0x34, 0x00,
		0x4C, 0x52, 0x52, 0x3C, 0x00,
		0x00, 0x24, 0x00, 0x00, 0x00,
		0x00, 0x40, 0x34, 0x00, 0x00,
		0x08, 0x14, 0x22, 0x41, 0x00,
		0x14, 0x14, 0x14, 0x14, 0x00,
		0x41, 0x22, 0x14, 0x08, 0x00,
		0x02, 0x59, 0x09, 0x06, 0x00,
		0x3C, 0x5A, 0x5A, 0x2C, 0x00,
		0x7E, 0x09, 0x09, 0x7E, 0x00,
		0x7F, 0x49, 0x49, 0x36, 0x00,
		0x3E, 0x41, 0x41, 0x22, 0x00,
		0x7F, 0x41, 0x41, 0x3E, 0x00,
		0x3E, 0x49, 0x49, 0x49, 0x00,
		0x7E, 0x09, 0x09, 0x09, 0x00,
		0x3E, 0x49, 0x49, 0x39, 0x00,
		0x7F, 0x0C, 0x0C, 0x7F, 0x00,
		0x41, 0x7F, 0x41, 0x00, 0x00,
		0x21, 0x41, 0x3F, 0x01, 0x00,
		0x7F, 0x08, 0x14, 0x63, 0x00,
		0x3F, 0x40, 0x40, 0x40, 0x00,
		0x7F, 0x02, 0x02, 0x7F, 0x00,
		0x7F, 0x06, 0x18, 0x7F, 0x00,
		0x3E, 0x41, 0x41, 0x3E, 0x00,
		0x7F, 0x09, 0x09, 0x06, 0x00,
		0x3E, 0x51, 0x21, 0x5E, 0x00,
		0x7F, 0x19, 0x29, 0x46, 0x00,
		0x46, 0x49, 0x49, 0x31, 0x00,
		0x01, 0x01, 0x7F, 0x01, 0x00,
		0x3F, 0x40, 0x20, 0x7F, 0x00,
		0x3F, 0x40, 0x40, 0x3F, 0x00,
		0x7F, 0x20, 0x20, 0x7F, 0x00,
		0x73, 0x0C, 0x0C, 0x73, 0x00,
		0x07, 0x78, 0x08, 0x07, 0x00,
		0x61, 0x59, 0x45, 0x43, 0x00,
		0x00, 0x7F, 0x41, 0x41, 0x00,
		0x03, 0x0C, 0x30, 0x40, 0x00,
		0x41, 0x41, 0x7F, 0x00, 0x00,
		0x04, 0x02, 0x02, 0x04, 0x00,
		0x40, 0x40, 0x40, 0x40, 0x00,
		0x00, 0x00, 0x07, 0x00, 0x00,
		0x20, 0x54, 0x34, 0x78, 0x00,
		0x7E, 0x48, 0x48, 0x30, 0x00,
		0x38, 0x44, 0x44, 0x28, 0x00,
		0x30, 0x48, 0x48, 0x7E, 0x00,
		0x38, 0x54, 0x54, 0x08, 0x00,
		0x04, 0x7E, 0x05, 0x00, 0x00,
		0x4C, 0x52, 0x52, 0x3C, 0x00,
		0x7E, 0x10, 0x10, 0x60, 0x00,
		0x00, 0x74, 0x00, 0x00, 0x00,
		0x20, 0x40, 0x3E, 0x00, 0x00,
		0x7E, 0x18, 0x66, 0x00, 0x00,
		0x3E, 0x40, 0x40, 0x00, 0x00,
		0x7C, 0x08, 0x08, 0x7C, 0x00,
		0x7C, 0x08, 0x04, 0x78, 0x00,
		0x38, 0x44, 0x44, 0x38, 0x00,
		0x7C, 0x12, 0x12, 0x0C, 0x00,
		0x0C, 0x12, 0x12, 0x7C, 0x00,
		0x7C, 0x08, 0x04, 0x04, 0x00,
		0x48, 0x54, 0x54, 0x24, 0x00,
		0x04, 0x3E, 0x44, 0x24, 0x00,
		0x3C, 0x40, 0x20, 0x7C, 0x00,
		0x3C, 0x40, 0x40, 0x3C, 0x00,
		0x7C, 0x20, 0x20, 0x7C, 0x00,
		0x6C, 0x10, 0x10, 0x6C, 0x00,
		0x4E, 0x50, 0x50, 0x3E, 0x00,
		0x64, 0x54, 0x54, 0x4C, 0x00,
		0x08, 0x36, 0x41, 0x41, 0x00,
		0x00, 0x77, 0x00, 0x00, 0x00,
		0x41, 0x41, 0x36, 0x08, 0x00,
		0x10, 0x28, 0x0A, 0x04, 0x00
};

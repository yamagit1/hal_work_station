/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: httpd.h
 * in this file :
 *============================================================================*/

#ifndef __HTTPD_H__
#define __HTTPD_H__

#include "header.h"
#include "net.h"

/*==============================================================================
 *	struct define
 *============================================================================*/

typedef struct tcp_http_prop {
	uint8_t macaddr_dst[6];					//MAC address Destination
	uint8_t ipaddr_dst[6];					//IP- address Destination
	volatile uint16_t port_dst;				// port of the recipient
	volatile uint32_t seq_num;  			// serial number of the byte
	volatile uint32_t seq_num_tmp; 			// the serial number of the byte is temporary
	volatile uint32_t ack_num; 				// confirmation number
	volatile uint32_t data_stat;			// data transfer status
	volatile uint32_t data_size; 			// data size to send
	volatile uint16_t last_data_part_size;	// the size of the last part of the data to send
	volatile uint16_t cnt_data_part;		// total number of pieces of data to send
	volatile uint16_t cnt_rem_data_part;	// the number of remaining parts of the data to send
	volatile uint16_t cnt_size_wnd; 		// the number of bytes transferred from the window
	volatile uint8_t http_doc;				// the version of the document to send
	char fname[20];							// the name of the file (document)
} __S_Http_Prop;

/*==============================================================================
 *	global variable
 *============================================================================*/

extern __uint8 gHttpFrame[ENC28J60_MAXFRAME];
extern osSemaphoreId httpBuffSemaphoreID;
extern __S_Http_Prop gHttpdProp;

/*==============================================================================
 *	function
 *============================================================================*/

void httpd_init();


#endif /* __HTTPD_H__ */

/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: httpd.h
 * in this file :
 *============================================================================*/

#ifndef __HTTPD_H__
#define __HTTPD_H__

#include "header.h"
#include "net.h"

#define HTTPD_TCP_PORT 	8080

#define HTTPD_FILE_HTML  0
#define HTTPD_FILE_JPEG  1

typedef struct tcp_prop {
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


extern __uint8 gHttpFrame[ENC28J60_MAXFRAME];
extern osSemaphoreId httpBuffSemaphoreID;
extern __S_Http_Prop gHttpdProp;


__uint8 httpd_send_http_one(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
void httpd_header_prepare(__S_Tcp_Pkt *tcp_pkt, __uint16 port, __uint8 fl, __uint16 len, __uint16 len_cs);
void httpd_ip_header_prepare(__S_Ip_Pkt *ip_pkt, __uint8 *ip_addr, __uint8 prt, __uint16 len);

__uint8 httpd_send_http_first(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 httpd_send_synack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 httpd_send_finack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 httpd_send_http_middle(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 httpd_send_http_last(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 httpd_send_http_data_end(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);

void httpd_read();
void httpd_pool();
void httpd_init();


#endif /* __HTTPD_H__ */

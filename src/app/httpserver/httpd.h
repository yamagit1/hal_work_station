/*
 * httpd.h
 *
 *  Created on: Apr 29, 2018
 *      Author: yama
 */

#ifndef SRC_APP_WEBSERVER_HTTPD_H_
#define SRC_APP_WEBSERVER_HTTPD_H_

#include "header.h"
#include "net.h"

#define HTTPD_TCP_PORT 	8080

#define HTTPD_FILE_HTML  0
#define HTTPD_FILE_JPEG  1

extern __uint8 gHttpFrame[ENC28J60_MAXFRAME];
extern osSemaphoreId httpBuffSemaphoreID;


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


#endif /* SRC_APP_WEBSERVER_HTTPD_H_ */

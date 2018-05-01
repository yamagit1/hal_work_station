/*
 * fptserver.h
 *
 *  Created on: Apr 30, 2018
 *      Author: yama
 */

#ifndef __FPTSERVER_H__
#define __FPTSERVER_H__

#include "header.h"
#include "enc28j60.h"
#include "net.h"

#define FPTD_TCP_PORT 	8888

#define FPT_CMD_KEY_VALUE		"FPT_COMMAND"
#define FPT_CMD_KEY_SIZE		11

#define FPT_USE_LEN				30
#define FPT_PASS_LEN			30
#define FPT_CMD_LEN				128


#define FPT_CMD_LOGIN			"LOGIN"
#define FPT_CMD_LOGOUT			"LOGOUT"
#define FPT_CMD_REBOOT			"REBOOT"
#define FPT_CMD_IPCFG			"IPCFG"
#define FPT_CMD_UPLOAD			"UPLOAD"
#define FPT_CMD_DOWLOAD			"DOWLOAD"
#define FPT_CMD_MKDIR			"MKDIR"
#define FPT_CMD_RMDIR			"RMDIR"
#define FPT_CMD_TOUCH			"TOUCH"
#define FPT_CMD_RMFIL			"RMFIL"


#define NVIC_AIRCR_VECTKEY    (0x5FA << 16)   /*!< AIRCR Key for write access   */
#define NVIC_SYSRESETREQ            2         /*!< System Reset Request         */


extern __uint8 gFptFrame[ENC28J60_MAXFRAME];


typedef struct tcp_fpt_prop {
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
	volatile uint8_t  fpt_doc;				// the version of the document to send
	char fname[20];							// the name of the file (document)

	char user_login[FPT_USE_LEN];			// user login
	char pass_login[FPT_PASS_LEN];			// pass login
	char command[FPT_CMD_LEN];				// command recv
	volatile __uint32 cmdNumArg;			//
	char cmdArgs[5][128];					// args for command
	__E_Bollen	currPermission;					// current permission access


} __S_Fpt_Prop;

void fptd_pool();
__uint8 ftpd_send_synack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
void ftpd_header_prepare(__S_Tcp_Pkt *tcp_pkt, __uint16 port, __uint8 fl, __uint16 len, __uint16 len_cs);
void ftpd_ip_header_prepare(__S_Ip_Pkt *ip_pkt, __uint8 *ip_addr, __uint8 prt, __uint16 len);
__uint8 fptd_send_finack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 fptd_send_http_data_end(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 fpt_send_data(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port, char *data);
void fptd_split_cmd(__uint8 *command);
void fptd_execute_cmd(__uint8 *cmd);


#endif /* __FPTSERVER_H__ */

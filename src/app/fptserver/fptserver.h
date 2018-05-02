/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: fptserver.h
 * in this file :
 *============================================================================*/
#ifndef __FPTSERVER_H__
#define __FPTSERVER_H__

#include "header.h"
#include "enc28j60.h"
#include "net.h"
#define FPTD_TCP_PORT 	8888

#define FPT_CMD_KEY_VALUE		"FPT_COMMAND"
#define FPT_CMD_KEY_SIZE		11

#define FPT_DATA_KEY_VALUE		"FPT_DATA"
#define FPT_DATA_KEY_SIZE		8

#define FPT_USE_LEN				30
#define FPT_PASS_LEN			30
#define FPT_CMD_LEN				128

// list command
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
#define FPT_CMD_STATUS			"STATUS"


#define FPT_NOTI_PERMIS_DENIED	"PERMISSION_DENIED"
#define FPT_NOTI_PERMIS_OK		"PERMISSION_OK"
#define FPT_NOTI_LOGIN_OK		"LOGIN_OK"
#define FPT_NOTI_LOGIN_FAIL		"LOGIN_FAIL"
#define FPT_NOTI_LOGOUT_OK		"LOGOUT_OK"
#define FPT_NOTI_LOGOUT_FAIL	"LOGOUT_FAIL"

#define FPT_NOTI_REBOOT_OK		"REBOOT_ACCEPT"
#define FPT_NOTI_REBOOT_REFUSE	"REBOOT_REFUSE"

#define FPT_NOTI_RMFIL_OK		"RMFIL_OK"
#define FPT_NOTI_RMFIL_FAIL		"RMFIL_FAIL"

#define FPT_NOTI_TOUCH_OK		"TOUCH_OK"
#define FPT_NOTI_TOUCH_FAIL		"TOUCH_FAIL"

#define FPT_NOTI_RMDIR_OK		"RMDIR_OK"
#define FPT_NOTI_RMDIR_FAIL		"RMDIR_FAIL"

#define FPT_NOTI_MKDIR_OK		"MKDIR_OK"
#define FPT_NOTI_MKDIR_FAIL		"MKDIR_FAIL"

#define FPT_NOTI_DOWLOAD_ACCEPT	"DOWLOAD_ACCEPT"
#define FPT_NOTI_DOWLOAD_REFUSE	"DOWLOAD_REFUSE"

#define FPT_NOTI_UPLOAD_ACCEPT	"UPLOAD_ACCEPT"
#define FPT_NOTI_UPLOAD_REFUSE	"UPLOAD_REFUSE"
#define FPT_NOTI_OPLOAD_RECV_OK		"RECV_OK"
#define FPT_NOTI_OPLOAD_RECV_FAIL	"RECV_FAIL"


#define NVIC_AIRCR_VECTKEY    (0x5FA << 16)   /*!< AIRCR Key for write access   */
#define NVIC_SYSRESETREQ            2         /*!< System Reset Request         */


#define FPT_UPLOAD_CONNECT			0
#define FPT_UPLOAD_DISCONNECT		1

typedef struct tcp_fpt_prop {
	//============for tcp packet================================================
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
	char fname[BUFF_SIZE_VERY_LOW];							// the name of the file (document)
//============= for command ====================================================
	char user_login[FPT_USE_LEN];			// user login
	char pass_login[FPT_PASS_LEN];			// pass login
	char command[FPT_CMD_LEN];				// command recv
	volatile __uint32 cmdNumArg;			//
	char cmdArgs[5][128];					// args for command
	__E_Bollen	currPermission;				// current permission access
//==============for upload file ================================================
	volatile __uint32 total_upload_size;	// toltal number of data upload
	volatile __uint32 remain_upload_size;	// the number of remaining parts of the data to upload
	volatile __uint8 upload_status;			// upload status
	char upload_fname[BUFF_SIZE_VERY_LOW];	// file name to save
	FIL up_file_obj;						// file name object

} __S_Fpt_Prop;

extern __uint8 gFptFrame[ENC28J60_MAXFRAME];
extern osSemaphoreId fptBuffSemaphoreID;
extern __S_Fpt_Prop gFptProp;

void fptd_pool();
__uint8 ftpd_send_synack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
void ftpd_header_prepare(__S_Tcp_Pkt *tcp_pkt, __uint16 port, __uint8 fl, __uint16 len, __uint16 len_cs);
void ftpd_ip_header_prepare(__S_Ip_Pkt *ip_pkt, __uint8 *ip_addr, __uint8 prt, __uint16 len);
__uint8 fptd_send_finack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 fptd_send_http_data_end(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port);
__uint8 fpt_send_data(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port, char *data);
void fptd_split_cmd(__uint8 *command);
void fptd_execute_cmd(__uint8 *cmd, __S_Enc28j60_Frame_Pkt *FptFrame, __S_Ip_Pkt *ip_pkt);
void fptd_init();
void fptd_send_file(__S_Enc28j60_Frame_Pkt *frame,__uint8 *ip_addr, __uint16 port);
__uint8 fptd_accept_dowload();

void fptd_accept_upload();
void fptd_recv_upload_file(__uint8 *data);

#endif /* __FPTSERVER_H__ */

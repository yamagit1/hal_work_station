/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
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

/*==============================================================================
 *	struct define
 *============================================================================*/

typedef struct tcp_fpt_prop
{
	__uint8 macaddr_dst[6];					//MAC address Destination
	__uint8 ipaddr_dst[6];					//IP- address Destination
	volatile __uint16 port_dst;				// port of the recipient
	volatile __uint32 seq_num;  			// serial number of the byte
	volatile __uint32 seq_num_tmp; 			// the serial number of the byte is temporary
	volatile __uint32 ack_num; 				// confirmation number
	volatile __uint32 data_stat;			// data transfer status
	volatile __uint32 data_size; 			// data size to send
	volatile __uint16 last_data_part_size;	// the size of the last part of the data to send
	volatile __uint16 cnt_data_part;		// total number of pieces of data to send
	volatile __uint16 cnt_rem_data_part;	// the number of remaining parts of the data to send
	volatile __uint16 cnt_size_wnd; 		// the number of bytes transferred from the window
	volatile __uint8  fpt_doc;				// the version of the document to send
	char fname[BUFF_SIZE_VERY_LOW];			// the name of the file (document)
	char user_login[FPT_USE_LEN];			// user login
	char pass_login[FPT_PASS_LEN];			// pass login
	char command[FPT_CMD_LEN];				// command recv
	volatile __uint32 cmdNumArg;			//
	char cmdArgs[5][128];					// args for command
	__E_Bollen	currPermission;				// current permission access
	volatile __uint32 total_upload_size;	// toltal number of data upload
	volatile __uint32 remain_upload_size;	// the number of remaining parts of the data to upload
	volatile __uint8 upload_status;			// upload status
	char upload_fname[BUFF_SIZE_VERY_LOW];	// file name to save
	FIL up_file_obj;						// file name object

} __S_Fpt_Prop;

/*==============================================================================
 *	global variable
 *============================================================================*/

extern __uint8 gFptFrame[ENC28J60_MAXFRAME];
extern osSemaphoreId fptBuffSemaphoreID;
extern __S_Fpt_Prop gFptProp;

/*==============================================================================
 *	function
 *============================================================================*/

void fptd_init();

#endif /* __FPTSERVER_H__ */

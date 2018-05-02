/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: fptserver.c
 * in this file :
 *============================================================================*/

#include "fptserver.h"
#include "net.h"
#include "console_serial_trace.h"
#include "sd.h"
#include "perform_manage.h"

__uint8 gFptFrame[ENC28J60_MAXFRAME];
__S_Fpt_Prop gFptProp;

volatile __uint16 fptd_size_wnd = 8192;
volatile __uint16 fptd_tcp_mss = 458;
volatile __uint8  fptd_tcp_stat = TCP_DISCONNECTED;

extern __uint8 ipaddr[4];

FATFS gFptdFsObj;//Flie system object
FIL gFptdFileObj;
FRESULT gFptdResult;

uint32_t gFptdBytesRead;
uint32_t gFptdBytesWrite;

extern char gDrivePath[4]; /* logical drive path */

osSemaphoreId fptBuffSemaphoreID;


void fptd_pool()
{
	__ENTER__

	__S_Enc28j60_Frame_Pkt *FptFrame = (void*)(gFptFrame);
	__S_Ip_Pkt *ip_pkt = (void*)(FptFrame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	__uint32 lenRcvData = 0;
	char *ss1;
	int ch1=' ';
	int ch2='.';

	__uint8 command[BUFF_SIZE_LOW];

	// wait for fptd buff ready to access
	if (osSemaphoreWait(fptBuffSemaphoreID, TIME_WAIT_SHORT) == osOK )
	{
		// get port
		gFptProp.port_dst = be16toword(tcp_pkt->port_src);

		// get MAC addr
		memcpy(gFptProp.macaddr_dst, FptFrame->addr_src, 6);

		// get IPv4
		memcpy(gFptProp.ipaddr_dst,ip_pkt->ipaddr_src,4);

		// get length data
		lenRcvData = be16toword(ip_pkt->len) - 20 - (tcp_pkt->len_hdr>>2);

		// check resquest data
		if (lenRcvData > 0)
		{
			console_serial_print_log("\t\t> packet ACK data available : %d", lenRcvData);
			tcp_pkt->data[lenRcvData] = '\0';
			console_serial_print_infor("TCP packet content : %s", tcp_pkt->data);

			// packet tcp contain ACK
			if (tcp_pkt->fl & TCP_ACK)
			{
				// packet "GET /", for protocol HTTP
				if (strncmp((char*)tcp_pkt->data, FPT_CMD_KEY_VALUE, FPT_CMD_KEY_SIZE) == 0)
				{
					memset(command, '\0' ,BUFF_SIZE_LOW);
					memcpy((void*)command, (void*)(tcp_pkt->data + FPT_CMD_KEY_SIZE +1), lenRcvData - FPT_CMD_KEY_SIZE -1);

					//				ss1 = strchr(gFptProp.fname, ch1);
					console_serial_print_infor("\t> packet contain command : %s --- len : %d", command, strlen(command));

					// exe command
					fptd_execute_cmd(command, FptFrame, ip_pkt);

				}
				else if (strncmp((char*)tcp_pkt->data, FPT_DATA_KEY_VALUE, FPT_DATA_KEY_SIZE) == 0)
				{
					console_serial_print_infor("\t> packet contain command : %s", "DATA");
					fptd_recv_upload_file(tcp_pkt->data);
				}
			}

		}


		if (tcp_pkt->fl == TCP_SYN)
		{
			console_serial_print_log("\t> fptd_pool : Packet request SYN");
			ftpd_send_synack(FptFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);
		}
		else if (tcp_pkt->fl == (TCP_FIN|TCP_ACK))
		{
			console_serial_print_log("\t> fptd_pool : Packet request FIN|ACK");

			fptd_send_finack(FptFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);
		}
		else if (tcp_pkt->fl == (TCP_PSH|TCP_ACK))
		{
			console_serial_print_log("\t> fptd_pool :Packet request PSH|ACK");

			if(!lenRcvData)
			{
				fptd_send_finack(FptFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);
			}
		}
		else if (tcp_pkt->fl == TCP_ACK)
		{
			console_serial_print_log("\t> fptd_pool : Packet request ACK");

			// send data
			fptd_send_file(FptFrame, gFptProp.ipaddr_dst, gFptProp.port_dst);

		}

	}

	// Release httpd buff
	osSemaphoreRelease(fptBuffSemaphoreID);

	__LEAVE__
}


__uint8 ftpd_send_synack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len=0;

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	gFptProp.seq_num = rand();
	gFptProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + 1);
	tcp_pkt->data[0]=2;//Maximum Segment Size (2)
	tcp_pkt->data[1]=4;//Length
	tcp_pkt->data[2]=(__uint8) (fptd_tcp_mss >> 8);//MSS = 458
	tcp_pkt->data[3]=(__uint8) fptd_tcp_mss;

	len = sizeof(__S_Tcp_Pkt)+4;

	// add header tcp-fpt
	ftpd_header_prepare(tcp_pkt, port, TCP_SYN|TCP_ACK, len, len);

	len+=sizeof(__S_Ip_Pkt);

	// add header ip-tcp-fpt
	ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

	//set address MAC Ethernet
	memcpy(frame->addr_dest,frame->addr_src,6);

	eth_send(frame, ETH_IP,len);

	// update httpd status
	fptd_tcp_stat = TCP_CONNECTED;

	return res;
}

__uint8 fptd_send_finack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len=0;

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	gFptProp.seq_num = tcp_pkt->num_ask;
	gFptProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + 1);

	len = sizeof(__S_Tcp_Pkt);

	// add header tcp-http
	ftpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

	len += sizeof(__S_Ip_Pkt);

	// add header ip-tcp-http
	ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

	//set address MAC Ethernet
	memcpy(frame->addr_dest,frame->addr_src,6);

	// send RTS
	eth_send(frame,ETH_IP,len);

	if(fptd_tcp_stat == TCP_DISCONNECTED)
	{
		return 0;
	}

	tcp_pkt->fl = TCP_FIN|TCP_ACK;
	len = sizeof(__S_Tcp_Pkt);
	tcp_pkt->cs = 0;
	tcp_pkt->cs=checksum((__uint8*)tcp_pkt-8, len+8, 2);
	len+=sizeof(__S_Ip_Pkt);

	//send FIN
	eth_send(frame,ETH_IP,len);

	fptd_tcp_stat = TCP_DISCONNECTED;

	return res;
}


void ftpd_header_prepare(__S_Tcp_Pkt *tcp_pkt, __uint16 port, __uint8 fl, __uint16 len, __uint16 len_cs)
{
	tcp_pkt->port_dst 	= be16toword(port);
	tcp_pkt->port_src 	= be16toword(FPTD_TCP_PORT);
	tcp_pkt->bt_num_seg = gFptProp.seq_num;
	tcp_pkt->num_ask 	= gFptProp.ack_num;
	tcp_pkt->fl 		= fl;
	tcp_pkt->size_wnd 	= be16toword(fptd_size_wnd);
	tcp_pkt->urg_ptr 	= 0;
	tcp_pkt->len_hdr 	= len << 2;
	tcp_pkt->cs 		= 0;
	tcp_pkt->cs			= checksum((__uint8*)tcp_pkt-8, len_cs+8, 2);
}


void ftpd_ip_header_prepare(__S_Ip_Pkt *ip_pkt, __uint8 *ip_addr, __uint8 prt, __uint16 len)
{
	ip_pkt->len		= be16toword(len);
	ip_pkt->id 		= 0;
	ip_pkt->ts 		= 0;
	ip_pkt->verlen  = 0x45;
	ip_pkt->fl_frg_of=0;
	ip_pkt->ttl		= 128;
	ip_pkt->cs 		= 0;
	ip_pkt->prt 	= prt;

	memcpy(ip_pkt->ipaddr_dst, ip_addr, 4);
	memcpy(ip_pkt->ipaddr_src, ipaddr, 4);

	ip_pkt->cs = checksum((void*)ip_pkt, sizeof(__S_Ip_Pkt), 0);
}

__uint8 fpt_send_data(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port, char *data)
{
	__uint8 res=0;
	__uint16 len=0;
	__uint16 sz_data=0;


	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	fptd_tcp_stat = TCP_CONNECTED;

	sz_data = be16toword(ip_pkt->len)-20-(tcp_pkt->len_hdr>>2);

	gFptProp.seq_num = tcp_pkt->num_ask;
	gFptProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + sz_data);

	len = sizeof(__S_Tcp_Pkt);

	// prepare tcp header
	ftpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

	len+=sizeof(__S_Ip_Pkt);

	// prepare ip-tcp header
	ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

	memcpy(frame->addr_dest,frame->addr_src,6);

	// send ACK
	eth_send(frame,ETH_IP,len);


	strcpy((char*)tcp_pkt->data, data);

	tcp_pkt->fl = TCP_ACK|TCP_PSH;

	len = sizeof(__S_Tcp_Pkt);
	len += strlen((char*)tcp_pkt->data);

	tcp_pkt->cs = 0;
	tcp_pkt->cs	= checksum((__uint8*)tcp_pkt-8, len+8, 2);

	len+=sizeof(__S_Ip_Pkt);

	ip_pkt->len	= be16toword(len);
	ip_pkt->cs 	= 0;
	ip_pkt->cs 	= checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);

	// send PSH | ACK
	eth_send(frame,ETH_IP,len);

	return res;
}


void fptd_execute_cmd(__uint8 *cmd, __S_Enc28j60_Frame_Pkt *FptFrame, __S_Ip_Pkt *ip_pkt)
{
	fptd_split_cmd(cmd);

	console_serial_print_log("--------- cmd : %s", gFptProp.command);
	console_serial_print_log("--------- num arg : %d", gFptProp.cmdNumArg);
	console_serial_print_log("--------- cmd : %s", gFptProp.cmdArgs[0]);
	console_serial_print_log("--------- cmd : %s", gFptProp.cmdArgs[1]);
	console_serial_print_log("--------- cmd : %s", gFptProp.cmdArgs[2]);
	console_serial_print_log("--------- cmd : %s", gFptProp.cmdArgs[3]);
	console_serial_print_log("--------- cmd : %s", gFptProp.cmdArgs[4]);

	if (strcmp(gFptProp.command, FPT_CMD_LOGIN) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_LOGIN acc : %s | pass : %s", gFptProp.cmdArgs[0], gFptProp.cmdArgs[1]);

		// check account
		if (strcmp(gFptProp.cmdArgs[0], gFptProp.user_login) == 0 &&\
				strcmp(gFptProp.cmdArgs[1], gFptProp.pass_login) == 0)
		{
			gFptProp.currPermission = eTRUE;

			console_serial_print_log("\t\t> FPT_CMD_LOGIN -----OK");

			// send response
			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_LOGIN_OK);

			return;
		}
		else
		{
			gFptProp.currPermission = eFALSE;

			console_serial_print_log("\t\t> FPT_CMD_LOGIN -----FAIL");

			// send response
			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_LOGIN_FAIL);

			return;
		}

	}
	else if (strcmp(gFptProp.command, FPT_CMD_LOGOUT) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_LOGOUT");

		if (gFptProp.currPermission == eTRUE)
		{
			gFptProp.currPermission = eFALSE;

			console_serial_print_log("\t\t> FPT_CMD_LOGOUT -----OK");

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_LOGOUT_OK);

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_LOGOUT -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}

	}
	else if (strcmp(gFptProp.command, FPT_CMD_REBOOT) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_REBOOT");

		if (gFptProp.currPermission == eTRUE)
		{
			console_serial_print_log("\t\t> FPT_CMD_REBOOT -----%s", "OK");

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_REBOOT_OK);

			SCB->AIRCR  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR & (0x700)) | (1<<NVIC_SYSRESETREQ)); /* Keep priority group unchanged */
			__DSB();                                                                                 /* Ensure completion of memory access */
			while(1);
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_REBOOT -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}
	}
	else if (strcmp(gFptProp.command, FPT_CMD_IPCFG) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_IPCFG");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_UPLOAD) == 0  )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_UPLOAD");

		if (gFptProp.currPermission == eTRUE)
		{
			// upload data acept
			fptd_accept_upload();

			//			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_ACCEPT);

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_UPLOAD -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}

	}
	else if (strcmp(gFptProp.command, FPT_CMD_DOWLOAD) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_DOWLOAD");

		if (gFptProp.currPermission == eTRUE)
		{
			console_serial_print_log("\t\t> FPT_CMD_DOWLOAD -----%s", FPT_NOTI_DOWLOAD_ACCEPT);

			fptd_accept_dowload();

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_DOWLOAD -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}

	}
	else if (strcmp(gFptProp.command, FPT_CMD_MKDIR) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_MKDIR");

		if (gFptProp.currPermission == eTRUE)
		{
			console_serial_print_log("\t\t> FPT_CMD_MKDIR -----%s", FPT_NOTI_MKDIR_OK);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_MKDIR_OK);

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_MKDIR -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}

	}
	else if (strcmp(gFptProp.command, FPT_CMD_TOUCH) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_TOUCH");

		if (gFptProp.currPermission == eTRUE)
		{
			console_serial_print_log("\t\t> FPT_CMD_TOUCH -----%s", FPT_NOTI_TOUCH_OK);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_TOUCH_OK);

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_TOUCH -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}

	}
	else if (strcmp(gFptProp.command, FPT_CMD_RMFIL) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_RMFIL");

		if (gFptProp.currPermission == eTRUE)
		{
			console_serial_print_log("\t\t> FPT_CMD_RMFIL -----%s", FPT_NOTI_RMFIL_OK);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_RMFIL_OK);

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_STATUS -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}


	}
	else if (strcmp(gFptProp.command, FPT_CMD_STATUS) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_STATUS");

		if (gFptProp.currPermission == eTRUE)
		{
			console_serial_print_log("\t\t> FPT_CMD_STATUS -----%s", FPT_NOTI_PERMIS_OK);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_OK);

			return;
		}
		else
		{
			console_serial_print_log("\t\t> FPT_CMD_STATUS -----%s", FPT_NOTI_PERMIS_DENIED);

			fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_PERMIS_DENIED);

			return;
		}
	}

}


void fptd_split_cmd(__uint8 *command)
{
	int offset =0;
	int len;

	// get cmd key
	len = 0;
	// find next char slip
	while(*(command + offset + len) != ' ' ){ len++;}

	strncpy(gFptProp.command, command + offset, len);
	gFptProp.command[len] = '\0';

	// clear table or space
	while(*(command + offset + len) == ' '){ offset++;}

	offset += len;

	gFptProp.cmdNumArg = 0;

	while(gFptProp.cmdNumArg < 5 && offset < strlen(command))
	{
		len = 0;
		// find next char slip
		while(*(command + offset + len) != ' ' && (offset + len) < strlen(command)){ len++; }

		strncpy(gFptProp.cmdArgs[gFptProp.cmdNumArg], command + offset, len);
		gFptProp.cmdArgs[gFptProp.cmdNumArg][len] = '\0';

		// numargs plus
		gFptProp.cmdNumArg++;

		if((offset + len) == strlen(command))
		{
			break;
		}

		// clear table or space
		while(*(command + offset + len) == ' '){ offset++; }
		offset += len;
	}

	//

}


void fptd_task_polling(void * argument)
{
	osEvent sig_rev;

	for (;;)
	{
		// wait for signal rev
		sig_rev = osSignalWait (0xffffffff, TIME_WAIT_LONG);

		if (sig_rev.status == osEventSignal && sig_rev.value.signals == SIG_FPTD_REV)
		{
			console_serial_print_log("fptd_task_polling receiver signal");
			fptd_pool();
		}
	}
}


void fptd_init()
{
	strcpy(gFptProp.user_login, "admin");
	strcpy(gFptProp.pass_login, "admin");
	gFptProp.currPermission = eFALSE;

	// Creating semaphore for microSD, it use to access to read, wire
	osSemaphoreDef(fpt_buff);
	fptBuffSemaphoreID = osSemaphoreCreate(osSemaphore(fpt_buff) , 1 );

	// Release httpd buff
	osSemaphoreRelease(fptBuffSemaphoreID);

	// create task fptd
	console_serial_print_log("Create task fptd poolling");
	osThreadDef(fptd_pool, fptd_task_polling, osPriorityNormal, 0, 500);

	gListPID[INDEX_FPT_SERVER] = osThreadCreate(osThread(fptd_pool), NULL);
}


__uint8 fptd_send_file_one(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__ENTER__

	__uint8 res=0;
	__uint16 len=0;
	__uint16 sz_data=0;

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	// wait for sd card ready to access
	// Release microSD device
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{
		sz_data = be16toword(ip_pkt->len)-20-(tcp_pkt->len_hdr>>2);

		gFptProp.seq_num = tcp_pkt->num_ask;
		gFptProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + sz_data);

		len = sizeof(__S_Tcp_Pkt);

		// prepare tcp-fpt header
		ftpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

		len += sizeof(__S_Ip_Pkt);

		// prepare ip-tcp-fpt header
		ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

		//get add dest from packet Ethernet
		memcpy(frame->addr_dest,frame->addr_src,6);

		// send ACK
		eth_send(frame, ETH_IP,len);

		// mount sdcard
		gFptdResult = f_mount(&gFptdFsObj, (TCHAR const*)gDrivePath, 0);
		console_serial_print_log("\t> httpd_send_http_one : f_mount: %d", gFptdResult);

		// open file to res
		gFptdResult = f_open(&gFptdFileObj, gFptProp.fname, FA_READ);
		console_serial_print_log("\t> httpd_send_http_one : f_open: %d  f_size: %lu", gFptdResult, gFptdFileObj.obj.objsize);

		// point to offset need read
		gFptdResult = f_lseek(&gFptdFileObj,0);
		console_serial_print_log("\t> httpd_send_http_one : f_lseek: %d", gFptdResult);

		// set coppy data to send
		gFptdResult = f_read(&gFptdFileObj, (void*)(tcp_pkt->data), (__uint16)gFptdFileObj.obj.objsize, (UINT *)&gFptdBytesRead);

		// close file
		f_close(&gFptdFileObj);

		len = sizeof(__S_Tcp_Pkt);
		len += gFptProp.data_size;

		tcp_pkt->fl 	= TCP_PSH|TCP_ACK;
		tcp_pkt->cs 	= 0;
		tcp_pkt->cs		= checksum((__uint8*)tcp_pkt-8, len+8, 2);

		len+=sizeof(__S_Ip_Pkt);

		ip_pkt->len		= be16toword(len);
		ip_pkt->cs 		= 0;
		ip_pkt->cs 		= checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);

		//send packet PSH|ACK
		eth_send(frame,ETH_IP,len);

		gFptProp.seq_num_tmp = be32todword(be32todword(gFptProp.seq_num) + gFptProp.data_size);
		gFptProp.data_stat = DATA_END;

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	__LEAVE__

	return res;

}


__uint8 fptd_send_file_first(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len=0;
	__uint16 sz_data=0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	// wait for sd card ready to access
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{
		sz_data = be16toword(ip_pkt->len)-20-(tcp_pkt->len_hdr>>2);
		gFptProp.seq_num = tcp_pkt->num_ask;
		gFptProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + sz_data);

		len = sizeof(__S_Tcp_Pkt);

		// prepare tcp-fpt header
		ftpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

		len+=sizeof(__S_Ip_Pkt);

		// prepare ip-tcp-fpt header
		ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

		//get address Ethernet
		memcpy(frame->addr_dest,frame->addr_src,6);

		// send ACK
		eth_send(frame,ETH_IP,len);


		// mount sdcard
		gFptdResult = f_mount(&gFptdFsObj, (TCHAR const*)gDrivePath, 0);
		console_serial_print_log("\t> fptd_send_file_first : f_mount: %d", gFptdResult);

		// open file to res
		gFptdResult = f_open(&gFptdFileObj, gFptProp.fname, FA_READ);
		console_serial_print_log("\t> fptd_send_file_first : f_open: %d  f_size: %lu", gFptdResult, gFptdFileObj.obj.objsize);

		// point to offset need read
		gFptdResult = f_lseek(&gFptdFileObj,0);
		console_serial_print_log("\t> httpd_send_http_first : f_lseek: %d", gFptdResult);

		// copy data to frame
		gFptdResult = f_read(&gFptdFileObj, (void*)(tcp_pkt->data), fptd_tcp_mss, (UINT *)&gFptdBytesRead);

		tcp_pkt->fl = TCP_PSH|TCP_ACK; //=========================================================
		len = sizeof(__S_Tcp_Pkt);
		len += fptd_tcp_mss;
		tcp_pkt->cs = 0;
		tcp_pkt->cs=checksum((__uint8*)tcp_pkt-8, len+8, 2);

		len+=sizeof(__S_Ip_Pkt);
		ip_pkt->len=be16toword(len);
		ip_pkt->cs = 0;
		ip_pkt->cs = checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);

		// send ACK
		eth_send(frame,ETH_IP,len);

		// set status next packet
		gFptProp.cnt_rem_data_part--;

		gFptProp.cnt_size_wnd += fptd_tcp_mss;

		if(gFptProp.cnt_rem_data_part > 1)
		{
			gFptProp.data_stat = DATA_MIDDLE;
		}
		else
		{
			gFptProp.data_stat = DATA_LAST;
		}

		//	 Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	else
	{
		res = 1;
	}

	return res;
}


__uint8 fptd_send_file_middle(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len_tcp=0, len=0;
	__uint32 offset = 0;

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	// wait for sd card ready to access
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{

		gFptProp.seq_num = be32todword(be32todword(gFptProp.seq_num) + fptd_tcp_mss);
		len_tcp = sizeof(__S_Tcp_Pkt);

		// point to offset need read
		offset = (uint32_t)fptd_tcp_mss * (gFptProp.cnt_data_part - gFptProp.cnt_rem_data_part);

		gFptdResult = f_lseek(&gFptdFileObj, offset);
		console_serial_print_log("\t> fptd_send_file_middle : f_lseek status: %d | offset : %d", gFptdResult, offset);

		// copy data to frame
		gFptdResult = f_read(&gFptdFileObj,(void*)tcp_pkt->data,fptd_tcp_mss,(UINT *)&gFptdBytesRead);

		len = len_tcp + fptd_tcp_mss;

		// prepare tcp-fpt header
		if ((fptd_size_wnd - gFptProp.cnt_size_wnd) > fptd_tcp_mss)
		{
			ftpd_header_prepare(tcp_pkt, port, TCP_ACK, len_tcp, len);
		}
		else
		{
			ftpd_header_prepare(tcp_pkt, port, TCP_PSH|TCP_ACK, len_tcp, len);

			gFptProp.cnt_size_wnd = 0;
		}

		len += sizeof(__S_Ip_Pkt);
		// prepare ip-tcp-fpt header
		ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

		memcpy(frame->addr_dest,gFptProp.macaddr_dst,6);

		// send ACK
		eth_send(frame,ETH_IP,len);

		// set status next packet
		gFptProp.cnt_rem_data_part--;

		gFptProp.cnt_size_wnd += fptd_tcp_mss;

		if(gFptProp.cnt_rem_data_part > 1)
		{
			gFptProp.data_stat = DATA_MIDDLE;
		}
		else
		{
			gFptProp.data_stat = DATA_LAST;
		}

		//		Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	else
	{
		res = 1;
	}

	return res;
}


__uint8 fptd_send_file_last(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len_tcp=0, len=0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	// wait for sd card ready to access
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{

		gFptProp.seq_num = be32todword(be32todword(gFptProp.seq_num) + fptd_tcp_mss);

		len_tcp = sizeof(__S_Tcp_Pkt);



		// point to offset need read
		gFptdResult = f_lseek(&gFptdFileObj,(fptd_tcp_mss * (gFptProp.cnt_data_part - 1)));
		console_serial_print_log("\t> fptd_send_file_last : f_lseek: %d\r\n", gFptdResult);

		// copy data to frame
		gFptdResult = f_read(&gFptdFileObj,(void*)tcp_pkt->data,gFptProp.last_data_part_size,(UINT *)&gFptdBytesRead);


		len = len_tcp + gFptProp.last_data_part_size;

		// prepare tcp-fpt header
		ftpd_header_prepare(tcp_pkt, port, TCP_PSH|TCP_ACK, len_tcp, len);
		len+=sizeof(__S_Ip_Pkt);

		// prepare ip-tcp-fpt header
		ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);
		// Ethernet
		memcpy(frame->addr_dest,gFptProp.macaddr_dst,6);

		// send PSH|ACK
		eth_send(frame,ETH_IP,len);
		//
		gFptProp.seq_num_tmp = be32todword(be32todword(tcp_pkt->bt_num_seg)+gFptProp.last_data_part_size);
		gFptProp.data_stat=DATA_END;

		//		Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	else
	{
		res = 1;
	}

	return res;
}

__uint8 fptd_send_file_end(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res		= 0;
	__uint16 len 	= 0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);
	gFptProp.seq_num = gFptProp.seq_num_tmp;
	gFptProp.ack_num = tcp_pkt->bt_num_seg;
	len = sizeof(__S_Tcp_Pkt);

	ftpd_header_prepare(tcp_pkt, port, TCP_FIN|TCP_ACK, len, len);
	len+=sizeof(__S_Ip_Pkt);

	ftpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

	memcpy(frame->addr_dest,frame->addr_src,6);

	// send FIN|ACK
	eth_send(frame,ETH_IP,len);

	gFptProp.data_stat = DATA_COMPLETED;
	fptd_tcp_stat = TCP_DISCONNECTED;

	//	// Release microSD device
	//	osSemaphoreRelease(microSDSemaphoreID);

	return res;
}

void fptd_send_file(__S_Enc28j60_Frame_Pkt *frame,__uint8 *ip_addr, __uint16 port)
{
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);

	if (gFptProp.data_stat == DATA_END)
	{
		fptd_send_file_end(frame, ip_pkt->ipaddr_src, gFptProp.port_dst);
	}
	else if (gFptProp.data_stat == DATA_MIDDLE)
	{
		fptd_send_file_middle(frame, gFptProp.ipaddr_dst, gFptProp.port_dst);
	}
	else if (gFptProp.data_stat == DATA_LAST)
	{
		gFptProp.data_stat = DATA_COMPLETED;
		fptd_send_file_last(frame, gFptProp.ipaddr_dst, gFptProp.port_dst);

		console_serial_print_log("-------------LAST-----------");
	}
}

__uint8 fptd_accept_dowload()
{
	__S_Enc28j60_Frame_Pkt *FptFrame = (void*)(gFptFrame);
	__S_Ip_Pkt *ip_pkt = (void*)(FptFrame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	gFptProp.cnt_size_wnd = 0;

	if (strcmp(gFptProp.cmdArgs[0], "") != 0)
	{
		strcpy(gFptProp.fname, gFptProp.cmdArgs[0]);
	}


	console_serial_print_log("\t>file request :--name : %s --size : %d", (__uint8*)gFptProp.fname, strlen(gFptProp.fname));

	// wait for sd card ready to access
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{
		// open sd card and prepare file for send
		gFptdResult = f_mount(&gFptdFsObj, (TCHAR const*)gDrivePath, 0);
		console_serial_print_log("\t> fptd_accept_dowload : f_mount: %d", gFptdResult);

		// open file in sd card
		gFptdResult = f_open(&gFptdFileObj, gFptProp.fname, FA_READ);

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}

	// prepare header to send repost
	if (gFptdResult == FR_OK)
	{
		console_serial_print_log("\t> fptd_accept_dowload : f_open: %d  f_size: %lu", gFptdResult, gFptdFileObj.obj.objsize);

		gFptProp.data_size = gFptdFileObj.obj.objsize;
	}
	else
	{
		console_serial_print_log("\t> fptd_accept_dowload : %s ", FPT_NOTI_DOWLOAD_REFUSE);

		fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_DOWLOAD_REFUSE);
		return 1;
	}

	gFptProp.cnt_rem_data_part = gFptProp.data_size / fptd_tcp_mss + 1;

	gFptProp.last_data_part_size = gFptProp.data_size % fptd_tcp_mss;

	if (gFptProp.last_data_part_size == 0)
	{
		gFptProp.last_data_part_size = fptd_tcp_mss;
		gFptProp.cnt_rem_data_part--;
	}

	gFptProp.cnt_data_part = gFptProp.cnt_rem_data_part;

	console_serial_print_log("data size:%lu; cnt data part:%u; last_data_part_size:%u; port dst:%u",
			(unsigned long)gFptProp.data_size, gFptProp.cnt_rem_data_part, gFptProp.last_data_part_size,gFptProp.port_dst);

	if (gFptProp.cnt_rem_data_part == 1)
	{
		gFptProp.data_stat = DATA_ONE;
	}
	else if (gFptProp.cnt_rem_data_part > 1)
	{
		gFptProp.data_stat = DATA_FIRST;
	}

	if(gFptProp.data_stat == DATA_ONE)
	{
		fptd_send_file_one(FptFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);
	}
	else if(gFptProp.data_stat == DATA_FIRST)
	{
		console_serial_print_log("-------------START-----------");
		fptd_send_file_first(FptFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);
	}

	return 0;
}

void fptd_accept_upload()
{
	__S_Enc28j60_Frame_Pkt *FptFrame = (void*)(gFptFrame);
	__S_Ip_Pkt *ip_pkt = (void*)(FptFrame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	if (strcmp(gFptProp.cmdArgs[0], "") != 0)
	{
		strcpy(gFptProp.upload_fname, gFptProp.cmdArgs[0]);
		gFptProp.total_upload_size = atoi(gFptProp.cmdArgs[1]);
		gFptProp.remain_upload_size = gFptProp.total_upload_size;
	}
	else
	{
		fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_REFUSE);
		return;
	}

	console_serial_print_infor("\t>> Request upload file : %s | %d", gFptProp.upload_fname, gFptProp.total_upload_size);

	// wait for sd card ready to access
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{
		// open sd card and prepare file for send
		gFptdResult = f_mount(&gFptdFsObj, (TCHAR const*)gDrivePath, 0);
		console_serial_print_log("\t> fpt_accept_upload : f_mount: %d", gFptdResult);

		// open file in sd card
		gFptdResult = f_open(&gFptProp.up_file_obj, gFptProp.upload_fname, FA_READ);

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}

	if (gFptdResult == FR_OK)
	{
		console_serial_print_log("\t\t> fptd_accept_upload -----%s", FPT_NOTI_UPLOAD_REFUSE);

		fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_REFUSE);

		gFptProp.upload_status = FPT_UPLOAD_DISCONNECT;

		return;
	}
	else
	{
		console_serial_print_log("\t\t> fptd_accept_upload -----%s", FPT_NOTI_UPLOAD_ACCEPT);

		fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_ACCEPT);

		gFptProp.upload_status = FPT_UPLOAD_CONNECT;
	}
}


void fptd_recv_upload_file(__uint8 *data)
{

	__S_Enc28j60_Frame_Pkt *FptFrame = (void*)(gFptFrame);
	__S_Ip_Pkt *ip_pkt = (void*)(FptFrame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	__uint32 lenRcvData;
	__uint32 lendata;
	lenRcvData = be16toword(ip_pkt->len) - 20 - (tcp_pkt->len_hdr>>2);


	lendata = lenRcvData - (FPT_DATA_KEY_SIZE +1);



	if (gFptProp.upload_status == FPT_UPLOAD_CONNECT)
	{
		if (gFptProp.remain_upload_size == gFptProp.total_upload_size)
		{
			// wait for sd card ready to access
			if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
			{
				// open sd card and prepare file for send
				gFptdResult = f_mount(&gFptdFsObj, (TCHAR const*)gDrivePath, 0);
				console_serial_print_log("\t> fptd_recv_upload_file 1 : f_mount: %d", gFptdResult);

				// open file to wire
				gFptdResult = f_open(&gFptProp.up_file_obj, gFptProp.upload_fname, FA_WRITE|FA_CREATE_ALWAYS);
				console_serial_print_log("\t> fptd_recv_upload_file 1 : f_open: %d", gFptdResult);

				if (gFptdResult != FR_OK)
				{
					console_serial_print_log("\t\t> fptd_recv_upload_file 1 -----%s", FPT_NOTI_UPLOAD_REFUSE);

					fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_REFUSE);
					gFptProp.upload_status = FPT_UPLOAD_DISCONNECT;

					// Release microSD device
					osSemaphoreRelease(microSDSemaphoreID);

					return;
				}

				gFptdResult = f_write(&gFptProp.up_file_obj, data + (FPT_DATA_KEY_SIZE +1), lendata, (void*)&gFptdBytesWrite);

				f_close(&gFptProp.up_file_obj);

				// Release microSD device
				osSemaphoreRelease(microSDSemaphoreID);
			}

			if (gFptdResult == FR_OK)
			{
				console_serial_print_log("\t\t> fptd_recv_upload_file 1 : %d-----|%s|", lendata, data + (FPT_DATA_KEY_SIZE +1));
				console_serial_print_log("\t\t> fptd_recv_upload_file 1 : f_write %d", gFptdBytesWrite);

				gFptProp.remain_upload_size -= lendata;

				fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_OPLOAD_RECV_OK);

				return;
			}
			else
			{
				console_serial_print_log("\t\t> fptd_recv_upload_file 1 : %s", FPT_NOTI_OPLOAD_RECV_FAIL);

				fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_OPLOAD_RECV_FAIL);

				return;
			}


		}
		else if (gFptProp.remain_upload_size < gFptProp.total_upload_size && gFptProp.remain_upload_size > 0)
		{
			if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
			{
				// open sd card and prepare file for send
				gFptdResult = f_mount(&gFptdFsObj, (TCHAR const*)gDrivePath, 0);
				console_serial_print_log("\t> fptd_recv_upload_file 2 : f_mount: %d", gFptdResult);
				// open file to wire
				gFptdResult = f_open(&gFptProp.up_file_obj, gFptProp.upload_fname, FA_WRITE|FA_OPEN_APPEND);
				console_serial_print_log("\t> fptd_recv_upload_file 2 : f_open: %d", gFptdResult);

				if (gFptdResult != FR_OK)
				{
					console_serial_print_log("\t\t> fptd_recv_upload_file 2-----%s", FPT_NOTI_UPLOAD_REFUSE);

					fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_REFUSE);
					gFptProp.upload_status = FPT_UPLOAD_DISCONNECT;

					// Release microSD device
					osSemaphoreRelease(microSDSemaphoreID);

					return;
				}

				gFptdResult = f_write(&gFptProp.up_file_obj, data + (FPT_DATA_KEY_SIZE +1), lendata, (void*)&gFptdBytesWrite);

				f_close(&gFptProp.up_file_obj);

				// Release microSD device
				osSemaphoreRelease(microSDSemaphoreID);
			}

			if (gFptdResult == FR_OK)
			{
				console_serial_print_log("\t\t> fptd_recv_upload_file 2 : %d-----|%s|", lendata, data + (FPT_DATA_KEY_SIZE +1));
				console_serial_print_log("\t\t> fptd_recv_upload_file 2 : f_write %d", gFptdBytesWrite);


				if (gFptProp.remain_upload_size > lendata)
				{
					gFptProp.remain_upload_size -= lendata;
				}
				else
				{
					gFptProp.remain_upload_size = 0;
					gFptProp.upload_status = FPT_UPLOAD_DISCONNECT;

				}

				fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_OPLOAD_RECV_OK);


				return;
			}
			else
			{
				console_serial_print_log("\t\t> fptd_recv_upload_file 2 : %s", FPT_NOTI_OPLOAD_RECV_FAIL);

				fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_OPLOAD_RECV_FAIL);

				return;
			}

		}
	}
	else
	{
		console_serial_print_log("\t\t> fptd_recv_upload_file -----%s", FPT_NOTI_UPLOAD_REFUSE);

		fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, FPT_NOTI_UPLOAD_REFUSE);

		return;
	}

}

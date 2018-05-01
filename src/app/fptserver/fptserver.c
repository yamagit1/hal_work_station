/*
 * fptserver.c
 *
 *  Created on: Apr 30, 2018
 *      Author: yama
 */

#include "fptserver.h"
#include "net.h"
#include "console_serial_trace.h"

__uint8 gFptFrame[ENC28J60_MAXFRAME];
__S_Fpt_Prop gFptProp;

volatile __uint16 fptd_size_wnd = 8192;
volatile __uint16 fptd_tcp_mss = 458;
volatile __uint8  fptd_tcp_stat = TCP_DISCONNECTED;

extern __uint8 ipaddr[4];
extern __uint8 ipgate[4];
extern __uint8 ipmask[4];


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
		console_serial_print_infor("TCP packet content : %s", tcp_pkt->data);

		// packet tcp contain ACK
		if (tcp_pkt->fl & TCP_ACK)
		{
			// packet "GET /", for protocol HTTP
			if (strncmp((char*)tcp_pkt->data, FPT_CMD_KEY_VALUE, FPT_CMD_KEY_SIZE) == 0)
			{
				memcpy((void*)command, (void*)(tcp_pkt->data + FPT_CMD_KEY_SIZE +1), FPT_CMD_LEN);
				//				ss1 = strchr(gHttpdProp.fname, ch1);
				console_serial_print_infor("\t> packet contain command : %s --- len : %d", command, strlen(command));

				fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, "CMD_REV_OK");

				// exe command
				fptd_execute_cmd(command);

			}
			else
			{
				console_serial_print_infor("\t> packet contain command : %s", "DATA");
				fpt_send_data(FptFrame, ip_pkt->ipaddr_src, gFptProp. port_dst, "DATA_REV_OK");

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

		//		fptd_send_http_data_end(FptFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);


		//			if (gFptProp.data_stat == DATA_END)
		//			{
		//				httpd_send_http_data_end(HttpFrame, ip_pkt->ipaddr_src, gFptProp.port_dst);
		//			}
		//			else if (gFptProp.data_stat == DATA_MIDDLE)
		//			{
		//				httpd_send_http_middle(HttpFrame, gFptProp.ipaddr_dst, gFptProp.port_dst);
		//			}
		//			else if (gFptProp.data_stat == DATA_LAST)
		//			{
		//				console_serial_print_log("-------------LAST-----------");
		//				gFptProp.data_stat = DATA_COMPLETED;
		//				httpd_send_http_last(HttpFrame, gFptProp.ipaddr_dst, gFptProp.port_dst);
		//			}
	}


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


__uint8 fptd_send_http_data_end(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
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

	//�������� ��������� Ethernet
	memcpy(frame->addr_dest,frame->addr_src,6);
	eth_send(frame,ETH_IP,len);

	gFptProp.data_stat = DATA_COMPLETED;
	fptd_tcp_stat = TCP_DISCONNECTED;

	//	// Release microSD device
	//	osSemaphoreRelease(microSDSemaphoreID);

	return res;
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


void fptd_execute_cmd(__uint8 *cmd)
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
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_LOGOUT acc : %s | pass : %s", gFptProp.cmdArgs[0], gFptProp.cmdArgs[1]);
	}
	else if (strcmp(gFptProp.command, FPT_CMD_LOGOUT) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_LOGOUT");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_REBOOT) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_REBOOT");

		 SCB->AIRCR  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR & (0x700)) | (1<<NVIC_SYSRESETREQ)); /* Keep priority group unchanged */
		  __DSB();                                                                                 /* Ensure completion of memory access */
		  while(1);
	}
	else if (strcmp(gFptProp.command, FPT_CMD_IPCFG) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_IPCFG");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_UPLOAD) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_UPLOAD");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_DOWLOAD) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_DOWLOAD");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_MKDIR) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_MKDIR");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_TOUCH) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_TOUCH");

	}
	else if (strcmp(gFptProp.command, FPT_CMD_RMFIL) == 0 )
	{
		console_serial_print_log("\t> fptd_execute_cmd : FPT_CMD_RMFIL");

	}

	//	if ()

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

	while(gFptProp.cmdNumArg < 5)
	{
		len = 0;
		// find next char slip
		while(*(command + offset + len) != ' ' && (offset + len) < strlen(command)){ len++; }

		strncpy(gFptProp.cmdArgs[gFptProp.cmdNumArg], command + offset, len);
		gFptProp.cmdArgs[gFptProp.cmdNumArg][len] = '\0';


		if((offset + len) == strlen(command))
		{
			break;
		}

		gFptProp.cmdNumArg++;
		// clear table or space
		while(*(command + offset + len) == ' '){ offset++; }
		offset += len;
	}

	//

}

/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: httpd.
 * in this file :
 *============================================================================*/

#include <micro_sd.h>
#include "httpd.h"
#include "net.h"
#include "console_serial_trace.h"
#include "tcp.h"
#include "perform_manage.h"
#include "cmsis_os.h"

/*==============================================================================
 *	global variable
 *============================================================================*/

// the buffer httpd frame
__uint8 gHttpFrame[ENC28J60_MAXFRAME];
// Struct data for fptd
__S_Http_Prop gHttpdProp;
// file system object and some variable

FATFS gHttpdFsObj;//Flie system object
FIL gHttpdFileObj;
extern char gDrivePath[4]; /* logical drive path */
FRESULT gHttpdResult;
uint32_t gHttpdBytesRead;

// header for http protocol
const char httpd_http_header[] = {"HTTP/1.0 200 OK\r\nServer: nginx\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"};
const char httpd_jpg_header[] = {"HTTP/1.0 200 OK\r\nServer: nginx\r\nContent-Type: image/jpeg\r\nConnection: close\r\n\r\n"};
const char httpd_error_header[] = {"HTTP/1.0 404 File not found\r\nServer: nginx\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"};
const __uint8 httpd_e404_htm[] = {"<html><head><title>Error 404</title></head><body align=\"center\"><h1>ERROR 404</h1></body></html>"};

// the parameter for tcp packet
volatile __uint16 httpd_size_wnd = 8192;
volatile __uint16 httpd_tcp_mss = 458;
volatile __uint8  httpd_tcp_stat = TCP_DISCONNECTED;

// ipv4 for packet ip
extern __uint8 ipaddr[4];

// semaphore access gHttpFrame
osSemaphoreId httpBuffSemaphoreID;

/*==============================================================================
 *	function define
 *============================================================================*/


void httpd_header_prepare(__S_Tcp_Pkt *tcp_pkt, __uint16 port, __uint8 fl, __uint16 len, __uint16 len_cs)
{
	tcp_pkt->port_dst 	= be16toword(port);
	tcp_pkt->port_src 	= be16toword(HTTPD_TCP_PORT);
	tcp_pkt->bt_num_seg = gHttpdProp.seq_num;
	tcp_pkt->num_ask 	= gHttpdProp.ack_num;
	tcp_pkt->fl 		= fl;
	tcp_pkt->size_wnd 	= be16toword(httpd_size_wnd);
	tcp_pkt->urg_ptr 	= 0;
	tcp_pkt->len_hdr 	= len << 2;
	tcp_pkt->cs 		= 0;
	tcp_pkt->cs			= checksum((__uint8*)tcp_pkt-8, len_cs+8, 2);
}


void httpd_ip_header_prepare(__S_Ip_Pkt *ip_pkt, __uint8 *ip_addr, __uint8 prt, __uint16 len)
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

__uint8 httpd_send_finack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len=0;

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	gHttpdProp.seq_num = tcp_pkt->num_ask;
	gHttpdProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + 1);

	len = sizeof(__S_Tcp_Pkt);

	// add header tcp-http
	httpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

	len += sizeof(__S_Ip_Pkt);

	// add header ip-tcp-http
	httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

	//set address MAC Ethernet
	memcpy(frame->addr_dest,frame->addr_src,6);

	// send RTS
	eth_send(frame,ETH_IP,len);

	if(httpd_tcp_stat == TCP_DISCONNECTED)
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

	httpd_tcp_stat = TCP_DISCONNECTED;

	return res;
}


__uint8 httpd_send_synack(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len=0;

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	gHttpdProp.seq_num = rand();
	gHttpdProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + 1);
	tcp_pkt->data[0]=2;//Maximum Segment Size (2)
	tcp_pkt->data[1]=4;//Length
	tcp_pkt->data[2]=(__uint8) (httpd_tcp_mss >> 8);//MSS = 458
	tcp_pkt->data[3]=(__uint8) httpd_tcp_mss;

	len = sizeof(__S_Tcp_Pkt)+4;

	// add header tcp-http
	httpd_header_prepare(tcp_pkt, port, TCP_SYN|TCP_ACK, len, len);

	len+=sizeof(__S_Ip_Pkt);

	// add header ip-tcp-http
	httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

	//set address MAC Ethernet
	memcpy(frame->addr_dest,frame->addr_src,6);

	eth_send(frame, ETH_IP,len);

	// update httpd status
	httpd_tcp_stat = TCP_CONNECTED;

	return res;
}



__uint8 httpd_send_http_one(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
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

		gHttpdProp.seq_num = tcp_pkt->num_ask;
		gHttpdProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + sz_data);

		len = sizeof(__S_Tcp_Pkt);

		httpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

		len+= sizeof(__S_Ip_Pkt);

		httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

		//get add dest from packet Ethernet
		memcpy(frame->addr_dest,frame->addr_src,6);
		eth_send(frame,ETH_IP,len);

		if ((gHttpdProp.http_doc == HTTPD_FILE_HTML) || (gHttpdProp.http_doc == HTTPD_FILE_JPEG))
		{

			// mount sdcard
			gHttpdResult = f_mount(&gHttpdFsObj, (TCHAR const*)gDrivePath, 0);
			console_serial_print_log("\t> httpd_send_http_one : f_mount: %d", gHttpdResult);

			// open file to res
			gHttpdResult = f_open(&gHttpdFileObj, gHttpdProp.fname, FA_READ);
			console_serial_print_log("\t> httpd_send_http_one : f_open: %d  f_size: %lu", gHttpdResult, gHttpdFileObj.obj.objsize);

			gHttpdResult = f_lseek(&gHttpdFileObj,0);
			console_serial_print_log("\t> httpd_send_http_one : f_lseek: %d", gHttpdResult);

			if (gHttpdProp.http_doc == HTTPD_FILE_HTML)
			{
				strcpy((char*)tcp_pkt->data, httpd_http_header);
				gHttpdResult = f_read(&gHttpdFileObj, (void*)(tcp_pkt->data+strlen(httpd_http_header)), \
						(__uint16)gHttpdFileObj.obj.objsize, (UINT *)&gHttpdBytesRead);
			}
			else if (gHttpdProp.http_doc == HTTPD_FILE_JPEG)
			{
				strcpy((char*)tcp_pkt->data, httpd_jpg_header);
				gHttpdResult = f_read(&gHttpdFileObj, (void*)(tcp_pkt->data+strlen(httpd_jpg_header)),\
						(__uint16)gHttpdFileObj.obj.objsize, (UINT *)&gHttpdBytesRead);
			}

			// close file
			f_close(&gHttpdFileObj);
		}
		else
		{
			strcpy((char*)tcp_pkt->data, httpd_error_header);
			memcpy((void*)(tcp_pkt->data+strlen(httpd_error_header)),(void*)httpd_e404_htm,sizeof(httpd_e404_htm));
		}

		len = sizeof(__S_Tcp_Pkt);
		len += gHttpdProp.data_size;

		tcp_pkt->fl 	= TCP_PSH|TCP_ACK;
		tcp_pkt->cs 	= 0;
		tcp_pkt->cs		= checksum((__uint8*)tcp_pkt-8, len+8, 2);

		len+=sizeof(__S_Ip_Pkt);

		ip_pkt->len		= be16toword(len);
		ip_pkt->cs 		= 0;
		ip_pkt->cs 		= checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);
		//send packet Ethernet
		eth_send(frame,ETH_IP,len);

		gHttpdProp.seq_num_tmp = be32todword(be32todword(gHttpdProp.seq_num) + gHttpdProp.data_size);
		gHttpdProp.data_stat = DATA_END;

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	__LEAVE__

	return res;

}


__uint8 httpd_send_http_first(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
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
		gHttpdProp.seq_num = tcp_pkt->num_ask;
		gHttpdProp.ack_num = be32todword(be32todword(tcp_pkt->bt_num_seg) + sz_data);

		len = sizeof(__S_Tcp_Pkt);
		httpd_header_prepare(tcp_pkt, port, TCP_ACK, len, len);

		len+=sizeof(__S_Ip_Pkt);
		httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

		//get address Ethernet
		memcpy(frame->addr_dest,frame->addr_src,6);
		eth_send(frame,ETH_IP,len);


		if ((gHttpdProp.http_doc == EXISTING_HTML)||(gHttpdProp.http_doc == EXISTING_JPG))
		{
			strcpy((char*)tcp_pkt->data, httpd_http_header);

			// open sd card
			gHttpdResult = f_mount(&gHttpdFsObj, (TCHAR const*)gDrivePath, 0);
			gHttpdResult = f_open(&gHttpdFileObj, gHttpdProp.fname, FA_READ);
			gHttpdResult = f_lseek(&gHttpdFileObj,0);

			if (gHttpdProp.http_doc == HTTPD_FILE_HTML)
			{
				strcpy((char*)tcp_pkt->data, httpd_http_header);
				gHttpdResult = f_read(&gHttpdFileObj, (void*)(tcp_pkt->data + strlen(httpd_http_header)),httpd_tcp_mss - strlen(httpd_http_header), (UINT *)&gHttpdBytesRead);
			}
			else if (gHttpdProp.http_doc == HTTPD_FILE_JPEG)
			{
				strcpy((char*)tcp_pkt->data, httpd_jpg_header);
				gHttpdResult = f_read(&gHttpdFileObj,(void*)(tcp_pkt->data+strlen(httpd_jpg_header)),httpd_tcp_mss-strlen(httpd_jpg_header),(UINT *)&gHttpdBytesRead);
			}
		}
		else
		{
			strcpy((char*)tcp_pkt->data, httpd_error_header);
			memcpy((void*)(tcp_pkt->data+strlen(httpd_error_header)),(void*)httpd_e404_htm, httpd_tcp_mss-strlen(httpd_e404_htm));
		}

		tcp_pkt->fl = TCP_ACK;
		len = sizeof(__S_Tcp_Pkt);
		len += httpd_tcp_mss;
		tcp_pkt->cs = 0;
		tcp_pkt->cs=checksum((__uint8*)tcp_pkt-8, len+8, 2);
		len+=sizeof(__S_Ip_Pkt);
		ip_pkt->len=be16toword(len);
		ip_pkt->cs = 0;
		ip_pkt->cs = checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);

		//http send data packet with  Ethernet
		eth_send(frame,ETH_IP,len);

		gHttpdProp.cnt_rem_data_part--;

		//check status next packet
		gHttpdProp.cnt_size_wnd += httpd_tcp_mss;

		if(gHttpdProp.cnt_rem_data_part>1)
		{
			gHttpdProp.data_stat = DATA_MIDDLE;
		}
		else
		{
			gHttpdProp.data_stat = DATA_LAST;
		}

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	else
	{
		res = 1;
	}

	return res;
}


__uint8 httpd_send_http_middle(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len_tcp=0, len=0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	// wait for sd card ready to access
	// Release microSD device
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{

		gHttpdProp.seq_num = be32todword(be32todword(gHttpdProp.seq_num) + httpd_tcp_mss);
		len_tcp = sizeof(__S_Tcp_Pkt);

		if ((gHttpdProp.http_doc == HTTPD_FILE_HTML)||(gHttpdProp.http_doc == HTTPD_FILE_JPEG))
		{
			if (gHttpdProp.http_doc == HTTPD_FILE_HTML)
			{
				gHttpdResult = f_lseek(&gHttpdFileObj,((uint32_t)httpd_tcp_mss*(gHttpdProp.cnt_data_part-gHttpdProp.cnt_rem_data_part))-strlen(httpd_http_header));
			}
			else if (gHttpdProp.http_doc == HTTPD_FILE_JPEG)
			{
				gHttpdResult = f_lseek(&gHttpdFileObj,((uint32_t)httpd_tcp_mss*(gHttpdProp.cnt_data_part-gHttpdProp.cnt_rem_data_part))-strlen(httpd_jpg_header));
			}
			gHttpdResult = f_read(&gHttpdFileObj,(void*)tcp_pkt->data,httpd_tcp_mss,(UINT *)&gHttpdBytesRead);
		}
		else
		{
			memcpy((void*)tcp_pkt->data,(void*)(httpd_e404_htm+(httpd_tcp_mss*(gHttpdProp.cnt_data_part-gHttpdProp.cnt_rem_data_part))-strlen(httpd_error_header)), httpd_tcp_mss);
		}
		len=len_tcp + httpd_tcp_mss;

		if ((httpd_size_wnd - gHttpdProp.cnt_size_wnd) > httpd_tcp_mss)
		{
			httpd_header_prepare(tcp_pkt, port, TCP_ACK, len_tcp, len);
		}
		else
		{
			httpd_header_prepare(tcp_pkt, port, TCP_PSH|TCP_ACK, len_tcp, len);

			gHttpdProp.cnt_size_wnd = 0;
		}
		len+=sizeof(__S_Ip_Pkt);
		httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);

		memcpy(frame->addr_dest,gHttpdProp.macaddr_dst,6);
		eth_send(frame,ETH_IP,len);

		gHttpdProp.cnt_rem_data_part--;

		gHttpdProp.cnt_size_wnd += httpd_tcp_mss;

		if(gHttpdProp.cnt_rem_data_part > 1)
		{
			gHttpdProp.data_stat = DATA_MIDDLE;
		}
		else
		{
			gHttpdProp.data_stat = DATA_LAST;
		}

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	else
	{
		res = 1;
	}

	return res;
}


__uint8 httpd_send_http_last(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res=0;
	__uint16 len_tcp=0, len=0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	// wait for sd card ready to access
	// Release microSD device
	if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
	{

		gHttpdProp.seq_num = be32todword(be32todword(gHttpdProp.seq_num) + httpd_tcp_mss);

		len_tcp = sizeof(__S_Tcp_Pkt);

		if ((gHttpdProp.http_doc == HTTPD_FILE_HTML)||(gHttpdProp.http_doc == HTTPD_FILE_JPEG))
		{
			if (gHttpdProp.http_doc == HTTPD_FILE_HTML)
			{
				gHttpdResult = f_lseek(&gHttpdFileObj,(httpd_tcp_mss*(gHttpdProp.cnt_data_part-1))-strlen(httpd_http_header));
			}
			else if (gHttpdProp.http_doc == HTTPD_FILE_JPEG)
			{
				gHttpdResult = f_lseek(&gHttpdFileObj,(httpd_tcp_mss*(gHttpdProp.cnt_data_part-1))-strlen(httpd_jpg_header));
			}
			console_serial_print_log("\t> httpd_send_http_last : f_lseek: %d\r\n", gHttpdResult);

			gHttpdResult = f_read(&gHttpdFileObj,(void*)tcp_pkt->data,gHttpdProp.last_data_part_size,(UINT *)&gHttpdBytesRead);
		}
		else
		{
			memcpy((void*)tcp_pkt->data,(void*)(httpd_e404_htm+(httpd_tcp_mss*(gHttpdProp.cnt_data_part-1))-strlen(httpd_error_header)),gHttpdProp.last_data_part_size);
		}
		len=len_tcp + gHttpdProp.last_data_part_size;
		httpd_header_prepare(tcp_pkt, port, TCP_PSH|TCP_ACK, len_tcp, len);
		len+=sizeof(__S_Ip_Pkt);
		httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);
		// Ethernet
		memcpy(frame->addr_dest,gHttpdProp.macaddr_dst,6);
		eth_send(frame,ETH_IP,len);
		//
		gHttpdProp.seq_num_tmp = be32todword(be32todword(tcp_pkt->bt_num_seg)+gHttpdProp.last_data_part_size);
		gHttpdProp.data_stat=DATA_END;

		// Release microSD device
		osSemaphoreRelease(microSDSemaphoreID);
	}
	else
	{
		res = 1;
	}

	return res;
}


__uint8 httpd_send_http_data_end(__S_Enc28j60_Frame_Pkt *frame, __uint8 *ip_addr, __uint16 port)
{
	__uint8 res		= 0;
	__uint16 len 	= 0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);
	gHttpdProp.seq_num = gHttpdProp.seq_num_tmp;
	gHttpdProp.ack_num = tcp_pkt->bt_num_seg;
	len = sizeof(__S_Tcp_Pkt);

	httpd_header_prepare(tcp_pkt, port, TCP_FIN|TCP_ACK, len, len);
	len+=sizeof(__S_Ip_Pkt);

	httpd_ip_header_prepare(ip_pkt, ip_addr, IP_TCP, len);
	//�������� ��������� Ethernet
	memcpy(frame->addr_dest,frame->addr_src,6);
	eth_send(frame,ETH_IP,len);

	gHttpdProp.data_stat = DATA_COMPLETED;
	httpd_tcp_stat = TCP_DISCONNECTED;

	// Release microSD device
	osSemaphoreRelease(microSDSemaphoreID);

	return res;
}


void httpd_pool()
{
	__ENTER__

	__S_Enc28j60_Frame_Pkt *HttpFrame = (void*)(gHttpFrame);
	__S_Ip_Pkt *ip_pkt = (void*)(HttpFrame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	__uint32 lenRcvData = 0;
	char *ss1;
	int ch1=' ';
	int ch2='.';


	// wait for httpd buff ready to access
	if (osSemaphoreWait(httpBuffSemaphoreID, TIME_WAIT_SHORT) == osOK )
	{
		// get port
		gHttpdProp.port_dst = be16toword(tcp_pkt->port_src);

		// get MAC addr
		memcpy(gHttpdProp.macaddr_dst,HttpFrame->addr_src,6);

		// get IPv4
		memcpy(gHttpdProp.ipaddr_dst,ip_pkt->ipaddr_src,4);

		// get length data
		lenRcvData = be16toword(ip_pkt->len)-20-(tcp_pkt->len_hdr>>2);

		// check resquest data
		if (lenRcvData)
		{
			console_serial_print_log("\t\t> packet ACK data available");
			console_serial_print_infor("TCP packet content : %s", tcp_pkt->data);

			// packet tcp contain ACK
			if (tcp_pkt->fl & TCP_ACK)
			{
				// packet "GET /", for protocol HTTP
				if (strncmp((char*)tcp_pkt->data, "GET /", 5) == 0)
				{
					gHttpdProp.cnt_size_wnd = 0;

					if((char)tcp_pkt->data[5]==' ')
					{
						// HTTP/GET no  target file
						console_serial_print_log("\t> HTTP/GET no  target file");

						strcpy(gHttpdProp.fname,"index.html");
						gHttpdProp.http_doc = HTTPD_FILE_HTML;
					}
					else
					{
						// HTTP/GET contain target file
						console_serial_print_log("\t> HTTP/GET contain target file");

						memcpy((void*)gHttpdProp.fname, (void*)(tcp_pkt->data + 5), 20);
						ss1 = strchr(gHttpdProp.fname, ch1);
						ss1[0] = 0;
					}

					console_serial_print_log("\t>file request :--name : %s --size : %d", (__uint8*)gHttpdProp.fname, strlen(gHttpdProp.fname));

					// wait for sd card ready to access
					// Release microSD device
					if (osSemaphoreWait(microSDSemaphoreID, TIME_WAIT_LONG) == osOK )
					{
						// open sd card and prepare file for send
						gHttpdResult = f_mount(&gHttpdFsObj, (TCHAR const*)gDrivePath, 0);
						console_serial_print_log("f_mount: %d", gHttpdResult);

						// open file in sd card
						gHttpdResult = f_open(&gHttpdFileObj, gHttpdProp.fname, FA_READ);
						console_serial_print_log("f_open: %d  f_size: %lu", gHttpdResult, gHttpdFileObj.obj.objsize);

						// Release microSD device
						osSemaphoreRelease(microSDSemaphoreID);
					}

					// prepare header to send repost
					if (gHttpdResult == FR_OK)
					{
						ss1 = strchr(gHttpdProp.fname,ch2);
						ss1++;

						// the file is jpg
						if (strncmp(ss1,"jpg", 3) == 0)
						{
							gHttpdProp.http_doc = HTTPD_FILE_JPEG;

							gHttpdProp.data_size = strlen(httpd_jpg_header);
						}
						else
						{
							gHttpdProp.http_doc = HTTPD_FILE_HTML;

							gHttpdProp.data_size = strlen(httpd_http_header);
						}

						gHttpdProp.data_size += gHttpdFileObj.obj.objsize;
					}
					else
					{
						gHttpdProp.http_doc = E404_HTML;

						gHttpdProp.data_size = strlen(httpd_error_header);

						gHttpdProp.data_size += sizeof(httpd_e404_htm);
					}

					gHttpdProp.cnt_rem_data_part = gHttpdProp.data_size / httpd_tcp_mss + 1;
					gHttpdProp.last_data_part_size = gHttpdProp.data_size % httpd_tcp_mss;

					if(gHttpdProp.last_data_part_size == 0)
					{
						gHttpdProp.last_data_part_size = httpd_tcp_mss;
						gHttpdProp.cnt_rem_data_part--;
					}

					gHttpdProp.cnt_data_part = gHttpdProp.cnt_rem_data_part;

					console_serial_print_log("data size:%lu; cnt data part:%u; last_data_part_size:%u; port dst:%u",
							(unsigned long)gHttpdProp.data_size, gHttpdProp.cnt_rem_data_part, gHttpdProp.last_data_part_size,gHttpdProp.port_dst);

					if (gHttpdProp.cnt_rem_data_part == 1)
					{
						gHttpdProp.data_stat = DATA_ONE;
					}
					else if (gHttpdProp.cnt_rem_data_part > 1)
					{
						gHttpdProp.data_stat = DATA_FIRST;
					}

					if(gHttpdProp.data_stat == DATA_ONE)
					{
						httpd_send_http_one(HttpFrame, ip_pkt->ipaddr_src, gHttpdProp.port_dst);
					}
					else if(gHttpdProp.data_stat == DATA_FIRST)
					{
						console_serial_print_log("-------------START-----------");
						httpd_send_http_first(HttpFrame, ip_pkt->ipaddr_src, gHttpdProp.port_dst);
					}
				}
			}

		}


		if (tcp_pkt->fl == TCP_SYN)
		{
			console_serial_print_log("\t> Packet request SYN");
			httpd_send_synack(HttpFrame, ip_pkt->ipaddr_src, gHttpdProp.port_dst);
		}
		else if (tcp_pkt->fl == (TCP_FIN|TCP_ACK))
		{
			console_serial_print_log("\t> Packet request FIN|ACK");
			httpd_send_finack(HttpFrame, ip_pkt->ipaddr_src, gHttpdProp.port_dst);
		}
		else if (tcp_pkt->fl == (TCP_PSH|TCP_ACK))
		{
			console_serial_print_log("\t> Packet request PSH|ACK");

			if(!lenRcvData)
			{
				httpd_send_finack(HttpFrame, ip_pkt->ipaddr_src, gHttpdProp.port_dst);
			}
		}
		else if (tcp_pkt->fl == TCP_ACK)
		{
			console_serial_print_log("\t> Packet request ACK");

			if (gHttpdProp.data_stat == DATA_END)
			{
				httpd_send_http_data_end(HttpFrame, ip_pkt->ipaddr_src, gHttpdProp.port_dst);
			}
			else if (gHttpdProp.data_stat == DATA_MIDDLE)
			{
				httpd_send_http_middle(HttpFrame, gHttpdProp.ipaddr_dst, gHttpdProp.port_dst);
			}
			else if (gHttpdProp.data_stat == DATA_LAST)
			{
				console_serial_print_log("-------------LAST-----------");
				gHttpdProp.data_stat = DATA_COMPLETED;
				httpd_send_http_last(HttpFrame, gHttpdProp.ipaddr_dst, gHttpdProp.port_dst);
			}
		}
	}
	else
	{
		console_serial_print_log("\t> httpd_pool : SD card time out: ");

	}

	// Release httpd buff
	osSemaphoreRelease(httpBuffSemaphoreID);

	__LEAVE__
}


void httpd_task_polling(void * argument)
{
	osEvent sig_rev;

	for (;;)
	{
		// wait for signal rev
		sig_rev = osSignalWait (0xffffffff, TIME_WAIT_LONG);

		if (sig_rev.status == osEventSignal && sig_rev.value.signals == SIG_HTTPD_REV)
		{
			httpd_pool();
		}
	}
}


void httpd_init()
{
	__ENTER__
	// create semaphore httpbuffer
	// Creating semaphore for microSD, it use to access to read, wire
	osSemaphoreDef(http_buff);
	httpBuffSemaphoreID = osSemaphoreCreate(osSemaphore(http_buff) , 1 );

	// Release httpd buff
	osSemaphoreRelease(httpBuffSemaphoreID);

	// create task httpd
	console_serial_print_log("Create task httpd poolling");
	osThreadDef(httpd_pool, httpd_task_polling, osPriorityNormal, 0, 500);

	gListPID[INDEX_HTTPD_SERVER] = osThreadCreate(osThread(httpd_pool), NULL);

	__LEAVE__
}

#include "net.h"
#include "enc28j60.h"
#include "console_serial_trace.h"

//-----------------------------------------------
__uint8 net_buf[ENC28J60_MAXFRAME];

__uint8 ipaddr[4]=IP_ADDR;
__uint8 ipgate[4]=IP_GATE;
__uint8 ipmask[4]=IP_MASK;
uint32_t clock_cnt=0;

uint32_t ping_cnt=0;

extern char str[20];
USART_prop_ptr usartprop;
extern ntp_prop_ptr ntpprop;

__uint8 macaddr[]={0x01,0x02,0x03,0x04,0x05,0x06};


osSemaphoreId networkSemaphoreID;

//-----------------------------------------------
void net_ini(void)
{
	__ENTER__

	usartprop.usart_buf[0]=0;
	usartprop.usart_cnt=0;
	usartprop.is_ip=0;

	console_serial_print_log("\t> mac addr : %d:%d:%d:%d:%d:%d", \
			macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

	// Init hardware device enc28j60
	console_serial_print_log("\t> Init hardware device enc28j60");
	enc28j60_init(macaddr);

	//
	ntpprop.set=0;
	ntpprop.ntp_cnt=0;
	ntpprop.ntp_timer=0;

	// init semaphore for network device
	console_serial_print_log("\t> init semaphore for network device");
	osSemaphoreDef(network_device);
	networkSemaphoreID = osSemaphoreCreate(osSemaphore(network_device) , 1 );

	// Release network device
	console_serial_print_log("\t> Release network device");
	osSemaphoreRelease(networkSemaphoreID);
	__LEAVE__
}
//-----------------------------------------------
__uint16 checksum(__uint8 *ptr, int16_t len, __uint8 type)
{
	uint32_t sum=0;
	if(type==1)
	{
		sum+=IP_UDP;
		sum+=len-8;
	}
	if(type==2)
	{
		sum+=IP_TCP;
		sum+=len-8;
	}
	while(len>1)
	{
		sum += (__uint16) (((uint32_t)*ptr<<8)|*(ptr+1));
		ptr+=2;
		len-=2;
	}
	if(len) sum+=((uint32_t)*ptr)<<8;
	while(sum>>16) sum=(__uint16)sum+(sum>>16);
	return ~be16toword((__uint16)sum);
}
//-----------------------------------------------
void eth_send(__S_Enc28j60_Frame_Pkt *frame, __uint16 type, __uint16 len)
{
	memcpy(frame->addr_src,macaddr,6);
	frame->type	=	type;
	// send packet with enc28j60 device
	enc28j60_packetSend((void*)frame, len + sizeof(__S_Enc28j60_Frame_Pkt));
}
//-----------------------------------------------
__uint8 ip_send(__S_Enc28j60_Frame_Pkt *frame, __uint16 len)
{
	__uint8 res = 0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	// create packet IP
	ip_pkt->len			=	be16toword(len);
	ip_pkt->fl_frg_of	=	0;
	ip_pkt->ttl			=	128;
	ip_pkt->cs			=	0;

	memcpy(ip_pkt->ipaddr_dst,ip_pkt->ipaddr_src,4);
	memcpy(ip_pkt->ipaddr_src,ipaddr,4);

	ip_pkt->cs	= checksum((void*)ip_pkt, sizeof(__S_Ip_Pkt),0);

	// create packet layout MAC and send
	eth_send(frame,ETH_IP,len);
	return res;
}
//-----------------------------------------------
__uint8 icmp_read(__S_Enc28j60_Frame_Pkt *frame, __uint16 len)
{
	__uint8 res=0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	icmp_pkt_ptr *icmp_pkt = (void*)(ip_pkt->data);

	if(len >= sizeof(icmp_pkt_ptr))
	{
		if(icmp_pkt->msg_tp == ICMP_REQ)
		{
			icmp_pkt->msg_tp 	= ICMP_REPLY;
			icmp_pkt->cs		= 0;
			icmp_pkt->cs		= checksum((void*)icmp_pkt,len,0);

			memcpy(frame->addr_dest,frame->addr_src,6);

			ip_send(frame, len + sizeof(__S_Ip_Pkt));

			console_serial_print_log("\t> Icmp request %d.%d.%d.%d-%d.%d.%d.%d", \
					ip_pkt->ipaddr_dst[0],ip_pkt->ipaddr_dst[1],ip_pkt->ipaddr_dst[2],ip_pkt->ipaddr_dst[3],
					ip_pkt->ipaddr_src[0],ip_pkt->ipaddr_src[1],ip_pkt->ipaddr_src[2],ip_pkt->ipaddr_src[3]);
		}
		else if (icmp_pkt->msg_tp == ICMP_REPLY)
		{
			console_serial_print_log("\t> Icmp reply%d.%d.%d.%d-%d.%d.%d.%d",
					ip_pkt->ipaddr_src[0],ip_pkt->ipaddr_src[1],ip_pkt->ipaddr_src[2],ip_pkt->ipaddr_src[3],
					ip_pkt->ipaddr_dst[0],ip_pkt->ipaddr_dst[1],ip_pkt->ipaddr_dst[2],ip_pkt->ipaddr_dst[3]);
		}
	}
	return res;
}
//-----------------------------------------------
__uint8 ip_read(__S_Enc28j60_Frame_Pkt *frame, __uint16 len)
{
	__uint8 res = 0;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);

	if((ip_pkt->verlen == 0x45) && (!memcmp(ip_pkt->ipaddr_dst, ipaddr, 4)))
	{

		len = be16toword(ip_pkt->len) - sizeof(__S_Ip_Pkt);

		if(ip_pkt->prt == IP_ICMP)
		{
			console_serial_print_infor("\t\t> ETH_IP packet type : IP_ICMP");
			icmp_read(frame,len);
		}
		else if(ip_pkt->prt == IP_TCP)
		{
			console_serial_print_infor("\t\t> ETH_IP packet type : IP_TCP");
			tcp_read(frame,len);
		}
		else if(ip_pkt->prt == IP_UDP)
		{
			console_serial_print_infor("\t\t> ETH_IP packet type : IP_UDP");
			udp_read(frame,len);
		}
	}
	return res;
}
//-----------------------------------------------
__uint8 icmp_request(__uint8* ip_addr)
{
	__uint8 res=0;
	__uint16 len;
	__S_Enc28j60_Frame_Pkt *frame=(void*) net_buf;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	icmp_pkt_ptr *icmp_pkt = (void*)ip_pkt->data;
	//�������� ��������� ������ ICMP
	icmp_pkt->msg_tp = 8;
	icmp_pkt->msg_cd = 0;
	icmp_pkt->id = be16toword(1);
	icmp_pkt->num = be16toword(ping_cnt);
	ping_cnt++;
	strcpy((char*)icmp_pkt->data,"abcdefghijklmnopqrstuvwabcdefghi");
	icmp_pkt->cs = 0;
	len = strlen((char*)icmp_pkt->data) + sizeof(icmp_pkt_ptr);
	icmp_pkt->cs=checksum((void*)icmp_pkt,len,0);
	//�������� ��������� ������ IP
	len+=sizeof(__S_Ip_Pkt);
	ip_pkt->len=be16toword(len);
	ip_pkt->id = 0;
	ip_pkt->ts = 0;
	ip_pkt->verlen = 0x45;
	ip_pkt->fl_frg_of=0;
	ip_pkt->ttl=128;
	ip_pkt->cs = 0;
	ip_pkt->prt=IP_ICMP;
	memcpy(ip_pkt->ipaddr_dst,ip_addr,4);
	memcpy(ip_pkt->ipaddr_src,ipaddr,4);
	ip_pkt->cs = checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);
	//�������� ����� Ethernet
	eth_send(frame,ETH_IP,len);
	return res;
}
//--------------------------------------------------
__uint16 port_extract(char* ip_str, __uint8 len)
{
	__uint16 port=0;
	int ch1=':';
	char *ss1;
	__uint8 offset = 0;
	ss1=strchr(ip_str,ch1);
	offset=ss1-ip_str+1;
	ip_str+=offset;
	port = atoi(ip_str);
	return port;
}
//--------------------------------------------------
void ip_extract(char* ip_str, __uint8 len, __uint8* ipextp)
{
	__uint8 offset = 0;
	__uint8 i;
	char ss2[5] = {0};
	char *ss1;
	int ch1 = '.';
	int ch2 = ':';
	for(i=0;i<3;i++)
	{
		ss1 = strchr(ip_str,ch1);
		offset = ss1-ip_str+1;
		strncpy(ss2,ip_str,offset);
		ss2[offset]=0;
		ipextp[i] = atoi(ss2);
		ip_str+=offset;
		len-=offset;
	}
	ss1=strchr(ip_str,ch2);
	if (ss1!=NULL)
	{
		offset=ss1-ip_str+1;
		strncpy(ss2,ip_str,offset);
		ss2[offset]=0;
		ipextp[3] = atoi(ss2);
		return;
	}
	strncpy(ss2,ip_str,len);
	ss2[len]=0;
	ipextp[3] = atoi(ss2);
}
//-----------------------------------------------
void eth_read(__S_Enc28j60_Frame_Pkt *frame, __uint16 len)
{
	__uint8 res=0;
	if(len>sizeof(__S_Enc28j60_Frame_Pkt))
	{
		if(frame->type==ETH_ARP)
		{
			console_serial_print_infor("\t> Receiver packet type : ETH_ARP");

			res = arp_read(frame, len - sizeof(__S_Enc28j60_Frame_Pkt));

			if(res==1)
			{
				console_serial_print_infor("\t\t> Send response arp");
				arp_send(frame);
			}
			else if(res==2)
			{
				arp_table_fill(frame);

				if((usartprop.is_ip==3)||(usartprop.is_ip==5)||(usartprop.is_ip==7))
				{
					memcpy(frame->addr_dest,frame->addr_src,6);
					net_cmd();
				}
			}
		}
		else if(frame->type==ETH_IP)
		{
			console_serial_print_infor("\t> Receiver packet type : ETH_IP  length : %d", len);
			ip_read(frame, len - sizeof(__S_Ip_Pkt));
		}
		else
		{
			console_serial_print_log("%02X:%02X:%02X:%02X:%02X:%02X-%02X:%02X:%02X:%02X:%02X:%02X; %d; %04X",
					frame->addr_src[0],frame->addr_src[1],frame->addr_src[2],frame->addr_src[3],frame->addr_src[4],frame->addr_src[5],
					frame->addr_dest[0],frame->addr_dest[1],frame->addr_dest[2],frame->addr_dest[3],frame->addr_dest[4],frame->addr_dest[5],
					len, be16toword(frame->type));
		}		
	}
}


//-----------------------------------------------
void net_poll(void)
{
	__uint16 len;

	__S_Enc28j60_Frame_Pkt *frame = (void*)net_buf;

	//	console_serial_print_log("\t\t> nett pool");

	if ((len = enc28j60_packetReceive(net_buf,sizeof(net_buf)))>0)
	{
		eth_read(frame,len);
	}

}


//-----------------------------------------------
void net_cmd(void)
{
	static __uint8 ip[4]={0};
	static __uint16 port=0;
	__S_Enc28j60_Frame_Pkt *frame=(void*)net_buf;
	if(usartprop.is_ip==1)//������ �������� ARP-�������
	{
		ip_extract((char*)usartprop.usart_buf,usartprop.usart_cnt,ip);
		arp_request(ip);
		usartprop.is_ip = 0;
		usartprop.usart_cnt=0;
	}
	else if(usartprop.is_ip==2)//������ ������� ��������� UDP-�����
	{
		ip_extract((char*)usartprop.usart_buf,usartprop.usart_cnt,ip);
		usartprop.is_ip=3;//������ �������� UDP-������
		usartprop.usart_cnt=0;
		arp_request(ip);//������ mac-�����
	}
	else if(usartprop.is_ip==3)//������ �������� UDP-������
	{
		port=port_extract((char*)usartprop.usart_buf,usartprop.usart_cnt);
		udp_send(ip,port);
		usartprop.is_ip=0;
	}
	else if(usartprop.is_ip==4)//������ ������� ��������� ICMP-�����
	{
		ip_extract((char*)usartprop.usart_buf,usartprop.usart_cnt,ip);
		usartprop.is_ip=5;//������ �������� ICMP-������
		usartprop.usart_cnt=0;
		arp_request(ip);//������ mac-�����
	}
	else if(usartprop.is_ip==5)//������ �������� ICMP-������
	{
		icmp_request(ip);
		usartprop.is_ip=0;
	}
	else if(usartprop.is_ip==6)//������ ������� ��������� NTP-�����
	{
		ip_extract((char*)usartprop.usart_buf,usartprop.usart_cnt,ip);
		memcpy(ntpprop.ip_dst,ip,4);
		usartprop.is_ip=7;//������ �������� NTP-������
		usartprop.usart_cnt=0;
		arp_request(ip);//������ mac-�����
	}
	else if(usartprop.is_ip==7)//������ �������� NTP-������
	{
		port=port_extract((char*)usartprop.usart_buf,usartprop.usart_cnt);
		ntpprop.port_dst = port;
		ntpprop.ntp_cnt = 10; //10 �������
		ntpprop.ntp_timer = 5;//5 ������ �� ��������� �������
		ntpprop.set=1;//���� ������� ������� �������
		memcpy(ntpprop.macaddr_dst,frame->addr_dest,6);
		ntp_request(ntpprop.ip_dst,ntpprop.port_dst);
		usartprop.is_ip=0;
	}
}
//-----------------------------------------------
void UART1_RxCpltCallback(void)
{
	__uint8 b;
	b=str[0];
	//���� ����� �������� �������� ����� ������
	if(usartprop.usart_cnt>25)
	{
		usartprop.usart_cnt=0;
	}
	else if(b == 'a')
	{
		usartprop.is_ip=1;//������ �������� ARP-�������
		net_cmd();
	}
	else if (b=='u')
	{
		usartprop.is_ip=2;//������ ������� ��������� UDP-�����
		net_cmd();
	}
	else if (b=='p')
	{
		usartprop.is_ip=4;//������ ������� ��������� ICMP-�����
		net_cmd();
	}
	else if (b=='n')
	{
		usartprop.is_ip=6;//������ ������� ��������� NTP-�����
		net_cmd();
	}
	else
	{
		usartprop.usart_buf[usartprop.usart_cnt] = b;
		usartprop.usart_cnt++;
	}
	HAL_UART_Receive_IT(&huart1, (__uint8*)str,1);
}
//-----------------------------------------------
void TIM_PeriodElapsedCallback(void)
{
	//������� ������� � ���������� �� � clock_cnt
	clock_cnt++;
	if (ntpprop.set)
	{
		ntpprop.ntp_timer--;
		if ((ntpprop.ntp_timer<0)&&(ntpprop.ntp_cnt>0))
		{
			ntpprop.ntp_timer = 5;
			ntpprop.ntp_cnt--;
			console_serial_print_log("ntp_cnt: %d\r\n",ntpprop.ntp_cnt);
			ntp_request(ntpprop.ip_dst,ntpprop.port_dst);
		}
		else if (ntpprop.ntp_cnt<=0)
		{
			//������� ��� ����� � ��������
			ntpprop.set=0;
			ntpprop.ntp_cnt=0;
			ntpprop.ntp_timer=0;
		}
	}
}
//-----------------------------------------------

#include "arp.h"
#include "console_serial_trace.h"

//--------------------------------------------------
extern uint8_t ipaddr[4];
extern uint8_t ipgate[4];
extern uint8_t ipmask[4];
extern uint8_t macaddr[6];

extern uint32_t clock_cnt;//������� ������
extern uint8_t net_buf[ENC28J60_MAXFRAME];
uint8_t macbroadcast[6]=MAC_BROADCAST;
uint8_t macnull[6]=MAC_NULL;
arp_record_ptr arp_rec[5];
uint8_t current_arp_index=0;
extern USART_prop_ptr usartprop;
//--------------------------------------------------
uint8_t arp_read(__S_Enc28j60_Frame_Pkt *frame, uint16_t len)
{
	uint8_t res=0;
	arp_msg_ptr *msg=(void*)(frame->data);
	if(len>sizeof(arp_msg_ptr))
	{
		if((msg->net_tp==ARP_ETH)&&(msg->proto_tp==ARP_IP))
		{
			if(!memcmp(msg->ipaddr_dst,ipaddr,4))
			{
				if(msg->op==ARP_REQUEST)
				{
					res=1;
				}
				else if(msg->op==ARP_REPLY)
				{
					console_serial_print_infor("\t> reply mac_src %02X:%02X:%02X:%02X:%02X:%02X", \
					msg->macaddr_src[0],msg->macaddr_src[1],msg->macaddr_src[2], \
					msg->macaddr_src[3],msg->macaddr_src[4],msg->macaddr_src[5]);

					console_serial_print_infor("\t> ip_src %d.%d.%d.%d", \
					msg->ipaddr_src[0],msg->ipaddr_src[1],msg->ipaddr_src[2],msg->ipaddr_src[3]);

					console_serial_print_infor("\t> mac_dst %02X:%02X:%02X:%02X:%02X:%02X",
					msg->macaddr_dst[0],msg->macaddr_dst[1],msg->macaddr_dst[2],
					msg->macaddr_dst[3],msg->macaddr_dst[4],msg->macaddr_dst[5]);

					console_serial_print_infor("ip_dst %d.%d.%d.%d\r\n",
					msg->ipaddr_dst[0],msg->ipaddr_dst[1],msg->ipaddr_dst[2],msg->ipaddr_dst[3]);

					res=2;
				} 
			}
		}
	}
	return res;
}
//-----------------------------------------------
void arp_send(__S_Enc28j60_Frame_Pkt *frame)
{
	arp_msg_ptr *msg=(void*)(frame->data);
	msg->op = ARP_REPLY;
	memcpy(msg->macaddr_dst,msg->macaddr_src,6);
	memcpy(msg->macaddr_src,macaddr,6);
	memcpy(msg->ipaddr_dst,msg->ipaddr_src,4);
	memcpy(msg->ipaddr_src,ipaddr,4);
	memcpy(frame->addr_dest,frame->addr_src,6);
	eth_send(frame,ETH_ARP,sizeof(arp_msg_ptr));

	console_serial_print_infor("%02X:%02X:%02X:%02X:%02X:%02X(%d.%d.%d.%d)-",
    frame->addr_dest[0],frame->addr_dest[1],frame->addr_dest[2],frame->addr_dest[3],frame->addr_dest[4],frame->addr_dest[5],
    msg->ipaddr_dst[0],msg->ipaddr_dst[1],msg->ipaddr_dst[2],msg->ipaddr_dst[3]);

	console_serial_print_infor("%02X:%02X:%02X:%02X:%02X:%02X(%d.%d.%d.%d) arp request",
    frame->addr_src[0],frame->addr_src[1],frame->addr_src[2],frame->addr_src[3],frame->addr_src[4],frame->addr_src[5],
    msg->ipaddr_src[0],msg->ipaddr_src[1],msg->ipaddr_src[2],msg->ipaddr_src[3]);

}
//-----------------------------------------------
uint8_t arp_request(uint8_t *ip_addr)
{
	uint8_t i, j;
	uint8_t ip[4];
  uint8_t iptemp = 0;
	for(i=0;i<4;i++)
	{
		iptemp += (ip_addr[i] ^ ipaddr[i]) & ipmask[i];
	}
	__S_Enc28j60_Frame_Pkt *frame=(void*)net_buf;

	if( iptemp == 0 ) memcpy(ip,ip_addr,4);
	else memcpy(ip,ipgate,4);

	for(j=0;j<5;j++)
  {

		if((clock_cnt-arp_rec[j].sec)>43200)
		{
			memset(arp_rec+(sizeof(arp_record_ptr)*j),0,sizeof(arp_record_ptr));
		}
		if(!memcmp(arp_rec[j].ipaddr,ip,4))
		{

			for(i=0;i<5;i++)
			{
				console_serial_print_infor("%d.%d.%d.%d - %02X:%02X:%02X:%02X:%02X:%02X - %lu",
				arp_rec[i].ipaddr[0],arp_rec[i].ipaddr[1],arp_rec[i].ipaddr[2],arp_rec[i].ipaddr[3],
				arp_rec[i].macaddr[0],arp_rec[i].macaddr[1],arp_rec[i].macaddr[2],
				arp_rec[i].macaddr[3],arp_rec[i].macaddr[4],arp_rec[i].macaddr[5],
				(unsigned long)arp_rec[i].sec);
			}

			memcpy(frame->addr_dest,arp_rec[j].macaddr,6);

			if((usartprop.is_ip==3)||(usartprop.is_ip==5)||(usartprop.is_ip==7))
			{
				net_cmd();
			}			
			return 0;
		}
	}
	arp_msg_ptr *msg=(void*)(frame->data);
	msg->net_tp = ARP_ETH;
	msg->proto_tp = ARP_IP;
	msg->macaddr_len = 6;
	msg->ipaddr_len = 4;
	msg->op = ARP_REQUEST;
	memcpy(msg->macaddr_src,macaddr,6);
	memcpy(msg->ipaddr_src,ipaddr,4);	
	memcpy(msg->macaddr_dst,macnull,6);
	memcpy(msg->ipaddr_dst,ip,4);

	eth_send(frame,ETH_ARP,sizeof(arp_msg_ptr));
	return 1;
}
//--------------------------------------------------
void arp_table_fill(__S_Enc28j60_Frame_Pkt *frame)
{
	uint8_t i;
	arp_msg_ptr *msg=(void*)(frame->data);
	memcpy(arp_rec[current_arp_index].ipaddr,msg->ipaddr_src,4);
	memcpy(arp_rec[current_arp_index].macaddr,msg->macaddr_src,6);
	arp_rec[current_arp_index].sec = clock_cnt;
	if(current_arp_index<4) current_arp_index++;
	else current_arp_index=0;
	for(i=0;i<5;i++)
  {
		console_serial_print_infor("%d.%d.%d.%d - %02X:%02X:%02X:%02X:%02X:%02X - %lu",
      arp_rec[i].ipaddr[0],arp_rec[i].ipaddr[1],arp_rec[i].ipaddr[2],arp_rec[i].ipaddr[3],
      arp_rec[i].macaddr[0],arp_rec[i].macaddr[1],arp_rec[i].macaddr[2],
      arp_rec[i].macaddr[3],arp_rec[i].macaddr[4],arp_rec[i].macaddr[5],
      (unsigned long)arp_rec[i].sec);

  } 
}
//--------------------------------------------------

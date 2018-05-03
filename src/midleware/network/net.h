#ifndef __NET_H
#define __NET_H
//--------------------------------------------------
#include "header.h"
#include "enc28j60.h"
#include "cmsis_os.h"
//--------------------------------------------------
#define IP_ADDR {10,2,9,195}
#define IP_GATE {10,2,9,1}
#define IP_MASK {255,255,255,0}
#define MAC_BROADCAST {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
#define MAC_NULL {0x00,0x00,0x00,0x00,0x00,0x00} 


extern osSemaphoreId networkSemaphoreID;

// Struct packet layout MAC
typedef struct enc28j60_frame
{
	__uint8 addr_dest[6];
	__uint8 addr_src[6];
	__uint16  type;
	__uint8 data[];
} __S_Enc28j60_Frame_Pkt;


typedef struct arp_msg{
	__uint16  net_tp;
	__uint16  proto_tp;
	__uint8 macaddr_len;
	__uint8 ipaddr_len;
	__uint16  op;
	__uint8 macaddr_src[6];
	__uint8 ipaddr_src[4];
	__uint8 macaddr_dst[6];
	__uint8 ipaddr_dst[4];
} arp_msg_ptr;
//--------------------------------------------------
typedef struct ip_pkt{
	__uint8 verlen;
	__uint8 ts;
	__uint16 len;
	__uint16 id;
	__uint16 fl_frg_of;
	__uint8 ttl;
	__uint8 prt;
	__uint16 cs;
	__uint8 ipaddr_src[4];
	__uint8 ipaddr_dst[4];
	__uint8 data[];
} __S_Ip_Pkt;
//--------------------------------------------------
typedef struct icmp_pkt{
	__uint8 msg_tp;
	__uint8 msg_cd;
	__uint16 cs;
	__uint16 id;
	__uint16 num;
	__uint8 data[];
} icmp_pkt_ptr;
//--------------------------------------------------
 typedef struct USART_prop{
  __uint8 usart_buf[25];
  __uint8 usart_cnt;
  __uint8 is_ip;
} USART_prop_ptr;
//-------------------------------------------------- 
#define be16toword(a)	((((a)>>8)&0xff)|(((a)<<8)&0xff00))
#define be32todword(a) ((((a)>>24)&0xff)|(((a)>>8)&0xff00)|(((a)<<8)&0xff0000)|(((a)<<24)&0xff000000))
//--------------------------------------------------
#define ETH_ARP	be16toword(0x0806)
#define ETH_IP	be16toword(0x0800)
//--------------------------------------------------
#define ARP_ETH	be16toword(0x0001)
#define ARP_IP	be16toword(0x0800)
#define ARP_REQUEST	be16toword(1)
#define ARP_REPLY	be16toword(2)
//--------------------------------------------------
#define IP_ICMP	1
#define IP_TCP	6
#define IP_UDP	17
//--------------------------------------------------
#define ICMP_REQ	8
#define ICMP_REPLY	0
//--------------------------------------------------

void net_ini(void);
void net_poll(void);
void eth_send(__S_Enc28j60_Frame_Pkt *frame, __uint16 type, __uint16 len);

void UART1_RxCpltCallback(void);

void TIM_PeriodElapsedCallback(void);

__uint8 ip_send(__S_Enc28j60_Frame_Pkt *frame, __uint16 len);
__uint16 checksum(__uint8 *ptr, __int16 len, __uint8 type);
void net_cmd(void);
//--------------------------------------------------
#include "arp.h"
#include "udp.h"
#include "ntp.h"
#include "tcp.h"
//--------------------------------------------------
#endif /* __NET_H */

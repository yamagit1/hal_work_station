#include "ntp.h"
//--------------------------------------------------
extern UART_HandleTypeDef huart1;
//-----------------------------------------------
extern char str1[60];
extern uint8_t net_buf[ENC28J60_MAXFRAME];
extern uint8_t macaddr[6];
extern uint8_t ipaddr[4];
ntp_prop_ptr ntpprop;
//--------------------------------------------------
uint8_t ntp_read(__S_Enc28j60_Frame_Pkt *frame, uint16_t len)
{
  uint8_t res=0;
	struct tm *timestruct;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	udp_pkt_ptr *udp_pkt = (void*)(ip_pkt->data);
	ntp_pkt_ptr *ntp_pkt = (void*)(udp_pkt->data);	
	sprintf(str1,"%lu\r\n", be32todword((unsigned long)ntp_pkt->tx_ts.sec));
	HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
	time_t rawtime = (time_t)(be32todword(ntp_pkt->tx_ts.sec)-2208988800+60UL*60*TIMEZONE);
	timestruct = localtime(&rawtime);
	sprintf(str1,"%02d.%02d.%04u %02d:%02d:%02d -%d-\r\n", timestruct->tm_mday,timestruct->tm_mon+1, //������ ��������� �� 0, � �� �� 1
		timestruct->tm_year+1900,timestruct->tm_hour,timestruct->tm_min,timestruct->tm_sec,timestruct->tm_wday);
	HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
  //������� ��� ����� � ��������
  ntpprop.set=0;
  ntpprop.ntp_cnt=0;
  ntpprop.ntp_timer=0;	
  return res;
}
//--------------------------------------------------
uint8_t ntp_request(uint8_t *ip_addr, uint16_t port)
{
  uint8_t res=0;
	uint16_t len;
	__S_Enc28j60_Frame_Pkt *frame=(void*) net_buf;
	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	udp_pkt_ptr *udp_pkt = (void*)(ip_pkt->data);
	ntp_pkt_ptr *ntp_pkt = (void*)(udp_pkt->data);
	//�������� ������ ��� ��������� ntp
	memset(ntp_pkt, 0, sizeof(ntp_pkt_ptr));
	//�������� ��������� NTP
	ntp_pkt->flags = 0x1b;
	//�������� ��������� UDP
	udp_pkt->port_dst = be16toword(port);
	udp_pkt->port_src = be16toword(LOCAL_PORT_FOR_NTP);
	len = sizeof(ntp_pkt_ptr) + sizeof(udp_pkt_ptr);
	udp_pkt->len = be16toword(len);
	udp_pkt->cs=0;
	udp_pkt->cs=checksum((uint8_t*)udp_pkt-8, len+8, 1);
	//�������� ��������� ������ IP
	len+=sizeof(__S_Ip_Pkt);
	ip_pkt->len=be16toword(len);
	ip_pkt->id = 0;
	ip_pkt->ts = 0;
	ip_pkt->verlen = 0x45;
	ip_pkt->fl_frg_of=0;
	ip_pkt->ttl=128;
	ip_pkt->cs = 0;
	ip_pkt->prt=IP_UDP;
	memcpy(ip_pkt->ipaddr_dst,ip_addr,4);
	memcpy(ip_pkt->ipaddr_src,ipaddr,4);
	ip_pkt->cs = checksum((void*)ip_pkt,sizeof(__S_Ip_Pkt),0);
	//�������� ����� Ethernet
	memcpy(frame->addr_dest,ntpprop.macaddr_dst,6);
	eth_send(frame,ETH_IP,len);
  return res;
}
//--------------------------------------------------

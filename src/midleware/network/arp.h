#ifndef ARP_H_
#define ARP_H_
//--------------------------------------------------
#include "header.h"
#include "enc28j60.h"
#include "net.h"
//--------------------------------------------------
uint8_t arp_read(__S_Enc28j60_Frame_Pkt *frame, uint16_t len);
void arp_send(__S_Enc28j60_Frame_Pkt *frame);
uint8_t arp_request(uint8_t *ip_addr);
void arp_table_fill(__S_Enc28j60_Frame_Pkt *frame);
//--------------------------------------------------
typedef struct arp_record{
  uint8_t ipaddr[4];
  uint8_t macaddr[6];
  uint32_t sec; //����� ���� ���������� ������ � ���������� clock_cnt, ����� ���� ������� ������
} arp_record_ptr;
//-------------------------------------------------- 
#endif /* ARP_H_ */

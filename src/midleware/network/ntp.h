#ifndef NTP_H_
#define NTP_H_
//--------------------------------------------------
#include "header.h"
#include "enc28j60.h"
#include "net.h"
#include <time.h>
//--------------------------------------------------
#define LOCAL_PORT_FOR_NTP 14444
#define TIMEZONE 3
//--------------------------------------------------
typedef struct ntp_ts {
  uint32_t sec;//����� �����
  uint32_t frac;//������� �����
} ntp_ts_ptr;
//--------------------------------------------------
typedef struct ntp_pkt {
  uint8_t flags; //�����
  uint8_t peer_clock_stratum;//������
  uint8_t peer_pooling_interval;//�������� ������
  uint8_t peer_clock_precision;//��������
  uint32_t root_delay;//��������
  uint32_t root_dispersion;//���������
  uint32_t ref_id;//������������� ���������
  ntp_ts_ptr ref_ts;//����� ����������
  ntp_ts_ptr orig_ts;//��������� �����
  ntp_ts_ptr rcv_ts;//����� �����
  ntp_ts_ptr tx_ts;//����� ��������
} ntp_pkt_ptr;
//--------------------------------------------------
typedef struct ntp_prop{
  uint8_t ntp_cnt; //���������� ���������� ������� �������� �����
  int32_t ntp_timer; //������ ��� ��������� �������
  uint8_t set; //���� ��������� �������
  uint8_t macaddr_dst[6];
  uint8_t ip_dst[4];
  uint16_t port_dst;//���� ����������
} ntp_prop_ptr;
//----------------------------------------------
uint8_t ntp_request(uint8_t *ip_addr, uint16_t port);
uint8_t ntp_read(__S_Enc28j60_Frame_Pkt *frame, uint16_t len);
//--------------------------------------------------
#endif /* NTP_H_ */

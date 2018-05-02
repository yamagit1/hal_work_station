#ifndef TCP_H_
#define TCP_H_
//--------------------------------------------------
#include "header.h"
#include "enc28j60.h"
#include "net.h"
#include "fatfs.h"
//--------------------------------------------------
#define LOCAL_PORT_TCP 80
//--------------------------------------------------
typedef struct tcp_pkt {
	uint16_t port_src;		//the sender's port
	uint16_t port_dst;		//the destination port
	uint32_t bt_num_seg;	//the serial number of the byte in the data stream (the pointer to the first byte in the data segment)
	uint32_t num_ask;		//the confirmation number (the first byte in the segment + the number of bytes in the segment + 1 or the number of the next expected byte)
	uint8_t len_hdr;		//header length
	uint8_t fl;				//TCP flags
	uint16_t size_wnd;		//the size of the window
	uint16_t cs;			//header checksum
	uint16_t urg_ptr;		//pointer to urgent data
	uint8_t data[];			//data
} __S_Tcp_Pkt;


#define TCP_CWR 0x80
#define TCP_ECE 0x40
#define TCP_URG 0x20
#define TCP_ACK 0x10
#define TCP_PSH 0x08
#define TCP_RST 0x04
#define TCP_SYN 0x02
#define TCP_FIN 0x01
//--------------------------------------------------

#define TCP_CONNECTED 1
#define TCP_DISCONNECTED 2
#define TCP_DISCONNECTING 3
//--------------------------------------------------

#define DATA_COMPLETED 0
#define DATA_ONE 1
#define DATA_FIRST 2
#define DATA_MIDDLE 3
#define DATA_LAST 4
#define DATA_END 5
//--------------------------------------------------

#define EXISTING_HTML 0
#define E404_HTML 1
#define EXISTING_JPG 2
//--------------------------------------------------
uint8_t tcp_read(__S_Enc28j60_Frame_Pkt *frame, uint16_t len);
//--------------------------------------------------
#endif /* TCP_H_ */

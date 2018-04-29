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
//--------------------------------------------------
typedef struct tcp_prop {
	uint8_t macaddr_dst[6];					//MAC address Destination
	uint8_t ipaddr_dst[6];					//IP- address Destination
	volatile uint16_t port_dst;				// port of the recipient
	volatile uint32_t seq_num;  			// serial number of the byte
	volatile uint32_t seq_num_tmp; 			// the serial number of the byte is temporary
	volatile uint32_t ack_num; 				// confirmation number
	volatile uint32_t data_stat;			// data transfer status
	volatile uint32_t data_size; 			// data size to send
	volatile uint16_t last_data_part_size;	// the size of the last part of the data to send
	volatile uint16_t cnt_data_part;		// total number of pieces of data to send
	volatile uint16_t cnt_rem_data_part;	// the number of remaining parts of the data to send
	volatile uint16_t cnt_size_wnd; 		// the number of bytes transferred from the window
	volatile uint8_t http_doc;				// the version of the document to send
	char fname[20];							//the name of the file (document)
} tcp_prop_ptr;
//-------------------------------------------------- 

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

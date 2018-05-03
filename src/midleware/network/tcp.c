#include "tcp.h"
#include "console_serial_trace.h"
#include "httpd.h"
#include "perform_manage.h"
#include "fptserver.h"

__uint8 tcp_read(__S_Enc28j60_Frame_Pkt *frame, __uint16 len)
{
	__uint8 res = 0;
	__uint16 len_data = 0;
	__uint16 i = 0;

	__uint32 lenEnc28j60Frame = 0;

	char *ss1;
	int ch1=' ';
	int ch2='.';

	__S_Ip_Pkt *ip_pkt = (void*)(frame->data);
	__S_Tcp_Pkt *tcp_pkt = (void*)(ip_pkt->data);

	if (be16toword(tcp_pkt->port_dst) == HTTPD_TCP_PORT)
	{
		//===============================================================
		// httpd server
		//===============================================================

		lenEnc28j60Frame = len + sizeof(__S_Ip_Pkt) + sizeof(__S_Enc28j60_Frame_Pkt);

		// wait for httpd buff ready to access
		if (osSemaphoreWait(httpBuffSemaphoreID, TIME_WAIT_SHORT) == osOK )
		{
			// copy packet receiver to httpd
			memcpy(gHttpFrame, frame, lenEnc28j60Frame);
			console_serial_print_log("\t> copy packet receiver to httpd size : %d ", lenEnc28j60Frame);

			// Release httpd buff
			osSemaphoreRelease(httpBuffSemaphoreID);
		}
		else
		{
			console_serial_print_error("\t> tcp_readcopy wait for httpd buff ready to access fail");

			return 0;
		}

		osSignalSet(gListPID[INDEX_HTTPD_SERVER], SIG_HTTPD_REV);
		console_serial_print_log("\t> Packet send to HTTPD : send signal to HTTPD");

		return 0;
		//===============================================================
	}
	else if (be16toword(tcp_pkt->port_dst) == FPTD_TCP_PORT)
	{
		//===============================================================
		//	FPT server
		//===============================================================
		console_serial_print_log("\t> Packet send to FPT : %d ----->%d" , be16toword(tcp_pkt->port_src), be16toword(tcp_pkt->port_dst));

		lenEnc28j60Frame = len + sizeof(__S_Ip_Pkt) + sizeof(__S_Enc28j60_Frame_Pkt);

		if (osSemaphoreWait(fptBuffSemaphoreID, TIME_WAIT_MEDIUM) == osOK )
		{
			memcpy(gFptFrame, frame, lenEnc28j60Frame);

			// Release httpd buff
			osSemaphoreRelease(fptBuffSemaphoreID);
		}
		else
		{
			console_serial_print_error("\t> tcp_readcopy wait for ftpd buff ready to access fail");
			return 0;

		}

		osSignalSet(gListPID[INDEX_FPT_SERVER], SIG_FPTD_REV);
		console_serial_print_log("\t> Packet send to FPTD : send signal to FPTD");

		return 0;
		//===============================================================
	}
	else
	{
		//===============================================================
		//	NONE server
		//===============================================================
		console_serial_print_log("\t> Packet send to NONE : %d ----->%d" , be16toword(tcp_pkt->port_src), be16toword(tcp_pkt->port_dst));
		//===============================================================
	}
	//===============================================================

	return res;
}
//--------------------------------------------------

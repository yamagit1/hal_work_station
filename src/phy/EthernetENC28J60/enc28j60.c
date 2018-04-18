#include "enc28j60.h"
#include "enc28j60_spi.h"
#include "console_serial_trace.h"

volatile __uint8 enc28j60_current_bank = 0;
volatile __uint16 enc28j60_rxrdpt = 0;

// Generic SPI read command
__uint8 enc28j60_read_op(__uint8 optionCmd, __uint8 addr)
{
	__uint8 data;

	enc28j60_Enable();

	enc28j60_Spi_Send(optionCmd | (addr & ENC28J60_ADDR_MASK));

	// throw out dummy byte
	// when reading MII/MAC register
	if (addr & 0x80)
	{
		enc28j60_Spi_Receive();
	}

	data = enc28j60_Spi_Receive();

	enc28j60_Disable();

	return data;
}


// Generic SPI write command
void enc28j60_write_op(__uint8 optionCmd, __uint8 addr, __uint8 data)
{
	enc28j60_Enable();

	enc28j60_Spi_Send(optionCmd | (addr & ENC28J60_ADDR_MASK));
	enc28j60_Spi_Send(data);

	enc28j60_Disable();
}


// Initiate software reset
void enc28j60_soft_reset()
{
	__ENTER__

	volatile __uint32 i;

	enc28j60_Enable();

	enc28j60_Spi_Send(ENC28J60_SPI_SWR);

	enc28j60_Disable();

	enc28j60_current_bank = 0;

	// Wait until device initializes
	for (i = 0; i < 20000; i++);
	//_delay_ms(1);

	__LEAVE__
}


// Set register bank
void enc28j60_set_bank(__uint8 addr)
{
	__uint8 bankReg;

	if ((addr & ENC28J60_ADDR_MASK) < ENC28J60_COMMON_CR )
	{
		bankReg = (addr >> 5) & 0x03; //BSEL1|BSEL0=0x03

		if (bankReg != enc28j60_current_bank)
		{
			enc28j60_write_op(ENC28J60_SPI_BFC, ECON1, 0x03);
			enc28j60_write_op(ENC28J60_SPI_BFS, ECON1, bankReg);

			enc28j60_current_bank = bankReg;
		}
	}
}

// Read register
__uint8 enc28j60_rcr(__uint8 addr)
{
	enc28j60_set_bank(addr);

	return enc28j60_read_op(ENC28J60_SPI_RCR, addr);
}

// Read register pair
__uint16 enc28j60_rcr16(__uint8 addr)
{
	enc28j60_set_bank(addr);

	return enc28j60_read_op(ENC28J60_SPI_RCR, addr) | (enc28j60_read_op(ENC28J60_SPI_RCR, addr+1) << 8);
}

// Write register
void enc28j60_wcr(__uint8 addr, __uint8 arg)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_WCR, addr, arg);
}

// Write register pair
void enc28j60_wcr16(__uint8 addr, __uint16 arg)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_WCR, addr, arg);
	enc28j60_write_op(ENC28J60_SPI_WCR, addr + 1, arg >> 8);
}

// Clear bits in register (reg &= ~mask)
void enc28j60_bfc(__uint8 addr, __uint8 mask)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_BFC, addr, mask);
}

// Set bits in register (reg |= mask)
void enc28j60_bfs(__uint8 addr, __uint8 mask)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_BFS, addr, mask);
}

// Read Rx/Tx buffer (at ERDPT)
void enc28j60_read_buffer(__uint8 *buf, __uint16 len)
{
	enc28j60_Enable();

	enc28j60_Spi_Send(ENC28J60_SPI_RBM);

	while (len--)
	{
		*(buf++) = enc28j60_Spi_Receive();
	}

	enc28j60_Disable();
}

// Write Rx/Tx buffer (at EWRPT)
void enc28j60_write_buffer(__uint8 *buf, __uint16 len)
{
	enc28j60_Enable();

	enc28j60_Spi_Send(ENC28J60_SPI_WBM);

	while (len--)
	{
		enc28j60_Spi_Send(*(buf++));
	}

	enc28j60_Disable();
}

// Read PHY register
__uint16 enc28j60_read_phy(__uint8 addr)
{
	enc28j60_wcr(MIREGADR, addr);
	enc28j60_bfs(MICMD, MICMD_MIIRD);

	while(enc28j60_rcr(MISTAT) & MISTAT_BUSY);

	enc28j60_bfc(MICMD, MICMD_MIIRD);

	return enc28j60_rcr16(MIRD);
}

// Write PHY register
void enc28j60_write_phy(__uint8 adr, __uint16 data)
{
	enc28j60_wcr(MIREGADR, adr);
	enc28j60_wcr16(MIWR, data);

	while(enc28j60_rcr(MISTAT) & MISTAT_BUSY);
}


/*
 * Init & packet Rx/Tx
 */

void enc28j60_init(__uint8 *macadr)
{
	__ENTER__

	__uint32 i;
	console_serial_print_log("Initialize module SPI2 ");
	ENC28J60_SPI_Init();

	enc28j60_Disable();

	console_serial_print_log("Wait module SPI2  reset");
	for (i = 0; i < 720000; i++)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

	// Reset ENC28J60
	console_serial_print_log("Reset ENC28J60");
	enc28j60_soft_reset();

	// Setup Rx/Tx buffer
	console_serial_print_log("Setup Rx/Tx buffer");
	enc28j60_wcr16(ERXST, ENC28J60_RXSTART);
	enc28j60_rcr16(ERXST);
	enc28j60_wcr16(ERXRDPT, ENC28J60_RXSTART);
	enc28j60_wcr16(ERXND, ENC28J60_RXEND);
	enc28j60_rxrdpt = ENC28J60_RXSTART;

	// Setup MAC
	console_serial_print_log("Setup MAC");
	console_serial_print_log("MAC address : %d", macadr);
	enc28j60_wcr(MACON1, MACON1_TXPAUS| // Enable flow control
			MACON1_RXPAUS|MACON1_MARXEN); // Enable MAC Rx
	enc28j60_wcr(MACON2, 0); // Clear reset
	enc28j60_wcr(MACON3, MACON3_PADCFG0| // Enable padding,
			MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX); // Enable crc & frame len chk
	enc28j60_wcr16(MAMXFL, ENC28J60_MAXFRAME);
	enc28j60_wcr(MABBIPG, 0x15); // Set inter-frame gap
	enc28j60_wcr(MAIPGL, 0x12);
	enc28j60_wcr(MAIPGH, 0x0c);
	enc28j60_wcr(MAADR5, macadr[0]); // Set MAC address
	enc28j60_wcr(MAADR4, macadr[1]);
	enc28j60_wcr(MAADR3, macadr[2]);
	enc28j60_wcr(MAADR2, macadr[3]);
	enc28j60_wcr(MAADR1, macadr[4]);
	enc28j60_wcr(MAADR0, macadr[5]);

	// Setup PHY
	console_serial_print_log("Setup PHY");
	enc28j60_write_phy(PHCON1, PHCON1_PDPXMD); // Force full-duplex mode
	enc28j60_write_phy(PHCON2, PHCON2_HDLDIS); // Disable loopback
	enc28j60_write_phy(PHLCON, PHLCON_LACFG2| // Configure LED ctrl
			PHLCON_LBCFG2|PHLCON_LBCFG1|PHLCON_LBCFG0|
			PHLCON_LFRQ0|PHLCON_STRCH);

	// Enable Rx packets
	console_serial_print_log("Enable Rx packets");
	enc28j60_bfs(ECON1, ECON1_RXEN);

	__LEAVE__
}

void enc28j60_packetSend(__uint8 *data, __uint16 len)
{
	while(enc28j60_rcr(ECON1) & ECON1_TXRTS)
	{
		// TXRTS may not clear - ENC28J60 bug. We must reset
		// transmit logic in cause of Tx error
		if(enc28j60_rcr(EIR) & EIR_TXERIF)
		{
			enc28j60_bfs(ECON1, ECON1_TXRST);
			enc28j60_bfc(ECON1, ECON1_TXRST);
		}
	}

	enc28j60_wcr16(EWRPT, ENC28J60_TXSTART);
	enc28j60_write_buffer((__uint8*)"\x00", 1);
	enc28j60_write_buffer(data, len);

	enc28j60_wcr16(ETXST, ENC28J60_TXSTART);
	enc28j60_wcr16(ETXND, ENC28J60_TXSTART + len);

	enc28j60_bfs(ECON1, ECON1_TXRTS); // Request packet send
}

__uint16 enc28j60_packetReceive(__uint8 *buf, __uint16 buflen)
{
	__uint16 len = 0, rxlen, status, temp;

	if (enc28j60_rcr(EPKTCNT))
	{
		enc28j60_wcr16(ERDPT, enc28j60_rxrdpt);

		enc28j60_read_buffer((void*)&enc28j60_rxrdpt, sizeof(enc28j60_rxrdpt));
		enc28j60_read_buffer((void*)&rxlen, sizeof(rxlen));
		enc28j60_read_buffer((void*)&status, sizeof(status));

		if (status & 0x80) //success
		{
			len = rxlen - 4; //throw out crc

			if(len > buflen)
			{
				len = buflen;
			}

			enc28j60_read_buffer(buf, len);	
		}

		// Set Rx read pointer to next packet
		temp = (enc28j60_rxrdpt - 1) & ENC28J60_BUFEND;
		enc28j60_wcr16(ERXRDPT, temp);

		// Decrement packet counter
		enc28j60_bfs(ECON2, ECON2_PKTDEC);
	}

	return len;
}

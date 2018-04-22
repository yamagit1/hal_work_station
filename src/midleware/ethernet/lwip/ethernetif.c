/**
 ******************************************************************************
 * File Name          : ethernetif.c
 * Description        : This file provides code for the configuration
 *                      of the ethernetif.c MiddleWare.
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2018 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"

#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>
#include "cmsis_os.h"

#include "console_serial_trace.h"
#include "enc28j60.h"

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */
__uint8 net_buffer[ENC28J60_MAXFRAME];

/* USER CODE END 0 */

/* Private define ------------------------------------------------------------*/

/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT ( portMAX_DELAY )

/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE ( 350 )

/* Network interface name */
#define IFNAME0 'w'
#define IFNAME1 's'

__uint8 macaddr[]={0x01,0x02,0x03,0x04,0x05,0x06};


/* Private variables ---------------------------------------------------------*/
//__ALIGN_BEGIN ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END;/* Ethernet Rx MA Descriptor */
//__ALIGN_BEGIN ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END;/* Ethernet Tx DMA Descriptor */

// Ethernet Receive Buffer
__ALIGN_BEGIN uint8_t gRx_Buff[ENC28J60_MAXFRAME] __ALIGN_END;

// Ethernet Transmit Buffer
__ALIGN_BEGIN uint8_t Tx_Buff[4][ENC28J60_MAXFRAME] __ALIGN_END;

// Semaphore to signal incoming packets
osSemaphoreId s_xSemaphore = NULL;


/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ENC28J60)
 *******************************************************************************/

// Forward declarations
//static void  ethernetif_input(struct netif *netif);

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */

//static
void low_level_init(struct netif *netif)
{ 
	__ENTER__

	uint32_t regvalue = 0;
	HAL_StatusTypeDef hal_eth_init_status;


	/* Init ETH */
	console_serial_print_log("\t> Initialize enc28j60");
	enc28j60_init(macaddr);

	// test bink led enc28j60
	enc28j60_write_phy(PHLCON, 0x0BA0);

#if LWIP_ARP || LWIP_ETHERNET

	/* set MAC hardware address length */
	netif->hwaddr_len = ETH_HWADDR_LEN;

	/* set MAC hardware address */
	netif->hwaddr[0] =  macaddr[0];
	netif->hwaddr[1] =  macaddr[1];
	netif->hwaddr[2] =  macaddr[2];
	netif->hwaddr[3] =  macaddr[3];
	netif->hwaddr[4] =  macaddr[4];
	netif->hwaddr[5] =  macaddr[5];

	/* maximum transfer unit */
	netif->mtu = ENC28J60_MAXFRAME;

	/* Accept broadcast address and ARP traffic */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
#if LWIP_ARP
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
	netif->flags |= NETIF_FLAG_BROADCAST;
#endif /* LWIP_ARP */

	/* create a binary semaphore used for informing ethernetif of frame reception */
	console_serial_print_log("\t> TODO____Create a binary semaphore used for informing ethernetif of frame reception");
	osSemaphoreDef(SEM);
	s_xSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1 );
	osSemaphoreRelease (s_xSemaphore);

	/* create the task that handles the ETH_MAC */
	console_serial_print_log("\t> TODO____create the task that handles the ETH_MAC");
	osThreadDef(EthIf, ethernetif_input, 1, 0, INTERFACE_THREAD_STACK_SIZE);
	osThreadCreate (osThread(EthIf), netif);

#endif /* LWIP_ARP || LWIP_ETHERNET */

	__LEAVE__
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{

	__ENTER__

	err_t errval;
	__uint16 sendCount = 0;
	struct pbuf *qHead;

	if (p != NULL)
	{
		for(qHead = p; qHead != NULL; qHead = qHead->next)
		{
			console_serial_print_infor("low_level_output send packet length : %s ", qHead->len);
			enc28j60_packetSend(qHead->payload, qHead->len);
		}
	}
	else
	{
		console_serial_print_infor("low_level_output send packet NULL");

		//		errval = ERR_BUF;
	}

	errval = ERR_OK;

	__LEAVE__

	return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
//static
struct pbuf * low_level_input(struct netif *netif)
{
	struct pbuf *pbufRev = NULL;
	__uint16 revLen = 0;

	if ((revLen = enc28j60_packetReceive(gRx_Buff, sizeof(gRx_Buff))) > 0)
	{
		console_serial_print_infor("\t\t> low_level_input receive packet length : %d", revLen);
		/* We allocate a pbuf chain of pbufs from the pool. */
		pbufRev = pbuf_alloc(PBUF_RAW, revLen, PBUF_POOL);

		if (pbufRev != NULL)
		{
			console_serial_print_log("\t\t> low_level_input  allocate a pbuf chain of pbufs from the pool :");
			console_serial_print_infor(" \t\t\t\t> Current : %d, total %d", pbufRev->len, pbufRev->tot_len);
			pbufRev->len = (u16_t)revLen;
			pbufRev->tot_len = (u16_t)revLen;
			/* Copy data to pbuf */
			memcpy( (uint8_t*)pbufRev->payload, (uint8_t*) gRx_Buff, revLen);
		}

	}
	else
	{
		return NULL;
	}

	return pbufRev;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input( void const * argument ) 
{
	__ENTER__

	int currentLop = 0;
	struct netif *pnetif = (struct netif *) argument;
	struct pbuf *pbufRev = NULL;
	struct eth_hdr *ethhdr;

	for( ;; )
	{

		if (osSemaphoreWait( s_xSemaphore, TIME_WAITING_FOR_INPUT) == osOK)
		{
			console_serial_print_log("\t> net poll input.... %d", currentLop++);

			do
			{
				pbufRev = low_level_input( pnetif );

				if (pbufRev != NULL)
				{
					console_serial_print_infor("\t> ethernetif_input receive packet.... %d", pbufRev->len);

					ethhdr = pbufRev->payload;

					switch (htons(ethhdr->type))
					{
					case ETHTYPE_IP:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_IP ");
						break;

					case ETHTYPE_ARP:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_ARP");
						break;

					case ETHTYPE_WOL:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_WOL");
						break;

					case ETHTYPE_RARP:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_RARP");
						break;

					case ETHTYPE_VLAN:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_VLAN");
						break;

					case ETHTYPE_IPV6:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_IPV6");
						break;

					case ETHTYPE_PPPOEDISC:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_PPPOEDISC");
						break;

					case ETHTYPE_PPPOE:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_PPPOE");
						break;

					case ETHTYPE_JUMBO:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_JUMBO");
						break;

					case ETHTYPE_PROFINET:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_PROFINET");
						break;

					case ETHTYPE_ETHERCAT:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_ETHERCAT");
						break;

					case ETHTYPE_LLDP:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_LLDP");
						break;

					case ETHTYPE_SERCOS:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_SERCOS");
						break;

					case ETHTYPE_MRP:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_MRP");
						break;

					case ETHTYPE_PTP:
						console_serial_print_infor("\t\t\t> Receive packet type : ETHTYPE_PTP");
						break;

					default:
						console_serial_print_infor("\t\t\t> Receive packet type : NONE");
						break;
					}

					if (pnetif->input(pbufRev, pnetif) != ERR_OK )
					{
						console_serial_print_infor("\t\t> Cannot decode packet------------");
						pbuf_free(pbufRev);
					}
					else
					{
						console_serial_print_infor("\t\t> Decode packet successfully ------------");
					}

					//					pbuf_free(pbufRev);
				}

			} while(pbufRev!=NULL);

			while (osSemaphoreRelease (s_xSemaphore) == osOK);
		}
		else
		{
			console_serial_print_log("\t> Cannot handle device.... %d", currentLop++);
		}

		osDelay(100);
	}

	__LEAVE__
}

#if !LWIP_ARP
/**
 * This function has to be completed by user in case of ARP OFF.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if ...
 */
static err_t low_level_output_arp_off(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{  
	err_t errval;
	errval = ERR_OK;

	/* USER CODE BEGIN 5 */

	/* USER CODE END 5 */

	return errval;

}
#endif /* LWIP_ARP */ 

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
	__ENTER__

	LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	console_serial_print_log("\t>Name network interface : %c%c", IFNAME0, IFNAME1);
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP

	console_serial_print_log("\t>Setup output function for lwip");
	netif->output = etharp_output;
#else
	/* The user should write ist own code in low_level_output_arp_off function */
	netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */
	//
#if LWIP_IPV6
	netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

	console_serial_print_log("\t>Setup linkoutput function for lwip");
	netif->linkoutput = low_level_output;

	/* initialize the hardware */
	console_serial_print_log("\t>Initialize the hardware network");
	low_level_init(netif);

	__LEAVE__

	return ERR_OK;
}

/* USER CODE BEGIN 6 */

/**
 * @brief  Returns the current time in milliseconds
 *         when LWIP_TIMERS == 1 and NO_SYS == 1
 * @param  None
 * @retval Time
 */
u32_t sys_jiffies(void)
{
	return HAL_GetTick();
}

/**
 * @brief  Returns the current time in milliseconds
 *         when LWIP_TIMERS == 1 and NO_SYS == 1
 * @param  None
 * @retval Time
 */
u32_t sys_now(void)
{
	return HAL_GetTick();
}

void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)
{
	//===============================================
	//  GPIO_InitTypeDef GPIO_InitStruct;
	//  if(ethHandle->Instance==ETH)
	//  {
	//  /* USER CODE BEGIN ETH_MspInit 0 */
	//
	//  /* USER CODE END ETH_MspInit 0 */
	//    /* Enable Peripheral clock */
	//    __HAL_RCC_ETH_CLK_ENABLE();
	//
	//    /**ETH GPIO Configuration
	//    PE2     ------> ETH_TXD3
	//    PC1     ------> ETH_MDC
	//    PC2     ------> ETH_TXD2
	//    PC3     ------> ETH_TX_CLK
	//    PA1     ------> ETH_RX_CLK
	//    PA2     ------> ETH_MDIO
	//    PA7     ------> ETH_RX_DV
	//    PC4     ------> ETH_RXD0
	//    PC5     ------> ETH_RXD1
	//    PB0     ------> ETH_RXD2
	//    PB1     ------> ETH_RXD3
	//    PB11     ------> ETH_TX_EN
	//    PB12     ------> ETH_TXD0
	//    PB13     ------> ETH_TXD1
	//    */
	//    GPIO_InitStruct.Pin = GPIO_PIN_2;
	//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//    GPIO_InitStruct.Pull = GPIO_NOPULL;
	//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	//    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//
	//    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
	//                          |GPIO_PIN_5;
	//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//    GPIO_InitStruct.Pull = GPIO_NOPULL;
	//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	//
	//    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
	//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//    GPIO_InitStruct.Pull = GPIO_NOPULL;
	//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//
	//    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11|GPIO_PIN_12
	//                          |GPIO_PIN_13;
	//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	//    GPIO_InitStruct.Pull = GPIO_NOPULL;
	//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//
	//    /* Peripheral interrupt init */
	//    HAL_NVIC_SetPriority(ETH_IRQn, 5, 0);
	//    HAL_NVIC_EnableIRQ(ETH_IRQn);
	//    HAL_NVIC_SetPriority(ETH_WKUP_IRQn, 5, 0);
	//    HAL_NVIC_EnableIRQ(ETH_WKUP_IRQn);
	//  /* USER CODE BEGIN ETH_MspInit 1 */
	//
	//  /* USER CODE END ETH_MspInit 1 */
	//  }
}
void HAL_ETH_MspDeInit(ETH_HandleTypeDef* ethHandle)
{
	//  if(ethHandle->Instance==ETH)
	//  {
	//  /* USER CODE BEGIN ETH_MspDeInit 0 */
	//
	//  /* USER CODE END ETH_MspDeInit 0 */
	//    /* Peripheral clock disable */
	//    __HAL_RCC_ETH_CLK_DISABLE();
	//
	//    /**ETH GPIO Configuration
	//    PE2     ------> ETH_TXD3
	//    PC1     ------> ETH_MDC
	//    PC2     ------> ETH_TXD2
	//    PC3     ------> ETH_TX_CLK
	//    PA1     ------> ETH_RX_CLK
	//    PA2     ------> ETH_MDIO
	//    PA7     ------> ETH_RX_DV
	//    PC4     ------> ETH_RXD0
	//    PC5     ------> ETH_RXD1
	//    PB0     ------> ETH_RXD2
	//    PB1     ------> ETH_RXD3
	//    PB11     ------> ETH_TX_EN
	//    PB12     ------> ETH_TXD0
	//    PB13     ------> ETH_TXD1
	//    */
	//    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);
	//
	//    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
	//                          |GPIO_PIN_5);
	//
	//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7);
	//
	//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_11|GPIO_PIN_12
	//                          |GPIO_PIN_13);
	//
	//    /* Peripheral interrupt Deinit*/
	//    HAL_NVIC_DisableIRQ(ETH_IRQn);
	//
	//    HAL_NVIC_DisableIRQ(ETH_WKUP_IRQn);
	//
	//  /* USER CODE BEGIN ETH_MspDeInit 1 */
	//
	//  /* USER CODE END ETH_MspDeInit 1 */
	//  }
}

/**
 * @brief  Ethernet Rx Transfer completed callback
 * @param  heth: ETH handle
 * @retval None
 */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
	//  osSemaphoreRelease(s_xSemaphore);
}
/* USER CODE END 6 */

/* USER CODE BEGIN 7 */

/* USER CODE END 7 */

#if LWIP_NETIF_LINK_CALLBACK
/**
 * @brief  Link callback function, this function is called on change of link status
 *         to update low level driver configuration.
 * @param  netif: The network interface
 * @retval None
 */
void ethernetif_update_config(struct netif *netif)
{
	__IO uint32_t tickstart = 0;
	uint32_t regvalue = 0;

	if(netif_is_link_up(netif))
	{
		/* Restart the auto-negotiation */
		if(heth.Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE)
		{
			/* Enable Auto-Negotiation */
			HAL_ETH_WritePHYRegister(&heth, PHY_BCR, PHY_AUTONEGOTIATION);

			/* Get tick */
			tickstart = HAL_GetTick();

			/* Wait until the auto-negotiation will be completed */
			do
			{
				HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);

				/* Check for the Timeout ( 1s ) */
				if((HAL_GetTick() - tickstart ) > 1000)
				{
					/* In case of timeout */
					goto error;
				}
			} while (((regvalue & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

			/* Read the result of the auto-negotiation */
			HAL_ETH_ReadPHYRegister(&heth, PHY_SR, &regvalue);

			/* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
			if((regvalue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
			{
				/* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
				heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
			}
			else
			{
				/* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
				heth.Init.DuplexMode = ETH_MODE_HALFDUPLEX;
			}
			/* Configure the MAC with the speed fixed by the auto-negotiation process */
			if(regvalue & PHY_SPEED_STATUS)
			{
				/* Set Ethernet speed to 10M following the auto-negotiation */
				heth.Init.Speed = ETH_SPEED_10M;
			}
			else
			{
				/* Set Ethernet speed to 100M following the auto-negotiation */
				heth.Init.Speed = ETH_SPEED_100M;
			}
		}
		else /* AutoNegotiation Disable */
		{
			error :
			/* Check parameters */
			assert_param(IS_ETH_SPEED(heth.Init.Speed));
			assert_param(IS_ETH_DUPLEX_MODE(heth.Init.DuplexMode));

			/* Set MAC Speed and Duplex Mode to PHY */
			HAL_ETH_WritePHYRegister(&heth, PHY_BCR, ((uint16_t)(heth.Init.DuplexMode >> 3) |
					(uint16_t)(heth.Init.Speed >> 1)));
		}

		/* ETHERNET MAC Re-Configuration */
		HAL_ETH_ConfigMAC(&heth, (ETH_MACInitTypeDef *) NULL);

		/* Restart MAC interface */
		HAL_ETH_Start(&heth);
	}
	else
	{
		/* Stop MAC interface */
		HAL_ETH_Stop(&heth);
	}

	ethernetif_notify_conn_changed(netif);
}

/* USER CODE BEGIN 8 */
/**
 * @brief  This function notify user about link status changement.
 * @param  netif: the network interface
 * @retval None
 */
__weak void ethernetif_notify_conn_changed(struct netif *netif)
{
	/* NOTE : This is function could be implemented in user file
            when the callback is needed,
	 */

}
/* USER CODE END 8 */ 
#endif /* LWIP_NETIF_LINK_CALLBACK */

/* USER CODE BEGIN 9 */

/* USER CODE END 9 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


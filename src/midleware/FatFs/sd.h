#ifndef SD_H_
#define SD_H_
//--------------------------------------------------
#include "header.h"
#include "enc28j60_spi.h"

//--------------------------------------------------

#define LD_ON 					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); //RED
#define LD_OFF 					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); //RED

//--------------------------------------------------
/* Card type flags (CardType) */
#define CT_MMC 0x01 /* MMC ver 3 */
#define CT_SD1 0x02 /* SD ver 1 */
#define CT_SD2 0x04 /* SD ver 2 */
#define CT_SDC (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK 0x08 /* Block addressing */
//--------------------------------------------------
typedef struct sd_info {
  volatile uint8_t type;//��� �����
} sd_info_ptr;
//--------------------------------------------------
void SD_PowerOn(void);
uint8_t sd_ini(void);
void SPI_Release(void);
uint8_t SD_Read_Block (uint8_t *buff, uint32_t lba);
uint8_t SD_Write_Block (uint8_t *buff, uint32_t lba);
uint8_t SPI_wait_ready(void);
//--------------------------------------------------
#endif /* SD_H_ */

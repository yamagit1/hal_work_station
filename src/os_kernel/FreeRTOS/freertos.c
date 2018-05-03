/*==============================================================================
 *  Author  : NONE
 *  Modify	: YAMA															   *
 *  email   : yamahi95@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: user_diskio.c
 * in this file :
 *============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;



/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

//extern void MX_FATFS_Init(void);
//extern void MX_LWIP_Init(void);
//extern void MX_LIBJPEG_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */


void MX_FREERTOS_Init(void) {

  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for FATFS */
//  MX_FATFS_Init();

  /* init code for LWIP */
//  MX_LWIP_Init();

  /* init code for LIBJPEG */
//  MX_LIBJPEG_Init();

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*==============================================================================
 *  Author  : YAMA
 *  Modify	: YAMA															   *
 *  email   : yamateamhaui@gmail.com										   *
 *  address : Ha Noi University ( Nhon - Bac Tu liem - Ha Noi - Viet Nam)	   *
 *-----------------------------------------------------------------------------*
 * file name	: perform_manage.h
 * in this file :
 *============================================================================*/

#ifndef __PERFORM_MANAGE_H__
#define __PERFORM_MANAGE_H__

#include "header.h"
#include "cmsis_os.h"


#define MAX_SIZE_LIST_PID		20

typedef struct
{
	__uint32 miliSection;
	__uint32 section;
	__uint32 miuter;
	__uint32 hour;
}__S_TIME;

extern osThreadId gListPID[MAX_SIZE_LIST_PID];
extern __S_TIME gTimeCount;


void PM_updateTimeCount();
void PM_update_home_page_html();//void * argument)
void PM_task_update_html(void * argument);


#endif /* __PERFORM_MANAGE_H__ */

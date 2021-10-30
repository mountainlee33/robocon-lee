/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-04-04 21:03:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\SRC\main.c
 */
#include "main.h"

u32 Pressure;
int main()
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	LED_Configuration();
	CAN1_Configuration();
	USART1_MyInit();
	presensor_Init();	
	Init_HX711pin();

	OSInit(); //任务创建
	OSTaskCreate(TaskStart, (void *)0, &start_task_stk[START_TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();

	return 0;
}

/*起始任务*/ //基本的创建函数OSTaskCreate
static void TaskStart(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit();
	OS_ENTER_CRITICAL(); //进入临界区（无法被中断打断）
	RUN = OSSemCreate(0);
	Led_Show();
	OSTaskCreate(TaskPres, (void *)0, &pres_task_stk[PRES_TASK_STK_SIZE - 1], PRES_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO); //挂起起始任务
	OS_EXIT_CRITICAL();				//退出临界区
}

static void TaskPres(void *pdata)
{
	pdata = pdata;		
	presensor[1].enable = 1;
	
	Get_Peel();
	delay_ms(1000);
	delay_ms(1000);
	Get_Peel();

}




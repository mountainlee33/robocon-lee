#include "main.h"
/**
 * @brief: 主函数main
 * @author: Dandelion
 * @Date: 2021-08-23 10:48:20
 * @return {*}
 */
 bool control_open;
int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	CAN1_Mode_Init();
	CAN2_Mode_Init();
	TIM2_Init();
	TIM3_Init();
	TIM5_Init();
	USART2_Init();
	USART1_Init();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	param_Init();
	control_open = 0;
	OSInit();
	OSTaskCreate(Task_Start, (void *)0, &START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
	OSStart(); //这句之后不要写东西
}

//开始任务
static void Task_Start(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit(); //重要！！！不写没有任务调度
	UsartLCDshow();
	OS_ENTER_CRITICAL(); //程序进入临界段，无法被中断打断

#ifdef USE_DJ
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1], MOTOR_TASK_PRIO);
#endif
#ifdef USE_VESC
	OSTaskCreate(Task_VESC, (void *)0, (OS_STK *)&VESC_TASK_STK[VESC_STK_SIZE - 1], VESC_TASK_PRIO);
#endif
	OSTaskCreate(Task_Timeout, (void *)0, (OS_STK *)&TIMEOUT_TASK_STK[TIMEOUT_STK_SIZE - 1], TIMEOUT_TASK_PRIO);

	OSTaskCreate(Task_DataScope, (void *)0, (OS_STK *)&DataSCOPE_TASK_STK[DataSCOPE_STK_SIZE - 1], DataSCOPE_TASK_PRIO);

	OSTaskSuspend(START_TASK_PRIO); //挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

static void Task_Motor(void *pdata)
{
	Beep_Show(2);//上电叫兩声
	Led8_show(0);
	while (1)
	{
	Led_Show();
	OSTimeDly(200);
	}
}

static void Task_VESC(void *pdata)
{
	while(1)
	{
	  Can_DeQueue(CAN2, &VESC_Sendqueue);
		OSTimeDly	(16);
	}
}
static void Task_Timeout(void *pdata)
{
	while (1)
	{
		for(int i = 0;i<3;i++)
		{
			if(motor[i].status.timeout == 1)
			Led8_show(i+1);
			OSTimeDly(100);
			if(VESCmotor[i].status.timeout == 1)
			{
			Led8_show(i+4);
			OSTimeDly(100);
			}
			if(motor[0].status.zero == 1 && motor[1].status.zero == 1 && motor[2].status.zero == 1)
			{
			Beep_Show(8);
			motor[0].status.zero = 0;
			motor[1].status.zero = 0;
			motor[2].status.zero = 0;
			}
		}
	OSTimeDly(100);
	}
}

static void Task_DataScope(void *pdata)
{
	while (1)
	{
		VS4Channal_Send(VESCmotor[2].ValSet.speed, VESCmotor[3].ValSet.speed,VESCmotor[2].ValReal.speed,VESCmotor[3].ValReal.speed);
		OSTimeDly(100);
	}
}

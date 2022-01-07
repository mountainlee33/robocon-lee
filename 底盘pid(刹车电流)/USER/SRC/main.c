#include "main.h"
/**
 * @brief: ������main
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
	Usart1_Init();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	param_Init();
	control_open = 0;
	OSInit();
	OSTaskCreate(Task_Start, (void *)0, &START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
	OSStart(); //���֮��Ҫд����
}

//��ʼ����
static void Task_Start(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit(); //��Ҫ��������дû���������
	UsartLCDshow();
	OS_ENTER_CRITICAL(); //��������ٽ�Σ��޷����жϴ��

#ifdef USE_DJ
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1], MOTOR_TASK_PRIO);
#endif
#ifdef USE_VESC
	OSTaskCreate(Task_VESC, (void *)0, (OS_STK *)&VESC_TASK_STK[VESC_STK_SIZE - 1], VESC_TASK_PRIO);
#endif
	OSTaskCreate(Task_DataScope, (void *)0, (OS_STK *)&DataSCOPE_TASK_STK[DataSCOPE_STK_SIZE - 1], DataSCOPE_TASK_PRIO);

	OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

static void Task_Motor(void *pdata)
{
	Beep_Show(3);//�ϵ������
	Led8_show(0);
	while (1)
	{
	Led_Show();
	OSTimeDly(200);
	}
}
static void Task_VESC(void *pdata)
{
	while (1)
	{
		for(int i = 0;i<4;i++)
		{
		if(motor[i].status.timeout == 1)
		{
			Led8_show(i + 1);
		OSTimeDly(100); 
		}
		if(VESCmotor[i].status.timeout == 1)
		{
			Led8_show(i + 5);
		OSTimeDly(100); 
		}
	}
		OSTimeDly(100); //��ʱ̫������λ��ģʽʱ��һ��һ��
	}
}

static void Task_DataScope(void *pdata)
{
	while (1)
	{
		VS4Channal_Send(VESCmotor[0].mode, VESCmotor[1].mode,VESCmotor[2].mode,VESCmotor[3].mode);
		OSTimeDly(100);
	}
}

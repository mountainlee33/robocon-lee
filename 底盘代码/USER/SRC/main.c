#include "main.h"

/**
 * @brief: ������main
 * @author: Dandelion
 * @Date: 2021-08-23 10:48:20
 * @return {*}
 */
u8 ELMOid, VALVE_DATA;
int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	TIM2_Int_Init(11999, 6);
	TIM5_Init();
	CAN1_Mode_Init();
	CAN2_Mode_Init();
	Usart1_Init();
	USART2_Init();
	USART3_Init();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	param_Init();
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
	OS_ENTER_CRITICAL(); //��������ٽ�Σ��޷����жϴ��

#ifdef USE_DJ
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1], MOTOR_TASK_PRIO);
#endif
	OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

static void Task_Motor(void *pdata)
{
	while (1)
	{
		OSTimeDly(2000);
	}
}

static void Task_DataScope(void *pdata)
{
	while (1)
	{
		DataScope_Get_Channel_Data(motor[0].valueReal.velocity, 1);
		DataScope_Get_Channel_Data(motor[0].valueReal.current, 2);
		DataScope_Get_Channel_Data(motor[0].valueReal.angle, 3);
		DataScope_Get_Channel_Data(4.0, 4);		 //������   д��ͨ�� 4
		DataScope_Get_Channel_Data(5.0, 5);		 //������   д��ͨ�� 5
		DataScope_Get_Channel_Data(6.0, 6);		 //������   д��ͨ�� 6
		DataScope_Get_Channel_Data(7.0, 7);		 //������   д��ͨ�� 7
		DataScope_Get_Channel_Data(8.0, 8);		 //������   д��ͨ�� 8
		DataScope_Get_Channel_Data(9.0, 9);		 //������   д��ͨ�� 9
		DataScope_Get_Channel_Data(10, 10);		 //������   д��ͨ�� 10
		OSTimeDly(100);
	}
}


#include "main.h"
/**
 * @brief: ������main
 * @author: Dandelion
 * @Date: 2021-08-23 10:48:20
 * @return {*}
 */
u8 ELMOid, VALVE_DATA;
extern AK_Motor AKmotor[5];
int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	TIM2_Int_Init(11999, 6);
	TIM3_Init();
	TIM5_Init();
	CAN1_Mode_Init();
	CAN2_Mode_Init();
	USART2_Init();
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
	UsartLCDshow();
	OS_ENTER_CRITICAL(); //��������ٽ�Σ��޷����жϴ��

#ifdef USE_DJ
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1], MOTOR_TASK_PRIO);
#endif
#ifdef USE_VESC
	OSTaskCreate(Task_VESC, (void *)0, (OS_STK *)&VESC_TASK_STK[VESC_STK_SIZE - 1], VESC_TASK_PRIO);
	OSTaskCreate(Task_VESCSEND, (void *)0, (OS_STK *)&VESCSEND_TASK_STK[VESCSEND_STK_SIZE - 1], VESCSEND_TASK_PRIO);
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
static void Task_VESC(void *pdata)
{
	while (1)
	{
		Can_DeQueue(CAN2, &VESC_Sendqueue);
		OSTimeDly(2); //��ʱ̫������λ��ģʽʱ��һ��һ��
	}
}
static void Task_VESCSEND(void *pdata)
{
	while (1)
	{
		ifvescstuck(1);
		ifvescstuck(2);
		ifvescstuck(3);
		ifvescstuck(4);		
		Can_DeQueue(CAN2, &VESC_Sendqueue);
		OSTimeDly(1000); //��ʱ̫������λ��ģʽʱ��һ��һ��
	}
}


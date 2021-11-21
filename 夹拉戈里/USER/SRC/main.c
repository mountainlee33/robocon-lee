#include "main.h"
/**
 * @brief: ������main
 * @author: Dandelion
 * @Date: 2021-08-23 10:48:20
 * @return {*}
 */
 u8 VALVE_DATA;//1296
vs16 paw_pos[5];
 bool procedure_begin;
int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	TIM2_Int_Init(11999, 6);
	TIM5_Init();
	CAN1_Mode_Init();
	CAN2_Mode_Init();
	Usart1_Init();
	CYLINDE_Configuration();
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

	OSTaskCreate(PROCEDURE, (void *)0, (OS_STK *)&PROCEDURE_TASK_STK[PROCEDURE_STK_SIZE - 1], PROCEDURE_TASK_PRIO);
	OSTaskCreate(Task_Motor, (void *)0, (OS_STK *)&MOTOR_TASK_STK[MOTOR_STK_SIZE - 1], MOTOR_TASK_PRIO);

	OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

static void Task_Motor(void *pdata)
{
	while (1)
	{
		OSTimeDly(2000);
		VS4Channal_Send(motor[0].valueReal.velocity,0,0,0);
	}
}

static void PROCEDURE(void *pdata)
{
	procedure_begin = 0;
	while(1)
	{
		if(procedure_begin)
		{
		SetZero(&motor[0]);
		motor[0].limit.posSPlimit = 9000;
		motor[0].mode = 5;
		motor[0].valueSet.angle= -2300;
		motor[0].enable= 1;
		motor[0].begin= 1;
		CYLINDE_CONTROL(VALVE_DATA);//��
		OSTimeDly(100);
		CYLINDE_CONTROL(VALVE_DATA);//��
		procedure_begin = 0;
		}
		OSTimeDly(100);
	}
}

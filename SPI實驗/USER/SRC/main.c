#include "main.h"
#include "elmo.h"
/**
 * @brief: ������main
 * @author: Dandelion
 * @Date: 2021-08-23 10:48:20
 * @return {*}
 */
u8 ELMOid, VALVE_DATA;
extern AK_Motor AKmotor[5];
bool MCP_Receive;
int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	TIM2_Init();
	TIM5_Init();
	CAN1_Mode_Init();
	CAN2_Mode_Init();
	Usart1_Init();
	USART2_Init();
	USART3_Init();
	SPI2_Init();
	MCP2515_Init();
	CYLINDE_Configuration();
	Can_SendqueueInit();
	MCP_Receive = 0;
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
#ifdef USE_ELMO
	OSTaskCreate(Task_Elmo, (void *)0, (OS_STK *)&ELMO_TASK_STK[ELMO_STK_SIZE - 1], ELMO_TASK_PRIO);
#endif
	OSTaskCreate(Task_DataScope, (void *)0, (OS_STK *)&DataSCOPE_TASK_STK[DataSCOPE_STK_SIZE - 1], DataSCOPE_TASK_PRIO);
#ifdef USE_VALVE
	OSTaskCreate(Valve_Control, (void *)0, (OS_STK *)&VALVE_TASK_STK[VALVE_STK_SIZE - 1], VALVE_TASK_PRIO);
#endif
#ifdef USE_VESC
	OSTaskCreate(Task_VESC, (void *)0, (OS_STK *)&VESC_TASK_STK[VESC_STK_SIZE - 1], VESC_TASK_PRIO);
	OSTaskCreate(Task_VESCSEND, (void *)0, (OS_STK *)&VESCSEND_TASK_STK[VESCSEND_STK_SIZE - 1], VESCSEND_TASK_PRIO);
#endif
#ifdef USE_ODRIVE
	OSTaskCreate(Task_ODRIVE, (void *)0, (OS_STK *)&ODRIVE_TASK_STK[ODRIVE_STK_SIZE - 1], ODRIVE_TASK_PRIO);
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

static void Task_ODRIVE(void *pdata)
{
	while (1)
	{
		//Odrv_query(1);
		if(MCP_Receive == 1 )
		{
		MCP_READ(BFPCTRL);
		MCP_READ(RXB0SIDH);
    MCP_READ(RXB0SIDL);
		}
		OSTimeDly(1000);
	}
}
static void Task_Elmo(void *pdata) //elmo����
{
	while (1)
	{
		elmo_control(ELMOid);
		OSTimeDly(50); //��ֹ������
		elmo_control(2);
		OSTimeDly(50); //��ֹ������
		elmo_control(3);
		OSTimeDly(50); //��ֹ������
		ELMO_motor_ASKvx(1, 0);
		ELMO_motor_ASKpx(1, 0);
		OSTimeDly(100);
		ELMO_motor_ASKvx(2, 0);
		ELMO_motor_ASKpx(2, 0);
		OSTimeDly(100);
		ELMO_motor_ASKvx(3, 0);
		ELMO_motor_ASKpx(3, 0);
	}
}

static void Valve_Control(void *pdata)
{

	while (1)
	{
		Led_Show();
		CYLINDE_CONTROL(VALVE_DATA);
		OSTimeDly(1000);
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
		Can_DeQueue(CAN2, &VESC_Sendqueue);
		OSTimeDly(1000); //��ʱ̫������λ��ģʽʱ��һ��һ��
	}
}
static void Task_DataScope(void *pdata)
{
	while (1)
	{
		VS4Channal_Send(AKmotor[3].valReal.torque, AKmotor[3].valReal.speed,0,0);

		OSTimeDly(100);
	}
}

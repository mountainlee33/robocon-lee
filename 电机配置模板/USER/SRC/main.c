#include "main.h"
#include "elmo.h"
/**
 * @brief: 主函数main
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
	TIM5_Init();
	CAN1_Mode_Init();
	CAN2_Mode_Init();
	Usart1_Init();
	USART2_Init();
	USART3_Init();
	CYLINDE_Configuration();
	Can_SendqueueInit();
	InitCANControlList(Can2_MesgSentList, CAN_2);
	param_Init();
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

	OSTaskSuspend(START_TASK_PRIO); //挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

static void Task_Motor(void *pdata)
{
	while (1)
	{
		OSTimeDly(2000);
	}
}

static void Task_Elmo(void *pdata) //elmo任务
{
	while (1)
	{
		elmo_control(ELMOid);
		OSTimeDly(50); //防止队列满
		elmo_control(2);
		OSTimeDly(50); //防止队列满
		elmo_control(3);
		OSTimeDly(50); //防止队列满
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
		OSTimeDly(2); //延时太长，跑位置模式时会一卡一卡
	}
}
static void Task_VESCSEND(void *pdata)
{
	while (1)
	{
		Can_DeQueue(CAN2, &VESC_Sendqueue);
		OSTimeDly(1000); //延时太长，跑位置模式时会一卡一卡
	}
}
static void Task_DataScope(void *pdata)
{
	u8 Send_Count;
	while (1)
	{
		DataScope_Get_Channel_Data(motor[0].valueReal.velocity, 1);
		DataScope_Get_Channel_Data(motor[0].valueReal.current, 2);
		DataScope_Get_Channel_Data(motor[0].valueReal.angle, 3);
		DataScope_Get_Channel_Data(4.0, 4);		 //将数据   写入通道 4
		DataScope_Get_Channel_Data(5.0, 5);		 //将数据   写入通道 5
		DataScope_Get_Channel_Data(6.0, 6);		 //将数据   写入通道 6
		DataScope_Get_Channel_Data(7.0, 7);		 //将数据   写入通道 7
		DataScope_Get_Channel_Data(8.0, 8);		 //将数据   写入通道 8
		DataScope_Get_Channel_Data(9.0, 9);		 //将数据   写入通道 9
		DataScope_Get_Channel_Data(10, 10);		 //将数据   写入通道 10
		Send_Count = DataScope_Data_Generate(3); //数据转换，输入有几个通道
		OSTimeDly(100);
	}
}


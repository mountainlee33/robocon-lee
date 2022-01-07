#ifndef __QUEUE_H
#define __QUEUE_H

#include "stm32f4xx_can.h"
#include "string.h"
#include "stdbool.h"
#include "ucos_ii.h"

#define CAN_1 (u8)(0x01<<0)
#define CAN_2 (u8)(0x01<<1)

//报文控制表
#define CAN_QUEUESIZE 50 					//发送报文队列长度
#define CAN_HAVESENTQUEUESIZE	 8		//已发送报文队列长度
#define CAN1_NodeNumber         6u      //不要超过8（故障标志位只有8位）
#define CAN2_NodeNumber 8u				//不要超过8（故障标志位只有8位）
#define Rear1	((Can1_Sendqueue.Rear + 1) % CAN_QUEUESIZE)
#define Rear2 ((Can2_Sendqueue.Rear + 1) % CAN_QUEUESIZE)
#define Rear3 ((VESC_Sendqueue.Rear + 1) % CAN_QUEUESIZE)
#define Rear4 ((AK_Sendqueue.Rear + 1) % CAN_QUEUESIZE)

//CAN报文发送结构体
typedef struct
{
	u32 ID;
	u8 DLC;
	u8 Data[8];
	u8 InConGrpFlag;
}Can_SendStructTypeDef;

//CAN报文队列 
typedef struct
{
	u16 Front,Rear;
	Can_SendStructTypeDef Can_DataSend[CAN_QUEUESIZE];
}Can_QueueTypeDef;

//报文控制块
typedef struct
{
	s32 SendNumber;
	s32 ReceiveNumber;
	u32 QUEUEFullTimeout;
	u8 TimeOut;
	s32 SendSem;
	Can_QueueTypeDef SentQueue;
}MesgControlGrpTypeDef;

//CAN流程号
typedef struct 
{
	volatile u8 order_valid;		//当前有效报文流程号
	volatile u8 order_now;			//当前报文流程号
	volatile u8 status;					//当前流程状态
}Can_ProcedureTypeDef;

extern Can_ProcedureTypeDef  Can_Procedure;
extern Can_SendStructTypeDef Can_Senstruct;
extern MesgControlGrpTypeDef Can2_MesgSentList[CAN2_NodeNumber],Can2_elmoMesgSentList[CAN2_NodeNumber],Can1_MesgSentList[CAN1_NodeNumber];
extern Can_QueueTypeDef Can_Sentqueue,Can2_Sendqueue,Can1_Sendqueue,VESC_Sendqueue,AK_Sendqueue;

extern void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSentList,Can_QueueTypeDef* can_queue , u8 CAN_x);

bool QueueEmpty(Can_QueueTypeDef *can_queue);
bool QueueFull(Can_QueueTypeDef *can_queue);
void Can_SendqueueInit(void);
void Can_DeQueue(CAN_TypeDef *CANx, Can_QueueTypeDef *can_queue);

#endif

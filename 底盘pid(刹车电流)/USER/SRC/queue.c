#include "queue.h"

//相关结构体
Can_ProcedureTypeDef  Can_Procedure;
Can_SendStructTypeDef Can_Senstruct;
MesgControlGrpTypeDef Can2_MesgSentList[CAN2_NodeNumber],Can1_MesgSentList[CAN1_NodeNumber],Can2_elmoMesgSentList[CAN2_NodeNumber];
Can_QueueTypeDef Can_Sentqueue,Can2_Sendqueue,Can1_Sendqueue,VESC_Sendqueue,AK_Sendqueue;

//基本数据
u8 CanSendCount;
int Can1_Tx_NoMailBox;
int Can2_Tx_NoMailBox;

//判断是否队空
bool QueueEmpty(Can_QueueTypeDef *can_queue)
{
	return (can_queue->Front == can_queue->Rear);
}

//判断是否队满
bool QueueFull(Can_QueueTypeDef *can_queue)
{
	return (((can_queue->Rear +1)% CAN_QUEUESIZE) == can_queue->Front);
}

//CAN队列初始化
void Can_SendqueueInit(void)
{
	Can1_Sendqueue.Front = Can1_Sendqueue.Rear = 0;
	VESC_Sendqueue.Front = VESC_Sendqueue.Rear = 0;
	Can2_Sendqueue.Front = Can2_Sendqueue.Rear = 0;
	AK_Sendqueue.Front = AK_Sendqueue.Rear = 0;
}

//CAN出队函数
void Can_DeQueue(CAN_TypeDef *CANx, Can_QueueTypeDef *can_queue)
{
	CanTxMsg ttTxMessage;
	if(QueueEmpty(can_queue))
	{
		return;
	}
	else
	{
		if((can_queue->Can_DataSend[can_queue->Front].ID &0xf0000000)!=0)//VESC报文的标志
		{
			ttTxMessage.IDE = CAN_ID_EXT;
			ttTxMessage.ExtId = can_queue->Can_DataSend[can_queue->Front].ID & 0x0fffffff;
			ttTxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			
			ttTxMessage.RTR = CAN_RTR_DATA;
			
			memcpy(ttTxMessage.Data,(can_queue->Can_DataSend[can_queue->Front].Data),ttTxMessage.DLC*sizeof(uint8_t));
		}
		if((can_queue->Can_DataSend[can_queue->Front].ID >=0x280)&&(can_queue->Can_DataSend[can_queue->Front].ID <=0x2A0))//CAN1
		{
			ttTxMessage.IDE = CAN_ID_STD;
			ttTxMessage.StdId = can_queue->Can_DataSend[can_queue->Front].ID;
			ttTxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			
			ttTxMessage.RTR = CAN_RTR_DATA;
			
			memcpy(ttTxMessage.Data,(can_queue->Can_DataSend[can_queue->Front].Data),ttTxMessage.DLC*sizeof(uint8_t));
		}
		if (CANx == CAN2) //电机的报文
		{
			Can_MesgCtrlList(Can2_MesgSentList, &Can2_Sendqueue, CAN_2);
			if (can_queue->Can_DataSend[can_queue->Front].InConGrpFlag == false)
						can_queue->Front = (can_queue->Front + 1) % CAN_QUEUESIZE;
			CAN_Transmit(CAN2, &ttTxMessage);
		}
		else if (CANx == CAN1)
		{
			Can_MesgCtrlList(Can1_MesgSentList, &Can1_Sendqueue, CAN_1);
			if (Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Front].InConGrpFlag == false)
				can_queue->Front = (can_queue->Front + 1) % CAN_QUEUESIZE;					
			CAN_Transmit(CAN1, &ttTxMessage);		
		}
	}
}


#include "queue.h"

//��ؽṹ��
Can_ProcedureTypeDef Can_Procedure;
Can_SendStructTypeDef Can_Senstruct;
MesgControlGrpTypeDef Can2_MesgSentList[CAN2_NodeNumber], Can1_MesgSentList[CAN1_NodeNumber], Can2_elmoMesgSentList[CAN2_NodeNumber];
Can_QueueTypeDef Can_Sentqueue, Can2_Sendqueue, Can1_Sendqueue, VESC_Sendqueue, AK_Sendqueue, Odrv_Sendqueue;

//��������
u8 CanSendCount;
int Can1_Tx_NoMailBox;
int Can2_Tx_NoMailBox;

//�ж��Ƿ�ӿ�
bool QueueEmpty(Can_QueueTypeDef *can_queue)
{
	return (can_queue->Front == can_queue->Rear);
}

//�ж��Ƿ����
bool QueueFull(Can_QueueTypeDef *can_queue)
{
	return (((can_queue->Rear + 1) % CAN_QUEUESIZE) == can_queue->Front);
}

//CAN���г�ʼ��
void Can_SendqueueInit(void)
{
	Can1_Sendqueue.Front = Can1_Sendqueue.Rear = 0;
	VESC_Sendqueue.Front = VESC_Sendqueue.Rear = 0;
	Can2_Sendqueue.Front = Can2_Sendqueue.Rear = 0;
	AK_Sendqueue.Front = AK_Sendqueue.Rear = 0;
}

//CAN���Ӻ���
void Can_DeQueue(CAN_TypeDef *CANx, Can_QueueTypeDef *can_queue)
{
	CanTxMsg ttTxMessage;
	if (QueueEmpty(can_queue))
	{
		return;
	}
	else
	{
		if (can_queue->Can_DataSend[can_queue->Front].ID > 0x300) //elmo����
		{
			ttTxMessage.IDE = CAN_ID_STD;
			ttTxMessage.StdId = can_queue->Can_DataSend[can_queue->Front].ID;

			if (CAN1 == CANx)
				Can_MesgCtrlList(Can1_MesgSentList, &Can1_Sendqueue, CAN_1);
			else
				Can_MesgCtrlList(Can2_MesgSentList, &Can2_Sendqueue, CAN_2);

			ttTxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			ttTxMessage.RTR = CAN_RTR_DATA;

			memcpy(ttTxMessage.Data, (can_queue->Can_DataSend[can_queue->Front].Data), ttTxMessage.DLC * sizeof(uint8_t));
		}
		else if (can_queue->Can_DataSend[can_queue->Front].ID < 0x008) //AK����
		{
			ttTxMessage.IDE = CAN_ID_STD;									  // IDE    0��ѡ��ʹ�ñ�׼��ʶ��    1��ѡ��ʹ����չ��ʶ��
			ttTxMessage.StdId = can_queue->Can_DataSend[can_queue->Front].ID; //��ʶ��Ϊ���õ�id��
			ttTxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			if (((can_queue->Can_DataSend[can_queue->Front].ID | 0x01F) != 0x009) || (can_queue->Can_DataSend[can_queue->Front].ID | 0x01F) != 0x014)
				ttTxMessage.RTR = CAN_RTR_DATA;
			else
				ttTxMessage.RTR = CAN_RTR_REMOTE; // RTR   0��ѡ��������֡   1��ѡ����ң��֡
			memcpy(ttTxMessage.Data, (can_queue->Can_DataSend[can_queue->Front].Data), ttTxMessage.DLC * sizeof(uint8_t));
		}
		else if ((can_queue->Can_DataSend[can_queue->Front].ID > 0x200)&&(can_queue->Can_DataSend[can_queue->Front].ID < 0x419)) //ODRIVE����
			{
				ttTxMessage.IDE = CAN_ID_STD;									  // IDE    0��ѡ��ʹ�ñ�׼��ʶ��    1��ѡ��ʹ����չ��ʶ��
				ttTxMessage.StdId = can_queue->Can_DataSend[can_queue->Front].ID; //��ʶ��Ϊ���õ�id��
				ttTxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
				if (can_queue->Can_DataSend[can_queue->Front].ID == (0x203 | 0x204 | 0x209 | 0x214 | 0x403 | 0x404 | 0x409 | 0x414))
					ttTxMessage.RTR = CAN_RTR_REMOTE;
				else
					ttTxMessage.RTR = CAN_RTR_DATA; // RTR   0��ѡ��������֡   1��ѡ����ң��֡
				memcpy(ttTxMessage.Data, (can_queue->Can_DataSend[can_queue->Front].Data), ttTxMessage.DLC * sizeof(uint8_t));
			}
		else if ((can_queue->Can_DataSend[can_queue->Front].ID & 0xf0000000) != 0) //VESC���ĵı�־
		{
			ttTxMessage.IDE = CAN_ID_EXT;
			ttTxMessage.ExtId = can_queue->Can_DataSend[can_queue->Front].ID & 0x0fffffff;
			ttTxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;

			ttTxMessage.RTR = CAN_RTR_DATA;

			memcpy(ttTxMessage.Data, (can_queue->Can_DataSend[can_queue->Front].Data), ttTxMessage.DLC * sizeof(uint8_t));
		}
	}
	if (CANx == CAN2) //����ı���
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

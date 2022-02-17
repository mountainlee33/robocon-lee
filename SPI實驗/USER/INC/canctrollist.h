#ifndef _CANCTRLLIST_H
#define _CANCTRLLIST_H

#include "includes.h"
#include "queue.h"
#include "param.h"

/****CAN�������̺�****/
typedef struct 
{
  vu8 order_valid; //��ǰ��Ч�������̺�
  vu8 order_real;  //��ǰ�������̺�
  vu8 status;      //��ǰ����״̬
  /*0x01�Ƿ�ӵ��	0x02�Ƿ���ȷ	0x04��ǰ���ͺ�	0x80�Ƿ����*/
} CAN_ProcedureTypeDef;

extern CAN_ProcedureTypeDef CAN_Procedure;

void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSentList,Can_QueueTypeDef* can_queue , u8 CAN_x);
void TraversalControlList(MesgControlGrpTypeDef* CAN_MesgSentList,Can_QueueTypeDef* can_queue , u8 CAN_x);
void InitCANControlList(MesgControlGrpTypeDef* CAN_MesgSentList , u8 CAN_x);

#endif

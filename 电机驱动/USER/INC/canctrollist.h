#ifndef _CANCTRLLIST_H
#define _CANCTRLLIST_H

#include "includes.h"
#include "queue.h"
#include "param.h"

/****CAN报文流程号****/
typedef struct 
{
  vu8 order_valid; //当前有效报文流程号
  vu8 order_real;  //当前报文流程号
  vu8 status;      //当前流程状态
  /*0x01是否拥有	0x02是否正确	0x04超前或滞后	0x80是否矫正*/
} CAN_ProcedureTypeDef;

extern CAN_ProcedureTypeDef CAN_Procedure;

void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSentList,Can_QueueTypeDef* can_queue , u8 CAN_x);
void TraversalControlList(MesgControlGrpTypeDef* CAN_MesgSentList,Can_QueueTypeDef* can_queue , u8 CAN_x);
void InitCANControlList(MesgControlGrpTypeDef* CAN_MesgSentList , u8 CAN_x);

#endif

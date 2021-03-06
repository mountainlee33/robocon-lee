#ifndef _CAN_H
#define _CAN_H

#include "includes.h"//任何文件必须引入
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "dianji.h"
#include "queue.h"
#include "macro.h"
#include "elmo.h"
#include "led.h"
#include "vesc.h"
#include "param.h"
#include "odrive.h"
#include "akmotor.h"
#include "numchange.h"

void CAN2_Mode_Init(void);
static void Elmo_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList);

u8 CAN2_Send_Msg(u8 *msg, u8 len);
u8 CAN2_Receive_Msg(u8 *buf);
extern u32 last_update_time[8];
extern u32 now_update_time[8];
extern u32 err_update_time[8];

#endif

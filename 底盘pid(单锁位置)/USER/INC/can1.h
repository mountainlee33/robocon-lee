#ifndef _CAN1_H
#define _CAN1_H

#include "includes.h" //�κ��ļ���������
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "queue.h"
#include "vesc.h"
#include "dianji.h"
#include "macro.h"

void CAN1_Mode_Init(void);
static void answer_turn(CanRxMsg *rx_message, u8 motorid, s16 *buf);

#endif

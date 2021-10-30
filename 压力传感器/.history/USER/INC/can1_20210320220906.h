/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-03-20 22:09:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\INC\can1.h
 */
#ifndef __CAN1_H
#define __CAN1_H

#include "sys.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define PreSensor 0

#if PreSensor == 0
#define ID_SELF 0x00004170
#define ID_BACK 0x00003240
#define BROADCAST_ID 0x00010000
#endif

#if PreSensor == 1
#define ID_SELF 0x00004171
#define ID_BACK 0x00003241
#define BROADCAST_ID 0x00010000
#endif

#if PreSensor == 2
#define ID_SELF 0x00004172
#define ID_BACK 0x00003242
#define BROADCAST_ID 0x00010000
#endif

void CAN1_Configuration(void);
static void answer_master(const CanRxMsg *rx_message);
void Can1_BusTest(void);
void Can1_Send_Pressure(void);

#endif

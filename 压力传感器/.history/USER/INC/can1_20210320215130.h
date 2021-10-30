#ifndef __CAN1_H
#define __CAN1_H

#include "sys.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define PreSensor 0

#if PreSensor == 0
#define ID_SELF 0x00004170
#define ID_BACK 0x00063
#define BROADCAST_ID 0x00010000
#endif

#if PreSensor == 1
#define ID_SELF 0x00010600
#define ID_BACK 0x00060101
#define BROADCAST_ID 0x00010000
#endif

#if PreSensor == 2
#define ID_SELF 0x00010600
#define ID_BACK 0x00060101
#define BROADCAST_ID 0x00010000
#endif

void CAN1_Configuration(void);
static void answer_master(const CanRxMsg *rx_message);
void Can1_BusTest(void);

#endif

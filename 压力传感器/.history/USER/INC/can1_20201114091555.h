#ifndef __CAN1_H
#define __CAN1_H

#include "sys.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define ID_SELF 0x00010600
#define ID_BACK 0x00060101
#define BROADCAST_ID 0x00010000



void CAN1_Configuration(void);
static void answer_master(const CanRxMsg *rx_message);
void Can1_BusTest(void);

#endif

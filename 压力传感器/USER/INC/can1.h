/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-03-22 22:14:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\INC\can1.h
 */
#ifndef __CAN1_H
#define __CAN1_H

#include "sys.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "HX711.h"
#include "param.h"

void CAN1_Configuration(void);
static void answer_master(const CanRxMsg *rx_message);
void Can1_Send_Pressure(void);
void answer_HX711_feedback(const CanRxMsg *rxmessage);
void HX711_error(PRESENSOR *Presensor);

#endif

#ifndef _TEST_H
#define _TEST_H

#include "stm32f4xx.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "can1.h"

#include "led.h"

/*���ڲ���ģʽѡ��*/
#define USART1_Test 1
#define USART2_Test 2
#define USART3_Test 3
#define USART4_Test 4
#define USART5_Test 5

/*can����:ѭ������WHU/����Ϊ---������*/
#define CAN1_SendTest 11
#define CAN1_ReceiveTest 12

#define CAN2_SendTest 21
#define CAN2_ReceiveTest 22


void USART_Test(void);
void CAN_Test(void);

#endif


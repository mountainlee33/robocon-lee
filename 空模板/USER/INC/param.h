#ifndef _PARAM_H_
#define _PARAM_H_

#include "stm32f4xx.h"
#include "includes.h"
#include "stdbool.h"
#include "led.h"
#include "led8.h"
#include "beep.h"

typedef enum
{
  USART1_Tx_BufferSize = 256, //����һ�������ݳ���
  USART2_Tx_BufferSize = 256, //���ڶ��������ݳ���
  USART3_Tx_BufferSize = 32,  //�������������ݳ���
  USART1_Rx_BufferSize = 32,  //����һ�������ݳ��ȣ������ж���ʹ�ã�
  USART2_Rx_BufferSize = 32,  //���ڶ��������ݳ��ȣ������ж���ʹ�ã�
  USART3_Rx_BufferSize = 32,  //�������������ݳ��ȣ������ж���ʹ�ã�
} Usart_Enum;

typedef struct
{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //����һ��������
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //����һ��������

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //���ڶ���������
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //���ڶ���������

  u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //��������������
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //��������������
} Usart_Struct;




void ChangeData(u8 *buff2, u8 *buff1);
void param_Init(void);



#endif

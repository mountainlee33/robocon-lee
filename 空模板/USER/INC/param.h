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
  USART1_Tx_BufferSize = 256, //串口一发送数据长度
  USART2_Tx_BufferSize = 256, //串口二发送数据长度
  USART3_Tx_BufferSize = 32,  //串口三发送数据长度
  USART1_Rx_BufferSize = 32,  //串口一接收数据长度（接收中断内使用）
  USART2_Rx_BufferSize = 32,  //串口二接收数据长度（接收中断内使用）
  USART3_Rx_BufferSize = 32,  //串口三接收数据长度（接收中断内使用）
} Usart_Enum;

typedef struct
{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //串口一发送数据
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //串口一接收数据

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //串口二发送数据
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //串口二接收数据

  u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //串口三发送数据
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //串口三接收数据
} Usart_Struct;




void ChangeData(u8 *buff2, u8 *buff1);
void param_Init(void);



#endif

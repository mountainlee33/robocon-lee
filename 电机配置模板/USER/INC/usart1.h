#ifndef __USART1_H
#define __USART1_H

#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "string.h"
#include "param.h"

void Usart1_Init(void);
void USART1_sendData(u8 *a,u8 count);
void DMA2_Stream7_IRQHandler(void);
unsigned short CRC_CHECK(unsigned char * Buf, unsigned char CRC_CNT);
void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4);

#endif

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
void  SendChar(unsigned char t);


#endif

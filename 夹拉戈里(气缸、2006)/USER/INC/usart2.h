#ifndef __USART2_H
#define __USART2_H


#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "param.h"
#include "dianji.h"

void USART2_Init(void);
void UsartLCDshow(void);

#endif


#ifndef _USART2_H
#define _USART2_H


#include "stm32f4xx.h"
#include "time.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "sys.h"
void USART2_MyInit(void);
void USART2_IRQHandler(void);

#endif


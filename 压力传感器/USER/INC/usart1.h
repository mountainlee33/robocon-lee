#ifndef _USART1_H
#define _USART1_H


#include "stm32f4xx.h"
#include "time.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "sys.h"
void USART1_MyInit(void);
void USART1_IRQHandler(void);

#endif

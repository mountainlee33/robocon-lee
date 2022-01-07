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
void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4);


#endif

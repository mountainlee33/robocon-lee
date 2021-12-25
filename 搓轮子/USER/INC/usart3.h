#ifndef __USART3_H

#define __USART3_H

#include "stm32f4xx.h"

#include "stm32f4xx_usart.h"

#include "stm32f4xx_gpio.h"

#include "stm32f4xx_rcc.h"

#include "stm32f4xx_dma.h"

#include "param.h"

void USART3_Init(void);

void USART3_sendData (u8 *data, u8 count);


#endif



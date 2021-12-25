#ifndef _BEEP_H

#define _BEEP_H

#include "stm32f4xx_gpio.h"

#include "stm32f4xx_rcc.h"

#include "includes.h"
#include "delay.h"
#include "param.h"

#define BEEP_OFF GPIOA->BSRRH = GPIO_Pin_8

#define BEEP_ON GPIOA->BSRRL = GPIO_Pin_8

#define BEEP_TOGGLE GPIOA->ODR ^= GPIO_Pin_8

void Beep_Init(void);

void Beep_Show(u8 num);

#endif

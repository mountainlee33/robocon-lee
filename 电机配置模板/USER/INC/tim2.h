#ifndef _TIM2_H
#define _TIM2_H


#include "includes.h"//任何文件必须引入
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "dianji.h"
#include "queue.h"
#include "canctrollist.h"
#include "elmo.h"
#include "akmotor.h"

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_IRQHandler(void);

#endif

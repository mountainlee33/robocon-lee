#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define Set_CYLCLK			GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define Reset_CYLCLK		GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define Set_CYLDATA 		GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define Reset_CYLDATA		GPIO_ResetBits(GPIOC,GPIO_Pin_11)

void CYLINDE_Configuration(void);
void CYLINDE_CONTROL(u8 data);


#endif

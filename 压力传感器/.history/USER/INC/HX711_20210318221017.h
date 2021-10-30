#ifndef _HX711_H
#define _HX711_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "sys.h"
#include "delay.h"

#define HX711_SCK 	PAout(0)// PB0,��PB0�������Ϊ����,			PA1
#define HX711_DOUT 	PAin(1)	// PB1����PB1��������Ϊ���ƣ�		PA0

extern u32 Weight_Shiwu;
extern u8 Flag_Error;


void Init_HX711pin(void);
u32 HX711_Read(void);	//����128
void Get_Peel(void);
void Get_Weight(void);
	
#endif


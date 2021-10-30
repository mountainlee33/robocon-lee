/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-03-20 21:21:25
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor\USER\INC\HX711.h
 */
#ifndef _HX711_H
#define _HX711_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "sys.h"
#include "delay.h"

#define RCC_SCK RCC_AHB1Periph_GPIOB
#define GPIO_SCK GPIOB
#define Pin_SCK GPIO_Pin_11

#define RCC_DOUT RCC_AHB1Periph_GPIOB
#define GPIO_DOUT GPIOB
#define Pin_DOUT GPIO_Pin_10

#define HX711_SCK 	PBout(11)
#define HX711_DOUT 	PBin(10)	

#define GapValue_0 252.5
#define GapValue_1 250
#define GapValue_2 245

extern u32 Weight_Shiwu;
extern u8 Flag_Error;


void Init_HX711pin(void);
u32 HX711_Read(void);	//增益128
void Get_Peel(void);
int Get_Weight(double GapValue);
	
#endif


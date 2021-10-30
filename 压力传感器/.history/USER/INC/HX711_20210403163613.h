/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-04-03 16:36:13
 * @LastEditors: Please set LastEditors
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
#include "param.h"



void Init_HX711pin(void);
u32 HX711_Read(struct PRESENSOR *presensor);	//增益128
void Get_Weight(void);
void Get_Peel(void);


#endif


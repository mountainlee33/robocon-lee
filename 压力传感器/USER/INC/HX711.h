/*
 * @Author: your name
 * @Date: 2021-03-20 20:53:30
 * @LastEditTime: 2021-04-04 20:27:49
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
#include "led.h"


void Init_HX711pin(void);
u32 HX711_Read(PRESENSOR *Presensor);	//增益128
void Get_Weight(void);
void Get_Peel(void);
void Set_Zero(void);


#endif


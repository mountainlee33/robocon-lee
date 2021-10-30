/*
 * @Author: your name
 * @Date: 2021-03-28 16:17:02
 * @LastEditTime: 2021-04-04 20:18:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PreSensor3\USER\INC\param.h
 */
#ifndef _PARAM_H
#define _PARAM_H

#include "stdint.h"
#include "stm32f4xx.h"
#include "sys.h"
#include "stdbool.h"

typedef struct
{
	volatile unsigned long * HX711_SCK;
	volatile unsigned long * HX711_DOUT;
	uint16_t Pin_SCK;
	uint16_t Pin_DOUT;
	GPIO_TypeDef *GPIO_DOUT;
	GPIO_TypeDef *GPIO_SCK;
}PreGPIO;


typedef struct 
{
	s32 pressure;

	PreGPIO pregpio;
	
	bool enable;
	bool SetZero;
}PRESENSOR;

typedef struct 
{
	int number;
	double GapValue;	
	u32 Weight_Peel;
	u32 HX711_Buffer;
}constPRE;


extern PRESENSOR presensor[3];

//压力传感器系数
#define GapValue_1 251.8964
#define GapValue_2 252.5205
#define GapValue_3 217.8712
#define GapValue_4 218.9204
#define GapValue_5 245.5215
#define GapValue_6 224.8360

#define ID_SELF 0x00000417
#define ID_BACK 0x00000714

void presensor_Init(void);

#endif





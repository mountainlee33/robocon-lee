/*
 * @Author: your name
 * @Date: 2021-03-28 16:17:02
 * @LastEditTime: 2021-04-04 22:14:47
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

//压力传感器IO参数设置
typedef struct
{
	volatile unsigned long * HX711_SCK;
	volatile unsigned long * HX711_DOUT;
	uint16_t Pin_SCK;
	uint16_t Pin_DOUT;
	GPIO_TypeDef *GPIO_DOUT;
	GPIO_TypeDef *GPIO_SCK;
}PreGPIO;

//压力传感器常量设置
typedef struct 
{
	int number;
	double GapValue;	
	u32 Weight_Peel;
	u32 HX711_Buffer;
}constPRE;

//压力传感器相关参数设置
typedef struct 
{
	bool enable;
	bool SetZero;
	bool DOUT_error;
}PreFlag;

//压力传感器总结构体
typedef struct 
{
	s32 pressure;

	constPRE constpre;
	PreGPIO pregpio;
	PreFlag preflag;
}PRESENSOR;


#define SensorNum 4

//压力传感器系数
#define GapValue_1 251.8964
#define GapValue_2 252.5205
#define GapValue_3 217.8712
#define GapValue_4 218.9204
#define GapValue_5 245.5215
#define GapValue_6 224.8360
#define GapValue_7 218.9708
#define GapValue_9 252.8575
#define GapValue_10 247.1733


#define ID_SELF 0x00004170
#define ID_BACK 0x00007140

extern PRESENSOR presensor[SensorNum];
extern bool transfer_enable;


void presensor_Init(void);

#endif





#ifndef _PID_H
#define _PID_H

#include "includes.h"//任何文件必须引入
#include "stm32f4xx.h"
#include "utils.h"
#include "math.h"

#define STANDARD_ 0x01
#define EXPECT_ 0x02
#define FILTER_ 0x04

enum{KP1,KP2,KI2,KD2,LIM};

typedef struct
{
	volatile float SubVal[3];//pid差值，set和real差值
	volatile float Udlt;//pid增量值
	volatile float AccVal;//pid输出累加值
	volatile float ExpVal;//期望值
	volatile float OutVal;//输出值
	volatile float AccVal_Last;//上次的输出值
	volatile u8 Proc_Flag;//决定是否进行pid运算，叠加期望速度，滤波处理
	volatile int Par_Group;//使用的参数
}Data_ProStr;


#endif

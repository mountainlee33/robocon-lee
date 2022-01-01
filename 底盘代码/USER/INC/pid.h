#ifndef _PID_H
#define _PID_H

#include "includes.h"//�κ��ļ���������
#include "stm32f4xx.h"
#include "utils.h"
#include "math.h"

#define STANDARD_ 0x01
#define EXPECT_ 0x02
#define FILTER_ 0x04

enum{KP1,KP2,KI2,KD2,LIM};

typedef struct
{
	volatile float SubVal[3];//pid��ֵ��set��real��ֵ
	volatile float Udlt;//pid����ֵ
	volatile float AccVal;//pid����ۼ�ֵ
	volatile float ExpVal;//����ֵ
	volatile float OutVal;//���ֵ
	volatile float AccVal_Last;//�ϴε����ֵ
	volatile u8 Proc_Flag;//�����Ƿ����pid���㣬���������ٶȣ��˲�����
	volatile int Par_Group;//ʹ�õĲ���
}Data_ProStr;


#endif

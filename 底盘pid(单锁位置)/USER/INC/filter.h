#ifndef _FILTER__H
#define _FILTER__H

#include "param.h"

//低通滤波器
typedef struct IIR
{
	float prv;			//前次滤波后的值
	float now;			//本次滤波后的值
	float alpha;		//低通滤波系数
}IIR_struct;

//卡尔曼滤波
typedef struct 
{
		float filterValue;//滤波后的值
    float kalmanGain;//Kalamn增益
    float A;//状态矩阵
    float H;//观测矩阵
	
		float P;//预测过程协方差矩阵     P(k|k)  
		float P_last;//上次预测过程协方差矩阵 P(k|k-1)
    float Q;//状态矩阵的方差
    float R;//观测矩阵的方差
	
		float X;						/*系统状态滤波输出矩阵，列矩阵*/
		float X_last;				/*系统状态预测矩阵，列矩阵*/
		float Gk;						/*卡尔曼增益，列矩阵*/
		float zero_point;		/*kalman filter zero_point*/					
		float input;				/*量测值，即Z(k)*/
}KALMN_struct;

extern IIR_struct IIR_filt;
extern KALMN_struct kal_filter;

void IIR_Init(IIR_struct *IIR_filt);
double IIR_Filter(IIR_struct *IIR_filt, double input);//一阶滞后滤波
void KAL_Init(KALMN_struct *k_flt);
float KALMN_Filter(KALMN_struct *k_flt, float input);


#endif

#include "filter.h"

IIR_struct IIR_filt={0};
KALMN_struct kal_filter = {0};

//低通滤波初始化
void IIR_Init(IIR_struct *IIR_filt)
{
	IIR_filt->prv = 0;
	IIR_filt->alpha = 660;		//低通滤波系数
}

//一阶低通滤波运算   
//算法：Y(n)=αX(n) (1-α)Y(n-1)
double IIR_Filter(IIR_struct *IIR_filt, double input)//一阶滞后滤波
{
	IIR_filt->now = (IIR_filt->alpha * input +  (1000 - IIR_filt->alpha) *IIR_filt->prv)/1000.f;
	IIR_filt->prv = IIR_filt->now;
	
	return IIR_filt->now;
}

//卡尔曼滤波初始化
void KAL_Init(KALMN_struct *k_flt)
{
	k_flt->A = 1;		//系统状态转移矩阵
	k_flt->H = 1; 	//观测矩阵，行矩阵
	
	k_flt->P_last = 1;   //上次预测过程协方差矩阵
	
	k_flt->Q = 0.00001;  //过程噪声
	k_flt->R = 0.1;			 //观测噪声
	
	k_flt->input = 0;    //初始测量值
}

float KALMN_Filter(KALMN_struct *k_flt, float input)
{
	k_flt->input = IIR_Filter(&IIR_filt,input);				//处理过的噪声
	
	//测量方程的更新
	k_flt->Gk	= (k_flt->P_last)/(k_flt->P_last + k_flt->R);//计算卡尔曼增益 kg(k) = P(k|k-1)*H(转置)/[H*P(k|k-1)*H(转置)+R]
	k_flt->X = k_flt->X_last + k_flt->Gk *(k_flt->input - k_flt->H*k_flt->X_last);//得到现在状态的最优化估算值 X(k|k) = X(k|k-1)+kg(k)*[Z(k)-H*X(k|k-1)]
	k_flt->P =  (1-k_flt->Gk *k_flt->H)*k_flt->P_last;//更新 k 状态下 X(k|k) 的协方差 C(k|k) = [I-kg(k)*H]*C(k|k-1)
	
	//时间更新方程
	k_flt->P_last = k_flt->P;
	k_flt->X_last = k_flt->X;
	
	return k_flt->X;
}

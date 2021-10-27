#ifndef _FILTER__H
#define _FILTER__H

#include "param.h"

//��ͨ�˲���
typedef struct IIR
{
	float prv;			//ǰ���˲����ֵ
	float now;			//�����˲����ֵ
	float alpha;		//��ͨ�˲�ϵ��
}IIR_struct;

//�������˲�
typedef struct 
{
		float filterValue;//�˲����ֵ
    float kalmanGain;//Kalamn����
    float A;//״̬����
    float H;//�۲����
	
		float P;//Ԥ�����Э�������     P(k|k)  
		float P_last;//�ϴ�Ԥ�����Э������� P(k|k-1)
    float Q;//״̬����ķ���
    float R;//�۲����ķ���
	
		float X;						/*ϵͳ״̬�˲���������о���*/
		float X_last;				/*ϵͳ״̬Ԥ������о���*/
		float Gk;						/*���������棬�о���*/
		float zero_point;		/*kalman filter zero_point*/					
		float input;				/*����ֵ����Z(k)*/
}KALMN_struct;

extern IIR_struct IIR_filt;
extern KALMN_struct kal_filter;

void IIR_Init(IIR_struct *IIR_filt);
double IIR_Filter(IIR_struct *IIR_filt, double input);//һ���ͺ��˲�
void KAL_Init(KALMN_struct *k_flt);
float KALMN_Filter(KALMN_struct *k_flt, float input);


#endif

#include "filter.h"

IIR_struct IIR_filt={0};
KALMN_struct kal_filter = {0};

//��ͨ�˲���ʼ��
void IIR_Init(IIR_struct *IIR_filt)
{
	IIR_filt->prv = 0;
	IIR_filt->alpha = 660;		//��ͨ�˲�ϵ��
}

//һ�׵�ͨ�˲�����   
//�㷨��Y(n)=��X(n) (1-��)Y(n-1)
double IIR_Filter(IIR_struct *IIR_filt, double input)//һ���ͺ��˲�
{
	IIR_filt->now = (IIR_filt->alpha * input +  (1000 - IIR_filt->alpha) *IIR_filt->prv)/1000.f;
	IIR_filt->prv = IIR_filt->now;
	
	return IIR_filt->now;
}

//�������˲���ʼ��
void KAL_Init(KALMN_struct *k_flt)
{
	k_flt->A = 1;		//ϵͳ״̬ת�ƾ���
	k_flt->H = 1; 	//�۲�����о���
	
	k_flt->P_last = 1;   //�ϴ�Ԥ�����Э�������
	
	k_flt->Q = 0.00001;  //��������
	k_flt->R = 0.1;			 //�۲�����
	
	k_flt->input = 0;    //��ʼ����ֵ
}

float KALMN_Filter(KALMN_struct *k_flt, float input)
{
	k_flt->input = IIR_Filter(&IIR_filt,input);				//�����������
	
	//�������̵ĸ���
	k_flt->Gk	= (k_flt->P_last)/(k_flt->P_last + k_flt->R);//���㿨�������� kg(k) = P(k|k-1)*H(ת��)/[H*P(k|k-1)*H(ת��)+R]
	k_flt->X = k_flt->X_last + k_flt->Gk *(k_flt->input - k_flt->H*k_flt->X_last);//�õ�����״̬�����Ż�����ֵ X(k|k) = X(k|k-1)+kg(k)*[Z(k)-H*X(k|k-1)]
	k_flt->P =  (1-k_flt->Gk *k_flt->H)*k_flt->P_last;//���� k ״̬�� X(k|k) ��Э���� C(k|k) = [I-kg(k)*H]*C(k|k-1)
	
	//ʱ����·���
	k_flt->P_last = k_flt->P;
	k_flt->X_last = k_flt->X;
	
	return k_flt->X;
}

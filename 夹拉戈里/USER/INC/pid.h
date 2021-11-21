#ifndef _PID_H
#define _PID_H

#include "includes.h"//�κ��ļ���������
#include "stm32f4xx.h"
#include "utils.h"

/*PID����*/
typedef struct{
	volatile s32 SetVal;//�趨ֵ
	float KP;//����ϵ��
	float KI;//���ֳ���
	float KD;//΢�ֳ���
	float KS;//���ٱ�
	volatile s32 Udlt;//pid����ֵ
	volatile s32 CurVal;//ʵ��ֵ
	volatile s32 SubVal[3];//��ֵ���棬�����ͷ����Ĳ�ֵ
}PID_SetTypeDef;

typedef struct
{
	float kp;
  float ki;
  float kd;
	float d_filter;
  float kd_filter;	
	float p_term;
  float i_term;
  float d_term;
	float output;
	volatile s32 error;
	volatile s32 prv_error;
	volatile s32 CurVal;
	volatile s32 SetVal;
	
}VESC_PID_setTypeDef;

void VESC_PID_Init(VESC_PID_setTypeDef* PID, float KP,float KI,float KD,float KD_Filter);
void VESC_PID_Operation(VESC_PID_setTypeDef *PID, s32 setval, s32 curval);
void PID_Init(PID_SetTypeDef *PID,float KP,float KD,float KI,float KS,s32 Set);
void PID_Operation(PID_SetTypeDef *PID);
#endif

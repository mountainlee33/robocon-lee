#include "pid.h"

//pid��ʼ��
void PID_Init(PID_SetTypeDef *PID,float KP,float KI,float KD,float KS, s32 Set)
{
	PID->CurVal= 0;
	PID->KD= KD;
	PID->KI= KI;
	PID->KP= KP;
	PID->KS= KS;
	PID->SetVal= Set;
	PID->SubVal[0]=0;
	PID->SubVal[1]=0;
	PID->SubVal[2]=0;
}

//VESC_PID��ʼ��
void VESC_PID_Init(VESC_PID_setTypeDef* PID, float KP,float KI,float KD,float KD_Filter)
{
  PID->kp = KP;
  PID->ki = KI;
  PID->kd = KD;
  PID->kd_filter = KD_Filter;
}

//pid����
void PID_Operation(PID_SetTypeDef *PID)
{
	PID->SubVal[0]= PID->SetVal - PID->CurVal;
	PID->Udlt = PID->KP *(PID->SubVal[0] - PID->SubVal[1])+PID->KI* PID->SubVal[0]+PID->KD *(PID->SubVal[0]-2*PID->SubVal[1]+PID->SubVal[2]);
	PID->SubVal[2] = PID->SubVal[1];
	PID->SubVal[1] = PID->SubVal[0];
}

void VESC_PID_Operation(VESC_PID_setTypeDef *PID, s32 setval, s32 curval)
{
	PID->SetVal = setval;
	PID->CurVal = curval;
	PID->error = PID->SetVal - PID->CurVal;
	
	// Too low RPM set. Reset state and return.
	if(ABS(PID->SetVal)<20)
	{
		PID->i_term = 0.0f;
		PID->prv_error = PID->error;
		return;
	}	

  // Compute parameters	
	PID->p_term = PID->error * PID->kp /100.f;
	PID->i_term += PID->error * PID->ki / 1000.f / 100.f;
	PID->d_term = (PID->error - PID->prv_error) * PID->kd;
	
	// Filter D   
  UTILS_LP_FAST(PID->d_filter, PID->d_term, PID->kd_filter);//���������㷨������Ŀ��ֵͻȻ�仯�����Ķ������ɶԲ���ֵ����΢������
	PID->d_term = PID->d_filter;  
	
	// I-term wind-up protection
	utils_truncate_number(&PID->i_term, -1.0f, 1.0f);
	
  // Store previous error
	PID->prv_error = PID->error;

	// Calculate output
	PID->output = PID->p_term + PID->i_term + PID->d_term;
	utils_truncate_number(&PID->output, -1.0f, 1.0f);  	
}

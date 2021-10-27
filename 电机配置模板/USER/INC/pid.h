#ifndef _PID_H
#define _PID_H

#include "includes.h"//任何文件必须引入
#include "stm32f4xx.h"
#include "utils.h"

/*PID参数*/
typedef struct{
	volatile s32 SetVal;//设定值
	float KP;//比例系数
	float KI;//积分常数
	float KD;//微分常数
	float KS;//减速比
	volatile s32 Udlt;//pid增量值
	volatile s32 CurVal;//实际值
	volatile s32 SubVal[3];//差值保存，给定和反馈的差值
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

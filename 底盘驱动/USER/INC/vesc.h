#ifndef __VESC_H
#define __VESC_H

#include "param.h"
#include "queue.h"
#include "canctrollist.h"
#include "utils.h"
#include "pid.h"
#include "numchange.h"
#include "dianji.h"
#include "math.h"
#include "beep.h"
#include "led8.h"

extern u8 all_speed,all_enable,all_begin;

/****VESC�ڲ�****/
typedef struct
{
	u8 POLE_PAIRS;
	float MAX_CURRENT;
	s32 MAX_RPM;
} VESCParam;

/****VESC���****/
typedef struct
{
	volatile float current; //����
	volatile vs32 speed;	//�ٶ�
	float angle;			//�Ƕ�
	volatile float duty;	//ռ�ձ�
	volatile vs32 position; //λ��
} VESCVal;

/****VESC״̬****/
typedef struct
{
	bool stuck;		//�����ת
	bool isSetZero; //���õ�ǰλ��Ϊ���
	bool timeout;	//can���ķ�����ʱ
	bool arrived;	//����ָ��λ��
} VESCStatus;

/****VESC���Ʊ���****/
typedef struct
{
	float BrakeCurrent;	 //ɲ������
	float MaxCurrentSet; //
	bool isPosLimitOn;
	s32 maxPosition; //λ�����Ƶ����λ��
	bool isPosSpLimitOn;
	s32 posSpLimit; //λ�û�������ٶ�
	bool stuckmotion;
	bool timeoutmotion;
} VESCLimit;

/****����Ҫ���ĵ�һЩ����****/
typedef struct
{
	vs16 distance; //���νǶȵĲ�ֵ
	bool firstPos;
	vu16 angleNow;
	vu16 anglePrv;
	float lockAngle;
	s32 lockPosition;
	vs32 difPosition;
	bool timeout;
	u16 stuckCnt;	  //��ת�ü���ֵ
	u8 timeoutCnt;	  //��ʱ�ü���ֵ
	u32 lastRxTim;	  //�ϴν��յ����ĵ�ϵͳʱ��
	u16 timeoutTicks; //�жϳ�ʱ��ϵͳ������
	u16 stuckTicks;
} VESCArgum;

/****VESC����ṹ��****/
typedef struct
{
	bool enable;			 //���ʹ��
	bool begin;				 //�������
	bool Direction_Inverted; //��ת������ת
	u8 mode;
	VESCVal ValSet, ValReal, ValPrv; //����趨ֵ��ʵ��ֵ,�ϴ�ֵ
	VESCStatus status;
	PID_SetTypeDef PIDx, PIDs; //PID����
	VESC_PID_setTypeDef p_pid, s_pid;
	VESCArgum argum;	  //��ֵ����
	VESCLimit limit;	  //������Ʊ���
	VESCParam instrinsic; //����ڲ�
} VESCMotor;

/****VESC��can��������ö����****/
typedef enum
{
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE
} CAN_PACKET_ID_Enum;

extern VESCParam SUNNYSKY, VESC_U10, VESC_U8;
extern VESCLimit VESClimit;
extern VESCArgum VESCargum;
extern VESCMotor VESCmotor[4];

void VESC_Init(void);
void VESC_Setzero(VESCMotor *motor);
void VESC_RPM_mode_I(u8 id);
void VESC_position_mode_pos(u8 id);
void VESC_position_mode_I(u8 id);
void VESC_position_mode_rpm(u8 id);
void VESC_Set_Current(u8 controler_ID, float current, u8 InConGrpFlag);
void VESC_Set_Speed(u8 controller_ID, s32 speed, u8 InConGrpFlag);
void VESC_Set_Ang(u8 controller_ID, float angle, u8 InConGrpFlag);
void VESC_Set_Duty_Cycle(u8 controller_ID, float duty_cycle, u8 InConGrpFlag);
void VESC_Set_Pos(u8 controller_ID, float pos, u8 InConGrpFlag);
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 InConGrpFlag);
void VESC_Set_HandBrake_Current(u8 controller_ID, float handbrake_current, u8 InConGrpFlag);
u8 ifvescstuck(u8 controllerid);
void Control_all_VESC(void);

#endif

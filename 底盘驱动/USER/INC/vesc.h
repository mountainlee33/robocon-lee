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

/****VESC内参****/
typedef struct
{
	u8 POLE_PAIRS;
	float MAX_CURRENT;
	s32 MAX_RPM;
} VESCParam;

/****VESC外参****/
typedef struct
{
	volatile float current; //电流
	volatile vs32 speed;	//速度
	float angle;			//角度
	volatile float duty;	//占空比
	volatile vs32 position; //位置
} VESCVal;

/****VESC状态****/
typedef struct
{
	bool stuck;		//电机堵转
	bool isSetZero; //设置当前位置为零点
	bool timeout;	//can报文反馈超时
	bool arrived;	//到达指定位置
} VESCStatus;

/****VESC限制保护****/
typedef struct
{
	float BrakeCurrent;	 //刹车电流
	float MaxCurrentSet; //
	bool isPosLimitOn;
	s32 maxPosition; //位置限制的最大位置
	bool isPosSpLimitOn;
	s32 posSpLimit; //位置环下最大速度
	bool stuckmotion;
	bool timeoutmotion;
} VESCLimit;

/****不需要关心的一些参数****/
typedef struct
{
	vs16 distance; //两次角度的差值
	bool firstPos;
	vu16 angleNow;
	vu16 anglePrv;
	float lockAngle;
	s32 lockPosition;
	vs32 difPosition;
	bool timeout;
	u16 stuckCnt;	  //堵转用计数值
	u8 timeoutCnt;	  //超时用计数值
	u32 lastRxTim;	  //上次接收到报文的系统时间
	u16 timeoutTicks; //判断超时用系统节拍数
	u16 stuckTicks;
} VESCArgum;

/****VESC电机结构体****/
typedef struct
{
	bool enable;			 //电机使能
	bool begin;				 //电机启动
	bool Direction_Inverted; //旋转方向逆转
	u8 mode;
	VESCVal ValSet, ValReal, ValPrv; //外参设定值，实际值,上次值
	VESCStatus status;
	PID_SetTypeDef PIDx, PIDs; //PID参数
	VESC_PID_setTypeDef p_pid, s_pid;
	VESCArgum argum;	  //间值参数
	VESCLimit limit;	  //电机限制保护
	VESCParam instrinsic; //电机内参
} VESCMotor;

/****VESC的can报文命令枚举体****/
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

#ifndef _DIANJI_H
#define _DIANJI_H

#include "stdbool.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "pid.h"
#include "utils.h"
#include "can2.h"
#include "led.h"
enum MOTOR_MODE
{
	duty = 0,
	current,
	RPM,
	brake,
	zero,
	position,
	vesc_postion_rpm,
	vesc_postion_I,
	vesc_postion_Current,
	vesc_RPM_I
};
enum MOTOR_TYPE
{
	M2006,
	M3508
};

/****电机限制保护结构体****/
typedef struct
{
	bool isPosLimitON;		//是否对最大位置进行限制，(轴前角度)
	float maxAngle;			//最大旋转角度
	bool isPosSPLimitOn;	//是否对位置模式进行限速
	u16 posSPlimit;			//位置模式下的速度限制
	bool isRealseWhenStuck; //是否堵转时释放电机
	s16 zeroSP;				//寻零模式下的速度
	u16 zeroCurrent;		//寻零模式下的最大电流
	bool Motorstuck;
	bool Motortimeout;
} MotorLimit;

/*电机内参*/
typedef struct
{
	u16 LINE;		   //编码器线数
	float RATIO;	   //减速比  m3508为19：1
	u16 CURRENT_LIMIT; //最大输出电流限制
	double GEARRATIO;  //外部齿轮减速比
} MotorParam;

/*电机外参*/
typedef struct
{
	volatile vs16 angle;	   //轴前角度
	volatile vs16 current;	   //当前电流
	volatile vs16 velocity;	   //速度
	volatile vs32 pulse;	   //累计脉冲
	volatile vs16 pulseread;   //反馈脉冲
	volatile vs16 temperature; //当前温度
} MotorValue;

/*电机状态*/
typedef struct
{
	bool arrived;	 //位置模式到达指定位置
	bool zero;		 //寻零模式下到达零点
	bool stuck;		 //电机堵转
	bool timeout;	 //can报文反馈超时
	bool ouverspeed; //电机超速
	bool isSetZero;	 //置此位置为零点
	bool clearFlag;	 //清除误差
} MotorStatus;

/*不需要关心的一些参数*/
typedef struct
{
	u32 maxPulse;	  //最大旋转脉冲（相当于最大旋转角度）
	vs32 lockPulse;	  //锁定当前位置为初始位置
	s16 distance;	  //当前反馈脉冲与前次反馈脉冲差值（相当于累计脉冲pulse）
	s32 subPulseSet;  //当前累计脉冲值与设定脉冲差值
	u16 zeroCnt;	  //寻零用计数值，（为寻零状态zero）
	u16 stuckCnt;	  //堵转用计数值，（堵转状态stuck）
	u8 timeoutCnt;	  //超时用计数值，（超时状态timeout）
	u32 lastRxTim;	  //上次接收到报文的系统时间
	u16 timeoutTicks; //判断超时用系统节拍数
	u16 stuckTicks;
} MotorArgum; //判断超时结构体

/*DJ电机总结构体*/
typedef struct
{
	bool enable;							  //电机使能
	bool begin;								  //电机启动
	u8 mode;								  //电机模式
	MotorValue valueSet, valueReal, valuePrv; //外参设定值
	MotorStatus status;						  //电机状态
	MotorLimit limit;						  //电机保护
	MotorArgum argum;						  //计算所需间值
	MotorParam intrinsic;					  //电机内参
	PID_SetTypeDef PIDx, PIDs;				  //PID参数
} DJmotor;

/*DJ电机标志*/
typedef struct
{
	bool um;
	bool enable;
	bool begin;
	bool angle;
	bool speed;
} DJflag;

extern MotorArgum motorargum;
extern MotorLimit motorlimit;
extern MotorParam M3508instrin, M2006instrin;
extern DJmotor motor[8];
extern DJflag Djflag;

void SetM3508_1(short ID1, short ID2, short ID3, short ID4);
void SetZero(DJmotor *motor);
void Motor_Init(void);
void Speed_mode(s16 id);
void Zero_mode(s16 id);
void Position_mode(s16 id);
void peakcurrent(void);
void pulse_caculate(s16 id);
void ifdjtimeout(u16 id);
u8 ifdjstuck(u16 id);

#endif

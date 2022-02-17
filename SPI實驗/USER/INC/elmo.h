#ifndef __ELMO_H
#define __ELMO_H
#include "stm32f4xx.h"
#include "stdbool.h"
#include "can2.h"
#include "macro.h"

//ELMO CAN报文ID
/****ELMO CAN报文ID****/
#define Elmo_Motor1_RX 0x281
#define Elmo_Motor2_RX 0x282
#define Elmo_Motor3_RX 0x283
#define Elmo_Motor4_RX 0x284
#define Elmo_Motor5_RX 0x285
#define Elmo_Motor6_RX 0x286
#define Elmo_Motor7_RX 0x287
#define Elmo_Motor8_RX 0x288
#define Elmo_Motor1_TX 0x301
#define Elmo_Motor2_TX 0x302
#define Elmo_Motor3_TX 0x303
#define Elmo_Motor4_TX 0x304
#define Elmo_Motor5_TX 0x305
#define Elmo_Motor6_TX 0x306
#define Elmo_Motor7_TX 0x307
#define Elmo_Motor8_TX 0x308
#define Elmo_Motor1_error 0x81
#define Elmo_Motor2_error 0x82
#define Elmo_Motor3_error 0x83
#define Elmo_Motor4_error 0x84
#define Elmo_Motor5_error 0x85
#define Elmo_Motor6_error 0x86
#define Elmo_Motor7_error 0x87
#define Elmo_Motor8_error 0x88

//ELMO内参
typedef struct
{
	u16 PULSE; 				//编码器线数
	float RATIO; 			//减速比
	float GEAR_RATIO;
}ELMOParam;

//ELMO外参
typedef struct
{
	volatile vs16 angle;		//轴前角度
	volatile vs16 current;	//电流
	volatile vs32 speed1;		//速度（rpm,计算转换为JV与SP）
	volatile vs32 pulse;		//累积脉冲
	volatile vs32 position; //绝对位置
	volatile vs32 AC;
	volatile vs32 DC;
}ELMOValue;

//电机状态
typedef struct
{
	bool arrived;			//位置模式下到达指定位置
	bool zero;				//寻零完成
	bool stuck;				//电机堵转
	bool timeout;			//can报文反馈超时
}ELMOStatus;

/***电机标志位***/
typedef struct
{
  bool bg;
  bool mo;
  bool pa;
  bool sp;
  bool sv;
  bool um;
  bool jv;
  bool st;
  bool px;
  bool mp;
  bool qt;
  bool qv;
  bool rm;
} ELMOflag;

//ELMO限制保护结构体
typedef struct 
{
	bool isPosLimitON;		//是否对最大位置进行限制
	float maxAngle;				//最大旋转角度
	bool isPosSPLimitON;	//是否对位置模式进行限速
	u16 PosSPlimit;				//位置模式下的速度限制
	bool ReleaseWhenStuck;//是否堵转时释放电机
	u16 zeroSP;						//寻零模式下的速度
	u16 zeroCurrent;			//寻零模式下的最大电流
}ELMOLimit;

//其他参数
typedef struct
{
	u32 maxPulse;					//最大旋转脉冲[最大旋转角度]
	u16 zeroCnt;					//寻零用计数值[相当于寻零状态zero]
	vu16 stuckCnt;				//堵转用计数值[相当于堵转状态stuck]
	u16 timeoutCnt;				//超时用计数值[相当于超市状态timeout]
	u32 lastRxTim;				//上次接收到报文的系统时间[相当于超时状态timeout]
	u16 timeoutTicks;			//判断超时用系统节拍数[相当于超时状态timeout]
	u8 timeout;
}ELMOArgum;

//ELMO总结构体
typedef struct
{
	u8 mode;														//电机模式
	bool begin;													//电机启动
	bool enable;												//电机使能
	ELMOValue valSet,valReal,valLast;		//外参设定值，实际值
	ELMOStatus status;									//电机状态
	ELMOLimit	 limit;										//限制保护
	ELMOArgum  argum;										//计算所用间值
	ELMOParam  intrinsic;								//电机内参
}Elmo_motor;

extern ELMOParam U10;
extern ELMOLimit ELMOlimit;
extern ELMOArgum ELMOargum;
extern Elmo_motor ELMOmotor[8];
extern s32 Motor_Emer_Code;

void ELMO_motor_Init(void);
void elmo_control(u32 id);
void ELMO_Init(CAN_TypeDef *CANx);
void ELMO_motor_UM(u32 ID,u8 UM, u8 InConGrpFlag);
void ELMO_motor_SV(u32 ID,u8 InConGrpFlag);
void ELMO_motor_AC(u32 ID, s32 data , u8 InConGrpFlag);
void ELMO_motor_DC(u32 ID, s32 data , u8 InConGrpFlag);
void ELMO_motor_Enable_OR_Disable(u32 ID,u8 enable_or_disable, u8 InConGrpFlag);
void ELMO_motor_BG(u32 ID, u8 InConGrpFlag);
void ELMO_motor_ST(u32 ID, u8 InConGrpFlag);
void ELMO_motor_SP(u32 ID,s32 SP,u8 InConGrpFlag);
void ELMO_motor_PA(u32 ID,s32 PA,u8 InConGrpFlag);
void ELMO_motor_PR(u32 ID,s32 PR,u8 InConGrpFlag);
void ELMO_motor_PX(u32 ID,s32 data, u8 InConGrpFlag);
void ELMO_motor_JV(u32 ID, u8 JV,u8 InConGrpFlag);
void ELMO_motor_TC(u32 ID,s32 TC, u8 InConGrpFlag);
void ELMO_motor_RM(u32 ID, s32 data , u8 InConGrpFlag);
void ELMO_motor_PV(u32 ID, u8 PV, u8 InConGrpFlag);
void ELMO_motor_MP(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag);
void ELMO_motor_QP(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag);
void ELMO_motor_QT(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag);
void ELMO_motor_QV(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag);
void ELMO_motor_PT(u32 ID, u8 PT, u8 InConGrpFlag);
void ELMO_motor_ASKvx(u32 ID,u8 InConGrpFlag);
void ELMO_motor_ASKiq(u32 ID, u8 InConGrpFlag);
void ELMO_motor_ASKpa(u32 ID,u8 InConGrpFlag);
void ELMO_motor_ASKac(u32 ID,u8 InConGrpFlag);
void ELMO_motor_ASKdc(u32 ID, u8 InConGrpFlag);
void ELMO_motor_ASKpx(u32 ID,u8 InConGrpFlag);
void ELMO_motor_ASKsp(u32 ID, u8 InConGrpFlag);
void ELMO_motor_ASKmo(u32 ID, u8 InConGrpFlag);
void ELMO_motor_ASKum(u32 ID,u8 InConGrpFlag);


#endif

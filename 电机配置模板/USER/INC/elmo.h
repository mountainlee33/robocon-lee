#ifndef __ELMO_H
#define __ELMO_H
#include "stm32f4xx.h"
#include "stdbool.h"
#include "can2.h"
#include "macro.h"

//ELMO CAN����ID
/****ELMO CAN����ID****/
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

//ELMO�ڲ�
typedef struct
{
	u16 PULSE; 				//����������
	float RATIO; 			//���ٱ�
	float GEAR_RATIO;
}ELMOParam;

//ELMO���
typedef struct
{
	volatile vs16 angle;		//��ǰ�Ƕ�
	volatile vs16 current;	//����
	volatile vs32 speed1;		//�ٶȣ�rpm,����ת��ΪJV��SP��
	volatile vs32 pulse;		//�ۻ�����
	volatile vs32 position; //����λ��
	volatile vs32 AC;
	volatile vs32 DC;
}ELMOValue;

//���״̬
typedef struct
{
	bool arrived;			//λ��ģʽ�µ���ָ��λ��
	bool zero;				//Ѱ�����
	bool stuck;				//�����ת
	bool timeout;			//can���ķ�����ʱ
}ELMOStatus;

/***�����־λ***/
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

//ELMO���Ʊ����ṹ��
typedef struct 
{
	bool isPosLimitON;		//�Ƿ�����λ�ý�������
	float maxAngle;				//�����ת�Ƕ�
	bool isPosSPLimitON;	//�Ƿ��λ��ģʽ��������
	u16 PosSPlimit;				//λ��ģʽ�µ��ٶ�����
	bool ReleaseWhenStuck;//�Ƿ��תʱ�ͷŵ��
	u16 zeroSP;						//Ѱ��ģʽ�µ��ٶ�
	u16 zeroCurrent;			//Ѱ��ģʽ�µ�������
}ELMOLimit;

//��������
typedef struct
{
	u32 maxPulse;					//�����ת����[�����ת�Ƕ�]
	u16 zeroCnt;					//Ѱ���ü���ֵ[�൱��Ѱ��״̬zero]
	vu16 stuckCnt;				//��ת�ü���ֵ[�൱�ڶ�ת״̬stuck]
	u16 timeoutCnt;				//��ʱ�ü���ֵ[�൱�ڳ���״̬timeout]
	u32 lastRxTim;				//�ϴν��յ����ĵ�ϵͳʱ��[�൱�ڳ�ʱ״̬timeout]
	u16 timeoutTicks;			//�жϳ�ʱ��ϵͳ������[�൱�ڳ�ʱ״̬timeout]
	u8 timeout;
}ELMOArgum;

//ELMO�ܽṹ��
typedef struct
{
	u8 mode;														//���ģʽ
	bool begin;													//�������
	bool enable;												//���ʹ��
	ELMOValue valSet,valReal,valLast;		//����趨ֵ��ʵ��ֵ
	ELMOStatus status;									//���״̬
	ELMOLimit	 limit;										//���Ʊ���
	ELMOArgum  argum;										//�������ü�ֵ
	ELMOParam  intrinsic;								//����ڲ�
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

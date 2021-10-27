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

/****������Ʊ����ṹ��****/
typedef struct
{
	bool isPosLimitON;		//�Ƿ�����λ�ý������ƣ�(��ǰ�Ƕ�)
	float maxAngle;			//�����ת�Ƕ�
	bool isPosSPLimitOn;	//�Ƿ��λ��ģʽ��������
	u16 posSPlimit;			//λ��ģʽ�µ��ٶ�����
	bool isRealseWhenStuck; //�Ƿ��תʱ�ͷŵ��
	s16 zeroSP;				//Ѱ��ģʽ�µ��ٶ�
	u16 zeroCurrent;		//Ѱ��ģʽ�µ�������
	bool Motorstuck;
	bool Motortimeout;
} MotorLimit;

/*����ڲ�*/
typedef struct
{
	u16 LINE;		   //����������
	float RATIO;	   //���ٱ�  m3508Ϊ19��1
	u16 CURRENT_LIMIT; //��������������
	double GEARRATIO;  //�ⲿ���ּ��ٱ�
} MotorParam;

/*������*/
typedef struct
{
	volatile vs16 angle;	   //��ǰ�Ƕ�
	volatile vs16 current;	   //��ǰ����
	volatile vs16 velocity;	   //�ٶ�
	volatile vs32 pulse;	   //�ۼ�����
	volatile vs16 pulseread;   //��������
	volatile vs16 temperature; //��ǰ�¶�
} MotorValue;

/*���״̬*/
typedef struct
{
	bool arrived;	 //λ��ģʽ����ָ��λ��
	bool zero;		 //Ѱ��ģʽ�µ������
	bool stuck;		 //�����ת
	bool timeout;	 //can���ķ�����ʱ
	bool ouverspeed; //�������
	bool isSetZero;	 //�ô�λ��Ϊ���
	bool clearFlag;	 //������
} MotorStatus;

/*����Ҫ���ĵ�һЩ����*/
typedef struct
{
	u32 maxPulse;	  //�����ת���壨�൱�������ת�Ƕȣ�
	vs32 lockPulse;	  //������ǰλ��Ϊ��ʼλ��
	s16 distance;	  //��ǰ����������ǰ�η��������ֵ���൱���ۼ�����pulse��
	s32 subPulseSet;  //��ǰ�ۼ�����ֵ���趨�����ֵ
	u16 zeroCnt;	  //Ѱ���ü���ֵ����ΪѰ��״̬zero��
	u16 stuckCnt;	  //��ת�ü���ֵ������ת״̬stuck��
	u8 timeoutCnt;	  //��ʱ�ü���ֵ������ʱ״̬timeout��
	u32 lastRxTim;	  //�ϴν��յ����ĵ�ϵͳʱ��
	u16 timeoutTicks; //�жϳ�ʱ��ϵͳ������
	u16 stuckTicks;
} MotorArgum; //�жϳ�ʱ�ṹ��

/*DJ����ܽṹ��*/
typedef struct
{
	bool enable;							  //���ʹ��
	bool begin;								  //�������
	u8 mode;								  //���ģʽ
	MotorValue valueSet, valueReal, valuePrv; //����趨ֵ
	MotorStatus status;						  //���״̬
	MotorLimit limit;						  //�������
	MotorArgum argum;						  //���������ֵ
	MotorParam intrinsic;					  //����ڲ�
	PID_SetTypeDef PIDx, PIDs;				  //PID����
} DJmotor;

/*DJ�����־*/
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

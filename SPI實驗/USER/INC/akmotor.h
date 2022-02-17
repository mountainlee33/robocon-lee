#ifndef __AK80_H_
#define __AK80_H_

#include "stm32f4xx.h"
#include "stdbool.h"
#include "can2.h"
#include "queue.h"

#define CMD_MOTOR_MODE 0x01
#define CMD_RESET_MODE 0x02
#define CMD_ZERO_POSITION 0x03

typedef struct
{
	u16 PULSE;        //����������
	float RATIO;
  float GEAR_RATIO; //��μ��ٱ�
}AKParam;

//���״̬
typedef struct
{
		bool arrived;//λ��ģʽ�µ���ָ��λ��
    bool zero;//Ѱ�����
    bool stuck;//�����ת
    bool timeout;//can���ķ�����ʱ
    bool overspeed;//����
    bool isSetZero;//�Ƿ����õ�ǰλ��Ϊ���
}AKStatus;

typedef struct
{
  volatile float angle;  //�Ƕ�
  volatile float torque; //Ť��
  volatile float speed;  //�ٶȣ�rpm, �������ʱת��ΪJVֵ��SPֵ��
} AKValue;

typedef struct
{
  u8 mode; //���ģʽ
	bool enable;
	bool begin;
  AKValue valSet, valReal; //����趨ֵ��ʵ��ֵ
  AKParam intrinsic;       //����ڲ�
	AKStatus status;
	vs16 kp;								//λ�û�
  vs16 kd;								//�ٶȻ�
} AK_Motor;

extern AK_Motor AKmotor[5];
void AK_motor_Init(void);
static float uint_to_float(int x_int, float x_min, float x_max, int bits);
static int float_to_uint(float x, float x_min, float x_max, int bits);
void AK_Transmit(u8 id, float location, float vel, float f_kp, float f_kd, float curr_lim);
void TmotorCaculate(u8 id, AK_Motor *motor);
void reply_message(CanRxMsg rx_message);
void Enter_Motor_Control(u8 id,u8 cmd);

#endif

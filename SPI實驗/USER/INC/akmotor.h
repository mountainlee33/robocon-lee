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
	u16 PULSE;        //编码器线数
	float RATIO;
  float GEAR_RATIO; //外参减速比
}AKParam;

//电机状态
typedef struct
{
		bool arrived;//位置模式下到达指定位置
    bool zero;//寻零完成
    bool stuck;//电机堵转
    bool timeout;//can报文反馈超时
    bool overspeed;//超速
    bool isSetZero;//是否设置当前位置为零点
}AKStatus;

typedef struct
{
  volatile float angle;  //角度
  volatile float torque; //扭矩
  volatile float speed;  //速度（rpm, 后面计算时转换为JV值与SP值）
} AKValue;

typedef struct
{
  u8 mode; //电机模式
	bool enable;
	bool begin;
  AKValue valSet, valReal; //外参设定值，实际值
  AKParam intrinsic;       //电机内参
	AKStatus status;
	vs16 kp;								//位置环
  vs16 kd;								//速度环
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

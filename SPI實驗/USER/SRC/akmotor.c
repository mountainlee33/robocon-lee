#include "akmotor.h"

AKParam AK80;
AK_Motor AKmotor[5];
void AK_motor_Init(void)
{
	AK80.RATIO = 6.0f;		
	AK80.GEAR_RATIO = 1.0f; 
	AK80.PULSE = 4096;		

	AKmotor[0].intrinsic = AK80;
	AKmotor[0].valSet.torque = 0.f;
	AKmotor[0].valSet.speed = 0.f;
	AKmotor[0].valSet.angle = 0.f;
	AKmotor[0].enable = DISABLE;
	AKmotor[0].begin = false;
	AKmotor[0].kp = 100.f;
	AKmotor[0].kd = 1.f;

	AKmotor[1].intrinsic = AK80;
	AKmotor[1].valSet.torque = 0.f;
	AKmotor[1].valSet.speed = 0.f;
	AKmotor[1].valSet.angle = 0.f;
	AKmotor[1].enable = DISABLE;
	AKmotor[1].begin = false;
	AKmotor[1].kp = 100.f;
	AKmotor[1].kd = 1.f;

	AKmotor[2].intrinsic = AK80;
	AKmotor[2].valSet.torque = 0.f;
	AKmotor[2].valSet.speed = 0.f;
	AKmotor[2].valSet.angle = 0.f;
	AKmotor[2].enable = DISABLE;
	AKmotor[2].begin = false;
	AKmotor[2].kp = 100.f;
	AKmotor[2].kd = 1.f;

	AKmotor[3].intrinsic = AK80;
	AKmotor[3].valSet.torque = 0.f;
	AKmotor[3].valSet.speed = 0.f;
	AKmotor[3].valSet.angle = 0.f;
	AKmotor[3].enable = DISABLE;
	AKmotor[3].begin = false;
	AKmotor[3].kp = 100.f;
	AKmotor[3].kd = 1.f;

	AKmotor[4].intrinsic = AK80;
	AKmotor[4].valSet.torque = 0.f;
	AKmotor[4].valSet.speed = 0.f;
	AKmotor[4].valSet.angle = 0.f;
	AKmotor[4].enable = DISABLE;
	AKmotor[4].begin = false;
	AKmotor[4].kp = 100.f;
	AKmotor[4].kd = 1.f;

	for (int i = 0; i < 5; i++)
	{
		Enter_Motor_Control(i, CMD_MOTOR_MODE);
		OSTimeDly(500);
	}
}


static float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}


static int float_to_uint(float x, float x_min, float x_max, int bits)
{
	float span = x_max - x_min;
	float offset = x_min;
	return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

void Enter_Motor_Control(u8 id, u8 cmd)
{
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].ID = 0x000 + id;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].DLC = 8;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[0] = 0XFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[1] = 0XFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[2] = 0XFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[3] = 0XFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[4] = 0XFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[5] = 0XFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[6] = 0XFF;
	switch (cmd)
	{
	case CMD_MOTOR_MODE: 
		AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[7] = 0XFC;
		break;
	case CMD_RESET_MODE: 
		AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[7] = 0XFD;
		break;
	case CMD_ZERO_POSITION: 
		AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[7] = 0XFE;
		break;
	default:
		return;
	}
}

void AK_Transmit(u8 id, float angle, float vel, float f_kp, float f_kd, float tor)
{
	u16 p, v, kp, kd, torque;
	angle *= (float)PI/180*AKmotor[id-1].intrinsic.GEAR_RATIO*AKmotor[id-1].intrinsic.RATIO;
	vel *= (float)PI/180*AKmotor[id-1].intrinsic.GEAR_RATIO*AKmotor[id-1].intrinsic.RATIO;
	
	p = float_to_uint(angle, -95.5, 95.5, 16);//»¡¶È
	v = float_to_uint(vel, -30, 30, 12);
	kp = float_to_uint(f_kp, 0, 500, 12);
	kd = float_to_uint(f_kd, 0, 5, 12);
	torque = float_to_uint(tor, -18, 18, 12);

	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].ID = 0x000 + id;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].DLC = 8;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[0] = p >> 8;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[1] = p & 0xFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[2] = v >> 4;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[3] = ((v & 0xF) << 4) | (kp >> 8);
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[4] = kp & 0xFF;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[5] = kd >> 4;
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[6] = ((kd & 0xF) << 4) | (torque >> 8);
	AK_Sendqueue.Can_DataSend[AK_Sendqueue.Rear].Data[7] = torque & 0xff;

	AK_Sendqueue.Rear = Rear4;
}


void TmotorCaculate(u8 id, AK_Motor *motor)
{
	motor->valSet.angle = motor->valSet.angle * motor->intrinsic.RATIO * (float)PI / 180.f;
	if (motor->status.isSetZero)
	{
		motor->status.isSetZero = false;
		Enter_Motor_Control(id, CMD_ZERO_POSITION);
		motor->valReal.angle = 0;
	}
}


void reply_message(CanRxMsg rx_message)
{
	u8 id = rx_message.Data[0];
	u16 pos_reply = (rx_message.Data[1] << 8) | rx_message.Data[2];
	u16 vel_reply = (rx_message.Data[3] << 4) | (rx_message.Data[4] >> 4);
	u16 tor_reply = (rx_message.Data[4] & 0x0f) << 8 | rx_message.Data[5];

	float p = uint_to_float(pos_reply, -95.5f, 95.5f, 16);
	float v = uint_to_float(vel_reply, -30.f, 30.f, 12);

	AKmotor[id].valReal.torque = uint_to_float(tor_reply, -18.f, 18.f, 12);
	AKmotor[id].valReal.speed = 180.f * v / (float)PI / (AKmotor[id].intrinsic.RATIO * AKmotor[id].intrinsic.GEAR_RATIO);
	AKmotor[id].valReal.angle = 180.f * p / (float)PI / (AKmotor[id].intrinsic.RATIO * AKmotor[id].intrinsic.GEAR_RATIO);
}

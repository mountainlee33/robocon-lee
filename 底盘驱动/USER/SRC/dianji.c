#include "dianji.h"

MotorArgum motorargum;
MotorLimit motorlimit;
MotorParam M3508instrin, M2006instrin;
DJmotor motor[8];
SET_ALL_TypeDef MOTORAll;

//���������ʼ��
void Motor_Init(void)
{
	//����ڲ�
	M3508instrin.LINE = 8192;
	M3508instrin.GEARRATIO = 67 / 17.f; //ȫ�ֱ���
	M3508instrin.RATIO = 19.2f;
	M3508instrin.CURRENT_LIMIT = 14745; //14745

	M2006instrin.LINE = 8192;
	M2006instrin.RATIO = 36.0f;
	M2006instrin.CURRENT_LIMIT = 9000; //9000
	M2006instrin.GEARRATIO = 1.0f;	   //6.117f;    //��γ�����

	//������Ʊ�������
	motorlimit.isPosLimitON = false;
	motorlimit.maxAngle = 1800; //��������+-0.5pi
	motorlimit.isPosSPLimitOn = true;
	motorlimit.posSPlimit = 19500;
	motorlimit.isRealseWhenStuck = true;
	motorlimit.zeroCurrent = 2000;
	motorlimit.zeroSP = -800;
	motorlimit.Motorstuck = 1;//�򿪶�ת���

	//���������������
	motorargum.timeoutTicks = 40;
	motorargum.lockPulse = 0;
	motorargum.stuckTicks = 1000; //1000ms

	/*M2006��1�������ʼ��*/
	motor[0].intrinsic = M2006instrin;
	motor[0].enable = DISABLE;
	motor[0].begin = false;
	motor[0].mode = position; //λ��ģʽ
	motor[0].valueSet.current = 50;
	motor[0].valueSet.angle = 0;
	motor[0].valueSet.velocity = 100;
	motor[0].limit = motorlimit;
	PID_Init(&motor[0].PIDs, 3.5, 0.12, 0, 0.4, motor[0].valueSet.velocity);
	PID_Init(&motor[0].PIDx,  5, 0.3, 0.2, 1, motor[0].valueSet.pulse);

	/*M2006��2�������ʼ��*/
	motor[1].intrinsic = M2006instrin;
	motor[1].enable = DISABLE;
	motor[1].begin = false;
	motor[1].mode = position; //λ��ģʽ
	motor[1].valueSet.current = 50;
	motor[1].valueSet.angle = 0;
	motor[1].valueSet.velocity = 100;
	motor[1].limit = motorlimit;
	PID_Init(&motor[1].PIDs, 3.5, 0.12, 0, 0.4, motor[1].valueSet.velocity);
	PID_Init(&motor[1].PIDx, 5, 0.3, 0.2, 1, motor[1].valueSet.pulse);

	/*M2006��3�������ʼ��*/
	motor[2].intrinsic = M2006instrin;
	motor[2].enable = DISABLE;
	motor[2].begin = false;
	motor[2].mode = position; //λ��ģʽ
	motor[2].valueSet.current = 50;
	motor[2].valueSet.angle = 0;
	motor[2].valueSet.velocity = 100;
	motor[2].limit = motorlimit;
	PID_Init(&motor[2].PIDs, 3.5, 0.12, 0, 0.4, motor[2].valueSet.velocity);
	PID_Init(&motor[2].PIDx, 5, 0.3, 0.2, 1, motor[2].valueSet.pulse);

	/*M2006��4�������ʼ��*/
	motor[3].intrinsic = M2006instrin;
	motor[3].enable = DISABLE;
	motor[3].begin = false;
	motor[3].mode = position; //λ��ģʽ
	motor[3].valueSet.current = 50;
	motor[3].valueSet.angle = 0;
	motor[3].valueSet.velocity = 1000;
	motor[3].limit = motorlimit;
	PID_Init(&motor[3].PIDs,3.5, 0.12, 0, 0.4, motor[3].valueSet.velocity);
	PID_Init(&motor[3].PIDx,5, 0.3, 0.2, 1, motor[3].valueSet.pulse);

	for (int i = 0; i < 4; i++)
	{
		motor[i].argum = motorargum;
		motor[i].argum.maxPulse = motor[i].limit.maxAngle * motor[i].intrinsic.GEARRATIO * motor[i].intrinsic.RATIO * motor[i].intrinsic.LINE / 360.f;
		motor[i].valueSet.pulse = motor[i].valueSet.angle * motor[i].intrinsic.GEARRATIO * motor[i].intrinsic.RATIO * motor[i].intrinsic.LINE / 360.f;
	}

	for (int i = 0; i < 4; i++)
	{
		SetZero(&motor[i]);
		motor[i].limit.posSPlimit = 19500; //λ��ģʽ�µ��ٶ�����
	}
}
//�ٶ�ģʽ
void Speed_mode(s16 id)
{
	motor[id].PIDs.SetVal = motor[id].PIDx.KS * motor[id].valueSet.velocity;
	motor[id].PIDs.CurVal = motor[id].valueReal.velocity;
	PID_Operation(&motor[id].PIDs);
	motor[id].valueSet.current += motor[id].PIDs.Udlt;
}
//���õ�ǰλ��Ϊ���
void SetZero(DJmotor *motor)
{
	motor->status.isSetZero = false;
	motor->valueReal.pulse = 0;
	motor->valueReal.angle = 0;
}
//Ѱ��ģʽ
void Zero_mode(s16 id)
{
	motor[id].PIDs.SetVal = motor[id].limit.zeroSP;
	motor[id].PIDs.CurVal = motor[id].valueReal.velocity;
	PID_Operation(&motor[id].PIDs);
	motor[id].valueSet.current += motor[id].PIDs.Udlt;
	if ((motor[id].argum.distance == 0) && ABS(motor[id].valueReal.pulse) > 2500)
		motor[id].argum.zeroCnt++;
	else
		motor[id].argum.zeroCnt = 0;
	if (motor[id].argum.zeroCnt > 50) //Ѱ����ɣ���ʼ����㣬ת��Ϊλ��ģʽ�����
	{
		motor[id].argum.zeroCnt = 0;
		motor[id].valueReal.angle = 0;
		motor[id].valueReal.pulse = 0;
		motor[id].mode = position;
		motor[id].status.zero = 1;
		motor[id].valueSet.angle = 0;
		motor[id].enable = true;
	}
}

//λ��ģʽ
void Position_mode(s16 id)
{
	motor[id].valueSet.pulse = motor[id].valueSet.angle * motor[id].intrinsic.GEARRATIO * motor[id].intrinsic.RATIO * motor[id].intrinsic.LINE / 360.f;
	motor[id].PIDx.SetVal = motor[id].valueSet.pulse;
	if (motor[id].begin == 0)
		motor[id].PIDx.SetVal = motor[id].argum.lockPulse; //���Ϊ�����״̬��λ���趨���Σ���Ϊ��λ��
														   //  if (motor[id].limit.isPosLimitON)
														   //    PEAK(motor[id].PIDx.SetVal, motor[id].argum.maxPulse);
	motor[id].PIDx.CurVal = motor[id].valueReal.pulse;
	PID_Operation(&motor[id].PIDx);
	motor[id].PIDs.SetVal = motor[id].PIDx.KS * motor[id].PIDx.Udlt;
	if (motor[id].limit.isPosSPLimitOn)
		PEAK(motor[id].PIDs.SetVal, motor[id].limit.posSPlimit);
	motor[id].PIDs.CurVal = motor[id].valueReal.velocity;
	PID_Operation(&motor[id].PIDs);
	motor[id].valueSet.current += motor[id].PIDs.Udlt;
	if (ABS(motor[id].argum.subPulseSet) < 180)
		motor[id].status.arrived = true; //����ָ��λ��
	else
		motor[id].status.arrived = false;
}

//λ�ü���
void pulse_caculate(s16 id)
{
	motor[id].argum.distance = motor[id].valueReal.pulseread - motor[id].valuePrv.pulseread;
	motor[id].valuePrv = motor[id].valueReal;
	if (ABS(motor[id].argum.distance) > 4150) //last edit : 4085 ;
		motor[id].argum.distance -= SIG(motor[id].argum.distance) * motor[id].intrinsic.LINE;
	motor[id].valueReal.pulse += motor[id].argum.distance;																								  //�ۼ��������
	motor[id].argum.subPulseSet = motor[id].valueSet.pulse - motor[id].valueReal.pulse;																	  //�������
	motor[id].valueReal.angle = motor[id].valueReal.pulse * 360.f / motor[id].intrinsic.RATIO / motor[id].intrinsic.GEARRATIO / motor[id].intrinsic.LINE; //�����

	if (motor[id].begin)
		motor[id].argum.lockPulse = motor[id].valueReal.pulse;
	/* �ж��Ƿ���Ҫ������� */
	if (motor[id].status.isSetZero)
		SetZero(&motor[id]);
}

u8 ifdjstuck(u16 id)
{
	if (motor[id].enable == 0)
		return 1;//δʹ�ܿɷ�����
	if (motorlimit.Motorstuck)
	{
		if (motor[id].mode == position)
		{
			if (ABS(motor[id].valueReal.pulse - motor[id].valuePrv.pulse) < 200)
			{
				if (ABS(motor[id].valueReal.velocity) < 100 && (motor[id].status.arrived == false))
				{
					motor[id].argum.stuckCnt++;
				}
				else
				{
					motor[id].argum.stuckCnt = 0;
				}
				if (motor[id].argum.stuckCnt > motor[id].argum.stuckTicks)
				{
					motor[id].status.stuck = 1; //��ת��
				}
				else
					motor[id].status.stuck = 0;
				if (motor[id].status.stuck == 1)
				{
					BEEP_ON;
					motor[id].enable = 0;//ʹ��ͬʱ����
					return 1;
				}
				else
					return 0;
			}
		}
	}
	return 0;
}

void ifdjtimeout(u16 id)
{
	if (motor[id].enable == 1)
	{
		if (motor[id].limit.Motortimeout)
		{
			if (OSTimeGet() - last_update_time[id] >= 1500)
			{
				motor[id].argum.timeoutCnt++;
			}
			else
			{
				motor[id].argum.timeoutCnt = 0;
			}
			if (motor[id].argum.timeoutCnt > motor[id].argum.timeoutTicks)
			{
				motor[id].status.timeout = true;
				LED_RED_ON;
			}
			else
			{
				motor[id].status.timeout = 0;
			}
		}
	}
}
void SetM3508_1(short ID1, short ID2, short ID3, short ID4) //����1-4���
{
	unsigned short can_id = 0x200;
	CanTxMsg tx_message;

	tx_message.IDE = CAN_ID_STD;   //��׼֡
	tx_message.RTR = CAN_RTR_DATA; //����֡
	tx_message.DLC = 0x08;		   //8λ

	tx_message.StdId = can_id;
	if ( motor[0].enable == 1)// !ifdjstuck(0) &&
	{
		tx_message.Data[0] = (ID1 & 0xFF00) >> 8;
		tx_message.Data[1] = ID1 & 0xFF; //ID 1
	}
	else
	{
		tx_message.Data[0] = 0;
		tx_message.Data[1] = 0;
	}
	if (  motor[1].enable == 1)//!ifdjstuck(1) &&
	{
		tx_message.Data[2] = (ID2 & 0xFF00) >> 8;
		tx_message.Data[3] = ID2 & 0xFF; //ID 1
	}
	else
	{
		tx_message.Data[2] = 0;
		tx_message.Data[3] = 0;
	}
	if ( motor[2].enable == 1)// !ifdjstuck(2) &&
	{
		tx_message.Data[4] = (ID3 & 0xFF00) >> 8;
		tx_message.Data[5] = ID3 & 0xFF; //ID 1
	}
	else
	{
		tx_message.Data[4] = 0;
		tx_message.Data[5] = 0;
	}
	if (  motor[3].enable == 1)//!ifdjstuck(3) &&
	{
		tx_message.Data[6] = (ID4 & 0xFF00) >> 8;
		tx_message.Data[7] = ID4 & 0xFF; //ID 1
	}
	else
	{
		tx_message.Data[6] = 0;
		tx_message.Data[7] = 0;
	}
	CAN_Transmit(CAN2, &tx_message);
}

void peakcurrent(void)
{
	for (u8 id = 0; id < 8; id++)
		PEAK(motor[id].valueSet.current, motor[id].intrinsic.CURRENT_LIMIT);
}

void Control_all_MOTOR(void)
{
	for(u8 i =0;i<4;i++)
	{
		motor[i].begin = MOTORAll.set_all_begin;
		motor[i].enable = MOTORAll.set_all_enable;
		motor[i].valueSet.velocity = MOTORAll.set_all_speed;
		motor[i].valueSet.angle = MOTORAll.set_all_angle;
		motor[i].limit.posSPlimit = MOTORAll.set_all_speedlimit;
	}
}

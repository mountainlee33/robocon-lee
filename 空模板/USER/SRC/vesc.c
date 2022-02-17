#include "vesc.h"

VESCParam SUNNYSKY, VESC_U10, VESC_U8;
VESCLimit VESClimit;
VESCArgum VESCargum;
VESCMotor VESCmotor[4];
SET_ALL_TypeDef VESCAll;
extern bool stop_flag;

static float angle_gap = 90.f;
float dt = 1 / 20000.0f; //ASK:

void VESC_Init(void)
{
	//电机内参
	SUNNYSKY.POLE_PAIRS = 7;
	SUNNYSKY.MAX_CURRENT = 60.0f;
	SUNNYSKY.MAX_RPM = 10000;
	VESC_U10.POLE_PAIRS = 21;
	VESC_U10.MAX_RPM = 10000;
	VESC_U8.POLE_PAIRS = 21;
	VESC_U8.MAX_CURRENT = 10.0f;
	VESC_U8.MAX_RPM = 1000;

	//电机限制保护
	VESClimit.isPosLimitOn = false;
	VESClimit.maxPosition = 30000;
	VESClimit.isPosSpLimitOn = true;
	VESClimit.posSpLimit = 5000;
	VESClimit.MaxCurrentSet = 48.f;
	VESCargum.stuckTicks = 100;//100ms
	VESClimit.stuckmotion = 1;//开启堵转判断
	VESClimit.timeoutmotion = 1; //开启超时判断
	
	VESCargum.timeoutTicks = 40;


	//间值参数设置
	VESCargum.firstPos = true;
	
	VESCAll.set_all_begin = 0;
	VESCAll.set_all_enable = 0;
	VESCAll.set_all_speed = 0;

	/******0号电机初始化******/
	VESCmotor[0].instrinsic = VESC_U10;
	VESCmotor[0].enable = DISABLE;
	VESCmotor[0].begin = false;
	VESCmotor[0].mode = RPM;
	VESCmotor[0].ValSet.current = 0.0f;
	VESCmotor[0].ValSet.speed = 0;
	VESCmotor[0].ValSet.duty = 0;
	VESCmotor[0].ValSet.position = 0;
	PID_Init(&VESCmotor[0].PIDx, 3.84, 0.64, 0.54, 1, VESCmotor[0].ValSet.position);
	PID_Init(&VESCmotor[0].PIDs, 0.007, 0.001, 1, 1, VESCmotor[0].ValSet.speed);
	VESC_PID_Init(&VESCmotor[0].p_pid, 0.03, 0, 0.0004, 0.2);
	VESC_PID_Init(&VESCmotor[0].s_pid, 0.001, 0.0001, 0.000001, 0.2);

	/******1号电机初始化******/
	VESCmotor[1].instrinsic = VESC_U10;
	VESCmotor[1].enable = DISABLE;
	VESCmotor[1].begin = false;
	VESCmotor[1].mode = RPM;
	VESCmotor[1].ValSet.current = 0.0f;
	VESCmotor[1].ValSet.speed = 0;
	VESCmotor[1].ValSet.duty = 0;
	VESCmotor[1].ValSet.position = 0;
	PID_Init(&VESCmotor[1].PIDx, 3.84, 0.64, 0.54, 1, VESCmotor[0].ValSet.position);
	PID_Init(&VESCmotor[1].PIDs, 0.007, 0.001, 1, 1, VESCmotor[0].ValSet.speed);
	VESC_PID_Init(&VESCmotor[1].p_pid, 0.03, 0, 0.0004, 0.2);
	VESC_PID_Init(&VESCmotor[1].s_pid, 0.001, 0.0001, 0.000001, 0.2);

	/******2号电机初始化******/
	VESCmotor[2].instrinsic = VESC_U10;
	VESCmotor[2].enable = DISABLE;
	VESCmotor[2].begin = false;
	VESCmotor[2].mode = RPM;
	VESCmotor[2].ValSet.current = 0.0f;
	VESCmotor[2].ValSet.speed = 0;
	VESCmotor[2].ValSet.duty = 0;
	VESCmotor[2].ValSet.position = 0;
	PID_Init(&VESCmotor[2].PIDx, 3.84, 0.64, 0.54, 1, VESCmotor[0].ValSet.position);
	PID_Init(&VESCmotor[2].PIDs, 0.007, 0.001, 1, 1, VESCmotor[0].ValSet.speed);
	VESC_PID_Init(&VESCmotor[2].p_pid, 0.03, 0, 0.0004, 0.2);
	VESC_PID_Init(&VESCmotor[2].s_pid, 0.001, 0.0001, 0.000001, 0.2);

	/******3号电机初始化******/
	VESCmotor[3].instrinsic = VESC_U10;
	VESCmotor[3].enable = DISABLE;
	VESCmotor[3].begin = false;
	VESCmotor[3].mode = RPM;
	VESCmotor[3].ValSet.current = 0.0f;
	VESCmotor[3].ValSet.speed = 0;
	VESCmotor[3].ValSet.duty = 0;
	VESCmotor[3].ValSet.position = 0;
	PID_Init(&VESCmotor[3].PIDx, 3.84, 0.64, 0.54, 1, VESCmotor[0].ValSet.position);
	PID_Init(&VESCmotor[3].PIDs, 0.007, 0.001, 1, 1, VESCmotor[0].ValSet.speed);
	VESC_PID_Init(&VESCmotor[3].p_pid, 0.03, 0, 0.0004, 0.2);
	VESC_PID_Init(&VESCmotor[3].s_pid, 0.001, 0.0001, 0.000001, 0.2);

	for (int i = 0; i < 4; i++)
	{
		VESC_Setzero(&VESCmotor[i]);
		VESCmotor[i].limit = VESClimit;
		VESCmotor[i].argum = VESCargum;
	}
}

//开始时，位置清零
void VESC_Setzero(VESCMotor *motor)
{
	if (motor->status.isSetZero)
	{
		motor->status.isSetZero = false;
		motor->ValReal.position = 0;
	}
}

//vesc自带位置模式，上位机参数，给pos跑angle
void VESC_position_mode_pos(u8 id)
{
	//反转最大允许15000转速
	if (SIG(VESCmotor[id].argum.difPosition) == SIG(VESCmotor[id].ValReal.speed)) //TODO:速度保护，正转角度越大，反转速度会越小
		angle_gap = 170.f;
	else
		angle_gap = 90.f;
	if (ABS(VESCmotor[id].argum.difPosition) > (angle_gap * 10))
	{
		VESCmotor[id].ValSet.angle = VESCmotor[id].ValReal.angle + SIG(VESCmotor[id].argum.difPosition) * angle_gap;
	}
	else
	{
		if(stop_flag == 0)
		{
		float difpos = VESCmotor[id].argum.difPosition / 1e1f;
		VESCmotor[id].ValSet.angle = VESCmotor[id].ValReal.angle + difpos;
		}
	}
	utils_norm_angle(&VESCmotor[id].ValSet.angle);
	VESC_Set_Pos(id + 1, VESCmotor[id].ValSet.angle, 0);
}

//设定VESC电流
void VESC_Set_Current(u8 controller_ID, float current, u8 InConGrpFlag)
{
	int32_t send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (int32_t)(current * 1000), &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

//设定VESC转速
void VESC_Set_Speed(u8 controller_ID, s32 speed, u8 InConGrpFlag)
{
	s32 send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, speed, &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | (u32)CAN_PACKET_SET_RPM << 8;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

//设定VESC角度
void VESC_Set_Ang(u8 controller_ID, float angle, u8 InConGrpFlag)
{
	s32 send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (s32)(angle * 1e6f), &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((u32)CAN_PACKET_SET_POS << 8);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

//设定VESC占空比
void VESC_Set_Duty_Cycle(u8 controller_ID, float duty_cycle, u8 InConGrpFlag)
{
	s32 send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (s32)(duty_cycle * 1e6f), &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((u32)CAN_PACKET_SET_DUTY << 8);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

//设定VESC位置
void VESC_Set_Pos(u8 controller_ID, float pos, u8 InConGrpFlag)
{
	s32 send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (s32)(pos * 1e6f), &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((u32)CAN_PACKET_SET_POS << 8);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

//设定VESC刹车电流
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 InConGrpFlag)
{
	s32 send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (s32)(brake_current * 1e3f), &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((u32)CAN_PACKET_SET_CURRENT_BRAKE);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

//设定VESC锁机电流
void VESC_Set_HandBrake_Current(u8 controller_ID, float handbrake_current, u8 InConGrpFlag)
{
	s32 send_index = 0;

	buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (s32)(handbrake_current * 1e3f), &send_index);
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | (u32)CAN_PACKET_SET_CURRENT_HANDBRAKE;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
	VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	VESC_Sendqueue.Rear = Rear3;

	VESCmotor[controller_ID - 1].argum.timeout = 1;
	VESCmotor[controller_ID - 1].argum.lastRxTim = OSTimeGet();
}

u8 ifvescstuck(u8 controllerid)//本杰明堵转
{
	if (VESCmotor[controllerid - 1].enable == 0)
		return 1;
	if(VESCmotor[controllerid - 1].enable == 1)
	{
	if (VESClimit.timeoutmotion)
	{
		if (VESCmotor[controllerid - 1].mode == position)
		{
				if (VESCmotor[controllerid - 1].ValReal.current > VESClimit.MaxCurrentSet)
				{
					VESCmotor[controllerid - 1].argum.stuckCnt++;
				}
				else
				{
					VESCmotor[controllerid - 1].argum.stuckCnt = 0;
				}
				if (VESCmotor[controllerid - 1].argum.stuckCnt > VESCmotor[controllerid - 1].argum.stuckTicks)
				{
					VESCmotor[controllerid - 1].status.stuck = 1;
				}
				else
				{
					VESCmotor[controllerid - 1].status.stuck = 0;
				}
				if (VESCmotor[controllerid - 1].status.stuck == 1)
				{
					BEEP_ON;
					VESC_Set_Current(controllerid,0,0);
					Led8_show(controllerid);
					return 0;
				}
				else
					return 1;
			}
		}
	}
	return 1;
}

void ifVESCtimeout(u16 id)
{
	if (VESCmotor[id].enable == 1)
	{
		if (VESCmotor[id].limit.timeoutmotion)
		{
			if (OSTimeGet() - VESCmotor[id].argum.lastRxTim >= 1500 )
			{
				VESCmotor[id].argum.timeoutCnt++;
			}
			else
				VESCmotor[id].argum.timeoutCnt = 0;
			if (VESCmotor[id].argum.timeoutCnt > VESCmotor[id].argum.timeoutTicks)
				VESCmotor[id].status.timeout = true;
			else
				VESCmotor[id].status.timeout = false;

			if(VESCmotor[0].status.timeout == 0 && VESCmotor[1].status.timeout == 0 && VESCmotor[2].status.timeout == 0 )//&& VESCmotor[3].status.timeout == 0
			{
				Led8_show(0);	
			}				
		}
	}
}
void Control_all_VESC(void)
{
	for(u8 i =0;i<4;i++)
	{
		ifvescstuck(i+1);
		VESCmotor[i].begin = VESCAll.set_all_begin;
		VESCmotor[i].enable = VESCAll.set_all_enable;
		VESCmotor[i].ValSet.speed = VESCAll.set_all_speed;
	}
}

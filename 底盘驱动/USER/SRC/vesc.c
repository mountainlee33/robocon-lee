#include "vesc.h"

VESCParam SUNNYSKY, VESC_U10, VESC_U8;
VESCLimit VESClimit;
VESCArgum VESCargum;
VESCMotor VESCmotor[4];

static float angle_gap = 90.f;
float dt = 1 / 20000.0f; //ASK:

void VESC_Init(void)
{
	//电机内参
	SUNNYSKY.POLE_PAIRS = 7;
	SUNNYSKY.MAX_CURRENT = 60.0f;
	SUNNYSKY.MAX_RPM = 10000;
	VESC_U10.POLE_PAIRS = 12;
	VESC_U10.MAX_RPM = 10000;
	VESC_U8.POLE_PAIRS = 21;
	VESC_U8.MAX_CURRENT = 10.0f;
	VESC_U8.MAX_RPM = 1000;

	//电机限制保护
	VESClimit.isPosLimitOn = false;
	VESClimit.maxPosition = 30000;
	VESClimit.isPosSpLimitOn = true;
	VESClimit.posSpLimit = 5000;
	VESClimit.MaxCurrentSet = 30.f;
	VESCargum.stuckTicks = 100;//100ms

	//间值参数设置
	VESCargum.firstPos = true;

	/******0号电机初始化******/
	VESCmotor[0].instrinsic = VESC_U10;
	VESCmotor[0].enable = DISABLE;
	VESCmotor[0].begin = false;
	VESCmotor[0].mode = duty;
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
	VESCmotor[1].mode = duty;
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
	VESCmotor[2].mode = duty;
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
	VESCmotor[3].mode = duty;
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
		VESC_calculate(&VESCmotor[i]);
		VESCmotor[i].limit = VESClimit;
		VESCmotor[i].argum = VESCargum;
	}
}

//开始时，位置清零
void VESC_calculate(VESCMotor *motor)
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
		float difpos = VESCmotor[id].argum.difPosition / 1e1f;
		VESCmotor[id].ValSet.angle = VESCmotor[id].ValReal.angle + difpos;
	}
	utils_norm_angle(&VESCmotor[id].ValSet.angle);
	VESC_Set_Pos(id + 1, VESCmotor[id].ValSet.angle, 0);
}

//速度模式——通过电流模式实现
void VESC_RPM_mode_I(u8 id)
{ //返回值有什么作用
	utils_truncate_number_abs_s32((s32 *)&VESCmotor[id].ValSet.speed, VESCmotor[id].instrinsic.MAX_RPM);
	//1.VESC自身PID--直接计算法，对D单独低通滤波
	VESCmotor[id].s_pid.SetVal = VESCmotor[id].ValSet.speed;
	VESC_PID_Operation(&VESCmotor[id].s_pid, VESCmotor[id].ValSet.speed, VESCmotor[id].ValReal.speed);
	VESCmotor[id].ValSet.current = VESCmotor[id].s_pid.output * VESCmotor[id].instrinsic.MAX_CURRENT;
	//2.增量法
	//	static s32 error1=0,error2=0,error3=0;
	//	static float Udlt;
	//	static float p_term,i_term,d_term;
	//
	//	VESCmotor[id].PIDs.SetVal = VESCmotor[id].ValSet.speed;
	//	VESCmotor[id].PIDs.CurVal = VESCmotor[id].ValReal.speed;
	//
	//	error1 = VESCmotor[id].PIDs.SetVal - VESCmotor[id].PIDs.CurVal;
	//	p_term = VESCmotor[id].PIDs.KP * (error1 - error2);
	//	i_term = VESCmotor[id].PIDs.KI * error1;
	//	d_term = VESCmotor[id].PIDs.KD * (error1 + error3 - error2);
	//
	//	Udlt = p_term + i_term + d_term;
	//	Udlt *= VESCmotor[id].PIDs.KS;
	//	error3 = error2;
	//	error2 = error1;
	//	VESCmotor[id].ValSet.current += Udlt;

	utils_truncate_number_abs((float *)&VESCmotor[id].ValSet.current, VESCmotor[id].limit.MaxCurrentSet);
	VESC_Set_Current(id + 1, VESCmotor[id].ValSet.current, 0);
}

//位置模式——通过电流模式实现
void VESC_position_mode_I(u8 id)
{
	static float i_term_pos = 0;
	static float prv_angle_error = 0;
	float p_term;
	float d_term;

	//Compute parameters
	if (SIG(VESCmotor[id].argum.difPosition) == SIG(VESCmotor[id].ValReal.speed))
		angle_gap = 170.f; //无法给到180
	else
		angle_gap = 90.f;
	if (ABS(VESCmotor[id].argum.difPosition) > (angle_gap * 10)) //为什么乘10
	{
		VESCmotor[id].ValSet.angle = VESCmotor[id].ValReal.angle + SIG(VESCmotor[id].argum.difPosition) * angle_gap;
	}
	else
	{
		float difpos = VESCmotor[id].argum.difPosition / 1e1f; //实型数据的指数形式  1e1f表示1×101，其实就是10
		VESCmotor[id].ValSet.angle = VESCmotor[id].ValReal.angle + difpos;
	}
	utils_norm_angle(&VESCmotor[id].ValSet.angle);
	float angle_error = utils_angle_difference(VESCmotor[id].ValSet.angle, VESCmotor[id].ValReal.angle); //角度差
	if (VESCmotor[id].Direction_Inverted)
		angle_error = -angle_error;

	p_term = angle_error * VESCmotor[id].p_pid.kp;
	i_term_pos += angle_error * (VESCmotor[id].p_pid.ki * dt);

	static float dt_int = 0.0f;
	dt_int += dt;
	if (angle_error == prv_angle_error) //TODO:速度太低了它可能没有检测出来位置更新，也就是说前后误差不变。如果这样的话会丢失掉这段位置。
										//所以在一个大的时间段内进行微分。
										//如果正常运行的话那么这个微分累计时间就是微分时间不起作用
	{
		d_term = 0.0f;
	}
	else
	{
		d_term = (angle_error - prv_angle_error) * (VESCmotor[id].p_pid.kd / dt_int);
		dt_int = 0;
	}

	//滤波器
	static float d_filter_pos = 0.0f;
	UTILS_LP_FAST(d_filter_pos, d_term, VESCmotor[id].p_pid.kd_filter);
	d_term = d_filter_pos;

	// I-term wind-up protection
	utils_truncate_number_abs(&p_term, 1.0f);
	utils_truncate_number_abs(&i_term_pos, 1.0f - fabsf(p_term)); //fabsf求绝对值

	//储存之前的误差
	prv_angle_error = angle_error;

	//计算输出
	float output = p_term + i_term_pos + d_term;
	utils_truncate_number(&output, -1.0f, 1.0f);

	VESCmotor[id].ValSet.current = output * VESCmotor[id].instrinsic.MAX_CURRENT;
	PEAK(VESCmotor[id].ValSet.current, VESCmotor[id].limit.MaxCurrentSet);
	VESC_Set_Current(id + 1, VESCmotor[id].ValSet.current, 0);
}

//位置模式--通过速度模式实现
void VESC_position_mode_rpm(u8 id)
{
	VESCmotor[id].PIDx.SetVal = VESCmotor[id].ValSet.position;
	if (!VESCmotor[id].begin)
		VESCmotor[id].PIDx.SetVal = VESCmotor[id].argum.lockPosition;
	if (VESCmotor[id].limit.isPosLimitOn)
		PEAK(motor->PIDx.SetVal, VESCmotor[id].limit.maxPosition); //使用dj的motor的结构体
	VESCmotor[id].PIDx.CurVal = VESCmotor[id].ValReal.position;
	PID_Operation(&VESCmotor[id].PIDx);
	VESCmotor[id].PIDx.Udlt = IIR_Filter(&IIR_filt, VESCmotor[id].PIDx.Udlt); //微分作用很容易引进高频干扰，加低通滤波可以抑制高频影响
	if (VESCmotor->limit.isPosSpLimitOn)
		PEAK(VESCmotor[id].PIDx.Udlt, VESCmotor->limit.posSpLimit);
	if (ABS(VESCmotor[id].argum.difPosition) < 60)
		VESCmotor[id].status.arrived = true;
	else
		VESCmotor[id].status.arrived = false;
	VESC_Set_Speed(id + 1, VESCmotor[id].PIDx.Udlt * VESCmotor[id].instrinsic.POLE_PAIRS, 0);
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
	if (VESClimit.timeoutmotion)
	{
		if (VESCmotor[controllerid - 1].mode == RPM)
		{
			if (VESCmotor[controllerid - 1].ValReal.speed != 0)
			{
				if (ABS(VESCmotor[controllerid - 1].ValReal.speed) < 20) //电机速度小于阈值
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
					Beep_Show(1); //响一下即本杰明堵转
					VESC_Set_Current(controllerid,0,0);
					return 1;
				}
				else
					return 0;
			}
		}
	}
}
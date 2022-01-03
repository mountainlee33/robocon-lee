#include "odrive.h"

Odrv_Limit odrivelimit;
Odrv_Param U10_Param, V4004_Param;
Odrv_motor odrv_motor[4];
Odrv_Argum odriveargum;
//odrive的查询信息必须使用远程帧
//电机参数初始化
void Odrv_motor_Init(void)
{
    //电机内参
    U10_Param.Gear_ratio = 1;
    U10_Param.Pole_pairs = 21;
    U10_Param.CURRENT_limit = 35;
    U10_Param.VELOCITY_limit = 3000; //?待测

    V4004_Param.Gear_ratio = 1;
    V4004_Param.Pole_pairs = 12;
    V4004_Param.CURRENT_limit = 30;
    V4004_Param.VELOCITY_limit = 2000; //?待测

    //电机限制保护设置
    odrivelimit.PosSpLimitON = false;
    odrivelimit.maxAngle = 1800;
    odrivelimit.isPosLimitOn = true;
    odrivelimit.PosSpLimit = 2000;

    //电机其他参数设置
    odriveargum.stuckTicks = 1000;
    odriveargum.timeoutTicks = 40;
    //电机初始化
    odrv_motor[0].Param = U10_Param;
    odrv_motor[0].state = AXIS_STATE_UNDEFINED;
    odrv_motor[0].mode = position;
    odrv_motor[0].input_mode = INPUT_MODE_INACTIVE;
    odrv_motor[0].Value_Set.current = 0;
    odrv_motor[0].Value_Set.angle = 0;
    odrv_motor[0].Value_Set.velocity = 0;
    odrv_motor[0].limit = odrivelimit;

    odrv_motor[1].Param = U10_Param;
    odrv_motor[1].state = AXIS_STATE_UNDEFINED;
    odrv_motor[1].mode = position;
    odrv_motor[1].input_mode = INPUT_MODE_INACTIVE;
    odrv_motor[1].Value_Set.current = 0;
    odrv_motor[1].Value_Set.angle = 0;
    odrv_motor[1].Value_Set.velocity = 0;
    odrv_motor[1].limit = odrivelimit;

    odrv_motor[2].Param = U10_Param;
    odrv_motor[2].state = AXIS_STATE_UNDEFINED;
    odrv_motor[2].mode = position;
    odrv_motor[2].input_mode = INPUT_MODE_INACTIVE;
    odrv_motor[2].Value_Set.current = 0;
    odrv_motor[2].Value_Set.angle = 0;
    odrv_motor[2].Value_Set.velocity = 0;
    odrv_motor[2].limit = odrivelimit;

    odrv_motor[3].Param = U10_Param;
    odrv_motor[3].state = AXIS_STATE_UNDEFINED;
    odrv_motor[3].mode = position;
    odrv_motor[3].input_mode = INPUT_MODE_INACTIVE;
    odrv_motor[3].Value_Set.current = 0;
    odrv_motor[3].Value_Set.angle = 0;
    odrv_motor[3].Value_Set.velocity = 0;
    odrv_motor[3].limit = odrivelimit;

    for (int i = 0; i < 4; i++)
    {
        odrv_motor[i].Argum = odriveargum;
        odrv_motor[i].limit.PosSpLimit = 2000;
    }
}

//设置主轴模式(闭环控制、编码器整定)
void Odrv_AxisState(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x007;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x04;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = odrv_motor[id - 1].state;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

//输入模式(Input_mode)
void Odrv_Input_Mode(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x00B;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;
    switch (odrv_motor[id - 1].mode)
    {
    case position:
        Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 3;
    case RPM:
        Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 2;
    }
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = odrv_motor[id - 1].input_mode;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0;
    Odrv_Sendqueue.Rear = Rear5;
}

//重启驱动器
void Reboot(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x016;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x04;//对格式无限制
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

//清除错误
void clear_Error(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x018;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

void Show_Errors(CanRxMsg Rrx_message)
{
    if ((Rrx_message.StdId = 0x403) | (Rrx_message.StdId = 0x203))
    {
        DecodeS32Data(&odriveargum.Encoder_Error, &Rrx_message.Data[0]);
    }
    if ((Rrx_message.StdId = 0x404) | (Rrx_message.StdId = 0x204))
    {
        DecodeS32Data(&odriveargum.Motor_Error, &Rrx_message.Data[0]);
    }
}
//查询电机报错
void Check_MotorError(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x003;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

//查询编码器报错
void Check_Encoder_Error(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x004;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

//查询电流
void Odrv_motor_ASKiq(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x014;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

//查询速度、位置
void Odrv_motor_ASKvel_pos(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x009;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[1] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[2] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[3] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0;

    Odrv_Sendqueue.Rear = Rear5;
}

//输入位置
void Input_pos(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID = id << 9 | 0x00C;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;

		EcodeFloatData_to_4byte(&odrv_motor[id - 1].Value_Set.angle,&Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0]);

    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0x00;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0x00;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0x00;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0x00;

    Odrv_Sendqueue.Rear = Rear5;
}

//输入速度
void Input_Vel(u16 id)
{
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].ID =  id << 9 | 0x000D;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].DLC = 0x08;

    EcodeFloatData_to_4byte(&odrv_motor[id - 1].Value_Set.velocity, &Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[0]);

    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[4] = 0x00;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[5] = 0x00;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[6] = 0x00;
    Odrv_Sendqueue.Can_DataSend[Odrv_Sendqueue.Rear].Data[7] = 0x00;

    Odrv_Sendqueue.Rear = Rear5;
}

void Odrv_query(u16 id)
{
    if (odrv_motor[id - 1].Status.checkErrors == 1)
    {
        Check_Encoder_Error(id);
        Check_MotorError(id);
        odrv_motor[id - 1].Status.checkErrors = 0;
    }
    if (odrv_motor[id - 1].Status.clearErrors == 1)
    {
        clear_Error(id);
        odrv_motor[id - 1].Status.clearErrors = 0;
    }
    if (odrv_motor[id - 1].Status.ask_condition == 1)
    {
        Odrv_motor_ASKvel_pos(id);
        Odrv_motor_ASKiq(id);
    }
    if (odrv_motor[id - 1].Status.change_input == 1)
    {
        Odrv_Input_Mode(id);
			  odrv_motor[id - 1].Status.change_input = 0 ;
    }
    if (odrv_motor[id - 1].Status.change_axis == 1)
    {
        Odrv_AxisState(id);
			  odrv_motor[id - 1].Status.change_axis = 0;
    }
}

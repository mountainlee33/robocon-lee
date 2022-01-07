#include "elmo.h"
#include "queue.h"

ELMOParam U10, EC_4P_30,AK_80;
ELMOLimit ELMOlimit;
ELMOArgum ELMOargum;
Elmo_motor ELMOmotor[8];
ELMOflag ELMOFlag;
u32 speed1;
u8 Um;
u8 enable_or_disable;
s32 Pa;
s32 Jv;
s32 Px;
s32 Motor_Emer_Code = (0x1 << 28) | (0 << 16) | (0 << 8) | 0;

//电机参数初始化
void ELMO_motor_Init(void)
{
	{ //电机内参
		U10.PULSE = 4096;
		U10.RATIO = 5.0f;
		U10.GEAR_RATIO = 1.0f;
		EC_4P_30.PULSE = 1000;
		EC_4P_30.RATIO = 169.0f / 9;
		AK_80.RATIO = 6.0f;		
		AK_80.GEAR_RATIO = 1.0f; 
		AK_80.PULSE = 4096;
	}
	{ //电机限制保护
		ELMOlimit.isPosLimitON = false;
		ELMOlimit.maxAngle = 720.f;
		ELMOlimit.isPosSPLimitON = true;
		ELMOlimit.PosSPlimit = 800;
		ELMOlimit.ReleaseWhenStuck = true;
		ELMOlimit.zeroSP = 500;
		ELMOlimit.zeroCurrent = 4000;
	}
	{ //电机其他参数设置
		ELMOargum.timeoutTicks = 500;
	}
	/****0号电机初始化****/
	ELMOmotor[0].intrinsic = EC_4P_30;
	ELMOmotor[0].enable = DISABLE;
	ELMOmotor[0].begin = DISABLE;
	ELMOmotor[0].mode = position;
	ELMOmotor[0].valSet.angle = 0;
	ELMOmotor[0].valSet.speed1 = 100;
	ELMOmotor[0].valSet.current = 100;
	ELMOmotor[0].valSet.pulse = 0;

	/****1号电机初始化****/
	ELMOmotor[1].intrinsic = U10;
	ELMOmotor[1].enable = DISABLE;
	ELMOmotor[1].begin = DISABLE;
	ELMOmotor[1].mode = position;
	ELMOmotor[1].valSet.angle = 300;
	ELMOmotor[1].valSet.speed1 = 100;
	ELMOmotor[1].valSet.current = 100;
	ELMOmotor[1].valSet.pulse = 0;

	/****2号电机初始化****/
	ELMOmotor[2].intrinsic = U10;
	ELMOmotor[2].enable = DISABLE;
	ELMOmotor[2].begin = DISABLE;
	ELMOmotor[2].mode = position;
	ELMOmotor[2].valSet.angle = 300;
	ELMOmotor[2].valSet.speed1 = 100;
	ELMOmotor[2].valSet.current = 100;
	ELMOmotor[2].valSet.pulse = 0;

	/****3号电机初始化****/
	ELMOmotor[3].intrinsic = U10;
	ELMOmotor[3].enable = DISABLE;
	ELMOmotor[3].begin = DISABLE;
	ELMOmotor[3].mode = position;
	ELMOmotor[3].valSet.angle = 300;
	ELMOmotor[3].valSet.speed1 = 100;
	ELMOmotor[3].valSet.current = 100;
	ELMOmotor[3].valSet.pulse = 0;

	/****4号电机初始化****/
	ELMOmotor[4].intrinsic = U10;
	ELMOmotor[4].enable = DISABLE;
	ELMOmotor[4].mode = position;
	ELMOmotor[4].valSet.angle = 300;
	ELMOmotor[4].valSet.speed1 = 100;
	ELMOmotor[4].valSet.current = 100;
	ELMOmotor[4].valSet.pulse = 0;

	/****5号电机初始化****/
	ELMOmotor[5].intrinsic = U10;
	ELMOmotor[5].enable = DISABLE;
	ELMOmotor[5].begin = DISABLE;
	ELMOmotor[5].mode = position;
	ELMOmotor[5].valSet.angle = 300;
	ELMOmotor[5].valSet.speed1 = 100;
	ELMOmotor[5].valSet.current = 100;
	ELMOmotor[5].valSet.pulse = 0;

	/****6号电机初始化****/
	ELMOmotor[6].intrinsic = U10;
	ELMOmotor[6].enable = DISABLE;
	ELMOmotor[6].begin = DISABLE;
	ELMOmotor[6].mode = position;
	ELMOmotor[6].valSet.angle = 300;
	ELMOmotor[6].valSet.speed1 = 100;
	ELMOmotor[6].valSet.current = 100;
	ELMOmotor[6].valSet.pulse = 0;

	/****7号电机初始化****/
	ELMOmotor[7].intrinsic = U10;
	ELMOmotor[7].enable = DISABLE;
	ELMOmotor[7].begin = DISABLE;
	ELMOmotor[7].mode = position;
	ELMOmotor[7].valSet.angle = 300;
	ELMOmotor[7].valSet.speed1 = 100;
	ELMOmotor[7].valSet.current = 100;
	ELMOmotor[7].valSet.pulse = 0;

	for (int i = 0; i < 8; i++)
	{
		ELMOmotor[i].limit = ELMOlimit;
		ELMOmotor[i].argum = ELMOargum;
	}
}

//can初始化
void ELMO_Init(CAN_TypeDef *CANx)
{
	CanTxMsg tx_message;
	tx_message.StdId = 0x000;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.DLC = 8;
	tx_message.Data[0] = 0x01;
	tx_message.Data[1] = 0x00;
	tx_message.Data[2] = 0x00;
	tx_message.Data[3] = 0x00;
	tx_message.Data[4] = 0x00;
	tx_message.Data[5] = 0x00;
	tx_message.Data[6] = 0x00;
	tx_message.Data[7] = 0x00;
	CAN_Transmit(CANx, &tx_message);
}

//设置模式
void ELMO_motor_UM(u32 ID, u8 UM, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'U';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'M';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4] = UM;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7] = 0;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//保存
void ELMO_motor_SV(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'S';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'V';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//加速度
void ELMO_motor_AC(u32 ID, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'A';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'C';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//减速度
void ELMO_motor_DC(u32 ID, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'D';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'C';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//使能或失能
void ELMO_motor_Enable_OR_Disable(u32 ID, u8 enable_or_disable, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'M';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'O';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4] = enable_or_disable;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//开始运动
void ELMO_motor_BG(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'B';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'G';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//制动
void ELMO_motor_ST(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'S';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'T';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//位置模式下持续速度
void ELMO_motor_SP(u32 ID, s32 speed1, u8 InConGrpFlag)
{

	s32 S_P;
	S_P = ELMOmotor[ID - 1].intrinsic.PULSE * 4 * speed1 / 60;

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'S';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&S_P, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置绝对位置
void ELMO_motor_PA(u32 ID, s32 PA, u8 InConGrpFlag)
{

	s32 P_A;
	P_A = ELMOmotor[ID - 1].intrinsic.PULSE * 4 * PA * ELMOmotor[ID - 1].intrinsic.RATIO * ELMOmotor[ID - 1].intrinsic.GEAR_RATIO / 360.f;

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'A';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&P_A, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置相对位置
void ELMO_motor_PR(u32 ID, s32 PR, u8 InConGrpFlag)
{

	s32 P_R;
	P_R = ELMOmotor[ID - 1].intrinsic.PULSE * 4 * PR * ELMOmotor[ID - 1].intrinsic.RATIO / 360.f;

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'R';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&P_R, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置相对位置
void ELMO_motor_PX(u32 ID, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'X';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//速度模式持续速度
void ELMO_motor_JV(u32 ID, u8 JV, u8 InConGrpFlag)
{

	s32 J_V;
	J_V = ELMOmotor[ID - 1].intrinsic.PULSE * ELMOmotor[ID].valSet.speed1 / 60.f;

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'J';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'V';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&J_V, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//转矩命令
void ELMO_motor_TC(u32 ID, s32 TC, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'T';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'C';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&TC, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//PVT启动前的参考模式
void ELMO_motor_RM(u32 ID, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'R';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'M';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//PVT模式
void ELMO_motor_PV(u32 ID, u8 PV, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'V';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4] = PV;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置PVT数组
void ELMO_motor_MP(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'M';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = Sub;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置位置数组
void ELMO_motor_QP(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'Q';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = Sub;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置时间数组
void ELMO_motor_QT(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'Q';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'T';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = Sub;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//设置速度数组
void ELMO_motor_QV(u32 ID, u8 Sub, s32 data, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'Q';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'V';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = Sub;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	EncodeS32Data(&data, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//PT模式
void ELMO_motor_PT(u32 ID, u8 PT, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'T';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4] = PT;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询实际转速
void ELMO_motor_ASKvx(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'V';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'X';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询电流
void ELMO_motor_ASKiq(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'I';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'Q';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询位置
void ELMO_motor_ASKpa(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'A';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询加速度
void ELMO_motor_ASKac(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'A';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'C';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询减速度
void ELMO_motor_ASKdc(u32 ID, u8 InConGrpFlag)
{
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'D';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'C';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询相对位置
void ELMO_motor_ASKpx(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'X';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询位置转速
void ELMO_motor_ASKsp(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'S';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'P';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询使能
void ELMO_motor_ASKmo(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'M';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'O';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

//查询模式
void ELMO_motor_ASKum(u32 ID, u8 InConGrpFlag)
{

	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + ID;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0X04;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 'U';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1] = 'M';
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3] = 0;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;

	Can2_Sendqueue.Rear = Rear2;

	ELMOmotor[ID - 1].argum.timeout = 1;
	ELMOmotor[ID - 1].argum.lastRxTim = OSTimeGet();
}

void elmo_control(u32 id)
{
	if (ELMOFlag.um)
	{
		ELMO_motor_UM(id, Um, 0);
		ELMOFlag.um = 0;
	}
	if (ELMOFlag.mo)
	{
		ELMO_motor_Enable_OR_Disable(id, enable_or_disable, 0);
		ELMOFlag.mo = 0;
	}
	if (ELMOFlag.bg)
	{
		ELMO_motor_BG(id, 0);
		ELMOFlag.bg = 0;
	}
	//Delay_ms(50);
	if (ELMOFlag.jv)
	{
		ELMO_motor_JV(id, Jv, 0);
		ELMOFlag.jv = 0;
	}
	if (ELMOFlag.pa)
	{
		ELMO_motor_PA(id, Pa, 0);
		ELMOFlag.pa = 0;
	}
	if (ELMOFlag.sp)
	{
		ELMO_motor_SP(id, speed1, 0);
		ELMOFlag.sp = 0;
	}
	if (ELMOFlag.st)
	{
		ELMO_motor_ST(id, 0);
		ELMOFlag.st = 0;
	}
	if (ELMOFlag.sv)
	{
		ELMO_motor_SV(id, 0);
		ELMOFlag.sv = 0;
	}
	if (ELMOFlag.px)
	{
		ELMO_motor_PX(id, Px, 0);
		ELMOFlag.px = 0;
	}
}

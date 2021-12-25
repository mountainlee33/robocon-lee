#include "odrive.h"

void Set_Requested_State(u16 id,u16 cmd,bool sts)
{
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = (id << 5) + cmd;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	if(sts)
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x08;//进入闭环控制模式
	else
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x01;
	
	Can2_Sendqueue.Rear = Rear2;
}

void Input_pos(float pos,u16 id,u16 cmd)
{
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = (id<<5)+cmd;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;
	
	EcodeFloatData_to_4byte(&pos, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]);
	
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0xFF;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0xFF;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0xFF;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0xFF;

	Can2_Sendqueue.Rear = Rear2;
}

void Input_vel(float vel,u16 id,u16 cmd)
{
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = (id<<5) + cmd;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x08;

	EcodeFloatData_to_4byte(&vel, &Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]);
	
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0xFF;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0xFF;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0xFF;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0xFF;	

	Can2_Sendqueue.Rear = Rear2;

}

void ask_pos(u16 id,u16 cmd)
{
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = (id<<5) + cmd;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = 0x01;
	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0] = 0x00;
	
	Can2_Sendqueue.Rear = Rear2;
}




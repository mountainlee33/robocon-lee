#include "pid.h"

Data_ProStr Cooridnate_X = {0};
Data_ProStr Cooridnate_Y = {0};
Data_ProStr Coo_Angle = {0};

float PID_Para[8][5] = {0};

void PID_Init(void)
{
	PID_Para[0][KP1] = 1;
	PID_Para[0][KP2] = 2.9;
	PID_Para[0][KI2] = 0.002;
	PID_Para[0][KD2] = 0;
	PID_Para[0][LIM] = 110;

	PID_Para[1][KP1] = 1;
	PID_Para[1][KP2] = 2.9;
	PID_Para[1][KI2] = 0.002;
	PID_Para[1][KD2] = 0;
	PID_Para[1][LIM] = 110;	
	
	Cooridnate_X.Par_Group = 0;
	Cooridnate_X.Proc_Flag = STANDARD_;

	Cooridnate_Y.Par_Group = 1;
	Cooridnate_Y.Proc_Flag = STANDARD_;

	Coo_Angle.Par_Group = 2;
	Coo_Angle.Proc_Flag = STANDARD_;
}

void PID_TAN_fil_Operation(Data_ProStr *Data)
{
	
}
#ifndef _ODRIVE
#define _ODRIVE

#include "queue.h"
#include "numchange.h"

//CAN通讯协议
#define CMD6 0x006;//设置主轴节点ID
#define CMD7 0x007;//设置主轴位置状态
#define CMD9 0x009;//主轴位置和速度信息
#define CMDB 0x00B;//主轴输入模式和控制模式
#define CMDC 0x00C;//输入位置信息
#define CMDD 0x00D;//输入速度信息
#define CMDF 0x00F;//输入最大电流和速度限制
#define CMD14 0X014;//查询电流

enum AXIS_STATE{
AXIS_STATE_UNDEFINED =0,//未定义
AXIS_STATE_IDLE =1,//失能
AXIS_STATE_STARTUP_SEQUENCE =2,//进入开始程序
AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,
AXIS_STATE_MOTOR_CALIBRATION = 4,
AXIS_STATE_ENCODER_INDEX_SEARCH  = 6,
AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7,
AXIS_STATE_CLOSED_LOOP_CONTROL = 8,//进入闭环控制
AXIS_STATE_LOCKIN_SPIN = 9,
AXIS_STATE_ENCODER_DIR_FIND =10,
AXIS_STATE_HOMING =11,
AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION= 12,
AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION =13
};

void ask_pos(u16 id,u16 cmd);
void Input_vel(float vel,u16 id,u16 cmd);
void Input_pos(float pos,u16 id,u16 cmd);
void Set_Requested_State(u16 id,u16 cmd,bool sts);



#endif

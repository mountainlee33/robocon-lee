#ifndef _ODRIVE
#define _ODRIVE

#include "queue.h"
#include "numchange.h"

//CANͨѶЭ��
#define CMD6 0x006;//��������ڵ�ID
#define CMD7 0x007;//��������λ��״̬
#define CMD9 0x009;//����λ�ú��ٶ���Ϣ
#define CMDB 0x00B;//��������ģʽ�Ϳ���ģʽ
#define CMDC 0x00C;//����λ����Ϣ
#define CMDD 0x00D;//�����ٶ���Ϣ
#define CMDF 0x00F;//�������������ٶ�����
#define CMD14 0X014;//��ѯ����

enum AXIS_STATE{
AXIS_STATE_UNDEFINED =0,//δ����
AXIS_STATE_IDLE =1,//ʧ��
AXIS_STATE_STARTUP_SEQUENCE =2,//���뿪ʼ����
AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,
AXIS_STATE_MOTOR_CALIBRATION = 4,
AXIS_STATE_ENCODER_INDEX_SEARCH  = 6,
AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7,
AXIS_STATE_CLOSED_LOOP_CONTROL = 8,//����ջ�����
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

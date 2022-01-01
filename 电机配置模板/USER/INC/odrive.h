#ifndef _ODRIVE
#define _ODRIVE

#include "queue.h"
#include "numchange.h"
#include "dianji.h"

enum Axis_State
{
    AXIS_STATE_UNDEFINED = 0,
    AXIS_STATE_IDLE,
    AXIS_STATE_STARTUP_SEQUENCE,
    AXIS_STATE_FULL_CALIBRATION_SEQUENCE,
    AXIS_STATE_MOTOR_CALIBRATION,
    AXIS_STATE_SENSORLESS_CONTROL, //无感控制
    AXIS_STATE_ENCODER_INDEX_SEARCH,
    AXIS_STATE_ENCODER_OFFSET_CALIBRATION,
    AXIS_STATE_CLOSED_LOOP_CONTROL, //闭环控制
    AXIS_STATE_LOCKIN_SPIN,
    AXIS_STATE_ENCODER_DIR_FIND,
    AXIS_STATE_HOMING,
};

enum Input_Mode
{
    INPUT_MODE_INACTIVE = 0, //失能
    INPUT_MODE_PASSTHROUGH,  //直驱
    INPUT_MODE_VEL_RAMP,     //直线加速模式
    INPUT_MODE_POS_FILTER,   //滤波位置模式
    INPUT_MODE_MIX_CHANNELS,
    INPUT_MODE_TRAP_TRAJ, //梯形位置模式
    INPUT_MODE_TORQUE_RAMP,
    INPUT_MODE_MIRROR, //镜像模式
    INPUT_MODE_Tuning,
};

enum MotorErrors
{
    ERROR_NONE = 0x00000000,
    ERROR_PHASE_RESISTANCE_OUT_OF_RANGE = 0x00000001,
    ERROR_PHASE_INDUCTANCE_OUT_OF_RANGE = 0x00000002,
    ERROR_ADC_FAILED = 0x00000004,
    ERROR_DRV_FAULT = 0x00000008,
    ERROR_CONTROL_DEADLINE_MISSED = 0x00000010,
    ERROR_NOT_IMPLEMENTED_MOTOR_TYPE = 0x00000020,
    ERROR_BRAKE_CURRENT_OUT_OF_RANGE = 0x00000040,
    ERROR_MODULATION_MAGNITUDE = 0x00000080,
    ERROR_BRAKE_DEADTIME_VIOLATION = 0x00000100,
    ERROR_UNEXPECTED_TIMER_CALLBACK = 0x00000200,
    ERROR_CURRENT_SENSE_SATURATION = 0x00000400,
    ERROR_CURRENT_LIMIT_VIOLATION = 0x00001000,
    ERROR_BRAKE_DUTY_CYCLE_NAN = 0x00002000,
    ERROR_DC_BUS_OVER_REGEN_CURRENT = 0x00004000,
    ERROR_DC_BUS_OVER_CURRENT = 0x00008000,
};

enum AxisErrors
{
    ERROR_NONE1 = 0x00000000, //无报错
    ERROR_INVALID_STATE = 0x00000001,
    ERROR_DC_BUS_UNDER_VOLTAGE = 0x00000002,
    ERROR_DC_BUS_OVER_VOLTAGE = 0x00000004,         //过压
    ERROR_CURRENT_MEASUREMENT_TIMEOUT = 0x00000008, //电流检测超时
    ERROR_BRAKE_RESISTOR_DISARMED = 0x00000010,
    ERROR_MOTOR_DISARMED = 0x00000020,
    ERROR_MOTOR_FAILED = 0x00000040, //电机连接失败
    ERROR_SENSORLESS_ESTIMATOR_FAILED = 0x00000080,
    ERROR_ENCODER_FAILED = 0x00000100,    //编码器连接失败
    ERROR_CONTROLLER_FAILED = 0x00000200, //控制器连接失败
    ERROR_POS_CTRL_DURING_SENSORLESS = 0x00000400,
    ERROR_WATCHDOG_TIMER_EXPIRED = 0x00000800,
    ERROR_MIN_ENDSTOP_PRESSED = 0x00001000,
    ERROR_MAX_ENDSTOP_PRESSED = 0x00002000,
    ERROR_ESTOP_REQUESTED = 0x00004000,
    ERROR_HOMING_WITHOUT_ENDSTOP = 0x00020000,
    ERROR_OVER_TEMP = 0x00040000, //过温
};

enum EncoderErrors
{
    ERROR_NONE2 = 0x00000000,
    ERROR_UNSTABLE_GAIN = 0x00000001,
    ERROR_CPR_POLEPAIRS_MISMATCH = 0x00000002,
    ERROR_NO_RESPONSE = 0x00000004,
    ERROR_UNSUPPORTED_ENCODER_MODE = 0x00000008,
    ERROR_ILLEGAL_HALL_STATE = 0x00000010,
    ERROR_INDEX_NOT_FOUND_YET = 0x00000020,
    ERROR_ABS_SPI_TIMEOUT = 0x00000040,
    ERROR_ABS_SPI_COM_FAIL = 0x00000080,
    ERROR_ABS_SPI_NOT_READY = 0x00000100,
    ERROR_HALL_NOT_CALIBRATED_YET = 0x00000200
};

//电机限制保护结构体
typedef struct
{
    bool PosSpLimitON; //对位置模式进行限速
    u16 PosSpLimit;    //位置模式速度maximum
    float maxAngle;    //最大旋转角度
    bool isPosLimitOn; //是否对最大位置进行限制
    bool Motorstuck;
    bool Motortimeout;
} Odrv_Limit;

//电机内参
typedef struct
{
    u16 Pole_pairs;     //电机极对数
    double Gear_ratio;  //外部齿轮减速比
    u16 CURRENT_limit;  //最大输出电流限制
    u16 VELOCITY_limit; //最大速度限制
} Odrv_Param;

//电机外参
typedef struct
{
    volatile float angle;    //轴前角度
    volatile float current;  //当前电流
    volatile float velocity; //当前速度
} Odrv_Value;

//电机状态
typedef struct
{
    bool arrived;       //位置模式达到指定位置
    bool stuck;         //电机堵转
    bool overspeed;     //电机超速
    bool isSetZero;     //置此位置为零点
    bool clearErrors;   //清除误差
    bool checkErrors;   //查询错误
    bool ask_condition; //查询电流、速度、位置
    bool change_input;  //调整输入模式
    bool change_axis;   //调整主轴状态
} Odrv_Status;

//不必要参数
typedef struct
{
    vs32 lockPos;     //锁定当前位置为初始位置
    vu16 stuckCnt;    //堵转用计数值
    u8 timeoutCnt;    //超时用计数值
    u16 timeoutTicks; //判断超时用系统节拍数
    u16 stuckTicks;   //判断堵转用系统节拍数
    u8 Motor_Error;   //电机报错内容
    u8 Encoder_Error; //编码器报错内容
} Odrv_Argum;

//odrive电机总结构体
typedef struct
{
    u8 state;                         //进入控制模式
    u8 mode;                          //电机模式
    u8 input_mode;                    //电机具体操控模式
    Odrv_Status Status;               //电机状态
    Odrv_Param Param;                 //电机内参
    Odrv_Value Value_Set, Value_Real; //电机外参
    Odrv_Argum Argum;                 //检测指标
    Odrv_Limit limit;                 //限制因素
} Odrv_motor;

void query(u8 id);
void Input_Vel(u8 id);
void Input_pos(u8 id);
void Odrv_motor_ASKvel_pos(u8 id);
void Odrv_motor_ASKiq(u8 id);
void Check_Encoder_Error(u8 id);
void Check_MotorError(u8 id);
void clear_Error(u8 id);
void Reboot(u8 id);
void Odrv_Input_Mode(u8 id);
void Odrv_AxisState(u8 id);
void Odrv_motor_Init(void);

extern Odrv_Limit odrivelimit;
extern Odrv_Param U10_Param, V4004_Param;
extern Odrv_motor odrv_motor[4];
extern Odrv_Argum odriveargum;

#endif

/*
 * @Description: 继承前辈们的虚拟示波器代码
 * @Version: 1.0
 * @Author: Dandelion
 * @Date: 2021-08-22 14:36:36
 * @LastEditTime: 2021-08-22 20:38:27
 * @FilePath: \Projectc:\Users\24828\Desktop\主控模板\GeneralFunction\src\visual_scope.c
 */

#include "visual_scope.h"

VisualScopeStruct VisualScope;


/**
 * @brief: 意义不明，直接继承
 * @author: Dandelion
 * @Date: 2021-08-22 20:37:33
 * @return {*}
 * @param {unsigned char} *Buf
 * @param {unsigned char} CRC_CNT
 */
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i, j;
    CRC_Temp = 0xffff;

    for (i = 0; i < CRC_CNT; i++)
    {
        CRC_Temp ^= Buf[i];
        for (j = 0; j < 8; j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >> 1) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return (CRC_Temp);
}

unsigned char tempdat[10];


/**
 * @brief: 4通道虚拟示波器使用
 * @author: Dandelion
 * @Date: 2021-08-22 20:35:30
 * @return {*}
 * @param {int16_t} n_dataCH1
 * @param {int16_t} n_dataCH2
 * @param {int16_t} n_dataCH3
 * @param {int16_t} n_dataCH4
 */
void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4)
{
    //    unsigned char ii = 0;
    unsigned char SendData[10];
    unsigned short CRC16;
    SendData[1] = (n_dataCH1 & 0xFF00) >> 8;

    SendData[0] = (n_dataCH1 & 0x00FF);
    SendData[3] = (n_dataCH2 & 0xFF00) >> 8;
    SendData[2] = (n_dataCH2 & 0x00FF);
    SendData[5] = (n_dataCH3 & 0xFF00) >> 8;
    SendData[4] = (n_dataCH3 & 0x00FF);
    SendData[7] = (n_dataCH4 & 0xFF00) >> 8;
    SendData[6] = (n_dataCH4 & 0x00FF);

    CRC16 = CRC_CHECK(SendData, 8);
    SendData[8] = CRC16 % 256;
    SendData[9] = CRC16 / 256;
    *tempdat = *SendData;
    Usart_EnQueue(&usart.u1.send_queue, SendData, 10);
}


/**
 * @brief: / 帧率跟不上，最高帧数只能是25，即频率为25hz，要求高频的话，还是用4通道的吧
 * @author: Dandelion
 * @Date: 2021-08-22 20:36:13
 * @return {*}
 * @param {float} data1
 * @param {float} data2
 * @param {float} data3
 * @param {float} data4
 * @param {float} data5
 * @param {float} data6
 * @param {float} data7
 * @param {float} data8
 * @param {float} data9
 * @param {float} data10
 */
void VS10Channal_Send(float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float data9, float data10)
{
    u8 SendData[42] = {0};
    SendData[0] = '$';

    FloatTo4Byte(&data1, &SendData[1]);
    FloatTo4Byte(&data2, &SendData[5]);
    FloatTo4Byte(&data3, &SendData[9]);
    FloatTo4Byte(&data4, &SendData[13]);
    FloatTo4Byte(&data5, &SendData[17]);
    FloatTo4Byte(&data6, &SendData[21]);
    FloatTo4Byte(&data7, &SendData[25]);
    FloatTo4Byte(&data8, &SendData[29]);
    FloatTo4Byte(&data9, &SendData[33]);
    FloatTo4Byte(&data10, &SendData[37]);

    SendData[41] = 41;
    Usart_EnQueue(&usart.u1.send_queue, SendData, 42);
}
float vscope_buff[60] = {0};
void InitVscopeChannel(float *chx[60])
{
//    static flagu8 init_flag = True;
//    if (True == init_flag)
//    {
//        chx[0] = (float *)&(position_now.x), chx[1] = (float *)&(position_now.y), chx[2] = (float *)&(position_now.angle);
//        chx[3] = (float *)&(run_pid.err_now), chx[4] = (float *)&(angle_pid.err_now), chx[5] = (float *)&(run_time.run_use);
//        chx[6] = (float *)&(velocity_now.v), chx[7] = (float *)&(velocity_now.vx), chx[8] = (float *)&(velocity_now.vy), chx[9] = (float *)&(velocity_now.vw);
//        chx[10] = (float *)&(velocity_set.v), chx[11] = (float *)&(velocity_set.vx), chx[12] = (float *)&(velocity_set.vy), chx[13] = (float *)&(velocity_set.vw);

//        chx[14] = &(chassis.wheel[0].v), chx[15] = &(chassis.wheel[1].v), chx[16] = &(chassis.wheel[2].v), chx[17] = &(chassis.wheel[3].v);
//        chx[18] = &(chassis.wheel[0].driving_motor.speed), chx[19] = &(chassis.wheel[1].driving_motor.speed), chx[20] = &(chassis.wheel[2].driving_motor.speed), chx[21] = &(chassis.wheel[3].driving_motor.speed);
//        chx[22] = &(chassis.wheel[0].driving_motor.IQcurrent), chx[23] = &(chassis.wheel[1].driving_motor.IQcurrent), chx[24] = &(chassis.wheel[2].driving_motor.IQcurrent), chx[25] = &(chassis.wheel[3].driving_motor.IQcurrent);
//        chx[26] = &(chassis.wheel[0].turn_motor.angle), chx[27] = &(chassis.wheel[1].turn_motor.angle), chx[28] = &(chassis.wheel[2].turn_motor.angle), chx[29] = &(chassis.wheel[3].turn_motor.angle);
//        chx[34] = &(chassis.wheel[0].angle_set), chx[35] = &(chassis.wheel[1].angle_set), chx[36] = &(chassis.wheel[2].angle_set), chx[37] = &(chassis.wheel[3].angle_set);
//        chx[38] = &(chassis.wheel[0].angle_last), chx[39] = &(chassis.wheel[1].angle_last), chx[40] = &(chassis.wheel[2].angle_last), chx[41] = &(chassis.wheel[3].angle_last);

//        chx[46] = &(run_time.acc);
//        chx[47] = (float *)(&run_pid.out);
//    }
//    vscope_buff[30] = chassis.wheel[0].k, vscope_buff[31] = chassis.wheel[1].k, vscope_buff[32] = chassis.wheel[2].k, vscope_buff[33] = chassis.wheel[3].k;
//    vscope_buff[42] = chassis.wheel[0].quadrant, vscope_buff[43] = chassis.wheel[1].quadrant, vscope_buff[44] = chassis.wheel[2].quadrant, vscope_buff[45] = chassis.wheel[3].quadrant;

//    chx[30] = &(vscope_buff[30]), chx[31] = &(vscope_buff[31]), chx[32] = &(vscope_buff[32]), chx[33] = &(vscope_buff[33]);
//    chx[42] = &(vscope_buff[42]), chx[43] = &(vscope_buff[43]), chx[44] = &(vscope_buff[44]), chx[45] = &(vscope_buff[45]);
//    init_flag = False;
}

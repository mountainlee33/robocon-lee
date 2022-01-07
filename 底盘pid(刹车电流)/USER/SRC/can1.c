#include "can1.h"
u8 buf[4];
s32 speed;
s32 pos;
s16 num[4];
extern VESCMotor VESCmotor[4];
void CAN1_Mode_Init(void)
{
	
  CAN_InitTypeDef CAN_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
	
  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;         //非时间触发通道模式
  CAN_InitStructure.CAN_ABOM = ENABLE;         //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
  CAN_InitStructure.CAN_AWUM = DISABLE;         //睡眠模式由软件唤醒
  CAN_InitStructure.CAN_NART = DISABLE;         //禁止报文自动发送，即只发送一次，无论结果如何
  CAN_InitStructure.CAN_RFLM = DISABLE;         //报文不锁定，新的覆盖旧的
  CAN_InitStructure.CAN_TXFP = DISABLE;         //发送FIFO的优先级由标识符决定
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN硬件工作在正常模式

  /* Seting BaudRate */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用8个时间单位
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用3个时间单位
  CAN_InitStructure.CAN_Prescaler = 3;     //分频系数（Fdiv）
  CAN_Init(CAN1, &CAN_InitStructure);      //初始化CAN1

  //转向电机
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X305 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0X306 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X307 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X308 << 5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  //底盘驱动
  CAN_FilterInitStructure.CAN_FilterNumber = 2;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0304 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0X0301 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X0302 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X0303 << 5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterNumber = 3;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0300 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0X0320 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* 波特率计算公式: BaudRate = APB1时钟频率/Fdiv/（SJW+BS1+BS2） */
  /* 42MHz/3/(1+9+4)=1Mhz */


  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

//与原报文一致
static void answer_drivemotor(CanRxMsg *rx_message, u8 motorid)
{
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].ID = 0x280 + motorid;
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].DLC = sizeof(rx_message);
  memcpy(&Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[0], &rx_message->Data[0], rx_message->DLC);	
  Can1_Sendqueue.Rear = Rear1;
}

//查询电流，速度，位置
static void answer_query(CanRxMsg *rx_message, u8 motorid, u8 *buf)
{
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].ID = 0x280 + motorid;
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].DLC = 0x08;
  memcpy(&Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[0], &rx_message->Data[0], rx_message->DLC);
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[4] = buf[0];
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[5] = buf[1];
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[6] = buf[2];
  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[7] = buf[3];
	
  Can1_Sendqueue.Rear = Rear1;
}
//转向反馈真实值
static void answer_turn(CanRxMsg *rx_message, u8 motorid, s16 *buff)
{
	CanTxMsg TxMessage;
	TxMessage.StdId=0x280 + motorid; 
	TxMessage.IDE=CAN_ID_STD; 
	TxMessage.RTR=CAN_RTR_DATA; 
	TxMessage.DLC=0x08; 
	for(int i=0;i<4;i++)
	EncodeS16Data(&num[i],&TxMessage.Data[2*i]);
	
	CAN_Transmit(CAN1, &TxMessage); 
}

void CAN1_RX0_IRQHandler(void)
{
  CanRxMsg rx_message;
  if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
  {
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    CAN_ClearFlag(CAN1, CAN_IT_FMP0);
    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);

    u8 motorid = rx_message.StdId - 0x300;
    if (rx_message.StdId == 0x300) //广播帧,发送所有电机的状态
    {
      int i;
      if (rx_message.Data[0] == 'M' && rx_message.Data[1] == 'O')
      {
				rx_message.DLC = 0x04;
        for (i = 0; i < 4; i++)
        {
          VESCmotor[i].enable = rx_message.Data[4] & 0x01;
          if (VESCmotor[i].enable != 0)
            VESCmotor->begin = 0;
        }
        answer_drivemotor(&rx_message, motorid);
      }
      if (rx_message.Data[0] == 'S' && rx_message.Data[1] == 'T')
      {
				 for (i = 0; i < 4; i++)
        {
				VESCmotor[i].status.brake_flag = 1;
				}
        answer_drivemotor(&rx_message, motorid);
      }
      if (rx_message.Data[0] == 'B' && rx_message.Data[1] == 'G')
      {
        for (i = 0; i < 4; i++)
        {
          VESCmotor[i].mode = 2;
          VESCmotor[i].begin = 1;
					VESCmotor[i].status.stop_flag = 0;
        }
        answer_drivemotor(&rx_message, motorid);
      }
      if (rx_message.Data[0] == 'P' && rx_message.Data[1] == 'X')
      {
        motorid = motorid + 0x001;
        for (i = 0; i < 4; i++)
        {
          Ecodeint32Data_to_4byte(&VESCmotor[i].ValReal.position, &buf[0]);
          answer_query(&rx_message, motorid,buf);
        }
      }
      if (rx_message.Data[0] == 'V' && rx_message.Data[1] == 'X')
      {
        motorid = motorid + 0x002;
        for (i = 0; i < 4; i++)
        {
          Ecodeint32Data_to_4byte(&VESCmotor[i].ValReal.speed, &buf[0]);
          answer_query(&rx_message, motorid,buf);
        }
      }
      if (rx_message.Data[0] == 'I' && rx_message.Data[1] == 'Q')//查询电流
      {
        motorid = motorid + 0x003;
        for (i = 0; i < 4; i++)
        {
          EcodeFloatData_to_4byte(&VESCmotor[i].ValSet.current, &buf[0]);
          answer_query(&rx_message, motorid,buf);
        }
      }
    }
    //底盘驱动设置速度
    if ((rx_message.StdId >= 0x301) && (rx_message.StdId <= 0x304)) //底盘速度
    {
			motorid = rx_message.StdId - 0x301;
      if (rx_message.Data[0] == 'J' && rx_message.Data[1] == 'V')
      {
        DecodeS32Data(&speed,&rx_message.Data[4] );
				VESCmotor[motorid].ValSet.speed = speed;
        answer_drivemotor(&rx_message, motorid);
      }
    }
    //转向电机
    if ((rx_message.StdId >= 0x305) && (rx_message.StdId <= 0x308))
    {
      if (rx_message.Data[0] == 'S' && rx_message.Data[1] == 'T')
      {
				motorid = rx_message.StdId - 0x305;
				motor[motorid].begin = 0;
				
        answer_drivemotor(&rx_message, motorid);
      }
      if (rx_message.Data[0] == 'B' && rx_message.Data[1] == 'G')
      {
				motorid = rx_message.StdId - 0x305;
				motor[motorid].begin = 1;
				
        answer_drivemotor(&rx_message, motorid);
      }
      if (rx_message.Data[0] == 'P' && rx_message.Data[1] == 'X') //位置模式返回值乘100倍
      {
        motorid = rx_message.StdId - 0x300;
        get_s32_from_buffer(&rx_message.Data[4], &pos);
        motor[motorid-0x05].valueSet.angle = pos / 1e2f;
        answer_drivemotor(&rx_message, motorid);
      }
      if (rx_message.Data[0] == 'M' && rx_message.Data[1] == 'O') //单独使能
      {
        motorid = rx_message.StdId - 0x300;
        motor[motorid-0x05].enable = rx_message.Data[4];
        answer_drivemotor(&rx_message, motorid);
      }
    }

    if (rx_message.StdId == 0x320)
    {
      motorid = rx_message.StdId - 0x320;
      if (rx_message.Data[0] == 'M' && rx_message.Data[1] == 'O')
      {
        for (int i = 0; i < 4; i++)
        {
          motor[i].enable = rx_message.Data[5];
          if (motor[i].enable == 0)
            motor[i].begin = 0;
        }
        answer_drivemotor(&rx_message, motorid);
      }
      else if (rx_message.Data[0] == 'S' && rx_message.Data[1] == 'P') //位置模式下的速度限制
      {
        for (int i = 0; i < 4; i++)
        {
          get_s32_from_buffer(&rx_message.Data[4], &speed);
          motor[i].limit.posSPlimit = speed;
          answer_drivemotor(&rx_message, motorid);
        }
      }
      else if (rx_message.Data[0] == 'P' && rx_message.Data[1] == 'X')//查询位置
      {
        answer_drivemotor(&rx_message, motorid);
      }
      else//转到位置
      {
				for(int i = 0;i<4;i++)
				{
					DecodeS16Data(&num[i],&rx_message.Data[2*i]);
				  motor[i].valueSet.angle = num[i]/100;
					num[i] = 100*motor[i].valueReal.angle;
				}
        answer_turn(&rx_message, motorid,num);
      }
    }
  }
}

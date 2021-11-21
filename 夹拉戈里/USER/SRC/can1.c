#include "can1.h"
u8 buf[4];
s32 speed;
s32 pos;
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

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;         //��ʱ�䴥��ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM = DISABLE;         //�����CAN_MCR�Ĵ�����INRQλ��1�������0��һ����⵽128������11λ������λ�����˳�����״̬
  CAN_InitStructure.CAN_AWUM = DISABLE;         //˯��ģʽ���������
  CAN_InitStructure.CAN_NART = DISABLE;         //��ֹ�����Զ����ͣ���ֻ����һ�Σ����۽�����
  CAN_InitStructure.CAN_RFLM = DISABLE;         //���Ĳ��������µĸ��Ǿɵ�
  CAN_InitStructure.CAN_TXFP = DISABLE;         //����FIFO�����ȼ��ɱ�ʶ������
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CANӲ������������ģʽ

  /* Seting BaudRate */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //����ͬ����Ծ���Ϊһ��ʱ�䵥λ
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //ʱ���1ռ��8��ʱ�䵥λ
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //ʱ���2ռ��3��ʱ�䵥λ
  CAN_InitStructure.CAN_Prescaler = 3;     //��Ƶϵ����Fdiv��
  CAN_Init(CAN1, &CAN_InitStructure);      //��ʼ��CAN1

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //ת����
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

  //��������
  CAN_FilterInitStructure.CAN_FilterNumber = 2;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X320 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0X301 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X302 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X303 << 5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterNumber = 3;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0X0300 << 5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0X000 << 5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* �����ʼ��㹫ʽ: BaudRate = APB1ʱ��Ƶ��/Fdiv/��SJW+BS1+BS2�� */
  /* 42MHz/3/(1+9+4)=1Mhz */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

//��ԭ����һ��
//static void answer_drivemotor(CanRxMsg *rx_message, u8 motorid)
//{
//  Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID = 0x300 + motorid;
//  Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC = sizeof(rx_message);
//}

//��ѯ�������ٶȣ�λ��
//static void answer_query(CanRxMsg *rx_message, u8 motorid, u8 *buf)
//{
//  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].ID = 0x300 + motorid;
//  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].DLC = 0x08;
//  memcpy(&Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[0], &rx_message->Data[0], rx_message->DLC);
//  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[4] = buf[0];
//  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[5] = buf[1];
//  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[6] = buf[2];
//  Can1_Sendqueue.Can_DataSend[Can1_Sendqueue.Rear].Data[7] = buf[3];
//  Can1_Sendqueue.Rear = Rear1;
//}
//void CAN1_RX0_IRQHandler(void)
//{
//  CanRxMsg rx_message;
//  if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
//  {
//    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
//    CAN_ClearFlag(CAN1, CAN_IT_FMP0);
//    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);

//    u8 motorid = rx_message.StdId - 0x300;
//    if (rx_message.StdId == 0x300) //�㲥֡,�������е����״̬
//    {
//      int i;
//      if (rx_message.Data[0] == 'M' && rx_message.Data[1] == 'O')
//      {
//        for (i = 0; i < 4; i++)
//        {
//          VESCmotor[i].enable = rx_message.Data[4] & 0x01;
//          if (VESCmotor[i].enable != 0)
//            VESCmotor->begin = 0;
//        }
//        answer_drivemotor(&rx_message, motorid);
//      }
//      if (rx_message.Data[0] == 'S' && rx_message.Data[1] == 'T')
//      {
//        for (i = 0; i < 4; i++)
//        {
//          VESC_Set_Pos(i, VESCmotor[i].ValReal.position, 0);
//        }
//        answer_drivemotor(&rx_message, motorid);
//      }
//      if (rx_message.Data[0] == 'B' && rx_message.Data[1] == 'G')
//      {
//        for (i = 0; i < 4; i++)
//        {
//          VESCmotor[i].begin = 1;
//        }
//        answer_drivemotor(&rx_message, motorid);
//      }
//      if (rx_message.Data[0] == 'P' && rx_message.Data[1] == 'X')
//      {
//        motorid = motorid + 0x001;
//        for (i = 0; i < 4; i++)
//        {
//          Ecodeint32Data_to_4byte(&VESCmotor[i].ValReal.position, &buf[0]);
//          answer_query(&rx_message, motorid,buf);
//        }
//      }
//      if (rx_message.Data[0] == 'V' && rx_message.Data[1] == 'X')
//      {
//        motorid = motorid + 0x002;
//        for (i = 0; i < 4; i++)
//        {
//          Ecodeint32Data_to_4byte(&VESCmotor[i].ValReal.speed, &buf[0]);
//          answer_query(&rx_message, motorid,buf);
//        }
//      }
//      if (rx_message.Data[0] == 'I' && rx_message.Data[1] == 'Q')
//      {
//        motorid = motorid + 0x003;
//        for (i = 0; i < 4; i++)
//        {
//          EcodeFloatData_to_4byte(&VESCmotor[i].ValReal.current, &buf[0]);
//          answer_query(&rx_message, motorid,buf);
//        }
//      }
//    }
//    //�������������ٶ�
//    if ((rx_message.StdId >= 0x301) && (rx_message.StdId <= 0x304)) //�����ٶ�
//    {
//			motorid = rx_message.StdId - 0x301;
//      if (rx_message.Data[0] == 'J' && rx_message.Data[1] == 'V')
//      {
//        get_s32_from_buffer(&rx_message.Data[3], &speed);
//				VESCmotor[motorid].ValReal.speed = speed;
//        answer_drivemotor(&rx_message, motorid);
//      }
//    }
//    //ת����
//    if ((rx_message.StdId >= 305) && (rx_message.StdId <= 308))
//    {
//      if (rx_message.Data[0] == 'S' && rx_message.Data[1] == 'T')
//      {
//				motorid = rx_message.StdId - 0x305;
//				VESCmotor[motorid].begin = 0;
//				
//        motorid = 0x020;
//        answer_drivemotor(&rx_message, motorid);
//      }
//      if (rx_message.Data[0] == 'B' && rx_message.Data[1] == 'G')
//      {
//				motorid = rx_message.StdId - 0x305;
//				VESCmotor[motorid].begin = 0;
//				
//        motorid = 0x020;
//        answer_drivemotor(&rx_message, motorid);
//      }
//      if (rx_message.Data[0] == 'P' && rx_message.Data[1] == 'X') //λ��ģʽ����ֵ��100��
//      {
//        motorid = rx_message.StdId - 305;
//        get_s32_from_buffer(&rx_message.Data[4], &pos);
//        motor[motorid].valueSet.angle = pos / 1e2f;
//        answer_drivemotor(&rx_message, motorid);
//      }
//      if (rx_message.Data[0] == 'M' && rx_message.Data[1] == 'O') //����ʹ��
//      {
//        motorid = rx_message.StdId - 305;
//        motor[motorid].enable = rx_message.Data[4];
//        answer_drivemotor(&rx_message, motorid);
//      }
//    }

//    if (rx_message.StdId == 0x320)
//    {
//      motorid = rx_message.StdId - 0x320;
//      if (rx_message.Data[0] == 'M' && rx_message.Data[1] == 'O')
//      {
//        for (int i = 0; i < 4; i++)
//        {
//          motor[i].enable = rx_message.Data[5];
//          if (motor[i].enable == 0)
//            motor[i].begin = 0;
//        }
//        answer_drivemotor(&rx_message, motorid);
//      }
//      else if (rx_message.Data[0] == 'S' && rx_message.Data[1] == 'P') //λ��ģʽ�µ��ٶ�����
//      {
//        for (int i = 0; i < 4; i++)
//        {
//          get_s32_from_buffer(&rx_message.Data[4], &speed);
//          motor[i].limit.posSPlimit = speed;
//          answer_drivemotor(&rx_message, motorid);
//        }
//      }
//      else if (rx_message.Data[0] == 'P' && rx_message.Data[1] == 'X')
//      {
//				for(int i = 0;i<4;i++)
//				{
//					buf[i] = 100 * motor[i].valueReal.angle;
//					rx_message.Data[2*i] = buf[i] >> 8;
//					rx_message.Data[2*i+1] = buf[i];
//				}
//        answer_drivemotor(&rx_message, motorid);
//      }
//      else
//      {
//				for(int i = 0;i<4;i++)
//				{
//					buf[i] = 100*motor[i].valueReal.angle;
//					rx_message.Data[2*i] = buf[i]>>8;
//					rx_message.Data[2*i+1] = buf[i];
//				}
//        answer_drivemotor(&rx_message, motorid);
//      }
//    }
//  }
//}

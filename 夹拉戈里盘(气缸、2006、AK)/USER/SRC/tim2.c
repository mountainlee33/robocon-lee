#include "tim2.h"

extern AK_Motor AKmotor[5];
extern DJmotor motor[8];

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //��ʹ�� TIM2 ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//�˲�����ʱ���ź�
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);// �ڳ�ʼ����ʱ�� TIM2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ�� 2 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //��Ӧ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// �ܳ�ʼ�� NVIC 
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE); //��ʹ�ܶ�ʱ�� 2
}
//��ʱ�� 2 �жϷ�����
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //����жϱ�־λ
		
#if USE_DJ		
		for (int id = 0; id < 5; id++)//��������ֻ��������for���ѭ�������ͬʱ���������ݣ�Ҫ�ֿ���
    {
      if (motor[id].enable)
      {
        if (motor[id].begin)
        {
          switch (motor[id].mode)
          {
         case current:;
            break;
          case RPM:
            Speed_mode(id);
            break; //�ٶ�ģʽ
          case position:
            Position_mode(id);
            break; //λ��ģʽ
          case zero:
            Zero_mode(id);
            break; //Ѱ��ģʽ
          default:
            break;
          }
        }
        else
          Position_mode(id);
      }
		}
		SetM3508_1(motor[0].valueSet.current, motor[1].valueSet.current, motor[2].valueSet.current, motor[3].valueSet.current);
		peakcurrent(); //DJ��������
		Can_DeQueue(CAN2, &Can2_Sendqueue); //ELMO EPOS
		if(flag.Can2CtrlList_Enable) TraversalControlList(Can2_MesgSentList,&Can2_Sendqueue,CAN_2);
#endif
#if USE_AK		
	  //AK	
		for (int i = 0; i < 6; i++)
    {
      if (AKmotor[i].enable)
      {
        if (AKmotor[i].begin)
        {
          switch (AKmotor[i].mode)
          {
          case current:
            AK_Transmit(i, 0, 0, 0, 0, AKmotor[i].valSet.torque);
            break;
          case RPM:
            AK_Transmit(i, 0, AKmotor[i].valSet.speed, 0, AKmotor[i].kd, AKmotor[i].valSet.torque);
            break;
          case position:
            AK_Transmit(i, AKmotor[i].valSet.pos, AKmotor[i].valSet.speed, AKmotor[i].kp, AKmotor[i].kd, AKmotor[i].valSet.torque);
            break;
          default:
            AK_Transmit(i, 0, 0, 0, 0, 0);
            break;
          }
        }
      }
    }
		Can_DeQueue(CAN2,&AK_Sendqueue);
#endif
	}
}

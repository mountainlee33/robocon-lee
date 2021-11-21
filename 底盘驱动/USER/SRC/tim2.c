#include "tim2.h"
extern DJmotor motor[8];

void TIM2_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //��ʹ�� TIM2 ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = 11999; //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=6; //��ʱ����Ƶ
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
		
#ifdef USE_DJ		
		for (int id = 0; id < 4; id++)//��������ֻ��������for���ѭ�������ͬʱ���������ݣ�Ҫ�ֿ���
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
	}
}

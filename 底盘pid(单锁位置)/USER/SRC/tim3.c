#include "tim3.h"
//fout = ((1+TIM_Prescaler)/72M)*(1+TIM_Period )
void TIM3_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///ʹ��TIM3ʱ��

  TIM_TimeBaseInitStructure.TIM_Period = 119999;//50ms                   //�Զ���װ��ֵ   699999
  TIM_TimeBaseInitStructure.TIM_Prescaler = 3;                    //��ʱ����Ƶ     5
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //��ʱ��3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //�����ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //��ʼ��TIM3

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //����ʱ��3�����ж�
  TIM_Cmd(TIM3, ENABLE);                     //ʹ�ܶ�ʱ��3
}

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //����ж�
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //����жϱ�־λ
		
#ifdef USE_VESC
    for (u8 i = 0; i < 4; i++)
    {
      ifVESCtimeout(i);
    }
#endif

#ifdef USE_DJ
    for (int i = 0; i < 4; i++)
    {
      ifdjtimeout(i);
    }
#endif
 }
}

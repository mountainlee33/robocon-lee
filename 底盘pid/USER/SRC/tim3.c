#include "tim3.h"
//fout = ((1+TIM_Prescaler)/72M)*(1+TIM_Period )
void TIM3_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///使能TIM3时钟

  TIM_TimeBaseInitStructure.TIM_Period = 119999;//50ms                   //自动重装载值   699999
  TIM_TimeBaseInitStructure.TIM_Prescaler = 3;                    //定时器分频     5
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //定时器3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM3

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器3更新中断
  TIM_Cmd(TIM3, ENABLE);                     //使能定时器3
}

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
		
//#ifdef USE_VESC
//		for(u8 i = 0; i < 4; i++)//底盘轮系
//		{
//			//反馈超时判断
//			if((VESCmotor[i].argum.timeout == 1) && VESCmotor[i].enable &&((OSTimeGet()-VESCmotor[i].argum.lastRxTim)> VESCmotor[i].argum.timeoutTicks))
//				VESCmotor[i].argum.timeoutCnt ++;
//			else
//				VESCmotor[i].argum.timeoutCnt = 0;
//			if(VESCmotor[i].argum.timeoutCnt > 20)
//			{
//				insertError(Error.head, VESCERROR | ((i+1) << 4) | TIMEOUT );
//			}
//			else
//			{
//				VESCmotor[i].status.timeout = 0;
//				deletError(Error.head, ErrorFind(Error.head, VESCERROR | ((i+1) << 4) | TIMEOUT));
//			}
// 		}
//#endif
//#ifdef USE_DJ
//		for(int i =0; i < 4; i++)
//			ifdjtimeout(i);
//#endif

	Can_DeQueue(CAN2, &VESC_Sendqueue);
//	for(int i =0; i < 4; i++)
//	{
//		if(VESCmotor[i].ValSet.speed == 0)
//		{
//		VESCmotor[i].mode = position;
//		VESCmotor[i].ValSet.angle = VESCmotor[i].ValReal.angle;
//		}
//	}
 }
}

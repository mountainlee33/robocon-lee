#include "tim3.h"

void TIM3_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///使能TIM3时钟

  TIM_TimeBaseInitStructure.TIM_Period = 11999;                   //自动重装载值11999
  TIM_TimeBaseInitStructure.TIM_Prescaler = 6;                    //定时器分频
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //定时器3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //子优先级0
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
		
#ifdef USE_VESC
		for(u8 i = 0; i < 4; i++)
		{
			//反馈超时判断
			if((VESCmotor[i].argum.timeout == 1) && VESCmotor[i].enable &&((OSTimeGet()-VESCmotor[i].argum.lastRxTim)> VESCmotor[i].argum.timeoutTicks))
				VESCmotor[i].argum.timeoutCnt ++;
			else
				VESCmotor[i].argum.timeoutCnt = 0;
			if(VESCmotor[i].argum.timeoutCnt > 20)
			{
				insertError(Error.head, VESCERROR | ((i+1) << 4) | TIMEOUT );
			}
			else
			{
				VESCmotor[i].status.timeout = 0;
				deletError(Error.head, ErrorFind(Error.head, VESCERROR | ((i+1) << 4) | TIMEOUT));
			}
 		}
#endif
#ifdef USE_ELMO
		for (int i = 0 ; i<4 ; i++)
		{
			if(ELMOmotor[i].enable)
			{
				if((ELMOmotor[i].argum.timeout) &&  (OSTimeGet() - ELMOmotor[i].argum.lastRxTim) > ELMOmotor[i].argum.timeoutTicks)
					ELMOmotor[i].argum.timeoutCnt ++;
				else 
					ELMOmotor[i].argum.timeoutCnt = 0;
				if (ELMOmotor[i].argum.timeoutCnt > 10)
					ELMOmotor[i].status.timeout = 1;
				else
					ELMOmotor[i].status.timeout = 0;
			}	
		}
#endif
#ifdef USE_DJ
		for(int i =0; i < 8; i++)
			iftimeout(i);
#endif
	}
}

#include "tim5.h"
//整个产生的中断时间是=CPU时钟/(分频系数+1)*(重装载值+1)
void TIM5_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//配置定时器的基本参数的结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period =11999;//定时器时钟=系统时钟/分频psc
	TIM_TimeBaseStructure.TIM_Prescaler = 11;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		
#ifdef USE_VESC
		for(int i = 0 ; i < 4 ;  i++)
		{
			VESC_calculate(&VESCmotor[i]);
			if(VESCmotor[i].enable)
			{
				if(VESCmotor[i].begin)
				{
					switch(VESCmotor[i].mode)
					{
						case current:
							VESC_Set_Current(i+1,VESCmotor[i].ValSet.current,0);
							break;
						case RPM:
							VESC_Set_Speed(i+1,VESCmotor[i].ValSet.speed*VESCmotor[i].instrinsic.POLE_PAIRS,0 );
							break;
						case position:
							VESC_position_mode_pos(i);
							break;
						case brake:
              VESC_Set_Brake_Current(i + 1, VESCmotor[i].limit.BrakeCurrent, 0);
              break;
						case duty :
							VESC_Set_Duty_Cycle(i+1,VESCmotor[i].ValSet.duty,0);
							break;						
             case vesc_postion_rpm:
               VESC_position_mode_rpm(i);
               break;
             case vesc_postion_I:
               VESC_position_mode_I(i);
               break;
             case vesc_RPM_I:
               VESC_RPM_mode_I(i);
               break;
             default:
               break; 
					}
				}
			}
		}
#endif
	}
}

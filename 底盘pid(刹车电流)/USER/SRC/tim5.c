#include "tim5.h"
//整个产生的中断时间是=CPU时钟/(分频系数+1)*(重装载值+1)
u8 pos1=0;
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
              VESC_Set_HandBrake_Current(i + 1, VESCmotor[i].limit.BrakeCurrent, 0);
              break;
						case duty :
							VESC_Set_Duty_Cycle(i+1,VESCmotor[i].ValSet.duty,0);
							break;						
             case vesc_RPM_I:
               VESC_RPM_mode_I(i);
               break;
             default:
               break; 
					}
				}
				if((VESCmotor[i].status.stop_flag == 0)&&(VESCmotor[i].status.brake_flag == 1))
				{
					VESCmotor[i].argum.stopCnt++;
				if(VESCmotor[i].argum.stopCnt > VESCmotor[i].argum.stopTicks)
				{
				VESCmotor[i].status.stop_flag = 1;
				VESCmotor[i].status.brake_flag = 0;
				VESCmotor[i].mode = 5;
				VESCmotor[i].argum.stopCnt =0;
				}
				else
				{
				VESCmotor[i].mode = 3;
				pos1=VESCmotor[i].ValReal.angle;
				VESCmotor[i].ValSet.angle = pos1;
				}
			}
			}
      else
        VESC_Set_Current(i + 1, 0.0, 0);
		}

#endif
	}
			Can_DeQueue(CAN2, &VESC_Sendqueue);
			Can_DeQueue(CAN1, &Can1_Sendqueue);
}

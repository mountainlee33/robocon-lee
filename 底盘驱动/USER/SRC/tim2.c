#include "tim2.h"
extern DJmotor motor[8];

void TIM2_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //①使能 TIM2 时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = 11999; //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=6; //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//滤波器的时钟信号
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);// ②初始化定时器 TIM2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器 2 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ④初始化 NVIC 
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE); //⑤使能定时器 2
}
//定时器 2 中断服务函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位
		
#ifdef USE_DJ		
		for (int id = 0; id < 4; id++)//发送邮箱只有三个，for语句循环里最多同时发三个数据，要分开发
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
            break; //速度模式
          case position:
            Position_mode(id);
            break; //位置模式
          case zero:
            Zero_mode(id);
            break; //寻零模式
          default:
            break;
          }
        }
        else
          Position_mode(id);
      }
		}
		SetM3508_1(motor[0].valueSet.current, motor[1].valueSet.current, motor[2].valueSet.current, motor[3].valueSet.current);
		peakcurrent(); //DJ电流限制
		Can_DeQueue(CAN2, &Can2_Sendqueue); //ELMO EPOS
		if(flag.Can2CtrlList_Enable) TraversalControlList(Can2_MesgSentList,&Can2_Sendqueue,CAN_2);
#endif
	}
}

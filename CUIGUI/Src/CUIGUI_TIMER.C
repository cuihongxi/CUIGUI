#include "CUIGUI_TIMER.H"
#include "CUIGUI_EDIT.H"
#include "usart.h"

void Timer2_init(void)
{
		TIM_Struct	tim_stru;
		tim_stru.NVIC_IRQChannelPreemptionPriority = 1;
		tim_stru.NVIC_IRQChannelSubPriority = 1;
            tim_stru.Prescaler = 7199;
		tim_stru.Period = CUIGUI_TIME_BASE*10;//10MS		
		tim_stru.TIMx = TIM2;
		TIM_Init(&tim_stru);
}


//初始化定时器
void CUIGUI_TIMER_Init(void)
{
      Timer2_init();
}



 //中断服务函数
 void TIM2_IRQHandler(void)
{
      if(TIM_GetITStatus(TIM2,TIM_FLAG_Update)==SET)  //判断是不是这个中断
      {
            CUIGUITimer_Edit(&msg);
             
            TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update); //清除标志位
			
      }

		
}

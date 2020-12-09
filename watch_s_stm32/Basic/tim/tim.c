#include "tim.h"
#include "TM1640.h"

u16 time=0;//定时器时间
u16 show_time=0;//秒表
/*------------------------------------------------------------------
函数功能：初始化定时器中断
定时器时间计算公式：Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
	0.1秒定时，重装载值=999，预分频系数=7199
-------------------------------------------------------------------*/
void TIM3_Init(u16 arr,u16 psc){  //TIM3 初始化 arr重装载值 psc预分频系数
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
    TIM3_NVIC_Init (); //开启TIM3中断向量
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3初始化设置
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能TIM3中断    
    TIM_Cmd(TIM3,ENABLE); //使能TIM3
}

void TIM3_NVIC_Init (void){ //开启TIM3中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void){ //TIM3中断处理函数
		u8 i=0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{//判断是否是TIM3中断
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				time++;//0.1s位进位
				if(time==1002)
				{
					time=0;//清空0.1s位
//					for(i=0;i<8;i++)TM1640_display(i,20);	
				}
    }
}

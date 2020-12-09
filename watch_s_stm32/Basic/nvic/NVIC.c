#include "NVIC.h"
#include "delay.h"
#include "tim.h"
#include "TM1640.h"
u8 time_state=no_time;//秒表状态
u8 debug=display_debug;
void KEY_INT_INIT (void){	 //按键中断初始化
	NVIC_InitTypeDef  NVIC_InitStruct;	//定义结构体变量
	EXTI_InitTypeDef  EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //启动GPIO时钟 （需要与复用时钟一同启动）     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//配置端口中断需要启用复用时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  //定义 GPIO  中断
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;  //定义中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);  //定义 GPIO  中断
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line1;  //定义中断线	
	EXTI_Init(& EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line=EXTI_Line2;  //定义中断线
	EXTI_Init(& EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;  //定义中断线
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;   //中断线     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI1_IRQn;   //中断线     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;   //中断线     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;   //中断线     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);
}

void  EXTI1_IRQHandler(void){
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	//if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{//判断某个线上的中断是否发生 
		time_state=timing_state;//秒表切换到正在计时
		time=0;//清零秒表
	}
	EXTI_ClearITPendingBit(EXTI_Line1);   //清除 LINE 上的中断标志位
	
}

void  EXTI2_IRQHandler(void){
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{//判断某个线上的中断是否发生
		if(time_state==timing_state)time_state=stop_time;//秒表切换到停止计时
		else if(time_state==stop_time)
		{		
				time_state=timing_state;//秒表切换到正在计时
				time=show_time;//将秒表拨回到之前记录的时间
		}
	} 
	EXTI_ClearITPendingBit(EXTI_Line2);   //清除 LINE 上的中断标志位	
}

void  EXTI3_IRQHandler(void){
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		if(debug==no_display_debug)debug=display_debug;//显示调试信息
		else if(debug==display_debug)debug=no_display_debug;//不显示调试信息
	}
	EXTI_ClearITPendingBit(EXTI_Line3);   //清除 LINE 上的中断标志位
	
}


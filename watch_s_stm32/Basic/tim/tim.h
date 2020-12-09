#ifndef  __PWM_H
#define  __PWM_H
#include "sys.h"

void TIM3_Init(u16 arr,u16 psc);//初始化定时器中断
void TIM3_NVIC_Init (void);//开启TIM3中断向量
extern u16 time;//定时器时间
extern u16 show_time;//秒表
#endif

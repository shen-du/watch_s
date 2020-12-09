#ifndef __NVIC_H
#define __NVIC_H	 
#include "sys.h"
/*-------------------------------
秒表行为状态机
------------------------------*/
#define no_time 0//没有计时
#define timing_state 1//正在计时
#define stop_time 2//停止计时
#define time_over 3//计时溢出
extern u8 time_state;//秒表行为状态

/*-------------------------------
调试信息状态机
------------------------------*/
#define display_debug 1//显示调试信息
#define no_display_debug 0//不显示调试信息
extern u8 debug;//是否显示调试信息

void KEY_INT_INIT (void); //按键中断初始化

#endif

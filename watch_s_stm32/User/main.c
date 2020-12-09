#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "led.h"
#include "key.h"
#include "NVIC.h"
#include "tim.h"
#include "can.h"
#include "TM1640.h"
u8 oled_display_debug[16]={0};//测试时oled所显示的秒表信息字符串
void BSP_init(void);//外设初始化
void developer_debug(u8 state,u16 tim);//调试显示函数
void time_behaviour_state_ctrl(u8 state);//外设初始化

/*-----------------------------------------------------------------------------
开机时秒表为没有计时状态，数码管显示向左流动的8
无论什么状态下，按下KEY1时，秒表清零并进入计时状态，数码管实时显示走过的时间
计时状态下，当秒表计时到99.9时，秒表进入溢出状态，此时数码管一直显示99.9
计时状态下，按下KEY2时，秒表为停止计时状态，数码管显示最终的时间
停止计时状态下，按下KEY2时，秒表接着上次的时间继续计时，
无论什么状态下，当按下KEY3时，隐藏调试信息，当再次按下KEY3时，显示调试信息
------------------------------------------------------------------------------*/
int main (void)//主程序
{
	BSP_init();//外设初始化
	while(1)time_behaviour_state_ctrl(time_state);
}

/*-----------------------------------------------------------
函数功能：初始化数码管，定时器中断，按键和按键外部中断外设
定时器时间计算公式：Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
	0.1秒定时，重装载值=999，预分频系数=7199
-----------------------------------------------------------*/
void BSP_init(void)
{
	delay_ms(500); //由于学习版自生硬件的原因，上电时需要等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化
	I2C_Configuration();//I2C初始化
	OLED0561_Init(); //oled初始化
	TIM3_Init(999,7199);//定时器初始化，周期0.1秒
	TM1640_Init();//数码管初始化	
	KEY_INT_INIT();//KEY外部中断初始化

	/*---------------------------------
	显示开发者信息:姓名,学号,任务
	------------------------------*/
	OLED_DISPLAY_8x16_BUFFER(0," Name: XXXXXXXX "); //显示姓名拼音
	OLED_DISPLAY_8x16_BUFFER(2," Num: 1XXXXXXXXX "); //显示学号
	OLED_DISPLAY_8x16_BUFFER(4,"   Task:TIMER "); //显示任务
}


/*-------------------------------------
函数功能：秒表行为状态切换
入口参数：state（秒表行为状态）
					state=no_time //没有计时
					state=timing_state //正在计时
					state=stop_time //停止计时
					state=time_over //计时溢出
-----------------------------------------*/
void time_behaviour_state_ctrl(u8 state)
{
		if(state==timing_state)
		{//秒表正在计时		
				TM1640_display(0,show_time/100);//显示十位	10
				TM1640_display(1,(show_time/10)%10+10);	//显示个位		1		
				TM1640_display(2,show_time%10);//显示小数点后一位	0.1
				show_time=time;	//实时获取定时器时间
				if(show_time==1000)time_state=3;//如果秒表溢出，秒表切换到溢出状态			
		}
		else if(state==stop_time)
		{//秒表停止计时
				TM1640_display(0,show_time/100);//显示十位	10
				TM1640_display(1,(show_time/10)%10+10);	//显示个位		1		
				TM1640_display(2,show_time%10);//显示小数点后一位	0.1
		}
		else if(state==time_over)
		{//秒表时间溢出，显示99.9
				show_time=999;//显示最大量程
				TM1640_display(0,show_time/100);//显示十位	10
				TM1640_display(1,(show_time/10)%10+10);	//显示个位		1		
				TM1640_display(2,show_time%10);//显示小数点后一位	0.1
		}
		else if(state==no_time)//秒表没有计时
		{//显示移动的8
				TM1640_display(2-(time/10-1)%3,20);//消掉前一个8
				TM1640_display(2-(time/10)%3,8);//显示后一个8
		}	
		developer_debug(state,time);//显示调试信息
}


/*------------------------------------------------------------
函数功能：显示定时器秒表调试的数据:定时器信息,数码管流水位置
入口参数：tim定时器时间
					state（秒表行为状态）
					state=no_time //没有计时时显示数码管流水位置
					state!=no_time //其他状态不显示数码管流水位置
time_bug==no_display_time//隐藏调试信息
time_bug==display_time//显示调试信息
-------------------------------------------------------------*/
void developer_debug(u8 state,u16 tim)
{	
	if(state==no_time)sprintf(oled_display_debug,"BUG:%5.1fs bit:%d", (float)tim/10,2-(tim/10)%3);//没有计时时显示数码管流水位置
	else sprintf(oled_display_debug,"BUG:%5.1fs bit:-", (float)tim/10);//其他状态不显示数码管流水位置
	if(debug==no_display_debug)sprintf(oled_display_debug,"----------------");//隐藏调试信息
	OLED_DISPLAY_8x16_BUFFER(6,oled_display_debug);//定时器信息和数码管流水位置
}


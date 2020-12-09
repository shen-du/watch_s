#include "stm32f10x.h" //STM32ͷ�ļ�
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
u8 oled_display_debug[16]={0};//����ʱoled����ʾ�������Ϣ�ַ���
void BSP_init(void);//�����ʼ��
void developer_debug(u8 state,u16 tim);//������ʾ����
void time_behaviour_state_ctrl(u8 state);//�����ʼ��

/*-----------------------------------------------------------------------------
����ʱ���Ϊû�м�ʱ״̬���������ʾ����������8
����ʲô״̬�£�����KEY1ʱ��������㲢�����ʱ״̬�������ʵʱ��ʾ�߹���ʱ��
��ʱ״̬�£�������ʱ��99.9ʱ�����������״̬����ʱ�����һֱ��ʾ99.9
��ʱ״̬�£�����KEY2ʱ�����Ϊֹͣ��ʱ״̬���������ʾ���յ�ʱ��
ֹͣ��ʱ״̬�£�����KEY2ʱ���������ϴε�ʱ�������ʱ��
����ʲô״̬�£�������KEY3ʱ�����ص�����Ϣ�����ٴΰ���KEY3ʱ����ʾ������Ϣ
------------------------------------------------------------------------------*/
int main (void)//������
{
	BSP_init();//�����ʼ��
	while(1)time_behaviour_state_ctrl(time_state);
}

/*-----------------------------------------------------------
�������ܣ���ʼ������ܣ���ʱ���жϣ������Ͱ����ⲿ�ж�����
��ʱ��ʱ����㹫ʽ��Tout = ((��װ��ֵ+1)*(Ԥ��Ƶϵ��+1))/ʱ��Ƶ��;
	0.1�붨ʱ����װ��ֵ=999��Ԥ��Ƶϵ��=7199
-----------------------------------------------------------*/
void BSP_init(void)
{
	delay_ms(500); //����ѧϰ������Ӳ����ԭ���ϵ�ʱ��Ҫ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ��
	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //oled��ʼ��
	TIM3_Init(999,7199);//��ʱ����ʼ��������0.1��
	TM1640_Init();//����ܳ�ʼ��	
	KEY_INT_INIT();//KEY�ⲿ�жϳ�ʼ��

	/*---------------------------------
	��ʾ��������Ϣ:����,ѧ��,����
	------------------------------*/
	OLED_DISPLAY_8x16_BUFFER(0," Name: XXXXXXXX "); //��ʾ����ƴ��
	OLED_DISPLAY_8x16_BUFFER(2," Num: 1XXXXXXXXX "); //��ʾѧ��
	OLED_DISPLAY_8x16_BUFFER(4,"   Task:TIMER "); //��ʾ����
}


/*-------------------------------------
�������ܣ������Ϊ״̬�л�
��ڲ�����state�������Ϊ״̬��
					state=no_time //û�м�ʱ
					state=timing_state //���ڼ�ʱ
					state=stop_time //ֹͣ��ʱ
					state=time_over //��ʱ���
-----------------------------------------*/
void time_behaviour_state_ctrl(u8 state)
{
		if(state==timing_state)
		{//������ڼ�ʱ		
				TM1640_display(0,show_time/100);//��ʾʮλ	10
				TM1640_display(1,(show_time/10)%10+10);	//��ʾ��λ		1		
				TM1640_display(2,show_time%10);//��ʾС�����һλ	0.1
				show_time=time;	//ʵʱ��ȡ��ʱ��ʱ��
				if(show_time==1000)time_state=3;//���������������л������״̬			
		}
		else if(state==stop_time)
		{//���ֹͣ��ʱ
				TM1640_display(0,show_time/100);//��ʾʮλ	10
				TM1640_display(1,(show_time/10)%10+10);	//��ʾ��λ		1		
				TM1640_display(2,show_time%10);//��ʾС�����һλ	0.1
		}
		else if(state==time_over)
		{//���ʱ���������ʾ99.9
				show_time=999;//��ʾ�������
				TM1640_display(0,show_time/100);//��ʾʮλ	10
				TM1640_display(1,(show_time/10)%10+10);	//��ʾ��λ		1		
				TM1640_display(2,show_time%10);//��ʾС�����һλ	0.1
		}
		else if(state==no_time)//���û�м�ʱ
		{//��ʾ�ƶ���8
				TM1640_display(2-(time/10-1)%3,20);//����ǰһ��8
				TM1640_display(2-(time/10)%3,8);//��ʾ��һ��8
		}	
		developer_debug(state,time);//��ʾ������Ϣ
}


/*------------------------------------------------------------
�������ܣ���ʾ��ʱ�������Ե�����:��ʱ����Ϣ,�������ˮλ��
��ڲ�����tim��ʱ��ʱ��
					state�������Ϊ״̬��
					state=no_time //û�м�ʱʱ��ʾ�������ˮλ��
					state!=no_time //����״̬����ʾ�������ˮλ��
time_bug==no_display_time//���ص�����Ϣ
time_bug==display_time//��ʾ������Ϣ
-------------------------------------------------------------*/
void developer_debug(u8 state,u16 tim)
{	
	if(state==no_time)sprintf(oled_display_debug,"BUG:%5.1fs bit:%d", (float)tim/10,2-(tim/10)%3);//û�м�ʱʱ��ʾ�������ˮλ��
	else sprintf(oled_display_debug,"BUG:%5.1fs bit:-", (float)tim/10);//����״̬����ʾ�������ˮλ��
	if(debug==no_display_debug)sprintf(oled_display_debug,"----------------");//���ص�����Ϣ
	OLED_DISPLAY_8x16_BUFFER(6,oled_display_debug);//��ʱ����Ϣ���������ˮλ��
}


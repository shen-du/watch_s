#include "NVIC.h"
#include "delay.h"
#include "tim.h"
#include "TM1640.h"
u8 time_state=no_time;//���״̬
u8 debug=display_debug;
void KEY_INT_INIT (void){	 //�����жϳ�ʼ��
	NVIC_InitTypeDef  NVIC_InitStruct;	//����ṹ�����
	EXTI_InitTypeDef  EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //����GPIOʱ�� ����Ҫ�븴��ʱ��һͬ������     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//���ö˿��ж���Ҫ���ø���ʱ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  //���� GPIO  �ж�
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;  //�����ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //�ж�ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //�ж�ģʽΪ �ж�
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);  //���� GPIO  �ж�
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line1;  //�����ж���	
	EXTI_Init(& EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line=EXTI_Line2;  //�����ж���
	EXTI_Init(& EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;  //�����ж���
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;   //�ж���     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI1_IRQn;   //�ж���     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;   //�ж���     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;   //�ж���     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);
}

void  EXTI1_IRQHandler(void){
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	//if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{//�ж�ĳ�����ϵ��ж��Ƿ��� 
		time_state=timing_state;//����л������ڼ�ʱ
		time=0;//�������
	}
	EXTI_ClearITPendingBit(EXTI_Line1);   //��� LINE �ϵ��жϱ�־λ
	
}

void  EXTI2_IRQHandler(void){
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{//�ж�ĳ�����ϵ��ж��Ƿ���
		if(time_state==timing_state)time_state=stop_time;//����л���ֹͣ��ʱ
		else if(time_state==stop_time)
		{		
				time_state=timing_state;//����л������ڼ�ʱ
				time=show_time;//������ص�֮ǰ��¼��ʱ��
		}
	} 
	EXTI_ClearITPendingBit(EXTI_Line2);   //��� LINE �ϵ��жϱ�־λ	
}

void  EXTI3_IRQHandler(void){
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		if(debug==no_display_debug)debug=display_debug;//��ʾ������Ϣ
		else if(debug==display_debug)debug=no_display_debug;//����ʾ������Ϣ
	}
	EXTI_ClearITPendingBit(EXTI_Line3);   //��� LINE �ϵ��жϱ�־λ
	
}


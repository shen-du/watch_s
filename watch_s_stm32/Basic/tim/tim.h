#ifndef  __PWM_H
#define  __PWM_H
#include "sys.h"

void TIM3_Init(u16 arr,u16 psc);//��ʼ����ʱ���ж�
void TIM3_NVIC_Init (void);//����TIM3�ж�����
extern u16 time;//��ʱ��ʱ��
extern u16 show_time;//���
#endif

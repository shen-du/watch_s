#ifndef __NVIC_H
#define __NVIC_H	 
#include "sys.h"
/*-------------------------------
�����Ϊ״̬��
------------------------------*/
#define no_time 0//û�м�ʱ
#define timing_state 1//���ڼ�ʱ
#define stop_time 2//ֹͣ��ʱ
#define time_over 3//��ʱ���
extern u8 time_state;//�����Ϊ״̬

/*-------------------------------
������Ϣ״̬��
------------------------------*/
#define display_debug 1//��ʾ������Ϣ
#define no_display_debug 0//����ʾ������Ϣ
extern u8 debug;//�Ƿ���ʾ������Ϣ

void KEY_INT_INIT (void); //�����жϳ�ʼ��

#endif

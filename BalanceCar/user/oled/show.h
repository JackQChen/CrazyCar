#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"

//�Ƕ�ӳ��
#define  ANGLE  S_Pitch
//��ѹӳ��
#define  BTV    btv

#define  LV    motor1
#define  RV    CCR2_Val

//��������ʾ
void oled_show(void);

//��ʼ����ʾ
/*
	type:  ��ʼ������
	state: ״̬

*/
void oled_show_init(u8 type,u8 state);

#endif

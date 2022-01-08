#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"

//角度映射
#define  ANGLE  S_Pitch
//电压映射
#define  BTV    btv

#define  LV    motor1
#define  RV    CCR2_Val

//主函数显示
void oled_show(void);

//初始化显示
/*
	type:  初始化类型
	state: 状态

*/
void oled_show_init(u8 type,u8 state);

#endif

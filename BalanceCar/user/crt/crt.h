#ifndef _CRT_H
#define _CRT_H

#include "config.h"

//速度滤波次数
#define SPEED_COUNT  20

typedef struct tagCtrParam
{
	int MotorSpeed_L;
	int MotorSpeed_R;

	float RunSpeed;			//控制速度变量
	int TurnSpeed;			//控制方向变量	

	int SpeedFilterBuf[SPEED_COUNT];
}strCtrParam;

extern strCtrParam CtrParam;

extern float cha1;		//速度积分

//壁障模式变量
extern u8 bizhang_state;

//跟随模式变量
extern u8 genshui_state;

//超声波控制
void csb_crt(void);

//跟随控制
void genshui_crt(void);

#define mabs(v) (v<0?-v:v)

//所有控制
void crt(void);
void ClearCtrParam(void);


#endif

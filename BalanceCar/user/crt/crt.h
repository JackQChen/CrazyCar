#ifndef _CRT_H
#define _CRT_H

#include "config.h"

//�ٶ��˲�����
#define SPEED_COUNT  20

typedef struct tagCtrParam
{
	int MotorSpeed_L;
	int MotorSpeed_R;
	
	float RunSpeed;			//�����ٶȱ���
	int TurnSpeed;			//���Ʒ������	
	
	int SpeedFilterBuf[SPEED_COUNT];
}strCtrParam;

extern strCtrParam CtrParam;

extern float cha1;		//�ٶȻ���

//����ģʽ����
extern u8 bizhang_state;

//����ģʽ����
extern u8 genshui_state;

//����������
 void csb_crt(void);
 
//�������
void genshui_crt(void);

#define mabs(v) (v<0?-v:v)

//���п���
void crt(void);
void ClearCtrParam(void);


#endif

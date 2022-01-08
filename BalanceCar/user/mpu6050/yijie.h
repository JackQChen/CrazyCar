#ifndef _YIJIE_H
#define _YIJIE_H

#include "sys.h"

#define P  0.1   //���ż��ٶȵĸ��ʱ�
#define T  0.01   //���ٶȻ���ʱ��

//У׼ֵ
#define  GX_JIAO  37  	 //������X��У׼ֵ
#define  GY_JIAO  -32   //������Y��У׼ֵ
#define  GZ_JIAO  25  		 //������Z��У׼ֵ

extern signed short Gyro_X;
extern signed short Gyro_Y;
extern signed short Gyro_Z;
	
extern float S_Pitch;
	
extern float S_Roll;

typedef struct 
{
	
		float x;
		float y;
		float z;
	
} Axis3f;
extern float S_Yaw,S_Pitch,S_Roll;  //ƫ���ǣ������ǣ�������

extern int Xgyro;
extern int Ygyro;
extern int Zgyro;

/*
ͨ��һ�׻�����ȡ�Ƕ�

����: ���ٶ�accele  ���ٶ�gyro

���أ��Ƕ�

���ٶ�accele ���Բ��ý��е�λת������Ϊ��ͬ������ϵ 

���ٶ�gyro ��Ҫ��λת��

*/
float  get_yijie_angle(float acceleXY,float acceleZ,float gyro,float  *angle);

//������У׼
void gyro_jiaozhun(float *gx,float *gy,float *gz);

//������̬
void up_angle(void);


void imuUpdate(Axis3f acc, Axis3f gyro, float dt);	/*�����ں� �����˲�*/
float invSqrt(float x);	/*���ٿ�ƽ����*/

#endif

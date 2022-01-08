#ifndef _YIJIE_H
#define _YIJIE_H

#include "sys.h"

#define P  0.1   //相信加速度的概率比
#define T  0.01   //角速度积分时间

//校准值
#define  GX_JIAO  37  	 //陀螺仪X的校准值
#define  GY_JIAO  -32   //陀螺仪Y的校准值
#define  GZ_JIAO  25  		 //陀螺仪Z的校准值

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
extern float S_Yaw,S_Pitch,S_Roll;  //偏航角，俯仰角，翻滚角

extern int Xgyro;
extern int Ygyro;
extern int Zgyro;

/*
通过一阶互补获取角度

参数: 加速度accele  角速度gyro

返回：角度

加速度accele 可以不用进行单位转换，因为是同比例关系 

角速度gyro 需要单位转换

*/
float  get_yijie_angle(float acceleXY,float acceleZ,float gyro,float  *angle);

//陀螺仪校准
void gyro_jiaozhun(float *gx,float *gy,float *gz);

//更新姿态
void up_angle(void);


void imuUpdate(Axis3f acc, Axis3f gyro, float dt);	/*数据融合 互补滤波*/
float invSqrt(float x);	/*快速开平方求倒*/

#endif

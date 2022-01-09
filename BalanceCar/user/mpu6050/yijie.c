#include "yijie.h"
#include "math.h"
#include "mpu6050.h"
#include "kaerman.h"
signed short Accel_X=0;
signed short Accel_Y=0;
signed short Accel_Z=0;

signed short Gyro_X=0;
signed short Gyro_Y=0;
signed short Gyro_Z=0;

signed short A_X=0;
signed short A_Y=0;
signed short A_Z=0;
	
float S_Pitch=0;
float S_Roll=0;

float get_yijie_angle(float acceleXY,float acceleZ,float gyro,float  *angle)
{
	float _angle;
	
	_angle=atan2(acceleXY,acceleZ)*180/3.14;    //计算倾角,	
	*angle=(1-P)*(*angle+gyro*T)  +   P*_angle;//一阶互补滤波
	return *angle;
}

void gyro_jiaozhun(float *gx,float *gy,float *gz)
{	
	#if  0
		printf("gx1:%4.2f,gy:%4.2f，gz:%4.2f\r\n",*gx,*gy,*gz);
	#endif

	*gx+=GX_JIAO;
	*gy+=GY_JIAO;
	*gz+=GZ_JIAO;

	#if  0
		printf("gx2:%4.2f,gy:%4.2f，gz:%4.2f\r\n",*gx,*gy,*gz);
	#endif

	*gx=(*gx/16.4); 
	*gy=(*gy/16.4); 
	*gz=(*gz/16.4);
}

// 四元数变量定义
#if 1
static float Kp = 0.8f;		/*比例增益*/
static float Ki = 0.001f;		/*积分增益*/
static float exInt = 0.0f;
static float eyInt = 0.0f;
static float ezInt = 0.0f;		/*积分误差累计*/

static float q0 = 1.0f;	/*四元数*/
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	

float S_Yaw,S_Pitch,S_Roll;  //偏航角，俯仰角，翻滚角
int Xgyro;
int Ygyro;
int Zgyro;

float invSqrt(float x)	/*快速开平方求倒*/
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


void imuUpdate(Axis3f acc, Axis3f gyro, float dt)	/*数据融合 互补滤波*/
{
	float normalise;
	float ex, ey, ez;
	float q0s, q1s, q2s, q3s;	/*四元数的平方*/
	//static float R11,R21;		/*矩阵(1,1),(2,1)项*/
	static float vecxZ, vecyZ, veczZ;	/*机体坐标系下的Z方向向量*/
	float halfT =0.5f * dt;
	float q0Last;
	float q1Last;
	float q2Last;
	float q3Last;
	

	gyro.x = gyro.x * 0.017453293f;	/* 度转弧度 */
	gyro.y = gyro.y * 0.017453293f;
	gyro.z = gyro.z * 0.017453293f;

	/* 某一个方向加速度不为0 */
	if((acc.x != 0.0f) || (acc.y != 0.0f) || (acc.z != 0.0f))
	{
		/*单位化加速计测量值*/
		normalise = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
		acc.x *= normalise;
		acc.y *= normalise;
		acc.z *= normalise;

		/*加速计读取的方向与重力加速计方向的差值，用向量叉乘计算*/
		ex = (acc.y * veczZ - acc.z * vecyZ);
		ey = (acc.z * vecxZ - acc.x * veczZ);
		ez = (acc.x * vecyZ - acc.y * vecxZ);
		
		/*误差累计，与积分常数相乘*/
		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;

		
		/*用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量*/
		gyro.x += Kp * ex + exInt;
		gyro.y += Kp * ey + eyInt;
		gyro.z += Kp * ez + ezInt;
	}
	/* 一阶近似算法，四元数运动学方程的离散化形式和积分 */
	q0Last = q0;
	q1Last = q1;
	q2Last = q2;
	q3Last = q3;
	q0 += (-q1Last * gyro.x - q2Last * gyro.y - q3Last * gyro.z) * halfT;
	q1 += ( q0Last * gyro.x + q2Last * gyro.z - q3Last * gyro.y) * halfT;
	q2 += ( q0Last * gyro.y - q1Last * gyro.z + q3Last * gyro.x) * halfT;
	q3 += ( q0Last * gyro.z + q1Last * gyro.y - q2Last * gyro.x) * halfT;

	/*单位化四元数*/
	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	/*四元数的平方*/
	q0s = q0 * q0;
	q1s = q1 * q1;
	q2s = q2 * q2;
	q3s = q3 * q3;
	
//	R11 = q0s + q1s - q2s - q3s;	/*矩阵(1,1)项*/
//	R21 = 2 * (q1 * q2 + q0 * q3);	/*矩阵(2,1)项*/

	/*机体坐标系下的Z方向向量*/
	vecxZ = 2 * (q1 * q3 - q0 * q2);/*矩阵(3,1)项*/
	vecyZ = 2 * (q0 * q1 + q2 * q3);/*矩阵(3,2)项*/
	veczZ = q0s - q1s - q2s + q3s;	/*矩阵(3,3)项*/
	
	if (vecxZ>1) vecxZ=1;
	if (vecxZ<-1) vecxZ=-1;
	
	/*计算roll pitch yaw 欧拉角*/
	S_Pitch = asinf(vecxZ) * 57.29578f; 
	S_Roll = atan2f(vecyZ, veczZ) * 57.29578f;
	//S_Yaw = atan2f(R21, R11) * 57.29578f;				//太飘了，舍弃吧	
}

#endif

#define ANGLE_COUNT  15

static float angleX_buf[ANGLE_COUNT]={0};
static float angleY_buf[ANGLE_COUNT]={0};
static float angleZ_buf[ANGLE_COUNT]={0};

//加速度滤波
static int angle_lvbo()
{
	u8  i=0;
	float Sum_AngleX = 0; 
	float Sum_AngleY = 0; 
	float Sum_AngleZ = 0; 

	for(i = 1 ; i<ANGLE_COUNT; i++)
	{
		angleX_buf[i - 1] = angleX_buf[i];
		angleY_buf[i - 1] = angleY_buf[i];
		angleZ_buf[i - 1] = angleZ_buf[i];
	}

	angleX_buf[ANGLE_COUNT - 1] =Accel_X;
	angleY_buf[ANGLE_COUNT - 1] =Accel_Y;
	angleZ_buf[ANGLE_COUNT - 1] =Accel_Z;

	for(i = 0 ; i < ANGLE_COUNT; i++)
	{
		Sum_AngleX += angleX_buf[i];
		Sum_AngleY += angleY_buf[i];
		Sum_AngleZ += angleZ_buf[i];
	}

	Accel_X = Sum_AngleX / ANGLE_COUNT;
	Accel_Y = Sum_AngleY / ANGLE_COUNT;
	Accel_Z = Sum_AngleZ / ANGLE_COUNT;
	return 1;
}

Axis3f acc;
Axis3f gyo;
//static float angle_dot;
float _angle_dot;
float _angle;
static short temperature;

void up_angle()
{	
	mpu6050_get_data(&Gyro_X, &Gyro_Y, &Gyro_Z, &Accel_X, &Accel_Y , &Accel_Z, &temperature);
	//printf("read:%5d %5d %5d %5d %5d %5d\r\n",Gyro_X,Gyro_Y,Gyro_Z,Accel_X,Accel_Y,Accel_Z);
	angle_lvbo();
	//校准的时候，先打印校准前的值
	//printf("校准前:%5d %5d %5d\r\n",Gyro_X,Gyro_Y,Gyro_Z);
	//在这里减去前面打印的值
	Gyro_X+= GX_JIAO;
	Gyro_Y+= GY_JIAO;
	Gyro_Z+= GZ_JIAO;
	//校准后的值要趋近0
	//printf("校准后:%3.1f %3.1f %3.1f\r\n",Gyro_X,Gyro_Y,Gyro_Z);

	acc.x=(Accel_X)/16384.0;
	acc.y=(Accel_Y)/16384.0;
	acc.z=(Accel_Z)/16384.0;

	gyo.x=Gyro_X* 0.060975;
	gyo.y=Gyro_Y* 0.060975;
	gyo.z=Gyro_Z* 0.060975;

	Xgyro=gyo.x;
	Ygyro=gyo.y;
	Zgyro=gyo.z;

	imuUpdate(acc,gyo,0.01);  //四元数解算，z轴很飘,这个函数其实差不多136us
	
	S_Pitch += GPitch_JIAO;
	
	//小车水平校准，拆下轮胎平放的误差值
	S_Roll += GRoll_JIAO;
	
}

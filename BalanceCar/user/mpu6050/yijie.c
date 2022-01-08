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
	
	_angle=atan2(acceleXY,acceleZ)*180/3.14;    //�������,	
	*angle=(1-P)*(*angle+gyro*T)  +   P*_angle;//һ�׻����˲�
	return *angle;
}

void gyro_jiaozhun(float *gx,float *gy,float *gz)
{	
	#if  0
		printf("gx1:%4.2f,gy:%4.2f��gz:%4.2f\r\n",*gx,*gy,*gz);
	#endif

	*gx+=GX_JIAO;
	*gy+=GY_JIAO;
	*gz+=GZ_JIAO;

	#if  0
		printf("gx2:%4.2f,gy:%4.2f��gz:%4.2f\r\n",*gx,*gy,*gz);
	#endif

	*gx=(*gx/16.4); 
	*gy=(*gy/16.4); 
	*gz=(*gz/16.4);
}

// ��Ԫ����������
#if 1
static float Kp = 0.8f;		/*��������*/
static float Ki = 0.001f;		/*��������*/
static float exInt = 0.0f;
static float eyInt = 0.0f;
static float ezInt = 0.0f;		/*��������ۼ�*/

static float q0 = 1.0f;	/*��Ԫ��*/
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	

float S_Yaw,S_Pitch,S_Roll;  //ƫ���ǣ������ǣ�������
int Xgyro;
int Ygyro;
int Zgyro;

float invSqrt(float x)	/*���ٿ�ƽ����*/
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


void imuUpdate(Axis3f acc, Axis3f gyro, float dt)	/*�����ں� �����˲�*/
{
	float normalise;
	float ex, ey, ez;
	float q0s, q1s, q2s, q3s;	/*��Ԫ����ƽ��*/
	//static float R11,R21;		/*����(1,1),(2,1)��*/
	static float vecxZ, vecyZ, veczZ;	/*��������ϵ�µ�Z��������*/
	float halfT =0.5f * dt;
	float q0Last;
	float q1Last;
	float q2Last;
	float q3Last;
	

	gyro.x = gyro.x * 0.017453293f;	/* ��ת���� */
	gyro.y = gyro.y * 0.017453293f;
	gyro.z = gyro.z * 0.017453293f;

	/* ĳһ��������ٶȲ�Ϊ0 */
	if((acc.x != 0.0f) || (acc.y != 0.0f) || (acc.z != 0.0f))
	{
		/*��λ�����ټƲ���ֵ*/
		normalise = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
		acc.x *= normalise;
		acc.y *= normalise;
		acc.z *= normalise;

		/*���ټƶ�ȡ�ķ������������ټƷ���Ĳ�ֵ����������˼���*/
		ex = (acc.y * veczZ - acc.z * vecyZ);
		ey = (acc.z * vecxZ - acc.x * veczZ);
		ez = (acc.x * vecyZ - acc.y * vecxZ);
		
		/*����ۼƣ�����ֳ������*/
		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;

		
		/*�ò���������PI����������ƫ�����������ݶ����е�ƫ����*/
		gyro.x += Kp * ex + exInt;
		gyro.y += Kp * ey + eyInt;
		gyro.z += Kp * ez + ezInt;
	}
	/* һ�׽����㷨����Ԫ���˶�ѧ���̵���ɢ����ʽ�ͻ��� */
	q0Last = q0;
	q1Last = q1;
	q2Last = q2;
	q3Last = q3;
	q0 += (-q1Last * gyro.x - q2Last * gyro.y - q3Last * gyro.z) * halfT;
	q1 += ( q0Last * gyro.x + q2Last * gyro.z - q3Last * gyro.y) * halfT;
	q2 += ( q0Last * gyro.y - q1Last * gyro.z + q3Last * gyro.x) * halfT;
	q3 += ( q0Last * gyro.z + q1Last * gyro.y - q2Last * gyro.x) * halfT;

	/*��λ����Ԫ��*/
	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	/*��Ԫ����ƽ��*/
	q0s = q0 * q0;
	q1s = q1 * q1;
	q2s = q2 * q2;
	q3s = q3 * q3;
	
//	R11 = q0s + q1s - q2s - q3s;	/*����(1,1)��*/
//	R21 = 2 * (q1 * q2 + q0 * q3);	/*����(2,1)��*/

	/*��������ϵ�µ�Z��������*/
	vecxZ = 2 * (q1 * q3 - q0 * q2);/*����(3,1)��*/
	vecyZ = 2 * (q0 * q1 + q2 * q3);/*����(3,2)��*/
	veczZ = q0s - q1s - q2s + q3s;	/*����(3,3)��*/
	
	if (vecxZ>1) vecxZ=1;
	if (vecxZ<-1) vecxZ=-1;
	
	/*����roll pitch yaw ŷ����*/
	S_Pitch = asinf(vecxZ) * 57.29578f; 
	S_Roll = atan2f(vecyZ, veczZ) * 57.29578f;
	//S_Yaw = atan2f(R21, R11) * 57.29578f;				//̫Ʈ�ˣ�������	
}

#endif

#define ANGLE_COUNT  15

static float angleX_buf[ANGLE_COUNT]={0};
static float angleY_buf[ANGLE_COUNT]={0};
static float angleZ_buf[ANGLE_COUNT]={0};

//���ٶ��˲�
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
	//У׼��ʱ���ȴ�ӡУ׼ǰ��ֵ
	//printf("У׼ǰ:%5d %5d %5d\r\n",Gyro_X,Gyro_Y,Gyro_Z);
	//�������ȥǰ���ӡ��ֵ
	Gyro_X+= 50;
	Gyro_Y+= 3;
	Gyro_Z+= 30;
	//У׼���ֵҪ����0
	//printf("У׼��:%3.1f %3.1f %3.1f\r\n",Gyro_X,Gyro_Y,Gyro_Z);

	acc.x=(Accel_X)/16384.0;
	acc.y=(Accel_Y)/16384.0;
	acc.z=(Accel_Z)/16384.0;

	gyo.x=Gyro_X* 0.060975;
	gyo.y=Gyro_Y* 0.060975;
	gyo.z=Gyro_Z* 0.060975;

	Xgyro=gyo.x;
	Ygyro=gyo.y;
	Zgyro=gyo.z;

	imuUpdate(acc,gyo,0.01);  //��Ԫ�����㣬z���Ʈ,���������ʵ���136us
	
	S_Pitch -= 4.6;
	
	//С��ˮƽУ׼��������̥ƽ�ŵ����ֵ
	S_Roll -= 0;
	
}

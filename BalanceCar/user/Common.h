#ifndef _COMMON_H__
#define _COMMON_H__

#include "stm32f10x.h"
#include "nrf24l01.h"
#include "Bluetooth.h"

#ifndef	TRUE
	#define		FALSE		0
	#define		TRUE		1
#endif
#ifndef	_OK_
	#define		_OK_		0
	#define		_ERR_		1
#endif

#define LIMIT( x,min,max )	( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define  REMOTE_MODE_PS2			0
#define  REMOTE_MODE_BLUETOOTH		1

#define REMOTE_CAR_TYPE_DEFAULT	REMOTE_CAR_TYPE_MINI_CAR	//Ĭ��С������
#define REMOTE_CAR_TYPE_BALANCE		0	//2��ƽ�⳵
#define REMOTE_CAR_TYPE_MINI_CAR	1	//2��MINIң�س�
#define REMOTE_CAR_TYPE_MAIK		2	//4�������ķ�ֳ�

#define REMOTE_MODE_DEFAULT	REMOTE_MODE_REMOTE	//Ĭ��ң��ģʽ
#define REMOTE_MODE_REMOTE	0	//2.4gң����ң��
#define REMOTE_MODE_APP		1	//�ֻ�appң��
#define REMOTE_MODE_IR		2	//����ң��

#define REMOTE_CAR_RUN_MODE_DEFAULT		REMOTE_CAR_RUN_MODE_REMOTE		//Ĭ������ģʽ
#define REMOTE_CAR_RUN_MODE_REMOTE		0	//ң�س�����ģʽ_ң��ģʽ
#define REMOTE_CAR_RUN_MODE_FREE		1	//ң�س�����ģʽ_��������ģʽ������������У�
#define REMOTE_CAR_RUN_MODE_LINE_WORK	2	//ң�س�����ģʽ_Ѳ��ģʽ

#define MOTOR_L0_PWM_CH 0		//�����pwmͨ��	
#define MOTOR_L1_PWM_CH 1

#define MOTOR_R0_PWM_CH 2	
#define MOTOR_R1_PWM_CH 3

#define PI 3.1415926f
#define squa( Sq )        (((float)Sq)*((float)Sq))
#define absu16( Math_X )  ((Math_X)<0? -(Math_X):(Math_X))
#define absFloat( Math_X )((Math_X)<0? -(Math_X):(Math_X))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define LIMIT( x,min,max )	( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;

typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;


/*******************************�ṹ��˵���� ϵͳ����********************/
typedef struct tagSysParam
{
//ϵͳ��־�ṹ��
	union 
	{
		u8	All;		//���б�־����
		struct
		{
			u8 FlashState:1;				//FLASH ״̬��0-err��1-ok
		}state;
	}Flag;	
	//u16 BatteryVoltage;							//��ص�ѹֵ

	u8 RemoteCarType;								//ң�س������ͣ�ƽ�⳵��miniС���������ķ�ֳ���
	u8 RemoteMode;									//ң��ģʽ��0-2.4Gң������1-�ֻ�����ң��ģʽ
	
	u8 RemoteCarRunMode;							//ң�س�����ģʽ��ң��ģʽ�����ɱ���ģʽ��Ѳ��ģʽ��
	u16 ConnectTimeCnt;								//��ʱ�ϴν��յ����ݵ����ڵ�ʱ��
	u16 RemoteConnectState;							//ң������״̬
}strSysParam;

typedef struct
{
	uint16_t roll;
	uint16_t pitch;
	uint16_t thr;
	uint16_t yaw;
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;	
}_st_Remote;

typedef volatile struct
{
	float desired;     //< set point
	float offset;      //
	float prevError;    //< previous error
	float integ;        //< integral
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float IntegLimitHigh;       //< integral limit
	float IntegLimitLow;
	float measured;
	float out;
	float OutLimitHigh;
	float OutLimitLow;
}PidObject;

typedef volatile struct
{
	uint8_t unlock;
	

}_st_ALL_flag;

#define MOTOR_SPEED_BUF_NUM	5
typedef struct tagMotorSpeed
{
	int table[MOTOR_SPEED_BUF_NUM];
}strMotorSpeed;

extern strSysParam		SysParam;
extern _st_Remote Remote;
extern _st_ALL_flag ALL_flag;
extern PidObject pidPitch; //�⻷PID����
extern PidObject pidRoll;
extern PidObject pidYaw;
u8 DataSumCheck(u8* dat,u16 dat_len);
int AverageFilter(int* dat,u8 read_times,u8 remove_num);	//�������ȥͷȥβ��ƽ��ֵ


#endif





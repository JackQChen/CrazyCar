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

#define REMOTE_CAR_TYPE_DEFAULT	REMOTE_CAR_TYPE_MINI_CAR	//默认小车类型
#define REMOTE_CAR_TYPE_BALANCE		0	//2轮平衡车
#define REMOTE_CAR_TYPE_MINI_CAR	1	//2轮MINI遥控车
#define REMOTE_CAR_TYPE_MAIK		2	//4轮麦克纳姆轮车

#define REMOTE_MODE_DEFAULT	REMOTE_MODE_REMOTE	//默认遥控模式
#define REMOTE_MODE_REMOTE	0	//2.4g遥控器遥控
#define REMOTE_MODE_APP		1	//手机app遥控
#define REMOTE_MODE_IR		2	//红外遥控

#define REMOTE_CAR_RUN_MODE_DEFAULT		REMOTE_CAR_RUN_MODE_REMOTE		//默认运行模式
#define REMOTE_CAR_RUN_MODE_REMOTE		0	//遥控车控制模式_遥控模式
#define REMOTE_CAR_RUN_MODE_FREE		1	//遥控车控制模式_自主运行模式（随机避障运行）
#define REMOTE_CAR_RUN_MODE_LINE_WORK	2	//遥控车控制模式_巡线模式

#define MOTOR_L0_PWM_CH 0		//电机的pwm通道	
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


/*******************************结构体说明： 系统变量********************/
typedef struct tagSysParam
{
//系统标志结构体
	union 
	{
		u8	All;		//所有标志集合
		struct
		{
			u8 FlashState:1;				//FLASH 状态，0-err，1-ok
		}state;
	}Flag;	
	//u16 BatteryVoltage;							//电池电压值

	u8 RemoteCarType;								//遥控车的类型（平衡车，mini小车，麦克纳姆轮车）
	u8 RemoteMode;									//遥控模式，0-2.4G遥控器，1-手机蓝牙遥控模式
	
	u8 RemoteCarRunMode;							//遥控车运行模式（遥控模式，自由避障模式，巡线模式）
	u16 ConnectTimeCnt;								//计时上次接收到数据到现在的时间
	u16 RemoteConnectState;							//遥控连接状态
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
extern PidObject pidPitch; //外环PID数据
extern PidObject pidRoll;
extern PidObject pidYaw;
u8 DataSumCheck(u8* dat,u16 dat_len);
int AverageFilter(int* dat,u8 read_times,u8 remove_num);	//求排序后去头去尾的平均值


#endif





#include "crt.h"
#include "Common.h"

strCtrParam CtrParam;

float cha1 = 0;		//速度积分
float up_kp = 310.0;	//平衡环
float speed_kp = 1.50, speed_ki = 0.001;//速度环
float trun_kp = 1.2;	//转向环

u8 bizhang_state = 0;		//壁障模式变量 ,默认关闭
u8 genshui_state = 0;		//跟随模式变量 ，默认关闭

//车轮转向处理
static void lun_dir(short left_motor, short right_motor)
{
	LDIR = left_motor > 0 ? 0 : 1;
	RDIR = right_motor > 0 ? 1 : 0;
}

//输出线性化
static int out_linear(int v)
{
	int out;

	if (v < 0)v *= -1;
	out = (1000000) / v;
	if (out >= 65535) return 65535;

	return out;
}

//限幅输出
static int limit_out(int motor)
{
	if (motor > MAX_FREQ) motor = MAX_FREQ;
	return motor;
}

//限幅速度表示
static int limit_speed_out(int motor, int MAX)
{
	if (motor >= MAX) motor = MAX;
	if (motor <= -MAX) motor = -MAX;
	return motor;
}

//直立p控制
static int up_right(float angle)
{
	int out = 0;

	out = up_kp * angle;
	return out;      //计算平衡控制	
}

//转向控制
static int clr_trun(float gyro)
{
	return trun_kp * (gyro - CtrParam.TurnSpeed);      //计算平衡控制
}

//速度滤波
static int speed_lvbo(int moto1, int moto2)
{
	u8  i = 0;
	int Sum_Speed = 0;

	for (i = 1; i < SPEED_COUNT; i++)
	{
		CtrParam.SpeedFilterBuf[i - 1] = CtrParam.SpeedFilterBuf[i];
	}
	//蓝牙控制在这里
	CtrParam.SpeedFilterBuf[SPEED_COUNT - 1] = (moto1 + moto2) / 2 - CtrParam.RunSpeed;

	for (i = 0; i < SPEED_COUNT; i++)
	{
		Sum_Speed += CtrParam.SpeedFilterBuf[i];
	}

	return (Sum_Speed / SPEED_COUNT);
}

//速度控制
int clr_speed(int moto1, int moto2)
{
	static float cha = 0;
	float out = 0;

	cha = speed_lvbo(moto1, moto2);
	cha1 += cha;
	if (cha1 > 50000) cha1 = 50000;
	if (cha1 < -50000) cha1 = -50000;

	out = speed_kp * cha + speed_ki * cha1;

	return out;
}

//超声波控制
void csb_crt()
{
	if (csb_juli < 20) CtrParam.RunSpeed = -2000;
	else CtrParam.RunSpeed = 0;
}

//跟随控制
void genshui_crt()
{
	if (csb_juli > 100)
	{
		CtrParam.RunSpeed = 0;
		return;
	}

	if (csb_juli < 20)
	{
		CtrParam.RunSpeed = -1500;
		return;
	}

	if (csb_juli > 25)
	{
		CtrParam.RunSpeed = 1500;
		return;
	}

	CtrParam.RunSpeed = 0;
	//printf("genshui_v:%d csb_juli:%d\r\n",genshui_v,csb_juli);
}

static void set_pwm(u16 left_motor, u16 right_motor)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = left_motor - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 30 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = left_motor >> 1;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = right_motor - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 30 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = right_motor >> 1;

	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
}

void crt()
{
	u16 sl, sr;
	int angle_out = 0;//平衡输出量
	int speed_out = 0;//速度输出量
	int trun_out = 0;//转向输出量

	angle_out = up_right(S_Pitch);	//角度控制
	speed_out = clr_speed(CtrParam.MotorSpeed_L, CtrParam.MotorSpeed_R);//速度控制
	trun_out = clr_trun(Zgyro);//转向控制

	CtrParam.MotorSpeed_L = angle_out + speed_out - trun_out + BluetoothKeyHandle.Handle.Ch1Value;
	CtrParam.MotorSpeed_R = angle_out + speed_out + trun_out + BluetoothKeyHandle.Handle.Ch2Value;

	//限制速度
	CtrParam.MotorSpeed_L = limit_speed_out(CtrParam.MotorSpeed_L, 6000);
	CtrParam.MotorSpeed_R = limit_speed_out(CtrParam.MotorSpeed_R, 6000);

	//这里添加其中一个轮子方向锁死的逻辑，避免转弯抖动

	//车轮方向
	lun_dir(CtrParam.MotorSpeed_L, CtrParam.MotorSpeed_R);
	sl = CtrParam.MotorSpeed_L > 0 ? CtrParam.MotorSpeed_L : -CtrParam.MotorSpeed_L;
	sr = CtrParam.MotorSpeed_R > 0 ? CtrParam.MotorSpeed_R : -CtrParam.MotorSpeed_R;

	//输出线性化
	sl = out_linear(sl);
	sr = out_linear(sr);

	//限幅输出
	sl = limit_out(sl);
	sr = limit_out(sr);

	//设置电机速度
	set_pwm(sl, sr);
}

void ClearCtrParam(void)
{
	u8 i;

	CtrParam.MotorSpeed_L = 0;
	CtrParam.MotorSpeed_R = 0;
	for (i = 0; i < SPEED_COUNT; i++)
	{
		CtrParam.SpeedFilterBuf[i] = 0;
	}
}





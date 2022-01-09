#include "config.h"
#include "niming_swj.h"
#include <stdio.h>
#include <math.h>
#include "Common.h"

u8 run_state = 0;			//小车控制标志
u32 Mcount = 0;			//计数

int main()
{
	u8 n = 10;

	LED_init('C', 14);
	LED_init('A', 4);	//LED初始化 

	LED1 = 1;

	serial1_init(9600);		//调试使用

	iic_init();						//mpu6050初始化
	delay_ms(100);
	printf("Initial device...\n");
	mpu6050_init();
	Adc_Init();						//初始化电池电压采集
	KEY_Init();						//初始化用户按键采集
	A4988_init();					//a4988初始化	
	A4988_en(1);					//关闭A4988
	csb_init();						//超声波初始化	
	NRF24L01_Configuration();
	while (n--)
	{
		LED1 = !LED1;
		delay_ms(50);
	}

	LED1 = 0;
	printf("Initialization complete.\n");
	SysParam.RemoteMode = REMOTE_MODE_APP; 			//设置遥控模式为应用

	while (1)
	{
		u8 sta = 0;
		delay_ms(10);

		if (Uart.RxState == UART_RX_OK)	//接收到应用数据
		{
			switch (BluetoothKeyHandle.KeyNum)
			{
			case CONTROL_MOTOR:
				run_state = run_state == 0 ? 1 : 0;
				LED1 = run_state;
				break;
			case CONTROL_LIGHT:
				break;
			case CONTROL_CONTROL:
				cha1 = 0;
				CtrParam.RunSpeed = (BluetoothKeyHandle.Handle.Ch1Value - 100) * 20;
				CtrParam.TurnSpeed = (BluetoothKeyHandle.Handle.Ch2Value - 100) * 20;
				break;
			}
			Uart.RxState = UART_RX_READY;
		}
		//更新姿态,如果更换了mpu6050，请在这里面校准
		up_angle();
		//发给上位机，查看姿态
		//if (Mcount % 100 == 0)
		//	printf("Pitch:%3.2f\n",S_Pitch);

#if 1
	//平衡小车控制-----------------------------------------
		if (run_state == 0 || S_Pitch > 45 || S_Pitch < -45)
		{
			if (run_state == 1)
			{
				run_state = 0;
				LED1 = 0;
				ClearCtrParam();
			}
			A4988_en(1);
		}
		else
		{
			crt();				//所有小车的控制-----------------------------------
			A4988_en(0);
		}
#endif	

		//更新按键，10*5ms=50ms-----------------------------
		if (Mcount % 10 == 0)
		{
			sta = up_key();
			if (sta == 1)
			{
				if (run_state == 0)
				{
					run_state = 1;
					LED1 = 1;
				}
				else
				{
					run_state = 0;
					LED1 = 0;
					ClearCtrParam();
				}
			}
		}
		//更新电池，100*5ms=500ms-----------------------------
		if (Mcount % 100 == 0)
		{
			up_btv();
			//	printf("V:%d\r\n",btv);	
			//低电压检测，低于7V报警------------------------------------
			if (btv < 7000)
			{
				if (btv < 7000 && btv>5000) LED1 = !LED1;
			}
		}
		if (Mcount >= 400) Mcount = 0;
		Mcount++;
	}
}










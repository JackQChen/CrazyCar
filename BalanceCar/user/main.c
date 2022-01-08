#include "config.h"
#include "niming_swj.h"
#include <stdio.h>
#include <math.h>
#include "Common.h"

u8 run_state=0;			//С�����Ʊ�־
u32 Mcount=0;			//����

int  main()
{
	u8 n=10;

	LED_init('C',14);
	LED_init('A',4);	//LED��ʼ��
	
	LED1=1;

	serial1_init(9600);			//����ģ��ʹ��
	serial2_init(256000);		//����ʹ��
	
	iic_init();						//mpu6050��ʼ��
	delay_ms(100);
	printf("��ʼ��MPU\r\n");
	mpu6050_init();
	Adc_Init();						//��ʼ����ص�ѹ�ɼ�
	KEY_Init();						//��ʼ���û������ɼ�
	A4988_init();					//a4988��ʼ��	
	A4988_en(1);					//�ر�A4988
	csb_init();						//��������ʼ��	
	NRF24L01_Configuration();
	while(n--)
	{
		LED1=!LED1;
		delay_ms(50);
	}
	
	LED1=0;	
	printf("ƽ�⳵Ӳ����ʼ�����\r\n");
	SysParam.RemoteMode = REMOTE_MODE_APP; 			//����ң��ģʽΪ����ң��	
	
	while(1)
	{
		u8 sta=0;			
		delay_ms(10);
		
		SysParam.ConnectTimeCnt++;				//����ֵ�Լ�
		if(RC_Analy() == _OK_)	//����2.4Gң������
		{
			SysParam.RemoteMode = REMOTE_MODE_REMOTE; 			//����ң��ģʽΪ����ң��
			SysParam.ConnectTimeCnt  = 0;				//�嶨ʱ����ֵ
			SysParam.RemoteConnectState = 1;			//ң���Ѿ�����
		}

		if(Uart.RxState == UART_RX_OK)	//���յ���������
		{
			SysParam.RemoteMode = REMOTE_MODE_APP; 			//����ң��ģʽΪ����ң��		
			SysParam.ConnectTimeCnt  = 0;				//�嶨ʱ����ֵ
			SysParam.RemoteConnectState = 1;			//ң���Ѿ�����	
			Uart.RxState = UART_RX_READY;
			
			cmdBluetoothData();
			UpdateKeyHandleValue();					
			if(BluetoothKeyHandle.KeyNum == L_LEFT_UP)	SysParam.RemoteCarRunMode	= REMOTE_CAR_RUN_MODE_FREE;
			if(BluetoothKeyHandle.KeyNum == R_RIGHT_UP)	SysParam.RemoteCarRunMode	= REMOTE_CAR_RUN_MODE_REMOTE;
			//UartSendData(pProtocolData,MAX_UART_DATA_LEN);
		}
		
		if(SysParam.ConnectTimeCnt >= 10)
		{
			SysParam.ConnectTimeCnt  = 0;				//�嶨ʱ����ֵ
			SysParam.RemoteConnectState = 0;
		}	

		if(SysParam.RemoteConnectState == 1)
		{
			cha1=0;
			if(SysParam.RemoteMode == REMOTE_MODE_REMOTE)
			{			
				CtrParam.RunSpeed	= (Remote.pitch - 1500) * 4;
				CtrParam.TurnSpeed	= (Remote.roll - 1500) * 4;
			}else if(SysParam.RemoteMode == REMOTE_MODE_APP)
			{
				CtrParam.RunSpeed	= (BluetoothKeyHandle.Handle.Ch3Value - 100) * 20;
				CtrParam.TurnSpeed	= (BluetoothKeyHandle.Handle.Ch4Value - 100) * 20;				
			}
		}else
		{
			if(SysParam.RemoteMode == REMOTE_MODE_REMOTE)
			{			
				Remote.pitch = 1500;
				Remote.roll  = 1500;
			}else if(SysParam.RemoteMode == REMOTE_MODE_APP)
			{
				Remote.pitch = 100;
				Remote.roll  = 100;	
			}		
			CtrParam.TurnSpeed	= 0;
			CtrParam.TurnSpeed	= 0;			
		}
		up_angle();			//������̬,���������mpu6050������������У׼		
		//����������λ����ʵʱ�鿴��̬
		usart2_report_imu((int)(S_Roll*100),(int)(10),(int)(10),0,0,0,(int)(S_Roll*100),(int)(100),(int)(10));				
		//	printf("Sroll:%3.2f\r\n",S_Roll);	

		#if 1
			//ƽ��С������-----------------------------------------
			if(run_state==0||S_Pitch>45||S_Pitch<-45)
			{
				if(run_state==1)
				{
					run_state=0;
					LED1=0;
					ClearCtrParam();					
				}
				A4988_en(1);
			}else
			{
				crt();				//����С���Ŀ���-----------------------------------
				A4988_en(0);
			}
		#endif	
		 
		//���°�����10*5ms=50ms-----------------------------
		if(Mcount%10==0)
		{
			sta=up_key();
			if(sta==1)
			{
				if(run_state==0)
				{
					run_state=1;
					LED1=1;
				}
				else
				{
					run_state=0;
					LED1=0; 
					ClearCtrParam();
				}
			}
		}
		//���³�������10*5ms=50ms--------------------------
		if(Mcount%10==0)
		{
			//Ĭ�Ϲرճ���������
			if(bizhang_state||genshui_state) csb_juli = start_ceju();
			else csb_juli = 50;
			//���������Ͽ���  ,�������޿�����ʹ��
			if(bizhang_state ==1) csb_crt();
			//�������������  ,�������޿�����ʹ��
			if(genshui_state ==1) genshui_crt();
			// printf("juli:%d\r\n",csb_juli);
		}		 
		//���µ�أ�100*5ms=500ms-----------------------------
		if(Mcount%100==0)
		{
			up_btv();
			//	printf("V:%d\r\n",btv);	
			//�͵�ѹ��⣬����7V����------------------------------------
			if(btv<7000)
			{
				if(btv<7000&&btv>5000) LED1=!LED1;	
			} 
		}
		if(Mcount>=400) Mcount=0;
		Mcount++;	 
	}	
}










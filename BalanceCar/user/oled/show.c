#include "show.h"
#include "config.h"


/**************************************************************************
�������ܣ�OLED��ʾ
**************************************************************************/
void oled_show(void)
{
		OLED_ShowString(60,0,(unsigned char*)"HES");
		
		//�Ƕ�
		if(ANGLE>=0)
		{
			OLED_ShowString(1,10,(unsigned char*)"Angle+");
			OLED_ShowNumber(60,10,(u32)ANGLE,5,12);
		}
		else
		{
			OLED_ShowString(1,10,(unsigned char*)"Angle-");
			OLED_ShowNumber(60,10,(u32)(ANGLE*-1),5,12);
		}
		
		//���ٶ�
		if(LV>=0)
		{
			OLED_ShowString(1,20,(unsigned char*)"Lmoto+");
			OLED_ShowNumber(60,20,LV,6,12);
		}
		else
		{
			OLED_ShowString(1,20,(unsigned char*)"Lmoto-");
			OLED_ShowNumber(60,20,(u32)(LV*-1),6,12);
		}
		
		//���ٶ�
		if(RV>=0)
		{
			OLED_ShowString(1,30,(unsigned char*)"Rmoto+");
			OLED_ShowNumber(60,30,RV,6,12);
		}
		else
		{
			OLED_ShowString(1,30,(unsigned char*)"Rmoto-");
			OLED_ShowNumber(60,30,(u32)(RV*-1),6,12);
		}
		
		//��ѹ
		if(BTV>=5000)
		{
			OLED_ShowString(1,40,(unsigned char*)"BTV");
			OLED_ShowNumber(60,40,(u32)BTV,6,12);
		}
		else
		{
			OLED_ShowString(1,40,(unsigned char*)"NOBTV");
		}
		
		
		OLED_Refresh_Gram();	
}

//��ʼ����ʾ
/*
	type:  ��ʼ������
	state: ״̬

*/
void oled_show_init(u8 type,u8 state)
{
	switch(type)
	{
		case 0x1:
			//��ʼ���ɹ�
			if(state==1)
			{
				OLED_ShowString(10,10,"MPU6050 ok");
			}
			else
			{
				OLED_ShowString(10,10,"MPU6050 erro!");
			}
		
		break;
		
		case 0x2:
			//��ʼ���ɹ�
			if(state==1)
			{
				
			}
			else
			{
				
			}
		break;
		
		case 0x3:
			//��ʼ���ɹ�
			if(state==1)
			{
				
			}
			else
			{
				
			}
		break;
		
	}
	
	OLED_Refresh_Gram();	
	
	
}



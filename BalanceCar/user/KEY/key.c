#include "key.h"
#include "config.h"
/////////////////	 
 	    
//按键初始化函数 

void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTC时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //端口配置为
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化端口		
} 

//按键处理函数
//返回按键值
u8 up_key()
{	 		  
	if(SYS_EN==0)	//判断按键
	{		
		delay_ms(10);	//延时10ms	
		if(SYS_EN==0)	//判断按键
		{
			while(SYS_EN==0);
			return 1;// 
		}	
	}	
	return 0;// 无按键按下
}










#include "led.h"

void LED_init(char GP,int IO)
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能PORTB,C时钟和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG

	//使能IO总线时钟
	switch(GP)
	{
		case 'A':RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 break;
		case 'B':RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 break;
		case 'C':RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 break;
		case 'D':RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE); 	 break;
		case 'E':RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);	 break;
	}
	/*  配置GPIO的模式和IO口 */
	switch(IO)
	{
		case 0:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0; break;
		case 1:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1; break;
		case 2:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;break;
		case 3:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;break;
		case 4:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;break;
		case 5:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;break;
		case 6:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;break;
		case 7:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;break;
		case 8:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;break;
		case 9:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;break;
		case 10:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;break;
		case 11:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;break;
		case 12:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;break;
		case 13:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;break;
		case 14:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;break;
		case 15:GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;break;

	}
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	switch(GP)
	{
		case 'A':GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* 初始化GPIO */ break;
		case 'B':GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* 初始化GPIO */ break;
		case 'C':GPIO_Init(GPIOC,&GPIO_InitStructure); 	   /* 初始化GPIO */break;
		case 'D':GPIO_Init(GPIOD,&GPIO_InitStructure); 	   /* 初始化GPIO */break;
		case 'E':GPIO_Init(GPIOE,&GPIO_InitStructure); 	   /* 初始化GPIO */break;
	}	
}

			
			
		

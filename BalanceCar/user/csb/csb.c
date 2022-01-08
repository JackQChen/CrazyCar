#include "csb.h"
#include "config.h"

u16 csb_juli;

//超声波初始化
void csb_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	/*TRIG触发信号*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructer);

	/*ECOH回响信号*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOB, & GPIO_InitStructer);

	TIM4_Int_Init(999,71);//1Mhz的计数频率，计数到1000为1ms 
}

//获取距离
u16 start_ceju()
{
	u16 tim=0;
	u32 juli=0;

	//发送一个20us的高电平
	TRIG=1;
	delay_us(15);
	TRIG=0;

	/*等待回响信号,ECHO为高电平*/
	while(ECHO==0);
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	/*等待ECHO为低电平*/
	while(ECHO==1);//回响信号消失
	TIM_Cmd(TIM3,DISABLE);//关闭定时器	
	tim=TIM3->CNT;//获取计TIM3数寄存器中的计数值，一边计算回响信号时间
	//juli=(tim+counts*1000)/58;//通过回响信号计算距离
	//juli=(float)(tim+counts*1000)/2*0.346;//通过回响信号计算距离
	//  1/2C×T
	juli=(tim+Tcount*1000)/50;
	if(juli>345) juli=345;		//如果距离超过我们测的最大距离，则把距离值变为0

	TIM3->CNT=0;  //将TIM2计数寄存器的计数值清零
	Tcount=0;  		//中断溢出次数清零
	
	return juli;
}











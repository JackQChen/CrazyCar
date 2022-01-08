#include "csb.h"
#include "config.h"

u16 csb_juli;

//��������ʼ��
void csb_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	/*TRIG�����ź�*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructer);

	/*ECOH�����ź�*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOB, & GPIO_InitStructer);

	TIM4_Int_Init(999,71);//1Mhz�ļ���Ƶ�ʣ�������1000Ϊ1ms 
}

//��ȡ����
u16 start_ceju()
{
	u16 tim=0;
	u32 juli=0;

	//����һ��20us�ĸߵ�ƽ
	TRIG=1;
	delay_us(15);
	TRIG=0;

	/*�ȴ������ź�,ECHOΪ�ߵ�ƽ*/
	while(ECHO==0);
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	/*�ȴ�ECHOΪ�͵�ƽ*/
	while(ECHO==1);//�����ź���ʧ
	TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��	
	tim=TIM3->CNT;//��ȡ��TIM3���Ĵ����еļ���ֵ��һ�߼�������ź�ʱ��
	//juli=(tim+counts*1000)/58;//ͨ�������źż������
	//juli=(float)(tim+counts*1000)/2*0.346;//ͨ�������źż������
	//  1/2C��T
	juli=(tim+Tcount*1000)/50;
	if(juli>345) juli=345;		//������볬�����ǲ�������룬��Ѿ���ֵ��Ϊ0

	TIM3->CNT=0;  //��TIM2�����Ĵ����ļ���ֵ����
	Tcount=0;  		//�ж������������
	
	return juli;
}











#include "A4988.h"

//static u16  capture = 0;
u16 CCR1_Val = 60000;
u16 CCR2_Val = 60000;

//��ʼ��
void A4988_init()
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  //left setp
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  //right setp 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 7200 - 1;  // (Period + 1) * (Prescaler + 1) / 72M = 1ms  
	TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 3600;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //PB9  left
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //PB9  left
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);           

	/* TIM2 enable counter */
	TIM_Cmd(TIM3, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 7200 - 1;    
	TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 3600;

	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //PA1	right
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//MOE=1,����OC��OCN���
	/* TIM2 enable counter */
	TIM_Cmd(TIM1, ENABLE);  

	//���Ʋ����������
	LED_init('A',5);
	LED_init('B',0);

	//���Ʋ������en //1ֹͣ
	LED_init('A',7);
	LED_init('B',12);
	A4988_en(1);
}

//ʹ��
void A4988_en(u8 state)
{
	LEN=state;
	REN=state;
}

void TIM1_PWMConfig()
{	
	u16 right_motor;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTA,PORTCʱ��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�˿�����Ϊ
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ���˿�		
	 
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);  //ʹ����ӳ�书��
	
	//GPIO_Config( GPIOA,RCC_APB2Periph_GPIOA,GPIO_Mode_AF_PP,GPIO_Pin_8|GPIO_Pin_9);
	//GPIO_Config( GPIOB,RCC_APB2Periph_GPIOB,GPIO_Mode_AF_PP,GPIO_Pin_13|GPIO_Pin_14);

	right_motor = 1000;
	TIM_TimeBaseInitStructure.TIM_Period = right_motor - 1;    
	TIM_TimeBaseInitStructure.TIM_Prescaler = 30  - 1; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState =TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse =right_motor/2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_High; // �ߵ�ƽΪ��Ч
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	
	//TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;//TIM_Break_Enable;
	TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_DeadTime=28;
	TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;
	TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Disable;//TIM_OSSIState_Disable;//TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Disable;//TIM_OSSRState_Disable;//TIM_OSSRState_Enable;
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//MOE=1,����OC��OCN���
	TIM_Cmd(TIM1,ENABLE);
	
	//���Ʋ����������
	LED_init('A',5);
	LED_init('B',0);

	//���Ʋ������en //1ֹͣ
	LED_init('A',7);
	LED_init('B',12);
	A4988_en(0);	
//	
//	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
//	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
}


//����������
//���Ƹ߼���ʱ��ĳ��ͨ�����䷴��ͨ�����������ر�

void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx);
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN);





#include "serial.h"  
#include "Bluetooth.h"
#include "crt.h"

strUart Uart;
strProtocol	ProtocolData;
u8* pProtocolData = (u8*)&ProtocolData;


int fputc(int ch, FILE* p)       //在使用printf时系统自动调用此函数  
{
	USART_SendData(USART2, (u8)ch);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	return ch;
}
/*******************************************************************************
* 函 数 名         : serial_init
* 函数功能         : IO端口及串口1，时钟初始化函数    A9,A10
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial1_init(u32 bt)
{
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO  
	NVIC_InitTypeDef NVIC_InitStructure;     //中断结构体定义  
	USART_InitTypeDef  USART_InitStructure;   //串口结构体定义  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = bt;   //波特率设置为bt  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断 接收中断  
	USART_ClearFlag(USART1, USART_FLAG_TC);//清除USARTx的待处理标志位   
}

/*******************************************************************************
* 函 数 名         : serial2_init
* 函数功能         : IO端口及串口2，时钟初始化函数     A2,A3
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial2_init(u32 bt)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO  
	//使能串口的RCC时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能UART3所在GPIOB的时钟  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//串口使用的GPIO口配置  
	// Configure USART2 Rx (PB.11) as input floating    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART2 Tx (PB.10) as alternate function push-pull  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//配置串口  
	USART_InitStructure.USART_BaudRate = bt;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART3   
	USART_Init(USART2, &USART_InitStructure);//配置串口2

	// Enable USART1 Receive interrupts 使能串口接收中断  
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//串口发送中断在发送数据时开启  
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  

	// Enable the USART3   
	USART_Cmd(USART2, ENABLE);//使能串口3  

	//串口中断配置  
	//Configure the NVIC Preemption Priority Bits     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// Enable the USART3 Interrupt   
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函 数 名         : serial3_init
* 函数功能         : IO端口及串口3，时钟初始化函数   B10,B11
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial3_init(u32 bt)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO  
	//使能串口的RCC时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能UART3所在GPIOB的时钟  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//串口使用的GPIO口配置  
	// Configure USART2 Rx (PB.11) as input floating    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configure USART2 Tx (PB.10) as alternate function push-pull  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//配置串口  
	USART_InitStructure.USART_BaudRate = bt;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART3   
	USART_Init(USART3, &USART_InitStructure);//配置串口3  

   // Enable USART1 Receive interrupts 使能串口接收中断  
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//串口发送中断在发送数据时开启  
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  

	// Enable the USART3   
	USART_Cmd(USART3, ENABLE);//使能串口3  

	//串口中断配置  
	//Configure the NVIC Preemption Priority Bits     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// Enable the USART3 Interrupt   
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函 数 名         : serial1_send_char
* 函数功能         : 串口1发送一字节
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial1_send_char(u8 temp)
{
	USART_SendData(USART1, (u8)temp);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* 函 数 名         : serial1_send_buff
* 函数功能         : 串口1发送一字符串
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial1_send_buff(u8 buf[], u32 len)
{
	u32 i;
	for (i = 0; i < len; i++)
		serial1_send_char(buf[i]);
}

/*******************************************************************************
* 函 数 名         : serial2_send_char
* 函数功能         : 串口2发送一字节
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial2_send_char(u8 temp)
{
	USART_SendData(USART2, (u8)temp);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* 函 数 名         : serial2_send_buff
* 函数功能         : 串口2发送一字符串
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial2_send_buff(u8 buf[], u32 len)
{
	u32 i;
	for (i = 0; i < len; i++)
		serial2_send_char(buf[i]);
}

/*******************************************************************************
* 函 数 名         : serial3_send_char
* 函数功能         : 串口3发送一字节
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial3_send_char(u8 temp)
{
	USART_SendData(USART3, (u8)temp);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* 函 数 名         : serial3_send_buff
* 函数功能         : 串口3发送一字符串
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void serial3_send_buff(u8 buf[], u32 len)
{
	u32 i;
	for (i = 0; i < len; i++)
		serial3_send_char(buf[i]);
}

void USART1_IRQHandler(void)
{
	USART_ClearFlag(USART1, USART_FLAG_TC);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != Bit_RESET)//检查指定的USART中断发生与否
	{
		pProtocolData[Uart.RxCnt] = USART_ReceiveData(USART1);//USART_ReceiveData(USART1);

		if ((pProtocolData[Uart.RxCnt] == FRAME_TAIL) && (pProtocolData[0] == FRAME_HEAD))
		{
			Uart.RxCnt = 0;
			Uart.RxState = UART_RX_OK;
		}
		else if (pProtocolData[0] == 0XFD)
		{
			//SBUF = Uart.DataBuff[Uart.RxCnt];
			Uart.RxCnt++;
		}
		else Uart.RxCnt = 0;

		if (Uart.RxCnt > MAX_UART_DATA_LEN) Uart.RxCnt = 0;
	}
}

void USART2_IRQHandler(void)
{
	USART_ClearFlag(USART2, USART_FLAG_TC);
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != Bit_RESET)//检查指定的USART中断发生与否
	{
		u8	k = USART_ReceiveData(USART2);
	}
}




#include "serial.h"  
#include "Bluetooth.h"
#include "crt.h"

strUart Uart;
strProtocol ProtocolData;
u8 dataArray[16];
u8 dataControl, dataCheck;
u8* pProtocolData = (u8*)&ProtocolData;

int fputc(int ch, FILE* p)       //在使用printf时系统自动调用此函数  
{
	USART_SendData(USART1, (u8)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
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
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);    //使能USART1，GPIOA时钟
	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9

	//USART1_RX      PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器

   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bt;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口
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

u8 get_length(u8 control)
{
	switch (control)
	{
	case CONTROL_MOTOR:
	case CONTROL_LIGHT:
		return 2;
	case CONTROL_CONTROL:
		return 5;
	}
	return 0;
}

void process_data(u8 control)
{
	switch (control)
	{
	case CONTROL_MOTOR:
	case CONTROL_LIGHT:
		BluetoothKeyHandle.Handle.Ch1Value = dataArray[1];
		break;
	case CONTROL_CONTROL:
		BluetoothKeyHandle.Handle.Ch1Value = dataArray[1];
		BluetoothKeyHandle.Handle.Ch1Value = dataArray[2];
		break;
	}
}

// Control | Data | Check
void USART1_IRQHandler(void)
{
	USART_ClearFlag(USART1, USART_FLAG_TC);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != Bit_RESET)//检查指定的USART中断发生与否
	{
		dataArray[Uart.RxCnt] = USART_ReceiveData(USART1);
		dataCheck += dataArray[Uart.RxCnt];
		if (Uart.RxCnt == 0)
			dataControl = dataArray[0];
		if (Uart.RxCnt == get_length(dataControl))
		{
			if (++dataCheck == 0)
			{
				BluetoothKeyHandle.KeyNum = dataControl;
				process_data(dataControl);
				Uart.RxState = UART_RX_OK;
			}
			dataCheck = 0;
			Uart.RxCnt = 0;
		}
		else
			Uart.RxCnt++;
	}
	if (Uart.RxCnt > MAX_UART_DATA_LEN)
		Uart.RxCnt = 0;
}

void USART2_IRQHandler(void)
{
	USART_ClearFlag(USART2, USART_FLAG_TC);
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != Bit_RESET)//检查指定的USART中断发生与否
	{
		u8 k = USART_ReceiveData(USART2);
	}
}



#include "nrf24l01.h"
#include "SPI.h"
#include <string.h>
#include "Common.h"

//�������
const uint8_t TX_ADDRESS[]= {0x11,0x22,0x33,0x44,0x55};	//���ص�ַ
const uint8_t RX_ADDRESS[]= {0x11,0x22,0x33,0x44,0x55};	//���յ�ַRX_ADDR_P0 == RX_ADDR

//��ʼ��24L01��IO��
void NRF24L01_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����ʹ�ܸ��ù��ܲ��ܽ��йر�jtag
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//�ر�jtag����ΪPA15��jtag���ܽ�
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);    //ʹ��GPIO��ʱ��  CE
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;          //NRF24L01_CE  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;          //NRF24L01_CSN      
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                    //��ʼ��ʱ������
	Set_NRF24L01_CSN;                                   //��ʼ��ʱ������

    //����NRF2401��IRQ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;     //��������          //NRF24L01_IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);

	SPI_Config();                                //��ʼ��SPI
	Clr_NRF24L01_CE; 	                                //ʹ��24L01
	Set_NRF24L01_CSN;                                   //SPIƬѡȡ��
}
//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ�
//��ͬʱ����ֵ:0����ʾ��λ;���򷵻�1����ʾ����λ	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);              //����д��ĵ�ַ  	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;					   
	if(i!=5)return 1;                               //NRF24L01����λ	
	return 0;		                                //NRF24L01��λ
}	 	 
//ͨ��SPIд�Ĵ���
u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;                    //ʹ��SPI����
  	status =SPI_RW(regaddr); //���ͼĴ����� 
  	SPI_RW(data);            //д��Ĵ�����ֵ
  	Set_NRF24L01_CSN;                    //��ֹSPI����	   
  	return(status);       		         //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
	Clr_NRF24L01_CSN;                //ʹ��SPI����		
  	SPI_RW(regaddr);     //���ͼĴ�����
  	reg_val=SPI_RW(0XFF);//��ȡ�Ĵ�������
  	Set_NRF24L01_CSN;                //��ֹSPI����		    
  	return(reg_val);                 //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;                     //ʹ��SPI����
  	status=SPI_RW(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI_RW(0XFF);//��������
  	Set_NRF24L01_CSN;                     //�ر�SPI����
  	return status;                        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
	Clr_NRF24L01_CSN;                                    //ʹ��SPI����
  	status = SPI_RW(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI_RW(*pBuf++); //д������	 
  	Set_NRF24L01_CSN;                                    //�ر�SPI����
  	return status;                                       //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;   
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
	Set_NRF24L01_CE;                                     //��������	   
	while(READ_NRF24L01_IRQ!=0);                         //�ȴ��������
	state=NRF24L01_Read_Reg(STATUS);                     //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)                                     //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //�������
	{
		return SUCCESS;
	}
	return FAILED;                                         //����ԭ����ʧ��
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)                                 //���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //���RX FIFO�Ĵ��� 
		return SUCCESS; 
	}	   
	return FAILED;                                      //û�յ��κ�����
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
    //дRX�ڵ��ַ
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);

    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    
    //ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); 
	  //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,45);	  //(Ҫ�ĵ������ƣ��Ͱ�45�ĳɸ�ң��������һ���ġ��Ϳ��Ե���������)
    //ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
    //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+NCONFIG, 0x0f); 
    //CEΪ��,�������ģʽ 
  	Set_NRF24L01_CE;                                
}			

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,
//ѡ��RFƵ��,�����ʺ�LNA HCURR PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(void)
{														 
	Clr_NRF24L01_CE;	    
    //дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    
    //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 

    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     
    //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); 
    //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);
    //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,45);       //(Ҫ�ĵ������ƣ��Ͱ�45�ĳɸ�ң��������һ���ġ��Ϳ��Ե���������)
    //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //0x27  250K   0x07 1M
    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ,���������ж�
  	NRF24L01_Write_Reg(SPI_WRITE_REG+NCONFIG,0x0e);    
    // CEΪ��,10us����������
	Set_NRF24L01_CE;                                  
}		  

void NRF24L01_init(void)
{
	NRF24L01_Configuration();
	do
	{
		RX_Mode();
	}while(NRF24L01_Check() == FAILED);
}

uint8_t RC_rxData[32];
void remote_unlock(void);	


u8 RC_Analy(void)  
{
	static uint16_t cnt;
	
	//Receive  and check RC data 
	if(NRF24L01_RxPacket(RC_rxData)==SUCCESS)
	{ 	
		uint8_t i;
		uint8_t CheckSum=0;
		cnt = 0;
		for(i=0;i<31;i++)
		{
			CheckSum +=  RC_rxData[i];
		}
		if(RC_rxData[31]==CheckSum && RC_rxData[0]==0xAA && RC_rxData[1]==0xAF)  //������յ���ң��������ȷ
		{
			Remote.roll		= ((uint16_t)RC_rxData[4]<<8) | RC_rxData[5];		//ͨ��1
			Remote.pitch	= ((uint16_t)RC_rxData[6]<<8) | RC_rxData[7];		//ͨ��2
			Remote.thr		= ((uint16_t)RC_rxData[8]<<8) | RC_rxData[9];		//ͨ��3
			Remote.yaw		= ((uint16_t)RC_rxData[10]<<8) | RC_rxData[11];		//ͨ��4
			Remote.AUX1		= ((uint16_t)RC_rxData[12]<<8) | RC_rxData[13];		//ͨ��5  ���Ͻǰ���������ͨ��5  
			Remote.AUX2		= ((uint16_t)RC_rxData[14]<<8) | RC_rxData[15];		//ͨ��6  ���Ͻǰ���������ͨ��6 
			Remote.AUX3		= ((uint16_t)RC_rxData[16]<<8) | RC_rxData[17];		//ͨ��7  ���±߰���������ͨ��7 
			Remote.AUX4		= ((uint16_t)RC_rxData[18]<<8) | RC_rxData[19];		//ͨ��8  ���±߰���������ͨ��6  
			LIMIT(Remote.AUX2	,1000,2000);		
			LIMIT(Remote.roll	,1000,2000);
			LIMIT(Remote.pitch	,1000,2000);
			LIMIT(Remote.thr	,1000,2000);
			LIMIT(Remote.yaw	,1000,2000);
			LIMIT(Remote.AUX1	,1000,2000);
			LIMIT(Remote.AUX2	,1000,2000);
			LIMIT(Remote.AUX1	,1000,2000);

			const float roll_pitch_ratio = 0.04f;
//				const float yaw_ratio =  0.0015f;    
			pidPitch.desired	= -(Remote.pitch-1500)*roll_pitch_ratio;	 //��ң��ֵ��Ϊ���нǶȵ�����ֵ
			pidRoll.desired		= -(Remote.roll-1500)*roll_pitch_ratio;
			if(Remote.yaw>1820)
			{
				pidYaw.desired += 0.75f;	
			}
			else if(Remote.yaw <1180)
			{
				pidYaw.desired -= 0.75f;	
			}
			return _OK_;
		}
  }
//���3��û�յ�ң�����ݣ����ж�ң���źŶ�ʧ���ɿ����κ�ʱ��ֹͣ���У��������ˡ�
//���������ʹ���߿ɽ����ر�ң�ص�Դ������������3��������رգ��������ˡ�
//�����ر�ң�أ�����ڷ����л�ֱ�ӵ��䣬���ܻ��𻵷�������
	else
	{
		cnt++;
		if(cnt>800)
		{
			cnt = 0;
			ALL_flag.unlock = 0; 
			NRF24L01_init();
		}
	}

	return _ERR_;	
}

/*********************END OF FILE******************************************************/







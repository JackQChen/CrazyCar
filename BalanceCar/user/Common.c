#include "stm32f10x.h"
#include "Common.h"
#include "serial.h"

strSysParam		SysParam;
_st_Remote Remote; //ң��ͨ��ֵ
_st_ALL_flag ALL_flag; //ϵͳ��־λ������������־λ��
PidObject pidPitch; //�⻷PID����
PidObject pidRoll;
PidObject pidYaw;

/*------------------------------------------------------------------------------------------------------*/
//������	��uint16_t SumCheck(uint8_t* dat,uint16_t dat_len)
//����	���������ݰ��ͼ��麯��
//����	��dat	����ָ��,dat_len	���ݳ���
//����ֵ	����У����
u8 DataSumCheck(u8* dat,u16 dat_len)
{
	u8 sum_out = 0;	//��������
	u16 dat_index;
	//UartSendData((u8*)&dat_len,2);
	for(dat_index = 0;dat_index < dat_len;dat_index++)
	{
		sum_out += dat[dat_index];
		//UartSendData((u8*)&dat[dat_index],1);
	}
	return sum_out;
}

/************************************************************************************/
/*������	��CalculateGain()
**����	���������ȥͷȥβ��ƽ��ֵ
**����	��dat 			�˲����ݵ�ַָ��
**		  read_times	��ȡ����
**		  remove_num	�޳�����/2
**����ֵ	����
*/
int AverageFilter(int* dat,u8 read_times,u8 remove_num)		//�������ȥͷȥβ��ƽ��ֵ
{
	u16 times;
	u16 num;
	int sum = 0;
	int dat_temp = 0;
	int data_out = 0;	
	for(times = 0;times<read_times;times++)	//�Ƕ�����
	{
		for(num = times;num<read_times;num++)	//�Ƕ�����
		{
			if(dat[times]>dat[num])
			{
				dat_temp = dat[times];
				dat[times] = dat[num];
				dat[num] = dat_temp;				
			}
		}
	}
	
	for(times = remove_num;times<(read_times - remove_num);times++)
	{
		sum +=dat[times];		
	}
	
	data_out = sum/(read_times - 2*remove_num);
	
	return data_out;
}




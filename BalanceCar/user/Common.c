#include "stm32f10x.h"
#include "Common.h"
#include "serial.h"

strSysParam		SysParam;
_st_Remote Remote; //遥控通道值
_st_ALL_flag ALL_flag; //系统标志位，包含解锁标志位等
PidObject pidPitch; //外环PID数据
PidObject pidRoll;
PidObject pidYaw;

/*------------------------------------------------------------------------------------------------------*/
//函数名	：uint16_t SumCheck(uint8_t* dat,uint16_t dat_len)
//功能	：升级数据包和检验函数
//参数	：dat	数据指针,dat_len	数据长度
//返回值	：和校验结果
u8 DataSumCheck(u8* dat,u16 dat_len)
{
	u8 sum_out = 0;	//检验和输出
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
/*函数名	：CalculateGain()
**功能	：求排序后去头去尾的平均值
**参数	：dat 			滤波数据地址指针
**		  read_times	读取次数
**		  remove_num	剔除数量/2
**返回值	：无
*/
int AverageFilter(int* dat,u8 read_times,u8 remove_num)		//求排序后去头去尾的平均值
{
	u16 times;
	u16 num;
	int sum = 0;
	int dat_temp = 0;
	int data_out = 0;	
	for(times = 0;times<read_times;times++)	//角度排序
	{
		for(num = times;num<read_times;num++)	//角度排序
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




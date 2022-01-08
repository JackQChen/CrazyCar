#include "Common.h"
#include "Bluetooth.h"
#include "serial.h"

strBluetoothData		BluetoothData;			//Ҫ���õ�����ʹ��״̬
strBluetoothKeyHandle	BluetoothKeyHandle ;

u8 cmdBluetoothData(void)
{
	u8 sum_check;
	u8 state;
	
	sum_check = DataSumCheck((u8*)&ProtocolData,MAX_UART_DATA_LEN - 2);//ProtocolData.Length - 2);
	if(sum_check != ProtocolData.CmdData.BluetoothCmd.SumCheck)
	{
		//UartSendData("2",1);
//		UartSendData((u8*)&ProtocolData.Length,1);
//		UartSendData((u8*)&sum_check,1);
//		UartSendData((u8*)&ProtocolData.CmdData.BluetoothCmd.SumCheck,1);
//		Debug_PrintError("MusicEnable��У����� sum_check	= %x",CmdInfo.CmdData.BluetoothCmd.SumCheck);
//		Debug_PrintError("MusicEnable��У����� SumCheck()	= %x",SumCheck((uint8_t*)&CmdInfo,CmdInfo.Length - 3));
		state = _ERR_;		//���ݳ��ȳ��ṹ�峤�ȣ�ֱ������	
	}else
	{
		//UartSendData("3",1);
		//Debug_PrintInfor("�յ�BluetoothData");	
		//������������ģʽ
		SysParam.RemoteMode = REMOTE_MODE_BLUETOOTH;
		SysParam.ConnectTimeCnt = 0;					//���ʱ
		BluetoothData = ProtocolData.CmdData.BluetoothCmd.DataBuff;	//��ȡ��������
		
		state =  _OK_;
	}
	//����������ʱ����Ӧ��
	//MyCom.SendCmdAck(CmdInfo.CmdCode,SysSaveParam.DeviceAdd,CmdInfo.TxDeviceId,state,(uint8_t*)&state,0);	

	return state;
}

void UpdateKeyHandleValue(void)
{
	u8 key_num_s = 0,key_num_g = 0;
	u8 i,j;

	BluetoothKeyHandle.Handle.Ch1Value = BluetoothData.DataBuff[5];
	BluetoothKeyHandle.Handle.Ch2Value = BluetoothData.DataBuff[6];
	BluetoothKeyHandle.Handle.Ch3Value = BluetoothData.DataBuff[7];
	BluetoothKeyHandle.Handle.Ch4Value = BluetoothData.DataBuff[8];
	for(i = 0;i<5;i++)
	{
		BluetoothData.DataBuff[i] = ~BluetoothData.DataBuff[i];
	}
	
	for(i = 0;i<8;i++)
	{
		if(((BluetoothData.DataBuff[3]>>i)&1) == 0) key_num_s = i + 1;
	}
	
	for(j = 0;j<8;j++)
	{
		if(((BluetoothData.DataBuff[4]>>j)&1) == 0) key_num_g = j + 1;
	}	
	
	BluetoothKeyHandle.KeyNum = (key_num_s * 10 + key_num_g);
}






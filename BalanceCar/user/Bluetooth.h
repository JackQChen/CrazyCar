#ifndef _BLUETOOTH_H__
#define _BLUETOOTH_H__

#include "config.h"

#define L_LEFT_UP		1
#define L_LEFT_DOWN		3
#define R_RIGHT_UP		2
#define R_RIGHT_DOWN	4

#define L_UP			50
#define L_LEFT			60
#define L_DOWN			70
#define L_RIGHT			80
#define R_UP			5
#define R_LEFT			6
#define R_DOWN			7
#define R_RIGHT			8

#define BLU_MAX_CNT_VALUE	1000
//蓝牙数据控制指令---B5
typedef struct  tagKeyHandleValue
{ 
	u16 TimerCNT;
	u8 KeyNum;			//按键值
	struct
	{ 
	  int Ch1Value;
	  int Ch2Value;
	  int Ch3Value;
	  int Ch4Value;
	} Handle;
}strBluetoothKeyHandle;

extern strBluetoothKeyHandle BluetoothKeyHandle ;

u8 cmdBluetoothData(void);
void UpdateKeyHandleValue(void);


#endif


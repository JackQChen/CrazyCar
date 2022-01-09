#ifndef _A4988_H
#define _A4988_H

#include "config.h"

extern  u16 CCR1_Val;
extern  u16 CCR2_Val;


#define MAX_FREQ   65535
#define MIN_FREQ   1

#define RDIR  PAout(5)
#define LDIR  PBout(0)

//1停止
#define LEN   PAout(7)
#define REN   PBout(12)

//初始化
void A4988_init(void);

//使能
void A4988_en(u8 state);

#endif

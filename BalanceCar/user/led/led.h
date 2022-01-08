#ifndef __LED__
#define __LED__

#include "sys.h"

#define LED0 PCout(14)	// 	
#define LED1 PAout(4)	// 


void LED_init(char GP,int IO);

#endif

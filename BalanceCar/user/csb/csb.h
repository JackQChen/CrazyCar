#ifndef __CSB__
#define __CSB__

#include "sys.h"

#include "timer.h"

#define TRIG  PBout(6)
#define ECHO  PBin(7)


//��������ʼ��
void csb_init(void);

//��ȡ����
u16 start_ceju(void);


extern u16 csb_juli;


#endif

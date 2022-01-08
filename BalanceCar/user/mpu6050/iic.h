#ifndef __iic_H_
#define __iic_H_

#include "sys.h"

#define	WRITE 0
#define	READ  1



void iic_init(void);
void miic_rw(u8 *DataBuff,u8 ByteQuantity,u8 RegAddress,u8 SlaveAddres,u8 ControlByte);
void iic_rw(u8 *DataBuff,u8 ByteQuantity,u8 RegAddress,u8 SlaveAddres,u8 ControlByte);
//void iic_rw(u8* DataBuff, u8 ByteQuantity ,u8 RegAddress, u8 SlaveAddress);

//void iic_op(u8* DataBuff, u8 ByteQuantity, u8 RegAddress, u8 SlaveAddres, u8 ControByte);

#endif


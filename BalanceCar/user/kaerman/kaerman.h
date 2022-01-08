#ifndef __KAMAN_H
#define __KAMAN_H

#include "sys.h"

void kalman(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f);


#endif

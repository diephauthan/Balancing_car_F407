#ifndef __APP_CONTROL_H_
#define __APP_CONTROL_H_

#include "main.h" 

#define MPU6050_INT PBin(3)   //PB3 is connected to the interrupt pin of MPU6050

void Get_Angle(u8 way);

#endif


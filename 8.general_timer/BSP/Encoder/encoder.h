#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h" 


#define ENCODER_TIM_PERIOD (u16)(65535)   //???????65535 ???F103?K??????16??g?  It cannot be greater than 65535 because the F103 timer is 16 bits.

void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(Motor_ID);
void TIM4_IRQHandler(void);
void TIM3_IRQHandler(void);


#endif



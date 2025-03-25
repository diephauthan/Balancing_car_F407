#ifndef __MOTOR_H_
#define	__MOTOR_H_

#include "main.h"

#define L_PWMA   TIM8->CCR1  //PC6
#define L_PWMB   TIM8->CCR2  //PC7

#define R_PWMA   TIM8->CCR3  //PC8
#define R_PWMB   TIM8->CCR4  //PC9

void BalanceCar_PWM_Init(u16 arr,u16 psc);
void BalanceCar_Motor_Init(void);

#endif



#ifndef __TIMER_H_
#define __TIMER_H_

#include "main.h"

void TIM7_Init(void);


void power_decect(void);
void cotrol_led(void);

void delay_time(u16 time);
void my_delay(u16 s);


#endif


#ifndef __TIMER_H_
#define __TIMER_H_

#include "main.h"

void TIM7_Init(void);
uint32_t millis(void);
void TIM7_IRQHandler(void);


#endif


#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);


#endif


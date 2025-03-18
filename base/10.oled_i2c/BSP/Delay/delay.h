#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
void delay_init(void);
void SystemClockConfig(void);

#endif


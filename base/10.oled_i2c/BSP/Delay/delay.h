#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

void SysTick_Init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
void delay_init(void);
void SystemClockConfig(void);
void delay_ccms(uint32_t ms);

#endif


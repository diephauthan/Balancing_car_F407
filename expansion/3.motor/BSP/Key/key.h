#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"

#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_PIN  GPIO_Pin_0
#define KEY1_GPIO_RCC  RCC_AHB1Periph_GPIOA

#define KEY_INT PAin(0)

void Key1_GPIO_Init(void);

#endif



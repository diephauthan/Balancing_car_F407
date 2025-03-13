#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"

#define KEY1_GPIO_PORT GPIOC
#define KEY1_GPIO_PIN  GPIO_Pin_9
#define KEY1_GPIO_RCC  RCC_AHB1Periph_GPIOC

#define KEY_INT PCin(9)

void Key1_GPIO_Init(void);

#endif



#ifndef LED_H
#define LED_H

#include "main.h"

#define LED_RCC   RCC_AHB1Periph_GPIOD
#define LED_PORT  GPIOD
#define LED_PIN   GPIO_Pin_12
#define LED_ON    GPIO_SetBits(LED_PORT,LED_PIN) 
#define LED_OFF   GPIO_ResetBits(LED_PORT,LED_PIN)



#define LED  PDout(12) 
 

void Init_Led_GPIO(void);

#endif

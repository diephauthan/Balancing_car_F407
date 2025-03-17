#ifndef __BEEP_H_
#define __BEEP_H_

#include "main.h"

#define BEEP_RCC	RCC_AHB1Periph_GPIOA
#define BEEP_PORT	GPIOA
#define BEEP_PIN	GPIO_Pin_13

#define BEEP_ON		GPIO_SetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF	GPIO_ResetBits(BEEP_PORT, BEEP_PIN)

#define BEEP_BEEP PAout(11)


void Init_Beep(void);
void beep_timer(void);

#endif

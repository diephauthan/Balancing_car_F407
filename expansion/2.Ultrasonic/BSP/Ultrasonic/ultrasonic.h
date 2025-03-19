#ifndef __ULTRASONIC_H__
#define	__ULTRASONIC_H__

#include "main.h"

#define ULTRASONIC_RCC   RCC_AHB1Periph_GPIOA

#define TRIG_PORT  GPIOA
#define TRIG_PIN   GPIO_Pin_5 //5

#define ECHO_PORT  GPIOA
#define ECHO_PIN   GPIO_Pin_1

#define TRIG_SIG 	PAout(5) //5

void TIM2_Cap_Init(u16 arr,u16 psc);	
void Get_Distance(void);
// void App_Change_Car(void);
// void switch_wave_contorl(void);

// void APP_avoid(void);

#endif



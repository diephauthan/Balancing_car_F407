#ifndef __BATTERY_ADC_H_
#define __BATTERY_ADC_H_

#include "main.h"

#define BAT_GPIO_RCC    RCC_AHB1Periph_GPIOA
#define BAT_GPIO_PORT   GPIOA
#define BAT_GPIO_PIN    GPIO_Pin_5


#define BAT_ADC         ADC1
#define BAT_ADC_CH      ADC_Channel_5
#define BAT_ADC_RCC     RCC_APB2Periph_ADC1

void 	Battery_init(void);
int Get_Measure_Volotage(void);
float Get_Battery_Volotage(void);

uint16_t Battery_Get_Average(uint8_t ch, uint8_t times);


#endif


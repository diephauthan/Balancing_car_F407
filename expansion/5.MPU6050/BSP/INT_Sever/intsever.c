/*
* @par Copyright (C): 2018-2028, Shenzhen Yahboom Tech
* @file         // intsever.c
* @author       // lly
* @version      // V1.0
* @date         // 20240628
* @brief        // С������ƽ�����Ҫ�жϷ���  Main interruption service for trolley control balance
*/


#include "intsever.h"

void MPU6050_EXTI_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //External interrupt, SYSCFG clock needs to be enabled
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);         //Enable GPIO port clock

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	                  //Port configuration
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  //Pull up input
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);					              //Initialize GPIO according to the set parameters
  	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);
  	
    EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //Falling edge trigger
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	 //Initialize the external EXTI register based on the parameters specified in EXTI_InitStruct
	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			    //Enable the external interrupt channel where the button is located
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//Seize priority 2,
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//Sub priority 3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Enable external interrupt channel
  	NVIC_Init(&NVIC_InitStructure); 
}







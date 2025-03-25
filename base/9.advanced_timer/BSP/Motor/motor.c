#include "motor.h"

void BalanceCar_Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // Enable PC port clock

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	// port configuration
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      // Push pull output
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
    GPIO_Init(GPIOC, &GPIO_InitStructure);					      // Initialize GPOC according to the set parameters
}

void BalanceCar_PWM_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    TIM_DeInit(TIM8);
    TIM_TimeBaseStructure.TIM_Period = arr -1;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OC1Init(TIM8, &TIM_OCInitStructure);  // Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  // Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  // Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  // Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct

    TIM_CtrlPWMOutputs(TIM8, ENABLE); //  Main output enable

    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  // CH1 pre load enable
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  // CH2 pre load enable
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  // CH3 pre load enable
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  // CH4 pre load enable
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); // Enable TIMx pre loaded registers on ARR
	
	
	/* TIM8 enable counter */
	TIM_Cmd(TIM8, ENABLE);                   //	Enable Timer 8
}

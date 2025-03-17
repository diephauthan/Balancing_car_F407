 #include "delay.h"

uint8_t fac_us = 0;
uint16_t fac_ms = 0;
extern uint32_t millis(void);

void delay_us(uint32_t nus){
    TIM_SetCounter(TIM7, 0);
    while(TIM_GetCounter(TIM2) < nus){}
}

void Delay1Ms(){
    TIM_SetCounter(TIM7,0);
    while (TIM_GetCounter(TIM7) <1000){}
}

void delay_ms(uint16_t ms)
{
    uint32_t start = millis();
    while (millis() - start < ms) {
    }
}

void SystemClockConfig(void)
{
  // Declare configuration variables
  RCC_DeInit(); // Reset RCC configuration to default

  // Enable HSE (High Speed External oscillator)
  RCC_HSEConfig(RCC_HSE_ON);
  
  // Wait for HSE to stabilize
  if (RCC_WaitForHSEStartUp() == SUCCESS)
  {
    // Configure Flash latency
    FLASH_SetLatency(FLASH_Latency_5); // 5 wait states for 168MHz
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);
    
    // Configure clock division factors
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   // AHB = SYSCLK/1 = 168MHz
    RCC_PCLK1Config(RCC_HCLK_Div4);    // APB1 = HCLK/4  = 42MHz (max for APB1)
    RCC_PCLK2Config(RCC_HCLK_Div2);    // APB2 = HCLK/2  = 84MHz
    
    // Configure Main PLL
    RCC_PLLConfig(RCC_PLLSource_HSE,    // Use HSE as the PLL source
                 8,                     // PLLM = 8 (HSE/8 = 1MHz)
                 336,                   // PLLN = 336 (1MHz * 336 = 336MHz)
                 2,                     // PLLP = 2 (336MHz/2 = 168MHz for SYSCLK)
                 7);                    // PLLQ = 7 (336MHz/7 = 48MHz for USB)
    
    // Enable PLL
    RCC_PLLCmd(ENABLE);
    
    // Wait until PLL is ready
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
    
    // Select PLL as the SYSCLK source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    // Wait until PLL is used as the system clock source
    while(RCC_GetSYSCLKSource() != 0x08) {}
  }
}



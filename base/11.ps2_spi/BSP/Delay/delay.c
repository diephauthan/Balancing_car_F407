 #include "delay.h"

static uint8_t fac_us = 0;  //us��ʱ������ us delay multiplier
static uint16_t fac_ms = 0; //ms��ʱ������ ms delay multiplier

void delay_init(void)
{
	uint8_t SYSCLK = SystemCoreClock / 1000000;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // ѡ���ⲿʱ��  HCLK/8   Select external clock HCLK/8
	fac_us = SYSCLK / 8;
	fac_ms = (uint16_t)fac_us * 1000;
}


/**********************************************************
** Function name: delay_ms
** Function description: Delay nms
** Input parameter: nms
** Output parameter: None
** Note: SysTick->LOAD is a 24-bit register, so the maximum delay is:
	 nms<=0xffffff*8*1000/SYSCLK
	 SYSCLK unit is Hz, nms unit is ms
	 Under 72M conditions, nms<=1864
** ������: delay_ms
** ��������: ��ʱnms
** �������: nms
** �������: ��
** ˵����SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
		nms<=0xffffff*8*1000/SYSCLK
		SYSCLK��λΪHz,nms��λΪms
		��72M������,nms<=1864 
***********************************************************/
void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD = (uint32_t)nms * fac_ms; //ʱ�����(SysTick->LOADΪ24bit) Time loading (SysTick->LOAD is 24bit)
	SysTick->VAL = 0x00;			   //��ռ����� Clear counter
	SysTick->CTRL = 0x01;			   //��ʼ����  Start countdown
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16))); //�ȴ�ʱ�䵽�� Waiting time to arrive
	SysTick->CTRL = 0x00;						  //�رռ�����  Close Counter
	SysTick->VAL = 0X00;						  //��ռ�����  Clear counter
}

/**********************************************************
** Function name: delay_us
** Function description: Delay nus, nus is the number of us to delay.
** Input parameter: nus
** Output parameter: None
** ������: delay_us
** ��������: ��ʱnus��nusΪҪ��ʱ��us��.
** �������: nus
** �������: ��
***********************************************************/
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = nus * fac_us; //ʱ�����  Time loading
	SysTick->VAL = 0x00;		  //��ռ����� Clear counter
	SysTick->CTRL = 0x01;		  //��ʼ���� Start countdown
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16))); //�ȴ�ʱ�䵽�� Waiting time to arrive
	SysTick->CTRL = 0x00;						  //�رռ����� Close Counter
	SysTick->VAL = 0X00;						  //��ռ����� Clear counter
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



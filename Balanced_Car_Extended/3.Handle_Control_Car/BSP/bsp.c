#include "bsp.h"

/**
 * @brief  Configure SWD for debugging on STM32F407
 * SWD uses 2 pins:
 * - PA13 (SWDIO): Data I/O
 * - PA14 (SWCLK): Clock
 */

/**
 * @brief  Configure SWD or JTAG
 * @param  mode: Debug mode (SWD_ENABLE or JTAG_SWD_DISABLE)
 * @retval None
 */
void Debug_Config(uint8_t mode)
{
    /* Enable clock for GPIOA and SYSCFG */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;      // Enable clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;     // Enable clock for SYSCFG
    
    if(mode == SWD_ENABLE)
    {
        /* Configure PA13 (SWDIO) and PA14 (SWCLK) as Alternate Function */
        
        // Set Mode for GPIO to Alternate Function (10)
        GPIOA->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14);
        GPIOA->MODER |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1);
        
        // Set to High Speed
        GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR14);
        
        // Configure Pull-up
        GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR13 | GPIO_PUPDR_PUPDR14);
        GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR13_0 | GPIO_PUPDR_PUPDR14_0); // Pull-up
        
        // Configure Alternate Function 0 (AF0) for SWD
        GPIOA->AFR[1] &= ~(0x0F << (5 * 4) | 0x0F << (6 * 4));  // Clear AF bits for PA13, PA14
        // AF0 is default (0x0), so no bits need to be set
        
        // Configure SYSCFG to select SWD and disable JTAG
        SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;
    }
    else if(mode == JTAG_SWD_DISABLE)
    {
        // Configure PA13 and PA14 as GPIO input
        GPIOA->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14);
    }
}

/**
 * @brief  Initialize basic settings
 * @param  None
 * @retval None
 */
void bsp_init(void)
{
    DIY_NVIC_PriorityGroupConfig(2);    // Configure interrupt priority grouping
    SystemClockConfig();
    delay_init();
    uart_init(115200);	            // Serial port 1 initialization
	
    //led/beep
	Init_Led_GPIO();				//Onboard LED
    printf("0.1\r\n");
	Init_Beep();			        //Onboard buzzer
    printf("0.2\r\n");
	Key1_GPIO_Init();			    //Onboard buttons
    printf("0.3\r\n");
	
	
	BalanceCar_Motor_Init();        // Motor GPIO initialization
    printf("0.4\r\n");
	BalanceCar_PWM_Init(6720,0); 	// Initialize PWM 25Khz and motor hardware interface for driving the motor
	printf("0.5\r\n");
    Encoder_Init_TIM3();            // Initialize encoder 3
	printf("0.6\r\n");
    Encoder_Init_TIM4();            // Initialize encoder 4
	printf("0.7\r\n");

	delay_ms(300);
	
	IIC_MPU6050_Init();			    // Gyroscope I2C initialization
    printf("0.9\r\n");
	MPU6050_initialize();			// Gyroscope range initialization
    printf("1\r\n");
	DMP_Init();                     // DMP initialization
    printf("1.1\r\n");
	
	OLED_I2C_Init();				// OLED initialization
    printf("1.2\r\n");
	 

	PS2_Init();//�ֱ���ʼ��  Controller initialization
    printf("1.3\r\n");
	PS2_SetInit();
    printf("1.4\r\n");
	
	Battery_init();									// Initialization of battery level detection
	printf("1.5\r\n");

	//timer service 
	TIM7_Init();									// LED flashing, voltage detection service function
	
	printf("1.6\r\n");

}


/**************************************************************************
Function: Set NVIC group
Input   : NVIC_Group
Output  : None
Description: Set interrupt grouping
Input parameter: NVIC_Group: NVIC grouping 0~4, 5 groups in total
Note: It is necessary to clear previous settings
**************************************************************************/ 
void DIY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
    u32 temp, temp1;	  
    temp1 = (~NVIC_Group) & 0x07; // Take the last three bits
    temp1 <<= 8;
    temp = SCB->AIRCR;  // Read previous settings
    temp &= 0X0000F8FF; // Clear previous groups
    temp |= 0X05FA0000; // Write the key
    temp |= temp1;	   
    SCB->AIRCR = temp;  // Set grouping
}

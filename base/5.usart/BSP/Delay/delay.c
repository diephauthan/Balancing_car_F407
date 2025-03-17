#include "delay.h"

uint8_t fac_us = 0;
uint16_t fac_ms = 0;

// SysTick initialization function for STM32F407
void SysTick_Init(uint8_t SYSCLK)
{
    // SYSCLK is the system frequency in MHz
    SysTick->CTRL &= ~(1 << 2);   // Use the HCLK (system clock) as the clock source
    fac_us = SYSCLK;              // Set the microsecond factor
    fac_ms = (uint16_t)SYSCLK * 1000; // Set the millisecond factor
}

/**********************************************************
** Function name: delay_ms
** Function description: Delay nms (milliseconds)
** Input parameter: nms (number of milliseconds)
** Output parameter: None
** Note: SysTick->LOAD is a 24-bit register, so the maximum delay is:
    nms <= 0xFFFFFF * 8 * 1000 / SYSCLK
    SYSCLK unit is Hz, nms unit is ms
    Under 72M conditions, nms <= 1864
***********************************************************/

void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms;    // Set the value for LOAD register for nms
    SysTick->VAL = 0x00;                        // Clear the counter
    SysTick->CTRL = 0x01;                       // Enable the SysTick timer
    
    do
    {
        temp = SysTick->CTRL;                   // Read the status of the SysTick timer
    } while ((temp & 0x01) && !(temp & (1 << 16))); // Check if the timer has reached the required time
    
    SysTick->CTRL = 0x00;                       // Disable the timer
    SysTick->VAL = 0x00;                        // Clear the counter again
}

/**********************************************************
** Function name: delay_us
** Function description: Delay nus (microseconds), nus is the number of microseconds to delay.
** Input parameter: nus (number of microseconds)
** Output parameter: None
** Note: delay_us
** Description: Delays for the given number of microseconds (nus is the time in microseconds).
***********************************************************/
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us;    // Set the value for LOAD register for nus (microseconds)
    SysTick->VAL = 0x00;                        // Clear the counter
    SysTick->CTRL = 0x01;                       // Start the countdown
    
    do
    {
        temp = SysTick->CTRL;                   // Read the status of the SysTick timer
    } while (temp & 0x01 && !(temp & (1 << 16))); // Wait until the required time has passed
    
    SysTick->CTRL = 0x00;                       // Disable the SysTick timer
    SysTick->VAL = 0x00;                        // Clear the counter
}





#include"main.h"
#include"stm32f4xx.h"

// Ð?m b?o bi?n TimingDelay dã du?c khai báo toàn c?c
volatile uint32_t TimingDelay;

void TimingDelay_Decrement(void)
{
    if (TimingDelay > 0)
    {
        TimingDelay--;
    }
}


int main(void)
{
    // Kh?i t?o ph?n c?ng và các c?u hình khác
    while (1)
    {
        // Vòng l?p chính c?a chuong trình
    }
}

#include"main.h"
#include"stm32f4xx.h"

// �?m b?o bi?n TimingDelay d� du?c khai b�o to�n c?c
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
    // Kh?i t?o ph?n c?ng v� c�c c?u h�nh kh�c
    while (1)
    {
        // V�ng l?p ch�nh c?a chuong tr�nh
    }
}

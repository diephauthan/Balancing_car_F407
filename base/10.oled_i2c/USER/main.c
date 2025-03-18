#include "main.h"

// Declare the global variable TimingDelay
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
	bsp_init(); // System initialization
    delay_ms(600);
    OLED_Clear();
	char str[] = "";
    
	while(1)
	{
        static float process = 0;
		int x,y;
        OLED_Clear();
        for(x = 0; x < 128; x++){
            for(y = 10; y < 20; y++){
                SSD1306_DrawPixel(x, y, SSD1306_COLOR_WHITE);
            }
            process = x*100/127;
            sprintf(str, "process: %.2f%%", process);
            OLED_Draw_Line(str, 1, false, true);
            //LED_Draw_Line(str, 1, true, true);
        }

        OLED_Draw_Line("finish",2,true,true);
        delay_ms(500);
	}
}



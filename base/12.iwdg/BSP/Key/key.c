#include "key.h"

u16 g_key1_long_press = 0;

static u8 Key1_is_Press(void)
{
	if(!GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN))
	{
		return KEY_PRESS; // If the key is pressed, returns KEY_PRESS
	}
	return KEY_RELEASE;
}

// Read the long press status of button K1. If the long press time is reached, return 1. If not, return 0.
// timeout is the set time length, in seconds
u8 Key1_Long_Press(u16 timeout)
{
	if (g_key1_long_press > 0)
	{
		if (g_key1_long_press < timeout * 100 + 2)
		{
			g_key1_long_press++;
			if (g_key1_long_press == timeout * 100 + 2)
			{
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

//Read the status of button K1, press to return to 1, release to return to 0
//Mode: Set mode, 0: Press and hold to return 1; 1: Press once to return 1
uint8_t Key1_State(uint8_t mode)
{
	static uint16_t key1_state = 0;

	if (Key1_is_Press() == KEY_PRESS)
	{
		if (key1_state < (mode + 1) * 2)
		{
			key1_state++;
		}
	}
	else
	{
		key1_state = 0;
		g_key1_long_press = 0;
	}
	if (key1_state == 2)
	{
		g_key1_long_press = 1;
		return KEY_PRESS;
	}
	return KEY_RELEASE;
}

void Key1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(KEY1_GPIO_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
}

void KEYAll_GPIO_Init(void)
{
	Key1_GPIO_Init();
	
}

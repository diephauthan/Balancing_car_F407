#include "app_ps2.h"


int g_PS2_LX, g_PS2_LY, g_PS2_RX, g_PS2_RY, g_PS2_KEY;
int g_flag = 1; // Mode print interlock flag


// Key value test function
void PS2_Data_Show(void)
{
	g_PS2_LX = PS2_AnologData(PSS_LX);
	g_PS2_LY = PS2_AnologData(PSS_LY);
	g_PS2_RX = PS2_AnologData(PSS_RX);
	g_PS2_RY = PS2_AnologData(PSS_RY);
	g_PS2_KEY = PS2_DataKey();

	
	if ((g_PS2_LX == 255) && (g_PS2_LY == 255) && (g_PS2_RX == 255) && (g_PS2_RY == 255))
	{
		if (g_flag == 1)
		{
			printf("PS2 mode is RED or GREEN mode \r\n");
			printf("Or PS2 disconnect! \r\n");
			g_flag = 0;
		}
	}

	else
	{
		if (g_flag == 0)
		{
			printf("PS2 mode is RED^GREEN mode \r\n");
			g_flag = 1;
		}
		// Only the red green mode has the correct joystick value
		if (g_PS2_LX > 130 || g_PS2_LX < 100)
			printf("PS2_LX = %d \r\n", g_PS2_LX);
		if (g_PS2_LY > 130 || g_PS2_LY < 100)
			printf("PS2_LY = %d \r\n", g_PS2_LY);
		if (g_PS2_RX > 130 || g_PS2_RX < 100)
			printf("PS2_RX = %d \r\n", g_PS2_RX);
		if (g_PS2_RY > 130 || g_PS2_RY < 100)
			printf("PS2_RY = %d \r\n", g_PS2_RY);
	}

	// for (int i = 0; i < 9; i++) {
    // printf("Data[%d] = %02X\r\n", i, Data[i]);
	// }

	switch (g_PS2_KEY)
	{
	case PSB_SELECT:
		printf("key = PSB_SELECT\r\n");
		break;
	case PSB_L3:
		printf("key = PSB_L3\r\n");
		break;
	case PSB_R3:
		printf("key = PSB_R3\r\n");
		break;
	case PSB_START:
		printf("key = PSB_START\r\n");
		break;
	case PSB_PAD_UP:
		printf("key = PSB_PAD_UP\r\n");
		break;
	case PSB_PAD_RIGHT:
		printf("key = PSB_PAD_RIGHT\r\n");
		break;
	case PSB_PAD_DOWN:
		printf("key = PSB_PAD_DOWN\r\n");
		break;
	case PSB_PAD_LEFT:
		printf("key = PSB_PAD_LEFT\r\n");
		break;
	case PSB_L2:
		printf("key = PSB_L2\r\n");
		break;
	case PSB_R2:
		printf("key = PSB_R2\r\n");
		break;
	case PSB_L1:
		printf("key = PSB_L1\r\n");
		break;
	case PSB_R1:
		printf("key = PSB_R1\r\n");
		break;
	case PSB_GREEN:
		printf("key = PSB_GREEN\r\n");
		break;
	case PSB_RED:
		printf("key = PSB_RED\r\n");
		break;
	case PSB_BLUE:
		printf("key = PSB_BLUE\r\n");
		break;
	case PSB_PINK:
		printf("key = PSB_PINK\r\n");
		break;
	}


	delay_ms(100); // Delay 100ms
}


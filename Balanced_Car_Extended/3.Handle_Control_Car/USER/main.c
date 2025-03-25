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

uint8_t GET_Angle_Way=2;                             	//Algorithm for obtaining angles, 1: Quaternion 2: Kalman 3: Complementary filtering
float Angle_Balance,Gyro_Balance,Gyro_Turn;     		//Balance tilt angle balance gyroscope steering gyroscope
int Motor_Left,Motor_Right;                 	  		//Motor PWM variable
int Temperature;                                		//Temperature variable
float Acceleration_Z;                           		//Z-axis accelerometer
int Voltage,Mid_Angle;                          		//Battery voltage sampling related variables, mechanical median
float Move_X,Move_Z;							 		//Move_X: Forward speed Move_Z: Steering speed
u8 Stop_Flag = 1; 			 							//0: Start 1: Stop

enCarState g_newcarstate;								//Car status sign

char showbuf[20]={'\0'};
extern uint8_t speed_flag;

int main(void)
{	
	Mid_Angle = 0; // Obtain based on the car
	
	
	bsp_init();
	
	MPU6050_EXTI_Init();					// This interrupt service function is placed last
	
	OLED_Draw_Line("put down key start!", 1, true, true); 

	while(!Key1_State(1));
	Stop_Flag = 0; // Start controlling

	
	OLED_Draw_Line("start control!", 1, true, true); 
	
	
	while(1)
	{
		
		PS2_Contorl_Car();
		
		
		sprintf(showbuf,"speed = %d  ",speed_flag);
		OLED_Draw_Line(showbuf, 3, false, true); 
	
	}
}




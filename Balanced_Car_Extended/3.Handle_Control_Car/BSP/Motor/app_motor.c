#include "app_motor.h"

#define MOTOR_IGNORE_PULSE (1300)//����  

float Velocity_Left,Velocity_Right;

//��������
int PWM_Ignore(int pulse)
{
	if (pulse > 0) return pulse + MOTOR_IGNORE_PULSE;
  if (pulse < 0) return pulse - MOTOR_IGNORE_PULSE;
	return pulse;
}

//int myabs(int a){
//    int temp;
//    if(a < 0)
//    {
//        temp = -a;
//    }else 
//        temp = a;
//    return temp;
//}

/**************************************************************************
Function: Assign to PWM register
Input   : motor_left��Left wheel PWM��motor_right��Right wheel PWM
Output  : none
**************************************************************************/
void Set_Pwm(int motor_left, int motor_right)
{
    if(motor_left == 0)
    {
        L_PWMA = 0;
        L_PWMB = 0;
    }
    if(motor_right == 0)
    {
        R_PWMA = 0;
        R_PWMB = 0;
    }

    if(motor_left>0)
	{
		L_PWMB = myabs(motor_left);
		L_PWMA = 0;
	}		
	else
	{
		L_PWMB = 0;
		L_PWMA = myabs(motor_left);
	}

    	if(motor_right>0)
	{
		R_PWMA = myabs(motor_right);
		R_PWMB = 0;
	}
	else 
	{
		R_PWMA = 0;
		R_PWMB = myabs(motor_right);	
	}
}

/**************************************************************************
Function: PWM limiting range
Input   : IN��Input  max��Maximum value  min��Minimum value
Output  : Output
**************************************************************************/
int PWM_Limit(int IN,int max,int min)
{
	int OUT = IN;
	if(OUT>max) OUT = max;
	if(OUT<min) OUT = min;
	return OUT;
}

/**************************************************************************
Function: Encoder reading is converted to speed (mm/s)
Input   : none
Output  : none
**************************************************************************/
void Get_Velocity_Form_Encoder(int encoder_left,int encoder_right)
{ 	
	float Rotation_Speed_L,Rotation_Speed_R; //Motor speed=Encoder reading (5ms each time) * Reading frequency/harmonics/reduction ratio/Encoder accuracy
	
    Rotation_Speed_L = encoder_left*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
	Velocity_Left = Rotation_Speed_L*PI*Diameter_67/10;     //Calculate the encoder speed=rotational speed * circumference/10 and convert it to cm
	
    Rotation_Speed_R = encoder_right*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
	Velocity_Right = Rotation_Speed_R*PI*Diameter_67/10;    //Calculate the encoder speed=rotational speed * circumference/10 and convert it to cm
}

/**************************************************************************
Function: If abnormal, turn off the motor
Input   : angle��Car inclination��voltage��Voltage
Output  : 1��abnormal��0��normal
**************************************************************************/	
uint8_t Turn_Off(float angle, float voltage)
{
	u8 temp;
	if(angle<-40||angle>40 || battery<9.6f || Stop_Flag==1)//��ص�ѹ����10V�رյ��  The battery voltage is lower than 10V and the motor is turned off || battery<9.6V
	{	                                                 //��Ǵ���40�ȹرյ�� Turn off the motor when the inclination angle is greater than 40 degrees
		temp=1;                                          
		L_PWMA = 0;
		L_PWMB = 0;
		R_PWMA = 0;
		R_PWMB = 0;
	}
	else
		temp=0;
	return temp;			
}

#ifndef __APP_MOTOR_H_
#define	__APP_MOTOR_H_

#include "main.h"

#define PI 3.14159265f                // PI constant
#define Control_Frequency 200.0       // Encoder read frequency
#define Diameter_67 67.0f             // Wheel diameter 67mm
#define EncoderMultiples 4.0          // Encoder pulse multiple
#define Encoder_precision 11.0        // Encoder precision 11 lines
#define Reduction_Ratio 30.0          // Reduction ratio 30
#define Perimeter 210.4867            // Perimeter in mm



void Set_Pwm(int motor_left,int motor_right);
int PWM_Limit(int IN,int max,int min);

void Get_Velocity_Form_Encoder(int encoder_left,int encoder_right);
uint8_t Turn_Off(float angle, float voltage);
	
int PWM_Ignore(int pulse);

#endif


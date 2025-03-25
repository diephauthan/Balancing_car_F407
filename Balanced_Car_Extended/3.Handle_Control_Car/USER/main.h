#ifndef __MAIN_H_
#define __MAIN_H_


// Header Files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#include "stm32f4xx.h"

#include "switch_function.h"
#include "myenum.h"

#include "delay.h"
#include "bsp.h"
#include "battery_adc.h"
#include "beep.h"
#include "led.h"
#include "timer.h"
#include "key.h"

//uart
#include "usart1.h"


//OLED
#include "oled.h"
#include "oled_i2c.h"

//Mpu6050
#include "ioi2c.h"
#include "MPU6050.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "intsever.h"

//Motor
#include "motor.h"
#include "encoder.h"
#include "app_motor.h"

//ps2
#include "ps2.h"
#include "app_ps2.h"

//Balance car overall control
#include "app_control.h"
#include "pid_control.h"

//filtering alforithm
#include "filter.h"
#include "KF.h"

//������ͨ�ñ��� General variables introduced
extern float Velocity_Left,Velocity_Right; 	    		//The speed of the wheels
extern uint8_t GET_Angle_Way;                           //Algorithm for obtaining angles, 1: Quaternion 2: Kalman 3: Complementary filtering
extern float Angle_Balance,Gyro_Balance,Gyro_Turn;      //Balance tilt angle balance gyroscope steering gyroscope
extern int Motor_Left,Motor_Right;                		//Motor PWM variable
extern int Temperature;                            		//Temperature variable
extern float Acceleration_Z;                       		//Z-axis accelerometer
extern int 	Mid_Angle;                          		//Mechanical median
extern float Move_X,Move_Z;						    	//Move_X:ǰ���ٶȣ�Forward speed��  Move_Z��ת���ٶ�(Turning speed)
extern float battery; 									//battery level 
extern u8 lower_power_flag; 						    //Low voltage sign, voltage recovery sign
extern u32 g_distance; 									//ֵUltrasonic distance value
extern u8 Flag_velocity; 								//Speed control related variables
extern u8 Stop_Flag;									//Stop sign
extern enCarState g_newcarstate;                        //Car status sign
extern float Car_Target_Velocity;
extern float Car_Turn_Amplitude_speed;



void TimingDelay_Decrement(void);

#endif


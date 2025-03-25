#include "app_control.h"


float battery = 12;//The initial state is fully charged 12v
u8 ccd_conut = 0;
//extern u8 angle_flag; //Get the angle flag
u8 ps2_count = 0;

void EXTI3_IRQHandler(void)
{
	int Encoder_Left,Encoder_Right;             					//Pulse counting of left and right encoders
	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;		  					// Balance loop PWM variable, speed loop PWM variable, steering loop PWM variable
	
  //  Check if any interruption events have occurred
  if(MPU6050_INT==0)		
	{   
		EXTI->PR=1<<3;                           					// Clear interrupt flag bit
		
		ps2_count++;
		if(ps2_count>20)//100ms
		{
			ps2_count =0;
//			PS2_Data_Show();
			PS2_Contorl_Car();
			
		}
		
		
		Get_Angle(GET_Angle_Way);                     			// Updating the posture once every 5ms, a higher sampling frequency can improve the effectiveness of Kalman filtering and complementary filtering
		Encoder_Left=Read_Encoder(MOTOR_ID_ML);            					// Read the value of the left wheel encoder, forward is positive, backward is negative
		Encoder_Right=-Read_Encoder(MOTOR_ID_MR);           					// Read the value of the right wheel encoder, forward is positive, backward is negative
			
				
		Balance_Pwm=Balance_PD(Angle_Balance,Gyro_Balance);    // Balance PID control gyro balance angular velocity polarity: forward tilt is positive, backward tilt is negative
		Velocity_Pwm=Velocity_PI(Encoder_Left,Encoder_Right);  // Speed loop PID control. Remember, speed feedback is positive feedback
			
		Turn_Pwm=Turn_PD(Gyro_Turn);														//   Steering loop PID control
		
		
		Motor_Left=Balance_Pwm+Velocity_Pwm+Turn_Pwm;       //PWM Calculate the final PWM of the left wheel motor
		Motor_Right=Balance_Pwm+Velocity_Pwm-Turn_Pwm;      //PWM Calculate the final PWM of the right wheel motor
																												//PWMֵ����ʹС��ǰ��������ʹС������  Positive PWM values cause the car to move forward, negative values cause the car to move backward
																												
		//Filter out dead zones
		Motor_Left = PWM_Ignore(Motor_Left);
		Motor_Right = PWM_Ignore(Motor_Right);
		
		// PWM limiting
		Motor_Left=PWM_Limit(Motor_Left,2600,-2600); //25khz->2592 
		Motor_Right=PWM_Limit(Motor_Right,2600,-2600);		

		
		if(Turn_Off(Angle_Balance,battery)==0)     					//	If there are no abnormalities
			Set_Pwm(Motor_Left,Motor_Right);         					//	Assign to PWM register
   }
	
}

/**************************************************************************
Function: Get angle
Input   : way��The algorithm of getting angle 1��DMP  2��kalman  3��Complementary filtering
Output  : none
**************************************************************************/	
void Get_Angle(u8 way)
{ 
	float gyro_x,gyro_y,accel_x,accel_y,accel_z;
	float Accel_Y,Accel_Z,Accel_X,Accel_Angle_x,Accel_Angle_y,Gyro_X,Gyro_Z,Gyro_Y;
	Temperature=Read_Temperature();      //��ȡMPU6050�����¶ȴ��������ݣ����Ʊ�ʾ�����¶ȡ� //Read the data from the MPU6050 built-in temperature sensor, which approximately represents the motherboard temperature.
	if(way==1)                           //DMP�Ķ�ȡ�����ݲɼ��ж϶�ȡ���ϸ���ѭʱ��Ҫ��  //The reading of DMP is interrupted during data collection, strictly following the timing requirements
	{	
		Read_DMP();                      	 //��ȡ���ٶȡ����ٶȡ����  //Read acceleration, angular velocity, and tilt angle
		Angle_Balance=Pitch;             	 //����ƽ�����,ǰ��Ϊ��������Ϊ�� //Update the balance tilt angle, with positive forward tilt and negative backward tilt
		Gyro_Balance=gyro[0];              //����ƽ����ٶ�,ǰ��Ϊ��������Ϊ��  //Update the balance angular velocity, with positive forward tilt and negative backward tilt
		Gyro_Turn=gyro[2];                 //����ת����ٶ� //Update steering angular velocity
		Acceleration_Z=accel[2];           //����Z����ٶȼ� //Update Z-axis accelerometer
	}			
	else
	{
		Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);    //��ȡX�������� //Read X-axis gyroscope
		Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY�������� //Read Y-axis gyroscope
		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ�������� //Read Z-axis gyroscope
		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ� //Read X-axis accelerometer
		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L); //��ȡX����ٶȼ� //Read Y-axis accelerometer
		Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ� //Read Z-axis accelerometer
		if(Gyro_X>32768)  Gyro_X-=65536;                 //��������ת��  Ҳ��ͨ��shortǿ������ת�� Data type conversion can also be enforced through short type conversion
		if(Gyro_Y>32768)  Gyro_Y-=65536;                 //��������ת��  Ҳ��ͨ��shortǿ������ת�� Data type conversion can also be enforced through short type conversion
		if(Gyro_Z>32768)  Gyro_Z-=65536;                 //��������ת�� Data type conversion
		if(Accel_X>32768) Accel_X-=65536;                //��������ת�� Data type conversion
		if(Accel_Y>32768) Accel_Y-=65536;                //��������ת�� Data type conversion
		if(Accel_Z>32768) Accel_Z-=65536;                //��������ת�� Data type conversion
		Gyro_Balance=-Gyro_X;                            //����ƽ����ٶ� Update balance angular velocity
		accel_x=Accel_X/1671.84f;
		accel_y=Accel_Y/1671.84f;
		accel_z=Accel_Z/1671.84f;
		gyro_x=Gyro_X/939.8f;                              //����������ת�� Gyroscope range conversion
		gyro_y=Gyro_Y/939.8f;                              //����������ת�� Gyroscope range conversion
		if(GET_Angle_Way==2)		  	
		{
			 Pitch= KF_X(accel_y,accel_z,-gyro_x)/PI*180;//�������˲� Kalman filtering 
			 Roll = KF_Y(accel_x,accel_z,gyro_y)/PI*180;
		}
		else if(GET_Angle_Way==3) 
		{  
				Accel_Angle_x = atan2(Accel_Y,Accel_Z)*180/PI; //��Accel_Y��accel_y�Ĳ����ó��ĽǶ���һ���ģ�ֻ�Ǳ߳���ͬ The angle obtained using Accel_Y and its parameters is the same, only the side length is different
				Accel_Angle_y = atan2(Accel_X,Accel_Z)*180/PI;
			
			 Pitch = -Complementary_Filter_x(Accel_Angle_x,Gyro_X/16.4f);//�����˲� Complementary filtering
			 Roll = -Complementary_Filter_y(Accel_Angle_y,Gyro_Y/16.4f);
		}
		Angle_Balance=Pitch;                              //����ƽ�����    Update the balance tilt angle
		Gyro_Turn=Gyro_Z;                                 //����ת����ٶ�  Update steering angular velocity
		Acceleration_Z=Accel_Z;                           //����Z����ٶȼ� Update Z-axis accelerometer
	}

}




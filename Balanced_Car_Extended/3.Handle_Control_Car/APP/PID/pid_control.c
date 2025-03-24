#include "pid_control.h"

//下面的pid为了好调,都放到100倍了 The following pids are all scaled to 100 times for easy adjustment.
//直立环PD控制参数
//Vertical loop PD control parameters
float Balance_Kp =9600;//范围0-288  Range 0-288
float Balance_Kd =48; //范围0-2 Range 0-2

//速度环PI控制参数
//PI control parameters for speed loop
float Velocity_Kp=6200; //范围0-72 Range 0-72
float Velocity_Ki=31;  //kp/200

//转向环PD控制参数
//Steering ring PD control parameters
float Turn_Kp=1700; //这个根据自己的需求调，只是平衡可以不调 This is adjusted according to your needs, but the balance can be left unchanged
float Turn_Kd=20; //范围 0-2 Range 0-2

//前进速度
//Forward speed
float Car_Target_Velocity=30; //0-50
//旋转速度
//Rotation speed
float Car_Turn_Amplitude_speed=48; //0-70

/**************************************************************************
Function: Absolute value function 
Input   : a：Number to be converted
Output  : unsigned int
函数功能：绝对值函数
入口参数：a：需要计算绝对值的数
返回  值：无符号整型
**************************************************************************/	
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a; 
	return temp;
}


/**************************************************************************
Function: Vertical PD control
Input   : Angle:angle；Gyro：angular velocity
Output  : balance：Vertical control PWM
函数功能：直立PD控制		
入口参数：Angle:角度；Gyro：角速度
返回  值：balance：直立控制PWM
**************************************************************************/	
int Balance_PD(float Angle,float Gyro)
{  
   float Angle_bias,Gyro_bias;
	 int balance;
	 Angle_bias=Mid_Angle-Angle;                       				//求出平衡的角度中值 和机械相关 Find the median angle and mechanical correlation for equilibrium
	 Gyro_bias=0-Gyro; 
	 balance=-Balance_Kp/100*Angle_bias-Gyro_bias*Balance_Kd/100; //计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数  Calculate the motor PWM PD control for balance control kp is the P coefficient kd is the D coefficient
	
	 return balance;
}


/**************************************************************************
Function: Speed PI control
Input   : encoder_left：Left wheel encoder reading；encoder_right：Right wheel encoder reading
Output  : Speed control PWM
函数功能：速度控制PWM		
入口参数：encoder_left：左轮编码器读数；encoder_right：右轮编码器读数
返回  值：速度控制PWM
**************************************************************************/
//修改前进后退速度，请修改Target_Velocity，比如，改成60
// To change the forward and backward speed, please modify Target_Velocity, for example, change it to 60
int Velocity_PI(int encoder_left,int encoder_right)
{  
    static float velocity,Encoder_Least,Encoder_bias,Movement;
	  static float Encoder_Integral;

		if(g_newcarstate==enRUN || g_newcarstate==enps2Fleft || g_newcarstate==enps2Fright)    	Movement=Car_Target_Velocity;	  //遥控前进信号 Remote control forward signal
		else if(g_newcarstate==enBACK || g_newcarstate==enps2Bleft || g_newcarstate==enps2Bright)	Movement=-Car_Target_Velocity;  //遥控后退信号 Remote control reverse signal
		else		Movement=Move_X;							         
	
		
   //================速度PI控制器 Speed PI controller=====================//	
		Encoder_Least =0-(encoder_left+encoder_right);                    //获取最新速度偏差=目标速度（此处为零）-测量速度（左右编码器之和） //Obtain the latest speed deviation=target speed (here zero) - measured speed (sum of left and right encoders) 
		Encoder_bias *= 0.84;		                                          //一阶低通滤波器      //First order low-pass filter  
		Encoder_bias += Encoder_Least*0.16;	                              //一阶低通滤波器，减缓速度变化 //First order low-pass filter to slow down speed changes
		Encoder_Integral +=Encoder_bias;                                  //积分出位移 积分时间：5ms //Integral offset time: 5ms
		Encoder_Integral=Encoder_Integral+Movement;                       //接收遥控器数据，控制前进后退 //Receive remote control data and control forward and backward movement
		if(Encoder_Integral>8000)  	Encoder_Integral=8000;             //积分限幅 //Integral limit
		if(Encoder_Integral<-8000)	  Encoder_Integral=-8000;            //积分限幅	 //Integral limit
		velocity=-Encoder_bias*Velocity_Kp/100-Encoder_Integral*Velocity_Ki/100;     //速度控制	//Speed control
		
		if(Turn_Off(Angle_Balance,battery)==1) Encoder_Integral=0;//电机关闭后清除积分   //Clear points after motor shutdown
		
	  return velocity;
} 



/**************************************************************************
Function: Turn control
Input   : Z-axis angular velocity
Output  : Turn control PWM
函数功能：转向控制 
入口参数：Z轴陀螺仪
返回  值：转向控制PWM
**************************************************************************/
int Turn_PD(float gyro)
{
	 static float Turn_Target,turn_PWM; 
	 float Kp=Turn_Kp,Kd;			//修改转向速度，请修改Turn_Amplitude即可 To modify the steering speed, please modify Turn_Smplitude
	//===================遥控左右旋转部分 Remote control left and right rotation part=================//

	if(g_newcarstate==enLEFT || g_newcarstate==enps2Fleft || g_newcarstate==enps2Bleft)	        Turn_Target=-Car_Turn_Amplitude_speed;
	else if(g_newcarstate==enRIGHT || g_newcarstate==enps2Fright || g_newcarstate==enps2Bright)	  Turn_Target=Car_Turn_Amplitude_speed; 
	
	//左旋右旋固定速度跑 Left turn, right turn, fixed speed running
	else if(g_newcarstate == enTLEFT) Turn_Target=-50;
	else if(g_newcarstate == enTRIGHT) Turn_Target=50;
	else
	{
		Turn_Target=0; 
	}
	
	//如果是遥控走直线 If it is remote control walking in a straight line
	if(g_newcarstate==enRUN || g_newcarstate==enBACK )
	{
		Kd=Turn_Kd; 
	} 
	else Kd=0; 

	

  //===================转向PD控制器 Turn to PD controller=================//
	 turn_PWM=Turn_Target*Kp/100+gyro*Kd/100+Move_Z; //结合Z轴陀螺仪进行PD控制    Combining Z-axis gyroscope for PD control
		
	 return turn_PWM;								 				 //转向环PWM右转为正，左转为负  Steering ring PWM: Right turn is positive, left turn is negative
}





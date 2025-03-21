#include "pid.h"


#define Position_KP (2.2f)
#define Position_KI (0.013f)
#define Position_KD (0.05f)

extern int16_t Position_Left,Position_Right; 
extern int16_t Left_target,Right_target; 

int motor_L,motor_R;

// TIM7 interruption: once every 10ms
void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)    // Check whether TIM update interruption occurs
    {
      TIM_ClearITPendingBit(TIM7, TIM_IT_Update);        // Clear TIMx update interrupt flag

      Position_Left = Read_Position(MOTOR_ID_ML);
      Position_Right = Read_Position(MOTOR_ID_MR);

      motor_L = Position_PID(Position_Left, Left_target);
      motor_R = Position_PID(Position_Right, Right_target);

      motor_L = PWM_Ignore(motor_L);
      motor_R = PWM_Ignore(motor_R);

      motor_L = PWM_Limit(motor_L, 2500, -2500);
      motor_R = PWM_Limit(motor_R, 2500, -2500);
       
    }
}

/**************************************************************************
Function: Position-based PID Controller
Entrance parameters: Encoder measurement position information, target position
Return value: Motor PWM

Description: Position-based PID control algorithm

Input parameters: 
   - Measured position from the encoder
   - Target position

Return value:
   - PWM output for the motor

Discrete PID formula in position form:
   pwm = Kp * e(k) + Ki * ∑e(k) + Kd * [e(k) - e(k-1)]

Where:
   - e(k) is the current error
   - e(k-1) is the previous error
   - ∑e(k) is the cumulative sum of the error over time (integral term)
   - k represents the time step (k = 1, 2, ..., k)

The function returns the PWM control signal.
**************************************************************************/
int Position_PID(int position, int target)
{
    static float error, Pwm, Integral_error, Last_error;
    error = target -position;
    Integral_error+=error;
    if(Integral_error > 1000)
        Integral_error = 1000;
    if(Integral_error < -1000)
        Integral_error = -1000;

    Pwm = Position_KP*error + Position_KI*Integral_error + Position_KD*(error-Last_error);
    
    Last_error = error;
    return Pwm;
}

#include "filter.h"

/**************************************************************************
Function: Simple Kalman filter
Input   : acceleration, angular velocity
Output  : none
**************************************************************************/
float dt = 0.005;		  // Filter every 5ms              
float Kalman_Filter_x(float Accel,float Gyro)		
{
	static float angle_dot;
	static float angle;
	float Q_angle=0.001; //  Covariance of process noise
	float Q_gyro=0.003;	//  The covariance of process noise is a matrix with one row and two columns.
	float R_angle=0.5;		// The covariance of the measurement noise is the measurement bias
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(Gyro - Q_bias) * dt; //  Prior estimates
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Differentiation of the prior estimate error covariance

	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Integral of the differential of the covariance of the prior estimate error
	PP[0][1] += Pdot[1] * dt;   // A priori estimate of error covariance
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - angle;	//  Prior estimates
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //Posterior Estimation Error Covariance
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	 // Posterior Estimation
	Q_bias	+= K_1 * Angle_err;	 // Posterior Estimation
	angle_dot   = Gyro - Q_bias;	 //Differentiation of the output value (posterior estimate) = angular velocity
	return angle;
}
/**************************************************************************
Function: First order complementary filtering
Input   : acceleration��angular velocity
Output  : none
**************************************************************************/
float Complementary_Filter_x(float angle_m, float gyro_m)
{
	static float angle;
	float K1 =0.02; 
	angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * dt);
	return angle;
}
/**************************************************************************
Function: Simple Kalman filter
Input   : acceleration��angular velocity
Output  : none
**************************************************************************/
float Kalman_Filter_y(float Accel,float Gyro)		
{
	static float angle_dot;
	static float angle;
	float Q_angle=0.001; // Covariance of process noise
	float Q_gyro=0.003;	// The covariance of process noise is a matrix with one row and two columns.
	float R_angle=0.5;		// The covariance of the measurement noise is the measurement bias
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(Gyro - Q_bias) * dt; // Prior estimates
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Differentiation of the prior estimate error covariance
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Integral of the differential of the covariance of the prior estimate error
	PP[0][1] += Pdot[1] * dt;   // A priori estimate of error covariance
	PP[1][0] += Pdot[2] * dt; 
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - angle;	// Prior estimates
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 // Posterior Estimation Error Covariance
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	   // Posterior Estimation
	Q_bias	+= K_1 * Angle_err;	 // Posterior Estimation
	angle_dot   = Gyro - Q_bias;	// Differentiation of the output value (posterior estimate) = angular velocity
	return angle;
}
/**************************************************************************
Function: First order complementary filtering
Input   : acceleration��angular velocity
Output  : none
**************************************************************************/
float Complementary_Filter_y(float angle_m, float gyro_m)
{
	static float angle;
	float K1 =0.02; 
	angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * dt);
	return angle;
}



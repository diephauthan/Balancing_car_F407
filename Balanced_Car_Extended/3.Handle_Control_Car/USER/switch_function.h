// This file is a function compilation file 0: not involved in compilation 1: involved in compilation

#ifndef __SWITCH_FUNCTION_H_
#define __SWITCH_FUNCTION_H_


#define DEBUG_USARTx USART2

// The following two cannot be used at the same time
#define IR_TRACK_SWITCH 0  // 4-way tracking switch
#define ELE_SWITCH      0  // Electromagnetic line patrol switch
#define CCD_SWITCH      0 //CCD switch

#define K210_SWITCH     0 // k210 initialization.

//Precautions for using radar
//1. Release all unused peripherals, especially ultrasonic and timers, to reduce CPU burden
//2. Radar data processing needs to be processed in the main function to reduce interruption time
#define Lidar_SWITCH    0 // Radar switch

#endif


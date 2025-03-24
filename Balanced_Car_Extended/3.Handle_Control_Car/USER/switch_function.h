//此文件是一个功能编译文件 0:不参与编译  1:参与编译 This file is a function compilation file 0: not involved in compilation 1: involved in compilation

#ifndef __SWITCH_FUNCTION_H_
#define __SWITCH_FUNCTION_H_


#define DEBUG_USARTx USART1

//以下两者不能同时使用 The following two cannot be used at the same time
#define IR_TRACK_SWITCH 0  //4路循迹开关 4-way tracking switch
#define ELE_SWITCH      0  //电磁巡线开关  Electromagnetic line patrol switch
#define CCD_SWITCH      0 //CCD开关 CCD switch

#define K210_SWITCH     0 //k210初始化. k210 initialization.

//使用雷达注意事项 //Precautions for using radar
//1.把不用的外设都给释放掉 特别是超声波和定时器 减小cpu负担 //1. Release all unused peripherals, especially ultrasonic and timers, to reduce CPU burden
//2.雷达数据处理需要在main函数里面处理吧，减少中断占用时间 //2. Radar data processing needs to be processed in the main function to reduce interruption time
#define Lidar_SWITCH    0 //雷达开关 // Radar switch

#endif


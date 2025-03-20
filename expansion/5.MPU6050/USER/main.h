/**
* @par Copyright (C): 2016-2026, Shenzhen Yahboom Tech
* @file         // ALLHeader.h
* @author       // lly
* @version      // V1.0
* @date         // 240628
* @brief        // ??????????l? All related header files
* @details      
* @par History  //
*               
*/


#ifndef __ALLHEADER_H
#define __ALLHEADER_H


//??l? Header Files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#include "stm32f4xx.h"

#include "myenum.h"

#include "delay.h"
#include "bsp.h"
#include "led.h"
#include "usart1.h"

//Mpu6050
#include "ioi2c.h"
#include "MPU6050.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "intsever.h"

#endif

void TimingDelay_Decrement(void);

#ifndef __BSP_H_
#define __BSP_H_

#include "main.h"


void bsp_init(void);
//void JTAG_Set(u8 mode);
void DIY_NVIC_PriorityGroupConfig(u8 NVIC_Group);

#endif


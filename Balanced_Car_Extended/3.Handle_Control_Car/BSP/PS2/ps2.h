#ifndef __PS2_H
#define __PS2_H

#include "main.h"

/* GPIO definitions for STM32F407 */
#define PS_PORT_DI     GPIOB
#define PS_PORT_DO     GPIOB
#define PS_PORT_CS     GPIOB
#define PS_PORT_CLK    GPIOB

#define PS_PIN_DI      GPIO_Pin_14
#define PS_PIN_DO      GPIO_Pin_15
#define PS_PIN_CS      GPIO_Pin_12
#define PS_PIN_CLK     GPIO_Pin_13

/* 
 * Macro for F4 GPIO bit access
 * Thay thế macro PBin, PBout trong F1
 */
#define GPIO_READ_PIN(GPIOx, GPIO_Pin)  GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)
#define GPIO_SET_PIN(GPIOx, GPIO_Pin)   GPIO_SetBits(GPIOx, GPIO_Pin)
#define GPIO_RESET_PIN(GPIOx, GPIO_Pin) GPIO_ResetBits(GPIOx, GPIO_Pin)

/* Redefine macros for DI, DO, CS, CLK for F407 */
#define DI            GPIO_READ_PIN(PS_PORT_DI, PS_PIN_DI)

#define DO_H          GPIO_SET_PIN(PS_PORT_DO, PS_PIN_DO)
#define DO_L          GPIO_RESET_PIN(PS_PORT_DO, PS_PIN_DO)

#define CS_H          GPIO_SET_PIN(PS_PORT_CS, PS_PIN_CS)
#define CS_L          GPIO_RESET_PIN(PS_PORT_CS, PS_PIN_CS)

#define CLK_H         GPIO_SET_PIN(PS_PORT_CLK, PS_PIN_CLK)
#define CLK_L         GPIO_RESET_PIN(PS_PORT_CLK, PS_PIN_CLK)

// Button constants remain the same
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16


//These are stick values
#define PSS_RX 5                //��ҡ��X������  Right joystick X-axis data
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

// Rest of the header remains the same
// ...

extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);
u8 PS2_DataKey(void);
u8 PS2_AnologData(u8 button);
void PS2_ClearData(void);
void PS2_Vibration(u8 motor1, u8 motor2);
void PS2_EnterConfing(void);
void PS2_TurnOnAnalogMode(void);
void PS2_VibrationMode(void);
void PS2_ExitConfing(void);
void PS2_SetInit(void);

#endif

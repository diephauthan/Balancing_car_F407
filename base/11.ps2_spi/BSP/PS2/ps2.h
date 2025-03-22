#ifndef __BSP_PS2_H
#define __BSP_PS2_H

#include "main.h"


#define PS_RCC_DI 	RCC_AHB1Periph_GPIOB
#define PS_RCC_DO 	RCC_AHB1Periph_GPIOB
#define PS_RCC_CS 	RCC_AHB1Periph_GPIOB
#define PS_RCC_CLK 	RCC_AHB1Periph_GPIOB


#define PS_PIN_DI 	GPIO_Pin_14
#define PS_PIN_DO 	GPIO_Pin_15
#define PS_PIN_CS 	GPIO_Pin_12
#define PS_PIN_CLK 	GPIO_Pin_13

#define PS_PORT_DI 		GPIOB
#define PS_PORT_DO 		GPIOB
#define PS_PORT_CS 		GPIOB
#define PS_PORT_CLK 	GPIOB

/*********************************************************
**********************************************************/	 
#define DI   PBin(14) //          
 
#define DO_H PBout(15)=1       //����λ��  Command bit high
#define DO_L PBout(15)=0       //����λ��  Command bit low

#define CS_H PBout(12)=1       //CS����  CS pull high
#define CS_L PBout(12)=0       //CS����  CS pull low

#define CLK_H PBout(13)=1      //ʱ������  Clock pull high
#define CLK_L PBout(13)=0      //ʱ������  Clock pull low
 
    
 
//These are our button constants
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
 
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16
 
//#define WHAMMY_BAR		8
 
//These are stick values
#define PSS_RX 5                //��ҡ��X������  Right joystick X-axis data
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
 
extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;
 
void PS2_Init(void);
u8 PS2_RedLight(void);   //�ж��Ƿ�Ϊ���ģʽ  Determine whether it is red light mode
void PS2_ReadData(void); //���ֱ�����  Read handle data
void PS2_Cmd(u8 CMD);		  //���ֱ���������   Sending commands to the controller
u8 PS2_DataKey(void);		  //����ֵ��ȡ  Read the key value
u8 PS2_AnologData(u8 button); //�õ�һ��ҡ�˵�ģ����  Get the analog value of a joystick
void PS2_ClearData(void);	  //������ݻ�����  Clear the data buffer
void PS2_Vibration(u8 motor1, u8 motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF  Vibration setting motor1 0xFF on, others off, motor2 0x40~0xFF
void PS2_ShortPoll(void);

void PS2_EnterConfing(void);	 //��������  Enter configuration
void PS2_TurnOnAnalogMode(void); //����ģ���� Send analog value
void PS2_VibrationMode(void);    //������  Vibration setting
void PS2_ExitConfing(void);	     //�������  Complete configuration
void PS2_SetInit(void);		     //���ó�ʼ�� Configuration initialization
 
#endif


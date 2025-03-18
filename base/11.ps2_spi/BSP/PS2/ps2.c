#include "ps2.h"

#define DELAY_TIME  delay_us(5);

u16 Handkey;	// Key value reading, zero time storage.
u8 Comd[2]={0x01,0x42};	 //Start command. Request data
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //Data storage array
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	

/**************************************************************************
Function function: The following is the initialization code for the PS2 receiver module
Entrance parameters: None
Return value: None
Key values and key specifications
Controller interface initialization
input  DI->PB14 
output DO->PB15    CS->PB12  CLK->PB13
**************************************************************************/
void PS2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //DI
    RCC_AHB1PeriphClockCmd(PS_RCC_DI,ENABLE);
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = PS_PIN_DI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PS_PORT_DI, &GPIO_InitStructure);

    //DO
	RCC_AHB1PeriphClockCmd(PS_RCC_DO,ENABLE);       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 
	GPIO_InitStructure.GPIO_Pin=PS_PIN_DO; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;   
	GPIO_Init(PS_PORT_DO,&GPIO_InitStructure);                         

	//CS
	RCC_AHB1PeriphClockCmd(PS_RCC_CS,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=PS_PIN_CS;  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(PS_PORT_CS,&GPIO_InitStructure);   
	
	//SCK
	RCC_AHB1PeriphClockCmd(PS_RCC_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            
	GPIO_InitStructure.GPIO_Pin=PS_PIN_CLK;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(PS_PORT_CLK,&GPIO_InitStructure);   
}

/**************************************************************************
Function function: Send commands to the controller
Entry parameter: CMD command
Return value: None
**************************************************************************/
void PS2_Cmd(u8 CMD)
{
    volatile u16 ref = 0x01;
    Data[1] = 0;
    for(ref = 0x01; ref < 0x0100; ref <<= 1)
    {
        if(ref&CMD)
        {
            DO_H;
        }else DO_L;
        CLK_H; //CLOCK UP
        DELAY_TIME;
        CLK_L;
        DELAY_TIME;
        CLK_H;
        if(DI) //when it is at a high level
            Data[1] = ref|Data[1];
    }
    delay_us(16);
}


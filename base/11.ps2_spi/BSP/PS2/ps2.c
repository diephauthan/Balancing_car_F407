#include "ps2.h"

/*********************************************************     
**********************************************************/	 
//ʹ�õĲ���Ӳ��SPI  Not using hardware SPI
//ʹ�õĲ���Ӳ��SPI  Not using hardware SPI
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
**************************************************************************/
void PS2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable GPIOB clock - STM32F4 uses AHB1 for GPIO */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    /* Configure DI (PB14) pin as input with pull-up */
    GPIO_InitStructure.GPIO_Pin = PS_PIN_DI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      // Input mode for F4
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      // Pull-up resistor
    GPIO_Init(PS_PORT_DI, &GPIO_InitStructure);
    
    /* Configure DO (PB15), CS (PB12), CLK (PB13) pins as output */
    GPIO_InitStructure.GPIO_Pin = PS_PIN_DO | PS_PIN_CS | PS_PIN_CLK;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     // Output mode for F4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // Push-pull output type
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // No pull-up/pull-down
    GPIO_Init(PS_PORT_DO, &GPIO_InitStructure);     // Same port for all pins
}

/**************************************************************************
Function function: Send commands to the controller
Entry parameter: CMD command
Return value: None
**************************************************************************/
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //���һλ����λ Output a control bit
		}
		else DO_L;

		delay_us(5);  // Thêm delay
		CLK_H;                        //ʱ������ Clock up
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		delay_us(5);  // Thêm delay
		if(DI) //Ϊ�ߵ�ƽ��ʱ�� When it is at a high level
			Data[1] = ref|Data[1];
	}
	delay_us(25); //16us
}
/**************************************************************************
Function function: Determine whether it is in red light mode, 0x41=Simulate green light, 0x73=Simulate red light
Entry parameter: CMD command
Return value: 0, Red light mode Other, Other modes
**************************************************************************/
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ���� Start command
	PS2_Cmd(Comd[1]);  //�������� Request data
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;
}
/**************************************************************************
Function function: Read controller data
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ���� Start command
	PS2_Cmd(Comd[1]);  //�������� Request data
	for(byte=2;byte<9;byte++)          //��ʼ�������� Start accepting data
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME; 
			CLK_H;			
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;
}
/**************************************************************************
Function function: Process the data read from PS2, only processing the key part
Entry parameter: CMD command
Return value: None
//When only one button is pressed, it is pressed as 0, and when not pressed, it is pressed as 1
**************************************************************************/
u8 PS2_DataKey()
{
	u8 index;
	PS2_ClearData();
	PS2_ReadData();
	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1 These are 16 buttons that are pressed to 0 and not pressed to 1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ������� No buttons pressed
}
/**************************************************************************
Function function: Send commands to the controller
Entrance parameters: Obtain the analog range of a joystick from 0 to 256
Return value: None
**************************************************************************/
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}
//������ݻ�����
//Clear data buffer
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function function: joystick vibration function,
Calls:		 void PS2_Cmd(u8 CMD);
Entrance parameters: motor1: Right small vibration motor 0x00 off, other on
Motor 2: Left side large vibration motor 0x40~0xFF motor on, the larger the value, the greater the vibration
Return value: None
******************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	CS_L;
	delay_us(16);
    PS2_Cmd(0x01);  //��ʼ���� Start command
	PS2_Cmd(0x42);  //�������� Request data
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
/**************************************************************************
Function function: short poll
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
/**************************************************************************
Function Function: Enter Configuration
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
/**************************************************************************
Function Function: Send Mode Settings
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00); //analog=0x01;digital=0x00  �������÷���ģʽ  //analog=0x01;digital=0x00 Software sets the sending mode
	PS2_Cmd(0xEE); //Ox03�������ã�������ͨ��������MODE������ģʽ�� // 0x03 latches the setting, that is, the mode cannot be set by pressing the "MODE" button.
				   //0xEE�������������ã���ͨ��������MODE������ģʽ�� //0xEE does not latch the software settings, and the mode can be set by pressing the "MODE" button.
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
/**************************************************************************
Function Function: Vibration Settings
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
/**************************************************************************
Function function: Complete and save configuration
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
/**************************************************************************
Function function: Controller configuration initialization
Entrance parameters: None
Return value: None
**************************************************************************/
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ Enter configuration mode
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴� Configure the "traffic light" mode and choose whether to save it or not
	//PS2_Vibration(0xff,0xff);
	//PS2_VibrationMode();	//������ģʽ Activate vibration mode
	PS2_ExitConfing();		//��ɲ��������� Complete and save configuration
}



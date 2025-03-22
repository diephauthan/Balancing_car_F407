// Updated PS2 controller driver using SPI2 (STM32F407 + SPL)

#include "ps2.h"

u16 Handkey;
u8 Comd[2] = {0x01, 0x42};
u8 Data[9] = {0};
u16 MASK[] = {
    PSB_SELECT, PSB_L3, PSB_R3, PSB_START,
    PSB_PAD_UP, PSB_PAD_RIGHT, PSB_PAD_DOWN, PSB_PAD_LEFT,
    PSB_L2, PSB_R2, PSB_L1, PSB_R1,
    PSB_GREEN, PSB_RED, PSB_BLUE, PSB_PINK
};

// SPI send/receive wrapper
u8 PS2_SPI_Transfer(u8 data)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, data);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI2);
}

void SPI2_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // SPI2: SCK (PB13), MISO (PB14), MOSI (PB15)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

    SPI_I2S_DeInit(SPI2);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

void PS2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // CS pin (PB12)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS High

    SPI2_Init();
    delay_ms(10);
}

void PS2_Cmd(u8 CMD)
{
    u8 res = PS2_SPI_Transfer(CMD);
    Data[1] = res;
    delay_us(20);
}

void PS2_ReadData(void)
{
    u8 i;
    PS2_ClearData();

    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS Low
    delay_us(20);

    PS2_SPI_Transfer(Comd[0]);
    Data[1] = PS2_SPI_Transfer(Comd[1]);

    for (i = 2; i < 9; i++)
    {
        Data[i] = PS2_SPI_Transfer(0x00);
        delay_us(20);
    }

    GPIO_SetBits(GPIOB, GPIO_Pin_12); // CS High
    delay_us(20);
}

u8 PS2_DataKey()
{
    u8 index;
	PS2_ClearData();
    PS2_ReadData();
    Handkey = (Data[4] << 8) | Data[3];
    for (index = 0; index < 16; index++)
    {
        if ((Handkey & (1 << (MASK[index] - 1))) == 0)
            return index + 1;
    }
    return 0;
}

u8 PS2_AnologData(u8 button)
{
    return Data[button];
}

void PS2_ClearData()
{
    u8 i;
    for (i = 0; i < 9; i++)
        Data[i] = 0;
}

// The rest of the functions (Vibration, Config, etc.) remain mostly the same.
// Replace PS2_Cmd() calls with PS2_SPI_Transfer() if needed in those functions.

void PS2_Vibration(u8 motor1, u8 motor2)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    delay_us(20);

    PS2_SPI_Transfer(0x01);
    PS2_SPI_Transfer(0x42);
    PS2_SPI_Transfer(0x00);
    PS2_SPI_Transfer(motor1);
    PS2_SPI_Transfer(motor2);
    PS2_SPI_Transfer(0x00);
    PS2_SPI_Transfer(0x00);
    PS2_SPI_Transfer(0x00);
    PS2_SPI_Transfer(0x00);

    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    delay_us(20);
}

/**************************************************************************
Function function: Controller configuration initialization
Entrance parameters: None
Return value: None
�������ܣ��ֱ����ó�ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ Enter configuration mode
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴� Configure the "traffic light" mode and choose whether to save it or not
	//PS2_VibrationMode();	//������ģʽ Activate vibration mode
	PS2_ExitConfing();		//��ɲ��������� Complete and save configuration
}

/**************************************************************************
Function function: short poll
Entrance parameters: None
Return value: None
�������ܣ�short poll
��ڲ�������
����  ֵ����
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
�������ܣ���������
��ڲ�������
����  ֵ����
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
Function function: Complete and save configuration
Entrance parameters: None
Return value: None
�������ܣ���ɲ���������
��ڲ�������
����  ֵ����
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
Function Function: Send Mode Settings
Entrance parameters: None
Return value: None
�������ܣ�����ģʽ����
��ڲ�������
����  ֵ����
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
�������ܣ�������
��ڲ�������
����  ֵ����
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
// ... giữ nguyên các hàm PS2_ShortPoll, PS2_SetInit, PS2_EnterConfing, ... như cũ nhưng thay PS2_Cmd thành PS2_SPI_Transfer nếu cần

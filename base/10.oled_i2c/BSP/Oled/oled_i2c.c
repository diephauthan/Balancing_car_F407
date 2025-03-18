#include "oled_i2c.h"

void OLED_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

    RCC_APB1PeriphClockCmd(OLEDI2C_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(OLED_RCC, ENABLE);

    GPIO_PinAFConfig(OLED_Port, GPIO_PinSource8, GPIO_AF_I2C1); // For SCL pin
    GPIO_PinAFConfig(OLED_Port, GPIO_PinSource9, GPIO_AF_I2C1); // For SDA pin

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_Pin | OLED_SDA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;

    GPIO_Init(OLED_Port, &GPIO_InitStructure);

    I2C_DeInit(OLEDI2C);
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = 0x10; // This address can be filled in at will, not as a slave

    I2C_Init(OLEDI2C, &I2C_InitStructure);
    I2C_Cmd(OLEDI2C, ENABLE);

    OLED_Init();//oled��ʼ�� OLED initialization
	OLED_Draw_Line("oled init success!", 1, true, true);
}

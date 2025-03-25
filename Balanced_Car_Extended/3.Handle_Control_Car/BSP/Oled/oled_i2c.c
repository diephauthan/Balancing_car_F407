#include "oled_i2c.h"

#define OLED_SDA_IN()  {                                                                            \
    GPIOB->MODER &= ~(3 << (9 * 2));              /* Clear mode configuration bits for PB8 */        \
    GPIOB->MODER |= (0 << (9 * 2));               /* Set mode to input */                           \
    GPIOB->PUPDR &= ~(3 << (9 * 2));              /* Clear pull-up/pull-down configuration */       \
    GPIOB->PUPDR |= (1 << (9 * 2));               /* Enable pull-up */                             \
}

#define OLED_SDA_OUT() {                                                                            \
    GPIOB->MODER &= ~(3 << (9 * 2));              /* Clear mode configuration bits for PB8 */        \
    GPIOB->MODER |= (1 << (9 * 2));               /* Set mode to output */                          \
    GPIOB->OTYPER &= ~(1 << 9);                   /* Set as push-pull output */                     \
    GPIOB->OSPEEDR &= ~(3 << (9 * 2));                                                             \
    GPIOB->OSPEEDR |= (2 << (8 * 2));                                                                   \
}

// IO operation function
#define OLED_IIC_SCL    PBout(8) //SCL
#define OLED_IIC_SDA    PBout(9) //SDA	 
#define OLED_READ_SDA   PBin(9)  //Input SDA

// The waiting time for the pin can be modified according to the chip clock as long as it meets the communication requirements.
#define DELAY_FOR_COUNT      10

static void Delay_For_Pin(vu8 nCount) 
{
    uint8_t i = 0;
    for(; nCount != 0; nCount--)
    {
        for (i = 0; i < DELAY_FOR_COUNT; i++); 
    }
}


void OLED_I2C_Init(void)
{	
		OLED_IIC_Init();
		OLED_Init();//OLED initialization
	
		OLED_Draw_Line("oled init success!", 1, true, true);
}

void OLED_IIC_Init(void)
{
    RCC->AHB1ENR |= 1 << 1;   // Enable peripheral IO PORTB clock first

    GPIOB->MODER &= ~(0x03 << (8 * 2));  // Xóa bit cũ của PB8 (2 bit cho mỗi chân)
    GPIOB->MODER |= (0x01 << (8 * 2));   // Thiết lập chế độ output cho PB8

    GPIOB->MODER &= ~(0x03 << (9 * 2));  // Xóa bit cũ của PB9 (2 bit cho mỗi chân)
    GPIOB->MODER |= (0x01 << (9 * 2));   // Thiết lập chế độ output cho PB9

    // Cấu hình kiểu đầu ra push-pull cho PB8 và PB9
    GPIOB->OTYPER |= (0x03 << 8);   // Đảm bảo kiểu output là push-pull cho PB8 và PB9
}

// void OLED_I2C_Init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStructure;
// 	I2C_InitTypeDef I2C_InitStructure;

//     RCC_APB1PeriphClockCmd(OLEDI2C_RCC, ENABLE);
// 	RCC_AHB1PeriphClockCmd(OLED_RCC, ENABLE);

//     GPIO_PinAFConfig(OLED_Port, GPIO_PinSource8, GPIO_AF_I2C1); // For SCL pin
//     GPIO_PinAFConfig(OLED_Port, GPIO_PinSource9, GPIO_AF_I2C1); // For SDA pin

//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//     GPIO_InitStructure.GPIO_Pin = OLED_SCL_Pin | OLED_SDA_Pin;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;

//     GPIO_Init(OLED_Port, &GPIO_InitStructure);

//     I2C_DeInit(OLEDI2C);
//     I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//     I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//     I2C_InitStructure.I2C_ClockSpeed = 400000;
//     I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//     I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//     I2C_InitStructure.I2C_OwnAddress1 = 0x10; // This address can be filled in at will, not as a slave

//     I2C_Init(OLEDI2C, &I2C_InitStructure);
//     I2C_Cmd(OLEDI2C, ENABLE);

//     OLED_Init();// OLED initialization
// 		OLED_Draw_Line("oled init success!", 1, true, true);
// }

int OLED_IIC_Start(void)
{
    OLED_SDA_OUT(); //sda����� sda line output
    OLED_IIC_SDA = 1;
    if (!OLED_READ_SDA)
        return 0;
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
    if (OLED_READ_SDA)
        return 0;
    Delay_For_Pin(2);
    OLED_IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ�������� Clamp the I2C bus and prepare to send or receive data
    return 1;
}

void OLED_IIC_Stop(void)
{
    OLED_SDA_OUT(); //sda����� sda line output
    OLED_IIC_SCL = 0;
    OLED_IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    Delay_For_Pin(2);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SDA = 1; //����I2C���߽����ź� Send I2C bus end signal
    Delay_For_Pin(2);
}

int OLED_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    OLED_SDA_IN(); //SDA����Ϊ���� SDA is set as input
    OLED_IIC_SDA = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    while (OLED_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 50)
        {
            OLED_IIC_Stop();
            return 0;
        }
        Delay_For_Pin(1);
    } 
    OLED_IIC_SCL = 0; //ʱ�����0 Clock output 0
    return 1;
}

/**
 * @Brief: ����ACKӦ�� Generate ACK response
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_Ack(void)
{
    OLED_IIC_SCL = 0;
    OLED_SDA_OUT();
    OLED_IIC_SDA = 0;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 0;
}

/**
 * @Brief: ����NACKӦ�� Generate NACK response
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_NAck(void)
{
    OLED_IIC_SCL = 0;
    OLED_SDA_OUT();
    OLED_IIC_SDA = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 0;
}

/**
 * @Brief: IIC����һ���ֽ� IIC sends a byte
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    OLED_SDA_OUT();
    OLED_IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ��� Pull the clock low to start data transmission
    for (t = 0; t < 8; t++)
    {
        OLED_IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        Delay_For_Pin(1);
        OLED_IIC_SCL = 1;
        Delay_For_Pin(1);
        OLED_IIC_SCL = 0;
        Delay_For_Pin(1);
    }
}


/**
 * @Brief: I2Cд���ݺ��� I2C write data function
 * @Note:
 * @Parm: addr:I2C��ַ(I2C address) | reg:�Ĵ���(register) | len:���ݳ���(data length) | data:����ָ��(data pointer)
 * @Retval: 0:ֹͣ(stop) | 1:����д(Continuous Write)
 */
int OLED_i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    int i;
    if (!OLED_IIC_Start())
        return 1;
    OLED_IIC_Send_Byte(addr << 1);
    if (!OLED_IIC_Wait_Ack())
    {
        OLED_IIC_Stop();
        return 1;
    }
    OLED_IIC_Send_Byte(reg);
    OLED_IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        OLED_IIC_Send_Byte(data[i]);
        if (!OLED_IIC_Wait_Ack())
        {
            OLED_IIC_Stop();
            return 0;
        }
    }
    OLED_IIC_Stop();
    return 0;
}

/**
 * @Brief: I2C������ I2C Read Function
 * @Note:
 * @Parm: ����ͬд���� Parameters are similar to those written
 * @Retval:
 */
int OLED_i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!OLED_IIC_Start())
        return 1;
    OLED_IIC_Send_Byte(addr << 1);
    if (!OLED_IIC_Wait_Ack())
    {
        OLED_IIC_Stop();
        return 1;
    }
    OLED_IIC_Send_Byte(reg);
    OLED_IIC_Wait_Ack();
    OLED_IIC_Start();
    OLED_IIC_Send_Byte((addr << 1) + 1);
    OLED_IIC_Wait_Ack();
    while (len)
    {
        if (len == 1)
            *buf = OLED_IIC_Read_Byte(0);
        else
            *buf = OLED_IIC_Read_Byte(1);
        buf++;
        len--;
    }
    OLED_IIC_Stop();
    return 0;
}

/**
 * @Brief: ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK Read 1 byte, when ack=1, send ACK, when ack=0, send nACK
 * @Note:
 * @Parm:
 * @Retval:
 */
uint8_t OLED_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    OLED_SDA_IN(); //SDA����Ϊ���� SDA is set as input
    for (i = 0; i < 8; i++)
    {
        OLED_IIC_SCL = 0;
        Delay_For_Pin(2);
        OLED_IIC_SCL = 1;
        receive <<= 1;
        if (OLED_READ_SDA)
            receive++;
        Delay_For_Pin(2);
    }
    if (ack)
        OLED_IIC_Ack(); //����ACK   Send ACK
    else
        OLED_IIC_NAck(); //����nACK   Send nACK
    return receive;
}

/**
 * @Brief: ��ȡָ���豸 ָ���Ĵ�����һ��ֵ Read a value of a specified register of a specified device
 * @Note: 
 * @Parm: I2C_Addr  Ŀ���豸��ַ(Target device address) | addr     �Ĵ�����ַ(Register Address)
 * @Retval: 
 */
unsigned char OLED_I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
    unsigned char res = 0;

    OLED_IIC_Start();
    OLED_IIC_Send_Byte(I2C_Addr); //����д���� Send a write command
    res++;
    OLED_IIC_Wait_Ack();
    OLED_IIC_Send_Byte(addr);
    res++; //���͵�ַ Shipping Address
    OLED_IIC_Wait_Ack();
	
    OLED_IIC_Start();
    OLED_IIC_Send_Byte(I2C_Addr + 1);
    res++; //�������ģʽ Entering receive mode
    OLED_IIC_Wait_Ack();
    res = OLED_IIC_Read_Byte(0);
    OLED_IIC_Stop(); //����һ��ֹͣ���� Generates a stop condition

    return res;
}

/**
 * @Brief: ��ȡָ���豸 ָ���Ĵ����� length��ֵ Read length values of the specified register of the specified device
 * @Note: 
 * @Parm: dev  Ŀ���豸��ַ(Target device address) | reg   �Ĵ�����ַ(Register Address) | length Ҫ�����ֽ���(The number of bytes to read) | *data  ���������ݽ�Ҫ��ŵ�ָ��(Pointer to where the read data will be stored0
 * @Retval: ���������ֽ����� The number of bytes read
 */
uint8_t OLED_OLED_IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
    uint8_t count = 0;

    OLED_IIC_Start();
    OLED_IIC_Send_Byte(dev); //����д���� Send a write command
    OLED_IIC_Wait_Ack();
    OLED_IIC_Send_Byte(reg); //���͵�ַ Shipping Address
    OLED_IIC_Wait_Ack();
    OLED_IIC_Start();
    OLED_IIC_Send_Byte(dev + 1); //�������ģʽ Entering receive mode
    OLED_IIC_Wait_Ack();

    for (count = 0; count < length; count++)
    {

        if (count != length - 1)
            data[count] = OLED_IIC_Read_Byte(1); //��ACK�Ķ����� Read data with ACK
        else
            data[count] = OLED_IIC_Read_Byte(0); //���һ���ֽ�NACK Last byte NACK
    }
    OLED_IIC_Stop(); //����һ��ֹͣ���� Generates a stop condition
    return count;
}

/**
 * @Brief: ������ֽ�д��ָ���豸 ָ���Ĵ��� Write multiple bytes to the specified device and register.
 * @Note: 
 * @Parm: dev  Ŀ���豸��ַ(Target device address) | reg   �Ĵ�����ַ(Register Address) | length Ҫд���ֽ���(The number of bytes to write) | *data  ��Ҫд�����ݵ��׵�ַ(The first address of the data to be written)
 * @Retval: �����Ƿ�ɹ� Returns whether it is successful
 */
uint8_t OLED_OLED_IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{

    uint8_t count = 0;
    OLED_IIC_Start();
    OLED_IIC_Send_Byte(dev); //����д����  Send a write command
    OLED_IIC_Wait_Ack();
    OLED_IIC_Send_Byte(reg); //���͵�ַ  Shipping Address
    OLED_IIC_Wait_Ack();
    for (count = 0; count < length; count++)
    {
        OLED_IIC_Send_Byte(data[count]);
        OLED_IIC_Wait_Ack();
    }
    OLED_IIC_Stop(); //����һ��ֹͣ���� Generates a stop condition

    return 1; //status == 0;
}

/**
 * @Brief: ��ȡָ���豸 ָ���Ĵ�����һ��ֵ Read a value of a specified register of a specified device
 * @Note: 
 * @Parm: dev  Ŀ���豸��ַ(Target device address) | reg   �Ĵ�����ַ(Register Address) | *data  ���������ݽ�Ҫ��ŵĵ�ַ(The address where the read data will be stored)
 * @Retval: 1
 */
uint8_t OLED_IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
{
    *data = OLED_I2C_ReadOneByte(dev, reg);
    return 1;
}

/**
 * @Brief: д��ָ���豸 ָ���Ĵ���һ���ֽ� Write a byte to the specified device and register
 * @Note: 
 * @Parm: dev  Ŀ���豸��ַ(Target device address) | reg   �Ĵ�����ַ(Register Address) | data  ��Ҫд����ֽ�(The bytes to be written)
 * @Retval: 1
 */
unsigned char OLED_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
    return OLED_OLED_IICwriteBytes(dev, reg, 1, &data);
}

/**
 * @Brief: �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ    Read   Modify   Write  Specified device    Specified register Multiple bits in a byte
 * @Note: 
 * @Parm: dev  Ŀ���豸��ַ(Target device address) | reg   �Ĵ�����ַ(Register Address) | bitStart  Ŀ���ֽڵ���ʼλ(The start bit of the target byte) | length   λ����(Bit Length) | data    ��Ÿı�Ŀ���ֽ�λ��ֵ(Store the value of the target byte to be changed)
 * @Retval: 1:�ɹ�(success) | 0:ʧ��(fail)
 */
uint8_t OLED_OLED_IICwriteBits(uint8_t dev, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data)
{

    uint8_t b;
    if (OLED_IICreadByte(dev, reg, &b) != 0)
    {
        uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return OLED_IICwriteByte(dev, reg, b);
    }
    else
    {
        return 0;
    }
}

/**
 * @Brief: �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ  Read   Modify   Write   Specified device   Specified register 1 bit in a byte
 * @Note: 
 * @Parm: dev  Ŀ���豸��ַ(Target device address) | reg   �Ĵ�����ַ(Register Address) | bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ(The bitNum bit of the target byte to be modified) | data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ(When it is 0, the target bit will be cleared to 0, otherwise it will be set)
 * @Retval: 1:�ɹ�(success) | 0:ʧ��(fail)
 */
uint8_t OLED_IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    OLED_IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return OLED_IICwriteByte(dev, reg, b);
}


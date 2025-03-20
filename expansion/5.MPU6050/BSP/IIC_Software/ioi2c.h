#ifndef __IOI2C_H_
#define __IOI2C_H_

#include "main.h"

//IO port operation macro definition
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

// Output data register addresses
#define GPIOA_ODR_Addr    (GPIOA_BASE+0x14) // STM32F4 ODR offset is 0x14, not 0x0C
#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14)
#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14)
#define GPIOD_ODR_Addr    (GPIOD_BASE+0x14)
#define GPIOE_ODR_Addr    (GPIOE_BASE+0x14)
#define GPIOF_ODR_Addr    (GPIOF_BASE+0x14)
#define GPIOG_ODR_Addr    (GPIOG_BASE+0x14)
#define GPIOH_ODR_Addr    (GPIOH_BASE+0x14) // New for F4
#define GPIOI_ODR_Addr    (GPIOI_BASE+0x14) // New for F4

// Input data register addresses
#define GPIOA_IDR_Addr    (GPIOA_BASE+0x10) // STM32F4 IDR offset is 0x10, not 0x08
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10)
#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10)
#define GPIOD_IDR_Addr    (GPIOD_BASE+0x10)
#define GPIOE_IDR_Addr    (GPIOE_BASE+0x10)
#define GPIOF_IDR_Addr    (GPIOF_BASE+0x10)
#define GPIOG_IDR_Addr    (GPIOG_BASE+0x10)
#define GPIOH_IDR_Addr    (GPIOH_BASE+0x10) // New for F4
#define GPIOI_IDR_Addr    (GPIOI_BASE+0x10) // New for F4

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //���    Output
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //����    Input

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //���   Output
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //����   Input

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���   Output
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����   Input



// IO direction setting
//#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//	   IO operation function
//#define IIC_SCL    PBout(10) //SCL
//#define IIC_SDA    PBout(11) //SDA	 
//#define READ_SDA   PBin(11)  //   Input SDA

//Because the hardware sends the exchange
#define SDA_IN()  {                                                                                 \
    GPIOB->MODER &= ~(3 << (8 * 2));              /* Xóa bit cấu hình mode cho chân PB8 */          \
    GPIOB->MODER |= (1 << (8 * 2));               /* Thiết lập mode là output (01) */               \
    GPIOB->OTYPER &= ~(1 << 8);                   /* Thiết lập output type là push-pull (0) */      \
    GPIOB->OSPEEDR &= ~(3 << (8 * 2));            /* Xóa bit tốc độ cho chân PB8 */                 \
    GPIOB->OSPEEDR |= (3 << (8 * 2));             /* Thiết lập tốc độ là very high speed (11) */    \
}

#define SDA_OUT() {											                                        \
    GPIOB->MODER &= ~(3 << (8*2));      /*Xóa bit mode cho PB8 (2 bit cho mỗi chân)*/	            \
    GPIOB->MODER |= (0 << (8*2));       /*Thiết lập mode là input (00)*/							\
    GPIOB->PUPDR &= ~(3 << (8*2));      /*Xóa bit pull-up/pull-down*/								\
    GPIOB->PUPDR |= (1 << (8*2));       /*Thiết lập pull-up (01)*/									\
}
     
// IO operation function
#define IIC_SCL    PBout(11) //SCL
#define IIC_SDA    PBout(10) //SDA	 
#define READ_SDA   PBin(10)  //  Input SDA

// I2C all operation functions
void IIC_MPU6050_Init(void);             //Initialize IIC IO port		 
int IIC_Start(void);					 //Send IIC start signal
void IIC_Stop(void);	  			     //Send IIC stop signal
void IIC_Send_Byte(u8 txd);		         //IIC sends a byte
u8 IIC_Read_Byte(unsigned char ack);     //IIC reads a byte
int IIC_Wait_Ack(void); 			     //IIC waits for ACK signal
void IIC_Ack(void);						 //IIC sends ACK signal
void IIC_NAck(void);					 //IIC does not send ACK signal

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif

//------------------End of File----------------------------

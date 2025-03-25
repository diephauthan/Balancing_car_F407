#ifndef __MYENUM_H_
#define __MYENUM_H_
//JTAG mode setting definition
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00

//For specific implementation ideas, refer to Chapter 5 (pages 87 to 92) of <<CM3 Authoritative Guide>>.
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
 
//IO port operation, only for a single IO port!
//Make sure the value of n is less than 16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  // Output
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  // Input

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  // Output
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  // Input

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  // Output
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  // Input

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  // Output
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  // Input

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  // Output
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  // Input

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  // Output
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  // Input

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  // Output
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  // Input

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  // Output - New for F4
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  // Input - New for F4

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  // Output - New for F4
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  // Input - New for F4

//Motor ML MR
typedef enum {
    MOTOR_ID_ML = 0,
    MOTOR_ID_MR,
    MAX_MOTOR
} Motor_ID;

/* Car running status enumeration */
typedef enum enCarState_t{
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT,
	
	// ps2 simulation value takes effect
	enps2Fleft, //Front left turn
	enps2Fright,//Front right turn
	enps2Bleft, // Back left turn
	enps2Bright,//Back right turn
	
	
	enAvoid, // Ultrasonic avoidance
	enFollow, //  Ultrasonic Follow
	enError
}enCarState;

#endif

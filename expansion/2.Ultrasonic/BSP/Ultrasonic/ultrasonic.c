#include "ultrasonic.h"

u32 g_distance = 0;// Ultrasonic distance


static void ultrasonic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(ULTRASONIC_RCC, ENABLE);

	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2); 
	
	GPIO_InitStructure.GPIO_Pin  = ECHO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ECHO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = TRIG_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);

}

/**************************************************************************
Function: Timer 2 channel 2 input capture initialization
Input parameters: arr: auto-reload value; psc: clock pre-division number
Return value: None
**************************************************************************/
void TIM2_Cap_Init(u16 arr,u16 psc)	
{
	TIM_ICInitTypeDef  		TIM2_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;
	
	ultrasonic_init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//Enable TIM2 clock
	
	// Initialize timer 2 TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; 				//Set the counter auto-reload value
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // Set clock division: TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM up counting mode
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		// Initializes the time base unit of TIMx according to the parameters specified in TIM_TimeBaseInitStruct
	
	// Initialize TIM2 input capture parameters
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; 		//CC1S=02  Select input IC2 to map to TI1
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	// Rising edge capture
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	//  Configure input frequency division, no frequency division
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;				// Configure input filter No filtering
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	// Interrupt group initialization
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2 interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // Preemption priority level 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  // From priority level 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ channel is enabled
	NVIC_Init(&NVIC_InitStructure);  // Initializes peripheral NVIC registers according to the parameters specified in NVIC_InitStruct
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//Allow update interrupt, allow CC2IE to capture interrupt
  	TIM_Cmd(TIM2,ENABLE ); 	//Enable timer 2
}

//Distance to obtain ultrasonic waves
u16 TIM2CH2_CAPTURE_STA,TIM2CH2_CAPTURE_VAL;
void Get_Distance(void)        
{   
	 TRIG_SIG = 1;         
	 delay_us(15);  
	 TRIG_SIG = 0;	
	 if(TIM2CH2_CAPTURE_STA&0X80)					//Successfully captured a high level once
	 {
		 g_distance=TIM2CH2_CAPTURE_STA&0X3F; 
		 g_distance*=65536;					        // Overflow time sum
		 g_distance+=TIM2CH2_CAPTURE_VAL;			// Get the total high level time
		 g_distance=g_distance*170/1000;      		//  Time * speed of sound/2 (round trip), one count 0.001ms
		 TIM2CH2_CAPTURE_STA=0;						// Start the next capture
	 }				
}

// Ultrasonic echo pulse width reading interruption
void TIM2_IRQHandler(void)
{ 		    		  			    
	u16 tsr;
	tsr=TIM2->SR;

	//LED = !LED; // LED status change
	//delay_ms(500);
	if((TIM2CH2_CAPTURE_STA&0X80)==0)					//	Not captured yet
	{
		if(tsr&0X01)									// Timer overflow
		{	    
			 if(TIM2CH2_CAPTURE_STA&0X40)				// The high level has been captured
			 {
				 if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)	// The high level is too long
				 {
					  TIM2CH2_CAPTURE_STA|=0X80;      	// Marks a successful capture
						TIM2CH2_CAPTURE_VAL=0XFFFF;
				 }else TIM2CH2_CAPTURE_STA++;
			 }	 
		}
		if(tsr&0x04)								// Capture 2 Capture event occurs //0x04
		{	
			if(TIM2CH2_CAPTURE_STA&0X40)		  	//Capture a falling edge	
			{	  	
         	    
				TIM2CH2_CAPTURE_STA|=0X80;		  	// The marker successfully captures a high level pulse width
				TIM2CH2_CAPTURE_VAL=TIM2->CCR2;	 	// Get the current capture value.
				TIM2->CCER&=~(1<<5);			    // CC2P=0 Set to rising edge capture
			}
			else  								    // Not started yet, first capture rising edge
			{
				 TIM2CH2_CAPTURE_STA=0;	 			// Clear
				 TIM2CH2_CAPTURE_VAL=0;
				 TIM2CH2_CAPTURE_STA|=0X40;			// The marker captures the rising edge
				 TIM2->CNT=0;						// Counter clear
				 TIM2->CCER|= (1<<5); 				// CC2P=1 is set to capture the falling edge
			}		    
		}			     	    					   
	}
	TIM2->SR=0;										// Clear interrupt flag
}






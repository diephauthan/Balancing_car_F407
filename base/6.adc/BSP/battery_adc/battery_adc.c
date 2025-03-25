#include "battery_adc.h"

//Initialization of battery level detection
void Battery_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	RCC_AHB1PeriphClockCmd(BAT_GPIO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(BAT_ADC_RCC, ENABLE);
	
	// 168M/6=28, ADC maximum input clock cannot exceed 14M
	// PA5 is used as analog channel input pin
	GPIO_InitStructure.GPIO_Pin = BAT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //Analog input
	
	GPIO_Init(BAT_GPIO_PORT, &GPIO_InitStructure);
	
	// Config Common parameters
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_DeInit();
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	
	ADC_Init(BAT_ADC, &ADC_InitStructure);					
	ADC_Cmd(BAT_ADC, ENABLE);			

}

static u16 Battery_Get(u8 channel)
{
	u16 timeout = 1000;
	
	// Set the specified ADC rule group channels, set their conversion order and sampling time
	ADC_RegularChannelConfig(BAT_ADC, channel, 1, ADC_SampleTime_144Cycles);
	
	// Channel 1, the regular sampling order value is 1, and the sampling time is 144 cycles
	
	ADC1->CR2 |= ADC_CR2_SWSTART;  // Start ADC conversion by software

	while (!ADC_GetFlagStatus(BAT_ADC, ADC_FLAG_EOC) && timeout--); // Wait for the conversion to finish
	return ADC_GetConversionValue(BAT_ADC); // Returns the most recent conversion result of the BAT_ADC rule group
}

//Obtain the average value of multiple ADC measurements, ch: channel value; Times: measurement frequency
uint16_t Battery_Get_Average(uint8_t ch, uint8_t times)
{
	uint16_t temp_val = 0;
	uint8_t t;
	for (t = 0; t < times; t++)
	{
		temp_val += Battery_Get(ch);
	}
	if (times == 4)
	{
		temp_val = temp_val >> 2;
	}
	else
	{
		temp_val = temp_val / times;
	}
	return temp_val;
}


//Obtain the measured original voltage value
// float Get_Measure_Voltage(void)
// {
// 	uint16_t adcx;
// 	float temp;
// 	adcx = Battery_Get(BAT_ADC_CH);
// 	temp = (float)adcx * (3.30f /4095.0f);
// 	return temp;
// }

float Get_Measure_Voltage(void)
{
    uint16_t adcx;
    float temp;
    
    // Đọc ADC nhiều lần để kiểm tra độ ổn định
    adcx = Battery_Get_Average(BAT_ADC_CH, 10);
    
    // In giá trị ADC raw
    printf("ADC Raw: %d\r\n", adcx);
    
    // Chuyển đổi giá trị ADC sang điện áp
    temp = (float)adcx * (3.30f / 4095.0f);
    
    // In giá trị điện áp
    printf("Voltage: %.4f V\r\n", temp);
    
    return temp;
}

//Obtain the actual voltage of the battery before voltage division
// float Get_Battery_Voltage(void)
// {
// 	float temp = Get_Measure_Voltage();
// 	//The actual measured value is slightly lower than the calculated value.
// 	temp = temp * 4.03f; //temp*(10+3.3)/3.3; 
// 	return temp;
// }


float Get_Battery_Voltage(void)
{
    float temp = Get_Measure_Voltage();
    return temp; // Loại bỏ nhân 4.03
}

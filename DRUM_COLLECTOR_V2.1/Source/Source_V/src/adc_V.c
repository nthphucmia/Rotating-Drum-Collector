#include <stm32f10x.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "serial.h"
#include "adc.h"

#define POWER_8V4BAT 0 // channel 10
#define POWER_5VMAIN 1 // channel 11
#define POWER_5VANLO 2 // channel 12
#define POWER_4V3SIM 3 // channel 13
#define POWER_3V3STM 4 // channel 15


extern NVIC_InitTypeDef NVIC_InitStructure;
extern GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

extern volatile uint8_t Flag_Control_ADC;
extern uint16_t adc_value_system, adc_value_system1, adc_value_system2, adc_value_system3, adc_value_system4;

void ADC_System_Init(void) 
{
	ADC_InitTypeDef  ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOC, ENABLE);

	// ADCx configuration ------------------------------------------------------
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);	
	 ADC_Init(ADC2, &ADC_InitStructure);// for ADC2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); // Channel configure Sensor
  ADC_Cmd(ADC1, ENABLE);  // Enable ADC1
	ADC_Cmd(ADC2, ENABLE);  // Enable ADC2
	ADC_TempSensorVrefintCmd(ENABLE); 
  ADC_ResetCalibration(ADC1);
	ADC_ResetCalibration(ADC2);// Reset and calibration ADC2
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1)); 
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);	   //Stop
	//for ADC 2
	while(ADC_GetResetCalibrationStatus(ADC2));
  ADC_StartCalibration(ADC2);
  while(ADC_GetCalibrationStatus(ADC2)); 
  ADC_SoftwareStartConvCmd(ADC2, DISABLE);	   //Stop
  
	////ADC_SoftwareStartConvCmd(ADC1, ENABLE);	   //bat dau chuyen doi
	
}

/*----------------------------------------------------------------------------*/
void ADC_System_Start (unsigned char channel) 
{
  ADC_SoftwareStartConvCmd(ADC1, DISABLE);  
	
	switch(channel)
	{
		case POWER_8V4BAT:
		  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5); 
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		
			break;
		
		case POWER_5VMAIN:
			ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5); 
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	
			break;
		
		case POWER_5VANLO:
      ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5); 
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	
			break;
		
		case POWER_4V3SIM:
      ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5); 
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	
			break;
		
			case POWER_3V3STM:
      ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5); 
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
			break;
	}
}

/*----------------------------------------------------------------------------*/
void ADC_System_Stop(void)
{
     ADC_SoftwareStartConvCmd(ADC1, DISABLE);
}

/*----------------------------------------------------------------------------*/
unsigned short int  Read_Adc_Channel(void)
{
	uint16_t data;
	while(!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)));
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	data=ADC_GetConversionValue(ADC1);
	return data;  
}

unsigned short int  Read_Adc2_Channel(void)
{
	uint16_t data;
	while(!(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)));
	ADC_ClearFlag(ADC2, ADC_FLAG_EOC);
	data=ADC_GetConversionValue(ADC2);
	return data;  
}
/*----------------------------------------------------------------------------*/
void Read_ADC_System(void)
{
		ADC_System_Start(POWER_8V4BAT);// get power from ADC channel 10 (PC.0)
		adc_value_system = Read_Adc_Channel();
		ADC_System_Stop();
	
		ADC_System_Start(POWER_5VMAIN);// get power from ADC channel 11 (PC.1)
		adc_value_system1 = Read_Adc_Channel();
		ADC_System_Stop();
	
		ADC_System_Start(POWER_5VANLO);// get power from ADC channel 12 (PC.2)
		adc_value_system2 = Read_Adc_Channel();
		ADC_System_Stop();
	
		ADC_System_Start(POWER_4V3SIM); // get power from ADC channel 13 (PC.3)
		adc_value_system3 = Read_Adc_Channel();
		ADC_System_Stop();
	
		ADC_System_Start(POWER_3V3STM); // get power from ADC channel 15 (PC.5)
		adc_value_system4 = Read_Adc_Channel();
		ADC_System_Stop();
	
		DBG_SER1_PutChar(0x55);
		DBG_SER1_PutChar(0xaa);
	
		DBG_SER1_PutChar(adc_value_system&0x00ff);
		DBG_SER1_PutChar((adc_value_system&0xff00)>>8);
	
		DBG_SER1_PutChar(adc_value_system1&0x00ff);
		DBG_SER1_PutChar((adc_value_system1&0xff00)>>8);
	
		DBG_SER1_PutChar(adc_value_system2&0x00ff);
		DBG_SER1_PutChar((adc_value_system2&0xff00)>>8);
	
		DBG_SER1_PutChar(adc_value_system3&0x00ff);
		DBG_SER1_PutChar((adc_value_system3&0xff00)>>8);
	
		DBG_SER1_PutChar(adc_value_system4&0x00ff);
		DBG_SER1_PutChar((adc_value_system4&0xff00)>>8);
}
// Read adc raw data from sensor
unsigned short int Read_Sensor_ADC_VCP(void)
{
		uint16_t acd_sensor_raw;
		ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		acd_sensor_raw = Read_Adc2_Channel();
		ADC_SoftwareStartConvCmd(ADC2, DISABLE);;
		return acd_sensor_raw;
}
// Reat adc filter from sensor
unsigned short int Read_Sensor_ADC_Filter_Signal(void)
{
		uint16_t acd_sensor_filter;
		ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		acd_sensor_filter = Read_Adc2_Channel();
		ADC_SoftwareStartConvCmd(ADC2, DISABLE);;
		return acd_sensor_filter;
}

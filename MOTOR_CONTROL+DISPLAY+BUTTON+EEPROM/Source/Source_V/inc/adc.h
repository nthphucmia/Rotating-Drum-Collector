#ifndef _ADC_H__
#define _ADC_H__

//typedef struct
//{
//	unsigned char M_Byte;
//	unsigned char L_Byte;
//}SYSTEM_DataTypeDef;

void ADC_System_Init(void);
void Read_ADC_System(void);
void ADC_Sensor_Init(void);

void ADC_Init2 (void);
void ADC_System_Start(unsigned char channel);
void ADC_System_Stop(void);
unsigned short int Read_Adc_Channel(void);
unsigned short int  Read_Adc2_Channel(void);
unsigned short int Read_Sensor_ADC_VCP(void);
unsigned short int Read_Sensor_ADC_Filter_Signal(void);

#endif

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "Usart_serial.h"
#include "FB_Control.h"
#include "GPIO_Configuration.h"
#include "Encoder_Configuration.h"
#include "DC_Motor_Control.h"
#include "stm32f10x_type.h"
#include "stdio.h"
#include "LiquidCrystal_I2C.h"
#include "ButControl.h"
#include "i2c.h"
#include "at24cxx.h"

extern TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
extern NVIC_InitTypeDef 				NVIC_InitStructure;
extern GPIO_InitTypeDef  			GPIO_InitStructure;
extern EXTI_InitTypeDef  			EXTI_InitStructure;
extern TIM_OCInitTypeDef 			TIM_OCInitStructure;
extern TIM_ICInitTypeDef 			TIM_ICInitStructure;
extern I2C_InitTypeDef		  		I2C_InitStructure;

uint16_t counterPulse=500;
extern uint16_t setSpeed;
extern uint16_t inputSpeed;
extern volatile uint8_t flagRun;
extern volatile uint8_t flagStop;
extern volatile uint8_t flagBut;
extern volatile uint8_t flagRunEPROM;
extern volatile uint8_t flagSaveEPROM;

extern	uint8_t Buff_Str[16];
extern	uint8_t Buff_Str1[16];

void EXTI4_Configuration(void) 
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
		/* mapping */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
		/* Clear the the EXTI line interrupt pending bit */
		EXTI_ClearITPendingBit(EXTI_Line4);
		/*EXTI line Configuration */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Line =EXTI_Line4;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
void EXTI9_5_Configuration(void)
	{
		//PA5
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
		/* mapping */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
		/* Clear the the EXTI line interrupt pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
		/*EXTI line Configuration */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Line =EXTI_Line5;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		// PB8,PB9
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);		
		/* mapping */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
		/* Clear the the EXTI line interrupt pending bit */
		EXTI_ClearITPendingBit(EXTI_Line8);
		EXTI_ClearITPendingBit(EXTI_Line9);
		/*EXTI line Configuration */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Line =EXTI_Line8|EXTI_Line9;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
}
	
void EXTI15_10_Configuration(void)
{  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15); 
	EXTI_ClearITPendingBit(EXTI_Line15);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;							
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
}
/*EXTI_IRQHandler*/
// BUTTON 2
void EXTI4_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line4);
				counterPulse=counterPulse+50;	
				inputSpeed=counterPulse;
		}
	}
// BUTTON 1	
void EXTI9_5_IRQHandler(void)
	{		
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line5);
				counterPulse=counterPulse-50;	
				inputSpeed=counterPulse;
		}		
				// SAVE DATA- BUTTON 
		if(EXTI_GetITStatus(EXTI_Line8) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line8);
				flagSaveEPROM=1;
		}
		
		// RUN DIRECTLY EPROM	- BUTTON 	
		if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line9);
				flagRunEPROM=1; 																	
		}
}
		// RUN	
void EXTI15_10_IRQHandler(void)
 {
		if(EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
		 EXTI_ClearITPendingBit(EXTI_Line15);	
		 flagRun=1;
		}
 }
 
int Keypad_Getkey (void)
{
		if(flagRun==1)
	{
	  flagRun=2;
		flagBut=flagRun;
	}
	
	if(flagSaveEPROM==1)
		{
			flagSaveEPROM=3;
			flagBut=flagSaveEPROM;
		}
		
	if(flagRunEPROM==1)
		{
			flagRunEPROM=4;
			flagBut=flagRunEPROM;
		}

	return flagBut;
}
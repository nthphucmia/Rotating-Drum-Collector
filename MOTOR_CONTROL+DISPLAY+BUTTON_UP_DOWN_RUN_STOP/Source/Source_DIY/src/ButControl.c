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

#define UP_MODE			0
#define DOWN_MODE		1
#define RUN		      2

//int butMode=DEFALT;
int butPin[]={4,5,6,7};

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

void EXTI4_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line4);
				counterPulse=counterPulse+50;	
				inputSpeed=counterPulse;
		}
	}
	
void EXTI9_5_IRQHandler(void)
	{		
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line5);
				counterPulse=counterPulse-50;	
				inputSpeed=counterPulse;
		}
		// RUN
		if(EXTI_GetITStatus(EXTI_Line8) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line8);
				flagRun=1;
		}
					
		// STOP
		if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line9);
				flagStop=1; 																	
		}
}
	int Keypad_Getkey (void)
{
		if(flagRun==1)
	{
	  flagRun=2;
		flagBut=flagRun;
		GPIO_WriteBit(GPIOB, GPIO_Pin_7,(BitAction)(1^GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)));
	}
	
	if(flagStop==1)
		{
			flagStop=3;
			flagBut=flagStop;
			GPIO_WriteBit(GPIOB, GPIO_Pin_5,(BitAction)(1^GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)));
			Stop_Motor_BRA();	
		}
	return flagBut;
}
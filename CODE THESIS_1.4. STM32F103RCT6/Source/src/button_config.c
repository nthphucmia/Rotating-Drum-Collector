#include "main.h"
#include "button_config.h"

extern NVIC_InitTypeDef 			NVIC_InitStructure;
extern GPIO_InitTypeDef  			GPIO_InitStructure;
extern EXTI_InitTypeDef  			EXTI_InitStructure;

extern  uint16_t inputSpeed;
/*******************************************************************************
* Name					 : Hong Phuc
* Description    : Button_Config
* Date					 : 2 July, 2019
*******************************************************************************/
void ButtonInit(void)
	{		
		//PA5
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		

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
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14;		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);  
		//mapping
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); 
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15); 
		/* Clear the the EXTI line interrupt pending bit */
		EXTI_ClearITPendingBit(EXTI_Line15);
		EXTI_ClearITPendingBit(EXTI_Line14);
		
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Line=EXTI_Line14|EXTI_Line15;					
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		NVIC_Configuration();
	}

/*******************************************************************************
* Name					 : Hong Phuc
* Description    : NVIC_Configuration
* Date					 : 2 July, 2019
*******************************************************************************/
void NVIC_Configuration(void)
	{
/*--------------------------------------------------------------------------*/			
		// Enable the TIM1 global interrupt
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
/*--------------------------------------------------------------------------*/
		// Enable the TIM4 global interrupt		
		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
/*BUTTON*/		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		// 0x0f=15: muc uu tien thap nhat
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	

	}
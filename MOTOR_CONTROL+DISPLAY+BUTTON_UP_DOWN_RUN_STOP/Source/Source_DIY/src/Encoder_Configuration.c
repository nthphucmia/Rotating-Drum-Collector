#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "Encoder_Configuration.h"
#include "stm32f10x_type.h"
#include "Usart_serial.h"

extern TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
extern NVIC_InitTypeDef 				NVIC_InitStructure;
extern GPIO_InitTypeDef  				GPIO_InitStructure;
extern EXTI_InitTypeDef  				EXTI_InitStructure;
extern TIM_OCInitTypeDef 				TIM_OCInitStructure;
extern TIM_ICInitTypeDef 				TIM_ICInitStructure;

void Encoder_System_Configuration(void)
{
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM3, ENABLE);

	/*----------------------------Timer1 base configuration-------------------------------------*/
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;				
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=0xffff;
	TIM_TimeBaseInitStructure.TIM_Prescaler=1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	/*-----------------Encoder 1 configuration - Input Capture Interface-----------------------*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1|TIM_Channel_2;							
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICFilter=15;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	/*Configures the TIM1 Encoder Interface*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI1, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		
	TIM_SetCounter(TIM3,0);
	
	TIM_Cmd(TIM3, ENABLE);
	
	// clear update flag
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); 		
	
	/* *Enable Update Interrupt of TIMER3*/ 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

}

/* TIM4 is interrupted every 10ms to calculate the velocity.*/
void Encoder_Reading_Configuration (void)
	{
	TIM_DeInit(TIM4);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// TIM4 Configuration
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);											// cho phep thanh ghi auto reload hoat dong-xay ra tran tu dong reset bo dem
	
	TIM_SetCounter(TIM4,0);
		
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  						// cho phep ngat khi tran
	TIM_ClearFlag(TIM4, TIM_FLAG_Update); 									// clear co bao tran de san sang cho lan tran dau tien
	 
	TIM_Cmd(TIM4, ENABLE); 											// enable timer 4		
}
		
void NVIC_Configuration(void)
	{
/*--------------------------------------------------------------------------*/			
		// Enable the TIM1 global interrupt
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
	 
/*--------------------------------------------------------------------------*/
		// Enable the TIM4 global interrupt		
		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
		
				/*NVIC Configuration*/
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

#include "main.h"
#include "sermotor_config.h"

extern TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
extern GPIO_InitTypeDef  				GPIO_InitStructure;
extern TIM_OCInitTypeDef 				TIM_OCInitStructure;

#define PWM_PERIOD					60000
/*******************************************************************************
* Name					 : Hong Phuc
* Description    : Pin Motor Configuration
* Date					 : 2 July, 2019
*******************************************************************************/	
void GPIO_Motor_Config (void)
 {		 
	 /* -----------------------------------
		|				PA2:  PWM/ TIM2-CH3					|			
		|				PC0: 	DIR										|
		|				PC1:	BRAKE								  |
	 --------------------------------------*/	
		// PWM 	PIN
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	 

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	 

	 }
/*******************************************************************************
* Name					 : Hong Phuc
* Description    : PWM Configuration
* Date					 : 2 July, 2019
*******************************************************************************/
void PWM_Config(uint16_t pulse)			// PA2: TIM2-CH3
{	
	GPIO_Motor_Config();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;														
  TIM_TimeBaseInitStructure.TIM_Period = PWM_PERIOD-1;															
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
																									
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// clear on compare match
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_Pulse=PWM_PERIOD-pulse-1;		
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, DISABLE);	
}	

void PWM_Update(uint16_t pulseUpdate)
{	
	PWM_Config(2);
	TIM_Cmd(TIM2,	DISABLE);
	TIM_OCInitStructure.TIM_Pulse=PWM_PERIOD-pulseUpdate-1;		
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);								
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}	

/*******************************************************************************
* Name					 : Hong Phuc
* Description    : Run Motor
									BRA: Low
									PWM: High
									DIR: High or Low	
									-----------------------------------
									|				PA2:  PWM/ TIM2-CH3					|			
									|				PC0: 	DIR										|
									|				PC1:	BRAKE								  |
								 --------------------------------------
* Date					 : 2 July, 2019
*******************************************************************************/	
void Run_Motor (uint16_t pulseRun)
	{
		 GPIOC->BSRR = GPIO_Pin_0;				// SetBit DIR1
		 GPIOC->BSRR = GPIO_Pin_1;				// SetBit BRA1
		 PWM_Update(pulseRun);
	}
/*******************************************************************************
* Name					 : Hong Phuc
* Description    : Stop motor (without brake)
															PWM: Low
															BRA: High
															DIR: X
* Date					 : 2 July, 2019
*******************************************************************************/
void Stop_Motor_no_BRA(void)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);			//DIR
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);			//BRA
		GPIO_SetBits(GPIOA,GPIO_Pin_2);			//PWM
		PWM_Update(60000);
	}	
/*******************************************************************************
* Name					 : Hong Phuc
* Description    : Stop motor (with brake)
									PWM, DIR, BRA: High
									PWM, BRA: High		and DIR: Low
									PWM: Low and DIR (X), Brake: High
* Date					 : 2 July, 2019
*******************************************************************************/		
void Stop_Motor_BRA(void)
{
//		GPIO_ResetBits(GPIOC,GPIO_Pin_0);			//DIR
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);			//BRA
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);			//PWM
		PWM_Update(0);
}		

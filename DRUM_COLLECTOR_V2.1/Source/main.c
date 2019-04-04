/********************************************************************************
 1. Use time1 with 4 channels to control 2 motors for PWM are TIM2_CH3 with frequency 2kHz. 
 2. Two encoders combined in motors have 2 phase A,B read by TIM3 with interrupt configuration.
 3. TIM4 is interrupted every 10ms to calculate the velocity.
 4. The calculated velocity is printed in PC by UART with baud rate: 115200
 
 The connected diagram
 STM32F103C8:
 PA2, PA3: Module UART2
 PA6, PA7: phase A, B encoder (motor2) - TIM3				
 PA0, PA1: upper and lower bridge driver motor1		
 PA2: TIM2-CH3
 ----------------------------------------------*/
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

TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
NVIC_InitTypeDef 				NVIC_InitStructure;
GPIO_InitTypeDef  			GPIO_InitStructure;
EXTI_InitTypeDef  			EXTI_InitStructure;
TIM_OCInitTypeDef 			TIM_OCInitStructure;
TIM_ICInitTypeDef 			TIM_ICInitStructure;
I2C_InitTypeDef		  		I2C_InitStructure;       

/*---------------------------------------LCD DISPLAY--------------------------------------------*/
	char buffer[40];
	char buffer1[40];
	int32_t actualSpeed;
	volatile uint8_t sampleCount;
	int varEncoderInput=0;
	
/*-------------------------------------STATE MACHINE-------------------------------------------------*/	
#define SETUP             0
#define FB_CONTROL    		1
#define UP_DOWN       		2
#define SAVE_DATA		      3
#define BRA_MOTOR         4
#define DISPLAY           5
#define DEFALT            6

int   state=DEFALT;
volatile uint8_t flagControl;
/*----------------------------------------------------------------------------------------*/
uint16_t encoderCnt, encoderPreCnt;
uint16_t encoderPulse;
uint32_t timer3Ovr=0;

int main(void)
{ 
	// LCD Init
//	SysTick_Config(SystemCoreClock/1000);  			// 1ms
//	I2C_LCD_Configuration();
//	lcd_init ();                                 // ham khoi dong LCD16x2
//	lcd_goto_XY(1,0);
//	lcd_send_string ("SET SPEED:");
//	lcd_goto_XY(2,0);
//	lcd_send_string("REAL SPEED:");
	
	//	GPIO_USART_Configuration();
	//	USARTx_Configuration(USART3);
	Motor_Pin_Configuration();
	Encoder_Pin_Configuraton();
	
	PWM_Configuration(2);								
	PWM_update(2);
//	PWM_update(5000);
	
	//Encoder Activating
	Encoder_System_Configuration();
	Encoder_Reading_Configuration();
	EXTI9_5_Configuration();
	NVIC_Configuration();
	
	while (1)
  {	
		switch(state)
			{
				case SETUP: 
					break;
								
				case FB_CONTROL:
						FB_Control(700);
					break;
				
				case UP_DOWN:
					break;
				
				case SAVE_DATA:
					break;
				
				case BRA_MOTOR:				
					break;
				
				case DISPLAY:
					break;
				
				case DEFALT:
					state=FB_CONTROL;
					break;
			}
	}
}


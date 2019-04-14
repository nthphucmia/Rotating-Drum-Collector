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
#include "ButControl.h"
#include "i2c.h"
#include "at24cxx.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
NVIC_InitTypeDef 				NVIC_InitStructure;
GPIO_InitTypeDef  			GPIO_InitStructure;
EXTI_InitTypeDef  			EXTI_InitStructure;
TIM_OCInitTypeDef 			TIM_OCInitStructure;
TIM_ICInitTypeDef 			TIM_ICInitStructure;
I2C_InitTypeDef		  		I2C_InitStructure;       

/*---------------------------------------LCD DISPLAY--------------------------------------------*/
	int32_t actualSpeed;
	volatile uint8_t sampleCount;
	int setSpeed=0;
	// display the string
	uint8_t buffer[40];
	uint8_t buffer1[40];
	//display the value
	uint8_t Buff_Str[16];
	uint8_t Buff_Str1[16];
/*-------------------------------------STATE MACHINE-------------------------------------------------*/	
#define SETUP             0
#define FB_CONTROL    		1
#define SET_DATA		      2
#define RUN_EPROM					3
#define DISPLAY           4
#define DEFALT            5

int   state=DEFALT;
volatile uint8_t flagControl=0;
volatile uint8_t flagRun=0;
volatile uint8_t flagMode=0;
volatile uint8_t flagStop=0;
volatile uint8_t flagRunEPROM=0;
volatile uint8_t flagSaveEPROM=0;
volatile uint8_t flagBut=0;
/*----------------------------------------------------------------------------------------*/
uint16_t encoderCnt, encoderPreCnt;
uint16_t encoderPulse;
uint32_t timer3Ovr=0;
extern uint32_t Time_ST;

extern uint16_t counterPulse;
uint16_t inputSpeed;

int main(void)
{ 		
	// LCD Init
		SysTick_Config(SystemCoreClock/1000);  			// 1ms
		I2C_LCD_Configuration();	
		Motor_Pin_Configuration();
		Encoder_Pin_Configuraton();
		GPIO_USART_Configuration();
		USARTx_Configuration(USART1);
		PWM_Configuration(2);
		I2CSOFT_Init();																	//eeprom
	
		PWM_update(2);
	
		Encoder_System_Configuration();
		Encoder_Reading_Configuration();
		EXTI9_5_Configuration();
		EXTI4_Configuration();
		EXTI15_10_Configuration();
		NVIC_Configuration();
			
		lcd_init ();   
		lcd_goto_XY(1,0);
		lcd_send_string ((uint8_t*)"SET SPEED   :");
		lcd_goto_XY(2,0);
		lcd_send_string((uint8_t*)"ACTUAL SPEED:");	
while (1)
  {			
		switch(state)
			{
				case SETUP:
						flagMode=Keypad_Getkey();
											if(flagMode==2)
												{
													FB_Control(counterPulse);
													state=DISPLAY;
													break;
												}			
											
											if(flagMode==3)
												{
													state=SET_DATA;
													break;
												}
												
											if(flagMode==4)
												{
													state=RUN_EPROM;
													break;
												}	
											else	state=DISPLAY;
					break;
								
				case FB_CONTROL:
						FB_Control(inputSpeed);
						state=DISPLAY;					
					break;
								
				case SET_DATA:
								EEPROM_Write(inputSpeed);
								state=DISPLAY;
					break;
				
				case RUN_EPROM:
								counterPulse=EEPROM_Read();
								inputSpeed=counterPulse;
								sprintf((char*)Buff_Str,"%d",inputSpeed);
								lcd_goto_XY(1,14);
								lcd_send_string((uint8_t*)"   ");
								lcd_goto_XY(1,14);
								lcd_send_string(Buff_Str);	
								state=FB_CONTROL;
					break;
				
				case DISPLAY:
					while(sampleCount>10){break;}
							{													
										sprintf((char*)buffer,"%d",inputSpeed);
										lcd_goto_XY(1,14);
										lcd_send_string((uint8_t*)"    ");
										lcd_goto_XY(1,14);
										lcd_send_string((uint8_t*)buffer);
										
										sprintf((char*)buffer1,"%d",actualSpeed);
										lcd_goto_XY(2,14);
										lcd_send_string((uint8_t*)"            ");
										lcd_goto_XY(2,14);
										lcd_send_string((uint8_t*)buffer1);
							}			
										state=SETUP;
					break;
				
				case DEFALT:
						state=SETUP;
					break;
			}
	}
}

/********************************************************************************
 1. Use time1 with 4 channels to control 2 motors for PWM are TIM2_CH3 with frequency 2kHz. 
 2. Two encoders combined in motors have 2 phase A,B read by TIM3 with interrupt configuration.
 3. TIM4 is interrupted every 10ms to calculate the velocity.
 4. The calculated velocity is printed in PC by UART with baud rate: 115200

 ----------------------------------------------*/
#include "def_stm32.h"
#include "delay.h"
#include "usart.h"
#include "pid.h"
#include "enc_config.h"
#include "sermotor_config.h"
#include "LiquidCrystal_I2C.h"
#include "button_config.h"
#include "i2c.h"
#include "at24cxx.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
NVIC_InitTypeDef 				NVIC_InitStructure;
GPIO_InitTypeDef  			GPIO_InitStructure;
EXTI_InitTypeDef  			EXTI_InitStructure;
TIM_OCInitTypeDef 			TIM_OCInitStructure;
TIM_ICInitTypeDef 			TIM_ICInitStructure;
I2C_InitTypeDef		  		I2C_InitStructure;       
USART_InitTypeDef				USART_InitStructure;
/*---------------------------------------LCD DISPLAY--------------------------------------------*/
	int32_t actualSpeed;
//	float actualSpeed;
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

int   	 state=DEFALT;
volatile uint8_t flagControl=0;
volatile uint8_t flagRun=0;
volatile uint8_t flagMode=0;
volatile uint8_t flagStop=0;
volatile uint8_t flagRunEPROM=0;
volatile uint8_t flagSaveEPROM=0;
volatile uint8_t flagBut=0;
/*-----------------------------------------FB CONTROL-----------------------------------------------*/
uint16_t encoderCnt, encoderPreCnt;
int16_t encoderPulse;
uint32_t timer3Ovr=0;
uint16_t inputSpeed;
uint8_t movingArray[8];

extern uint32_t Time_ST;
extern uint16_t counterPulse;

int main(void)
{ 		
		SysTick_Config(SystemCoreClock/1000);  					// 1ms
		I2C_LCD_Config();	
		USART_Config();
		I2CSOFT_Init();																	//eeprom
	
		Sermotor_Config();
		Enc_Config();
		Button_Config();
			 
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
												
												if(flagMode==5)
												{
//													Stop_Motor_no_BRA();
													Stop_Motor_BRA();
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
								lcd_goto_XY(1,18);
								lcd_send_string((uint8_t*)"*");
								state=DISPLAY;
					break;
				
				case RUN_EPROM:
								counterPulse=EEPROM_Read();
								inputSpeed=counterPulse;
								sprintf((char*)Buff_Str,"%d",inputSpeed);
								lcd_goto_XY(1,12);
								lcd_send_string((uint8_t*)"   ");
								lcd_goto_XY(1,12);
								lcd_send_string(Buff_Str);	
								state=FB_CONTROL;
					break;
				
				case DISPLAY:
					while(sampleCount>10){break;}
							{													
										sprintf((char*)buffer,"%d",inputSpeed);
										lcd_goto_XY(1,12);
										lcd_send_string((uint8_t*)"    ");
										lcd_goto_XY(1,12);
										lcd_send_string((uint8_t*)buffer);
										
										sprintf((char*)buffer1,"%d",actualSpeed);
										lcd_goto_XY(2,12);
										lcd_send_string((uint8_t*)"            ");
										lcd_goto_XY(2,12);
										lcd_send_string((uint8_t*)buffer1);
							}			
										state=SETUP;
					break;
				
				case DEFALT:
						lcd_init ();   
						lcd_goto_XY(1,0);
						lcd_send_string ((uint8_t*)"SET SPEED :");
						lcd_goto_XY(2,0);
						lcd_send_string((uint8_t*)"REAL SPEED:");	
				
						state=SETUP;
					break;
			}
	}
}

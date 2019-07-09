/*******************************************************************************
* Name					 : Hong Phuc
* Description    : ROTATING DRUM COLLECTOR
									 1. Use time1 with 4 channels to control motors for PWM are TIM2_CH3 
									 2. The encoders combined in motors have 2 phase A,B read by TIM3 with interrupt configuration.
									 3. TIM4 is interrupted every 100ms to calculate the velocity.
									 4. The calculated velocity is printed in PC by UART with baud rate: 115200
* Date					 : 2 July, 2019
*******************************************************************************/	
#include "main.h"
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
	int32_t actualSpeed;
	uint8_t buffer[5];
/*-------------------------------------STATE MACHINE-------------------------------------------------*/	
#define SETUP             0
#define FB_CONTROL    		1
#define SET_DATA		      2
#define RUN_EPROM					3
#define ACTDISPLAY        4
#define SETDISPLAY				5
#define DEFALT            6

uint8_t DriverDrumState=DEFALT;
uint8_t flagControl=0; 
uint8_t flagRun=0;
uint8_t flagMode=0;
uint8_t flagStop=0;
uint8_t flagRunEPROM=0;
uint8_t flagSaveEPROM=0;
uint8_t flagBut=0;
uint8_t flagDisplay=0;
/*-----------------------------------------FB CONTROL-----------------------------------------------*/
uint16_t encoderCnt, encoderPreCnt;
uint16_t encoderPulse;
uint16_t timer3Ovr=0;
uint16_t inputSpeed=450;
uint16_t saveSpeed;
uint8_t movingArray[8];

/****************************** USER FUNCTION *************************************/	
void ActSpeedDisplay(uint16_t actualSpeed );
void InputSpeedDisplay(uint16_t inputSpeed);
void MainDisplay(void);
//====================================================================
int main(void)
{ 		
		SysTick_Config(SystemCoreClock/1000);  					// 1ms
		I2C_LCD_Config();	
		USART_Config();
		I2CSOFT_Init();																	//eeprom
	
		PWM_Update(2);
		Enc_Config();
		Button_Config();
			 
while (1)
  {	
		switch(DriverDrumState)
			{
				//==============================================================================================
				case SETUP:
						flagMode=Keypad_Getkey();
											if(flagMode==2)										
												{
													DriverDrumState=FB_CONTROL;
													break;
												}			
											
											if(flagMode==3)
												{
													DriverDrumState=SET_DATA;
													break;
												}
												
											if(flagMode==4)
												{
													DriverDrumState=RUN_EPROM;
													break;
												}	
												
												if(flagMode==5)
												{
													Stop_Motor_BRA();
													break;
												}
										else	DriverDrumState=SETDISPLAY;
					break;
				//==============================================================================================				
				case FB_CONTROL:
						FB_Control(inputSpeed);
						DriverDrumState=ACTDISPLAY;	
					break;
				//==============================================================================================			
				case SET_DATA:
								EEPROM_Write(inputSpeed);
								DriverDrumState=FB_CONTROL;
					break;
				//==============================================================================================
				case RUN_EPROM:
								inputSpeed=EEPROM_Read();
								InputSpeedDisplay(inputSpeed);
								DriverDrumState=SETUP;
					break;
				//==============================================================================================
				case ACTDISPLAY:
							 if(actualSpeed!=saveSpeed)
							 {
								 saveSpeed=actualSpeed;
								 ActSpeedDisplay(saveSpeed);
							 }								 
								DriverDrumState=SETDISPLAY;
					break;
				//==============================================================================================
				case SETDISPLAY:
							if(flagDisplay==1)
							{
								flagDisplay=0;
								InputSpeedDisplay(inputSpeed);
							}	
							DriverDrumState=SETUP;		
					break;
				//==============================================================================================
				case DEFALT:
						MainDisplay();
						DriverDrumState=SETUP;
					break;
			}
	}
}
//==============================================================================================
void MainDisplay(void)
{
	lcd_init ();   
	lcd_goto_XY(1,0);
	lcd_send_string ((uint8_t*)"SET SPEED :");
	lcd_goto_XY(2,0);
	lcd_send_string((uint8_t*)"REAL SPEED:");	
	ActSpeedDisplay(actualSpeed);
	InputSpeedDisplay(inputSpeed);
}

void ActSpeedDisplay(uint16_t actualSpeed)
{	
	sprintf((char*)buffer,"%d",actualSpeed);	
	lcd_goto_XY(2,12);
	lcd_send_string((uint8_t*)"            ");
	lcd_goto_XY(2,12);
	lcd_send_string((uint8_t*)buffer);
}

void InputSpeedDisplay(uint16_t inputSpeed)
	{
		sprintf((char*)buffer,"%d",inputSpeed);
		lcd_goto_XY(1,12);
		lcd_send_string((uint8_t*)"            ");
		lcd_goto_XY(1,12);
		lcd_send_string((uint8_t*)buffer);
	}
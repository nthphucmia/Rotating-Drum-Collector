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
/*-LCD DISPLAY---------------------------------------------------------------------------------------*/
volatile uint8_t sampleCount;
uint8_t buffer[5];
/*-STATE MACHINE-------------------------------------------------------------------------------------*/	
#define SETUP             0
#define FEEDBACK    			1
#define RUNEPROM					2
#define DISPLAY        		3
#define INITIAL						4

#define SPEEDSETUP				0
#define SAVESETUP					1

#define ACTDISPLAY				0
#define SETDISPLAY				1
uint8_t   	DriverDrumState=INITIAL;
uint8_t   	MainSetupState;
uint8_t			DisplayState;
/*-FEEDBACK CONTROL VARIABLE-------------------------------------------------------------------------*/
uint16_t encoderCnt, encoderPreCnt;
uint16_t encoderPulse;
uint16_t timer3Ovr=0;
uint16_t inputSpeed=0;
int32_t actualSpeed=0;
uint16_t saveSpeed;
uint8_t movingArray[8];
// FLAG===================================================
uint8_t flagControl=0; 
uint8_t flagRun=0;
uint8_t flagMode=0;
uint8_t flagStop=0;
uint8_t flagRunEPROM=0;
uint8_t flagSaveEPROM=0;
uint8_t flagBut=0;
uint8_t flagDisplay=0;
uint8_t	flagSetup=0;
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
	
		ServoInit(2);
		EncoderInit();
		ButtonInit();
		
while (1)
  {	
		switch(DriverDrumState)
			{
				//==============================================================================================
				case SETUP:
					if(flagSetup==1)
					{
						flagSetup=0;
						MainSetupState=SPEEDSETUP;
					}	
					switch(MainSetupState)
					{
							case SPEEDSETUP:
										if(INCBUT==0)
										{
											while(!INCBUT);
											inputSpeed=inputSpeed+30;
											flagDisplay=1;
										}
									else if(DECBUT==0)
										{
											while(!DECBUT);
											inputSpeed=inputSpeed-30;	
											flagDisplay=1;
										}
									else if(flagStop==1)
										{
											flagStop=0;
											StopServo_BRA();
											Delete_LCD();
											MainDisplay();
											DriverDrumState=SETUP;
										}
									else if(flagRun==1)
										{
											flagRun=0;
											MainSetupState=SAVESETUP;
										}
									else 
									{
										  DisplayState=SETDISPLAY;
											DriverDrumState=DISPLAY;
									}
								break;
					//-----------------------------------
					// Choose MainSetup or Run from EPROM (Default)
					// Chuong trinh ngat chon vao setup
						case SAVESETUP:
							EEPROM_Write(inputSpeed);
							DriverDrumState=FEEDBACK;
							break;
						//-----------------------------------
						default:
									//Choose setup or run 
									if(flagRunEPROM==1)
									{
										DriverDrumState=RUNEPROM;
									}
							break;
					}		
					break;
				//==============================================================================================				
				case FEEDBACK:
						feedbackControl(inputSpeed);
						DisplayState=ACTDISPLAY;
						DriverDrumState=DISPLAY;
					break;
				//==============================================================================================
				case RUNEPROM:
								inputSpeed=EEPROM_Read();
								InputSpeedDisplay(inputSpeed);
								DriverDrumState=FEEDBACK;
					break;
				//==============================================================================================
				case DISPLAY:
						switch(DisplayState)
						{
							case SETDISPLAY:
										if(flagDisplay==1)
										{
											flagDisplay=0;
											InputSpeedDisplay(inputSpeed);
										}
										MainSetupState=SPEEDSETUP;
										DriverDrumState=SETUP;	
								break;
							//-----------------------------------
							case ACTDISPLAY:
									 if(actualSpeed!=saveSpeed)
									 {
										 saveSpeed=actualSpeed;
										 ActSpeedDisplay(saveSpeed);
									 }	
										else if(flagSetup==1)
										{
											flagSetup=0;
											MainSetupState=SPEEDSETUP;
											DriverDrumState=SETUP;
										}
									 else DriverDrumState=FEEDBACK;
								break;
						}
					break;
				//==============================================================================================
				case INITIAL:
						inputSpeed=450;
						MainDisplay();
						flagSetup=0;
						MainSetupState=5;	
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
/********************END***************************/
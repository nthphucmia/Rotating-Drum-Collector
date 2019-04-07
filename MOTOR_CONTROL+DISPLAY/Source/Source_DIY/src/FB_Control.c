#include "FB_Control.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "FB_Control.h"
#include "Usart_serial.h"
#include "GPIO_Configuration.h"
#include "stm32f10x_type.h"
#include "stdio.h"
#include "DC_Motor_Control.h"
#include "LiquidCrystal_I2C.h"
#include "stm32f10x_exti.h"

extern EXTI_InitTypeDef  				EXTI_InitStructure;

/*------------------------------ENCODER----------------------------------------------------------*/
bool CCW=FALSE;			
extern uint16_t encoderCnt, encoderPreCnt;
extern uint16_t encoderPulse;
extern uint32_t timer3Ovr;
extern int varEncoderInput;
volatile int32_t timer2Ovr;
/*------------------------------PID----------------------------------------------------------*/
double Kp=0.1;
double Kd=0.0;
double Ki=0.0;
double pPart=0, dPart=0, iPart=0; 
double Error, preError;
int32_t	desiredSpeed=0, controlSpeed=0;
extern  volatile uint8_t sampleCount;
extern  volatile uint8_t flagControl;
extern  int32_t actualSpeed;
extern 	int setSpeed;


#define INV_SAMPLING_TIME 	100											// 1/sampling_time
#define SAMPLING_TIME       10											// 10ms		
#define PWM_PERIOD					15000
/*---------------------------------------------------------------------------------------*/
extern	char buffer[40];
extern	char buffer1[40];
extern uint32_t Time_ST;

void TIM2_IRQHandler(void)																
{	
 if(TIM_GetITStatus( TIM2,TIM_IT_Update)!=RESET) 				
		{
			TIM_ClearFlag(TIM2, TIM_IT_Update);
			timer2Ovr++;		
    }
}

void TIM3_IRQHandler(void)																
{	
 if(TIM_GetITStatus( TIM3,TIM_IT_Update)!=RESET) 				
		{
			TIM_ClearFlag(TIM3, TIM_IT_Update);
			timer3Ovr++;	
    }
}

void TIM4_IRQHandler(void)
{	
 if(TIM_GetITStatus( TIM4,TIM_IT_Update)!=RESET) 
 {
		TIM_ClearFlag(TIM4, TIM_IT_Update);
		sampleCount++;		 
	  flagControl=1;
 }
}
/*--------------------------------------------------------------------------------------------------------------------------------*/
void FB_Control_PID(uint16_t speed)
{	 
	desiredSpeed=speed;
	Error=desiredSpeed-actualSpeed;

	pPart=Kp*Error;
	dPart=Kd*(Error-preError)*INV_SAMPLING_TIME;       
	iPart+=Ki*Error*SAMPLING_TIME/1000;    
	controlSpeed+=pPart+dPart+iPart;
	preError=Error;

	if(controlSpeed>0 && controlSpeed<15000)
	{
		PWM_update(controlSpeed);
	}
	if(controlSpeed>=15000)	controlSpeed=15000-1;
	if(controlSpeed<=0)			controlSpeed=0;		
}

void Encoder_Reading(void)
{
	encoderCnt=TIM_GetCounter(TIM3)+65536*timer3Ovr;
	encoderPulse=(float)(encoderCnt-encoderPreCnt);
		if(encoderPulse > 0)
					{
						encoderPulse=encoderPulse;
					}
		else if( encoderPulse < 0 )
					{
						encoderPulse= - encoderPulse;
					}
		else {encoderPulse=0;}
			actualSpeed=(encoderPulse*6000)/200;
		  encoderPreCnt=encoderCnt;
}

void FB_Control(uint16_t speed)
{
	if(flagControl==1)
	{
	  flagControl=0;
		Encoder_Reading();
		FB_Control_PID(speed);
	}
}
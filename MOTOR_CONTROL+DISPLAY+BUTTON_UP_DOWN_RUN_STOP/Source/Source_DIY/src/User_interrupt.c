#include "User_interrupt.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "User_interrupt.h"
#include "Usart_serial.h"
#include "GPIO_Configuration.h"
#include "stm32f10x_type.h"
#include "stdio.h"
#include "DC_Motor_Control.h"
#include "LiquidCrystal_I2C.h"
#include "stm32f10x_exti.h"
/*------------------------------ENCODER1----------------------------------------------------------*/
bool CCW=FALSE;			
uint16_t encoderSpeed;
uint16_t Pulse_value;
uint16_t encoder_cnt, encoder_cnt_pre;
uint16_t rate;
char str[50]; 
extern char buffer[40];
extern	char buffer1[40];
uint16_t timer1_temp=0;
uint16_t count_temp=0;
volatile uint32_t timer2_cnt_ovr=0;
extern int varEncoderInput;
/*------------------------------ENCODER2----------------------------------------------------------*/
uint16_t encoder_cnt2, encoder_cnt_pre2;
volatile int32_t timer3_cnt_ovr;
uint16_t rate2;
extern int varEncoderMotor;
int counter_pulse;
extern EXTI_InitTypeDef  				EXTI_InitStructure;
/*------------------------------PID----------------------------------------------------------*/
volatile double Kp=1.3, Kd=0.01, Ki=0.00; // for speed control -v1.2
volatile double pPart=0, dPart=0, iPart=0; // PID gains
volatile double Error, pre_Error;
volatile double u, u_1, PID_speed;
volatile int32_t PWM_value;
volatile int32_t	desired_speed=0, control_speed, desired_speed_1=0;
extern volatile uint8_t sample_count;
extern volatile int32_t actual_speed;
extern 	int varEncoderInput;

#define inv_sampling_time 	100											// 1/sampling_time
#define sampling_time       10											// 10ms		
#define PWM_Period					15000

/*---------------------------------------------------------------------------------------*/

void TIM2_IRQHandler(void)																
{	
 if(TIM_GetITStatus( TIM2,TIM_IT_Update)!=RESET) 				// ngat khi tran
		{
			TIM_ClearFlag(TIM2, TIM_IT_Update);
			timer2_cnt_ovr++;		
    }
}

void TIM3_IRQHandler(void)																
{	
 if(TIM_GetITStatus( TIM3,TIM_IT_Update)!=RESET) 				// ngat khi tran
		{
			TIM_ClearFlag(TIM3, TIM_IT_Update);
			timer3_cnt_ovr++;	
    }
}

void TIM4_IRQHandler(void)
{	
 if(TIM_GetITStatus( TIM4,TIM_IT_Update)!=RESET) 
 {
		TIM_ClearFlag(TIM4, TIM_IT_Update);
		sample_count++;		 
		Encoder1_reading();
		Motor_Speed_PID(varEncoderInput);
//		Motor_Speed_PID(5200);
 }
}

void Motor_Speed_PID(uint16_t control_speed)
{	  	
	desired_speed=control_speed;
	Error=desired_speed-actual_speed;

	pPart=Kp*Error;
	dPart=Kd*(Error-pre_Error)*inv_sampling_time;       
	iPart+=Ki*Error*sampling_time/1000;    
	desired_speed_1+=pPart+dPart+iPart;
	pre_Error=Error;

	if(desired_speed_1>0 && desired_speed_1<15000)
	{
		PWM_update(desired_speed_1);
	}
	if(desired_speed_1>=15000)	desired_speed_1=15000-1;
	if(desired_speed_1<=0)			desired_speed_1=0;		
}

void Encoder1_reading(void)
{
	encoder_cnt=TIM_GetCounter(TIM2)+65536*timer2_cnt_ovr;

	if(encoder_cnt > encoder_cnt_pre)
				{
					rate=(float)(encoder_cnt-encoder_cnt_pre);
				}
			else if( encoder_cnt < encoder_cnt_pre )
				{
					rate= 0 - (float)(encoder_cnt-encoder_cnt_pre);
				}
			else {rate=0;}
			actual_speed=(rate*6000)/400;
		  encoder_cnt_pre=encoder_cnt;
}

void EXTI9_5_IRQHandler(void)
	{		
		if(EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line7);
			
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==RESET)
				{
					counter_pulse=counter_pulse-50;
					if(counter_pulse <0 ) counter_pulse=0;
				}
					if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==SET)
				{
					counter_pulse=counter_pulse+50;
					if(counter_pulse >=5500 ) counter_pulse=5500;  		
				}
				varEncoderInput=counter_pulse;
		}
}		
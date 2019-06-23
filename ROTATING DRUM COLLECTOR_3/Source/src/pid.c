#include "def_stm32.h"
#include "pid.h"
#include "sermotor_config.h"

extern EXTI_InitTypeDef  				EXTI_InitStructure;

/*------------------------------ENCODER----------------------------------------------------------*/
bool CCW=FALSE;			
extern uint16_t encoderCnt, encoderPreCnt;
extern int16_t encoderPulse;
extern uint32_t timer3Ovr;
extern int varEncoderInput;
/*------------------------------PID----------------------------------------------------------*/
double Kp=0.7;
double Kd=0.3;
double Ki=0.0001;
double pPart=0, dPart=0, iPart=0; 
double Error, preError;
int32_t	desiredSpeed=0, controlSpeed=0;
int32_t speedPID=0;
extern  volatile uint8_t sampleCount;
extern  volatile uint8_t flagControl;
extern  int32_t actualSpeed;
//extern  float actualSpeed;
extern 	int setSpeed;


#define INV_SAMPLING_TIME 	100											// 1/sampling_time
#define SAMPLING_TIME       10											// 10ms		
#define PWM_PERIOD					60000
#define NUM_OF_SAMPLE				8
/*---------------------------------------------------------------------------------------*/
extern	char buffer[40];
extern	char buffer1[40];
extern 	uint32_t Time_ST;
extern 	uint16_t movingArray[8];

/*--------------------------------------------------------------------------------------------------------------------------------*/
uint32_t movingAverage(int firstComponent, uint16_t array[])
{
	uint32_t sum=0, avrValue=0;
	uint8_t 	i,j;
	// remove the first component
	for (i=NUM_OF_SAMPLE-1; i>0; i--)
	{
		array[i]=array[i-1];
	}
	// add new component
	array[0]=firstComponent;
	for(j=0; j<NUM_OF_SAMPLE; j++)
	{
		sum=sum+array[j];
	}
	//calculate average
	avrValue=sum/NUM_OF_SAMPLE;	
	
	return avrValue;
}

void Encoder_Reading(void)
{
	encoderCnt=TIM_GetCounter(TIM3)+65536*timer3Ovr;
	encoderPulse=(float)(encoderCnt-encoderPreCnt);
		if(encoderPulse>0)
					{
						encoderPulse=encoderPulse;
					}
		else if( encoderPulse<0)
					{
						encoderPulse= - encoderPulse;
					}
		else {encoderPulse=0;}
		actualSpeed=(encoderPulse*6000)/200;
		encoderPreCnt=encoderCnt;
}

uint32_t FB_Control_PID(uint16_t speed)
{	 
	desiredSpeed=speed;
	Error=desiredSpeed-actualSpeed;

	pPart=Kp*Error;
	dPart=Kd*(Error-preError)*INV_SAMPLING_TIME;       
	iPart+=Ki*Error*SAMPLING_TIME/1000;    
	speedPID+=pPart+dPart+iPart;
	preError=Error;
	
	return speedPID;	
}

void FB_Control(uint16_t speed)
{
	uint32_t firstComponent;
	if(flagControl==1)
		{
			flagControl=0;
			Encoder_Reading();
			firstComponent=FB_Control_PID(speed);
			controlSpeed=movingAverage(firstComponent, movingArray);
			if(controlSpeed>0 && controlSpeed<PWM_PERIOD)
				{
					Run_Motor(controlSpeed);
				}
			if(controlSpeed>=PWM_PERIOD)	controlSpeed=PWM_PERIOD-1;
			if(controlSpeed<=0)			controlSpeed=0;	
		}
}


#ifndef _PID_H
#define _PID_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
/*Define-----------------------------------------------------*/

#define INV_SAMPLING_TIME 	100											// 1/sampling_time
#define SAMPLING_TIME       10											// 10ms		
#define PWM_PERIOD					60000
#define NUM_OF_SAMPLE				8

uint32_t movingAverage(int firstComponent, uint16_t array[]);
void readEncoder(void);
int32_t pidValue(uint16_t desiredSpeed, uint8_t gain);
void feedbackControl(int16_t speed);

#endif /*_PID_H */

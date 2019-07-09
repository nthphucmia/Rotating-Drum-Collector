#ifndef _PID_H
#define _PID_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"

void TIM2_IRQHandler(void);	
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

uint32_t FB_Control_PID(uint16_t speed);
void Encoder_Reading(void);
void FB_Control(uint16_t speed);
uint32_t movingAverage(int firstComponent, uint16_t array[]);

#endif

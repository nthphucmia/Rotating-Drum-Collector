#ifndef _FB_CONTROL_H
#define _FB_CONTROL_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"

void TIM2_IRQHandler(void);	
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

void FB_Control_PID(uint16_t control_speed);
void Encoder_Reading(void);
void FB_Control(uint16_t speed);
#endif
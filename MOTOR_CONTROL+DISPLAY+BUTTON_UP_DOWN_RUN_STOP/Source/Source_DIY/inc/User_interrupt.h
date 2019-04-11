#ifndef _USER_INTERRUPT_H
#define _USER_INTERRUPT_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"

void TIM2_IRQHandler(void);	
//void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void Motor_Speed_PID(uint16_t control_speed);
void Encoder1_reading(void);
void Encoder2_reading(void);
void EXTI9_5_IRQHandler(void);
#endif
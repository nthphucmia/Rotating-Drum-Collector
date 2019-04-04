#ifndef _DC_Motor_Control_H
#define _DC_Motor_Control_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "DC_Motor_Control.h"

void PWM_Configuration(uint16_t pulse);
void PWM_update(uint16_t pulse);
void Run_Motor (uint16_t pulse);
void Stop_Motor_no_BRA(void);

#endif
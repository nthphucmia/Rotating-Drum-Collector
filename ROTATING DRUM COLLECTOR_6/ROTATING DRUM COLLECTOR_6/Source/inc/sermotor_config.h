#ifndef _SERMOTOR_CONFIG_H
#define _SERMOTOR_CONFIG_H

#include "main.h"

void GPIO_Motor_Config (void);
void PWM_Config(uint16_t pulse);
void PWM_Update(uint16_t pulseUpdate);
void Run_Motor (uint16_t pulse);
void Stop_Motor_no_BRA(void);
void Stop_Motor_BRA(void);

#endif

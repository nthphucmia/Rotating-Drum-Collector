#ifndef _GPIO_Configuration_H
#define _GPIO_Configuration_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "GPIO_Configuration.h"

void Motor_Pin_Configuration (void);
void Encoder_Pin_Configuraton (void);
void GPIO_USART_Configuration(void);
void Test_BlinkingLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_LED_Test(void);
#endif
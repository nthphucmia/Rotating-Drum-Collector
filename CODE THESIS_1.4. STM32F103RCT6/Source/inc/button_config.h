#ifndef _BUTTON_CONFIG_H
#define _BUTTON_CONFIG_H

#include  "main.h"
//==========USER BUTTON=================================
#define INCBUT 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define DECBUT 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

void ButtonInit(void);
void NVIC_Configuration(void);

#endif

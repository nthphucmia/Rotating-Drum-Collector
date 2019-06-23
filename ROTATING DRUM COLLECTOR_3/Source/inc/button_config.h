#ifndef _BUTTON_CONFIG_H
#define _BUTTON_CONFIG_H

void Button_Config(void);

void EXTI9_5_Configuration(void);
void EXTI15_10_Configuration(void);
void EXTI4_Configuration(void);

void EXTI9_5_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

void NVIC_Configuration(void);
int Keypad_Getkey (void);

#endif

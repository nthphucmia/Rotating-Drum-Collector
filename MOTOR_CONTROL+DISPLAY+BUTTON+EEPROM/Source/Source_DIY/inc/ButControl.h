#ifndef _BUT_CONTROL_H
#define _BUT_CONTROL_H

void EXTI9_5_Configuration(void);
void EXTI15_10_Configuration(void);
void EXTI4_Configuration(void);

void EXTI9_5_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

int Keypad_Getkey (void);

#endif
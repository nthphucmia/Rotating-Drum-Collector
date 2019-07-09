#ifndef _USART_H
#define _USART_H

#include "def_stm32.h"

/*---------------------------------------USART Configuration ---------------------------------------------------*/
void USART_Config(void);
void USARTx_Config(USART_TypeDef* USARTx);
void GPIO_USART_Config(void);
/*---------------------------------------Ham truyen ---------------------------------------------------*/
void USARTx_SendChar(USART_TypeDef* USARTx, uint8_t Data);
void USARTx_SendString(USART_TypeDef* USARTx, char *Str);

/*---------------------------------------Ham nhan ---------------------------------------------------*/
uint8_t USARTx_GetChar(USART_TypeDef* USARTx);
void USARTx_GetString(USART_TypeDef* USARTx, uint8_t *Str, unsigned char count);
/**************************************************************/
#endif

#ifndef _USART_SERIAL_H
#define _USART_SERIAL_H

#include "stm32f10x.h"

/*---------------------------------------USART Configuration ---------------------------------------------------*/
void USARTx_Configuration(USART_TypeDef* USARTx);

/*---------------------------------------Ham truyen ---------------------------------------------------*/
void USARTx_SendChar(USART_TypeDef* USARTx, uint8_t Data);
void USARTx_SendString(USART_TypeDef* USARTx, char *Str);

/*---------------------------------------Ham nhan ---------------------------------------------------*/
uint8_t USARTx_GetChar(USART_TypeDef* USARTx);
void USARTx_GetString(USART_TypeDef* USARTx, uint8_t *Str, unsigned char count);
/**************************************************************/
#endif
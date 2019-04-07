#include "delay.h"
#include "serial.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
extern USART_InitTypeDef USART_InitStructure;
extern GPIO_InitTypeDef GPIO_InitStructure;

  /* Private function prototypes -----------------------------------------------*/
  #ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
       set to 'Yes') calls __io_putchar() */
     #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #else
   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
   #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
  #endif /* __GNUC__ */
	
void UART1_DBG_Init()
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
     
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
      
    USART_Cmd(USART1, ENABLE);  
  }
	
	
void UART1_DBG_PutChar(uint8_t dat)
{
	USART_SendData(USART1, (uint8_t) dat);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	 {}
}

 unsigned char UART1_DBG_GetChar()
  {
		uint8_t dat;
    while (USART_GetFlagStatus(USART1, USART_IT_RXNE) == RESET)
    {}
			dat= USART_ReceiveData(USART1);
    return dat;
  }
  
    /**
    * @brief  Retargets the C library printf function to the USART.
    * @param  None
    * @retval None
    */
  GETCHAR_PROTOTYPE
  {
    return UART1_DBG_GetChar();
  }
    
  PUTCHAR_PROTOTYPE
  {
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);
   
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
     {}
   
    return ch;
  }
    
    

/*
 *  serial.c
 *	Purpose: 
 *  Created on: May 15, 2015
 *      Author: Tran Ngoc Viet
 *  Note(s): Belong to BioMedical Engineering - International University
						             - Vietnam National University, HCMC
 */
 
#include "stm32f10x.h"      
#include "stm32f10x_usart.h"
#include "serial.h"
#include "delay.h"
#include <stdio.h>

extern USART_InitTypeDef USART_InitStructure;
extern GPIO_InitTypeDef GPIO_InitStructure;
extern NVIC_InitTypeDef NVIC_InitStructure;
//unsigned char Frame_Communicate_PC[30]={};

Tx_PackageBufTypeDef	USART3_TxBuffer;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */




#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input  */
#endif

volatile uint32_t UART0Status, UART1Status, UART2Status;
volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1, UART2TxEmpty = 1;
volatile uint8_t UART0Buffer[1];//BUFSIZE_S];
volatile uint8_t UART1Buffer[BUFSIZE_S];
volatile uint8_t UART2Buffer[1];//BUFSIZE_S];
volatile uint32_t UART0Count = 0, UART1Count = 0, UART2Count = 0;
volatile uint8_t UART1BufferControlFlag;
//char *serial_send_buffer;
void DBG_SER1_Init(void)
{
    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;
     
    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | 
                           RCC_APB2Periph_GPIOA, ENABLE);
                            
    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN9 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa_init_struct);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);  
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart1_init_struct.USART_BaudRate =230400;   
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart1_init_struct.USART_StopBits = USART_StopBits_1;   
    usart1_init_struct.USART_Parity = USART_Parity_No ;
    usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
}
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void DBG_SER1_PutChar(char ch)
{
	
	USART_SendData(USART1, (char) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

}
/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
char DBG_SER1_GetChar(void)
{
		char data;
	  /* Wait the byte is entirely received by USARTz */  
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {
    }
    /* Store the received byte in the RxBuffer2 */
    data = USART_ReceiveData(USART1);
		
		return data;
}
void DBG_SER1_GetString(unsigned char *buffer,unsigned char count) 
{
	unsigned char index;
    // loop through entire string
    while (index<count) 
    {
      *buffer = DBG_SER1_GetChar();
			buffer++;
    }
}

/*----------------------------------------------------------------------------
  Write string of character to Serial Port
 *----------------------------------------------------------------------------*/
void DBG_SER1_PutString(char *s) 
{
    // loop through entire string
    while (*s) 
    {
        DBG_SER1_PutChar(*s);
        s++;
    }
}
void DBG_SER1_PutString_const(const char *s) 
{
    // loop through entire string
    while (*s) 
    {
        DBG_SER1_PutChar(*s);
        s++;
    }
}	
void DBG_SER1_Send(unsigned char *pData, unsigned char lenData)
{
     unsigned char i; 
     for(i=0;i<lenData;i++)
     {
        DBG_SER1_PutChar(pData[i]);// khong delay o day
     }
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

/*-------
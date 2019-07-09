#include "usart.h"
#include "def_stm32.h"

/*--------------------------------------------------------------------------------------------------*/
	#ifdef __GNUC__
	/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
		 set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#endif /* __GNUC__ */

	PUTCHAR_PROTOTYPE
	{
	/* Place your implementation of fputc here */
		/* e.g. write a character to the LCD */
	//lcd_Data_Write((u8)ch);
	USART_SendData(USART2,(u8)ch);

	/*Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
	{}

		return ch;
	}
/*--------------------------------------------------------------------------------------------------*/


extern GPIO_InitTypeDef			GPIO_InitStructure;
extern USART_InitTypeDef		USART_InitStructure;

void USART_Config(void)
	{		
		GPIO_USART_Config();
		USARTx_Config(USART1);
	}
	
void GPIO_USART_Config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 			//AFIO: Alternate function I/O
	
/********************************* PORTA - USART1 *************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
/*________________________________ OUTPUT - TX___________________________________*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
/*________________________________ INPUT - RX___________________________________*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
/********************************* PORTA - USART2*************************************/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
///*________________________________ OUTPUT - TX___________________________________*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);  
//	
///*________________________________ INPUT - RX___________________________________*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);  
//	
//	/********************************* PORTB - USART3*************************************/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
///*________________________________ OUTPUT - TX___________________________________*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);  

// 	
///*________________________________ INPUT - RX___________________________________*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void USARTx_Config(USART_TypeDef* USARTx)
{
	USART_InitTypeDef USART_InitStructure;
	
	if(USARTx==USART1)
		RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	else if (USARTx==USART2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	else if (USARTx==USART3)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// Resets the USARTx peripheral registers to their default reset values
	USART_DeInit(USARTx);			
	// USART initialization
	USART_InitStructure.USART_BaudRate=115200; 							// cau hinh baudrate
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;				
	USART_InitStructure.USART_Mode=	USART_Mode_Rx | USART_Mode_Tx; // cau hinh che do truyen nhan						
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1	; 	// cau hinh Bit stop trong khung truyen
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		// cau hinh so Bit du lieu trong 1 khung truyen
	
	USART_Init(USARTx, &USART_InitStructure);
	
	/*xoa co ngat chO lan dau su dung*/
	USART_ClearFlag(USARTx, USART_IT_RXNE); 
	USART_ClearFlag(USARTx, USART_IT_TXE);
	
	/*Cau hinh cho phep ngat nhan */
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);		// ngat khi thanh ghi du lieu cua USART day
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);			// ngat khi thanh ghi du lieu truyen cua USART trong
	
 //NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USARTx, ENABLE); 
}

/*---------------------------------------Ham truyen ---------------------------------------------------*/

// Write character to Serial Port (ham truyen mot ky tu len PC) 
void USARTx_SendChar(USART_TypeDef* USARTx, uint8_t Data)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);		//USART_FLAG_TC: Tranmission complete flag
	USART_SendData(USARTx, Data);
}

// (ham truyen mot chuoi ky tu tren PC)
void USARTx_SendString(USART_TypeDef* USARTx, char *str)
{
	// loop through entire string
	while (*str)
	{
		USARTx_SendChar(USARTx,*str);
		str++;
	}
}

/*---------------------------------------Ham nhan ---------------------------------------------------*/
/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
uint8_t USARTx_GetChar(USART_TypeDef* USARTx)
{
    uint8_t Data;
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    Data = (uint8_t)USART_ReceiveData(USARTx);
    return Data;
}


void USARTx_GetString(USART_TypeDef* USARTx, uint8_t *Str, unsigned char count)
{
	unsigned index;
	// loop through entire string
	while (index<count)
	{
		*Str=USARTx_GetChar(USARTx);
		Str++;
	}
}

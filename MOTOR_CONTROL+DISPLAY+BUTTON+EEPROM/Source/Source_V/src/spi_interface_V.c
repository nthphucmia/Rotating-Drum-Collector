/**
  ******************************************************************************
  * @file    SPI/CRC/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "serial.h"
#include "stm32f10x_gpio.h"
#include "system.h"
#include "delay.h"
#include "spi_interface.h"
extern volatile uint8_t ADC_Low_Byte,ADC_Medium_Byte,ADC_High_Byte,Two_Complement;
extern volatile uint8_t data_ready_flag;
// define CS of ADS1293
#define ADS1257_CS_DI GPIO_WriteBit( GPIOD, GPIO_Pin_5, Bit_SET) // High Level
#define ADS1257_CS_EN GPIO_WriteBit( GPIOD, GPIO_Pin_5, Bit_RESET) //Low Level

#define ADS1257_RS_EN GPIO_WriteBit( GPIOC, GPIO_Pin_6, Bit_SET)
#define ADS1257_RS_DI GPIO_WriteBit( GPIOC, GPIO_Pin_6, Bit_RESET)

#define ADS1257_PWDN_SYNC_EN GPIO_WriteBit( GPIOC, GPIO_Pin_7, Bit_SET)
#define ADS1257_PWDN_SYNC_DI GPIO_WriteBit( GPIOC, GPIO_Pin_7, Bit_RESET)


#define DRDY_IS_LOW()	(GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_4) == Bit_RESET)

/**------------------------------function start in here---------------------------------------------**/
void SPI_Interface_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO structs for block SPI
	RCC_PCLK2Config(RCC_HCLK_Div1); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_SPI1| RCC_APB2Periph_AFIO, ENABLE);
	/*
		1.PA.00: CS 
		2.PA.04: DRDYB
		3.PA.05: SCLK
		4.PA.06: MISO--SDO
		5.PA.07: MOSI--SDI 
		6.PA.08: RS-- Reset
	*/
	// Configure SPI1  PIN: PD.5--CS 
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//Configure SPI1  PIN: PD.7--SYNC/PWDN | PD.6--RS_ADS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	
  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure SPI1 pins: DRDY -----------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOD, GPIO_Pin_5, Bit_SET);

}
void ADS1257_Power_Up(void)
{
	ADS1257_RS_DI;
	ADS1257_PWDN_SYNC_DI;
	delay_ms(100);
	ADS1257_PWDN_SYNC_EN;
	ADS1257_RS_EN;
	delay_ms(100);
	ADS1257_WriteCmd(0xfe);
	ADS1256_WaitDRDY();
	
}
void SPI_Interface_Init(void)
{
	SPI_InitTypeDef   SPI_InitStructure;   //SPI Struct for SPI1
	SPI_Interface_Configuration();
  // SPI1 configuration
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// Prescaler is divided into PCLK2 (72/2MHz) to get SPI baud rate/clock speed
	// 256 => 140.562kHz
	// 128 => 281.125kHz
	// 64 => 562.5 kHz
	// 32 => 1.125MHz
	// 16 => 2.25 MHz
	// 8  => 4.5 MHz
	// 4  => 9 MHz
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);	
	
}

/*=========================Start Function=================================*/

//  Write a byte to ADS1257
unsigned char SPI_Interface_Send_Byte(unsigned char dat)
{
	unsigned char data;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);//wait for buffer empty
  SPI_I2S_SendData(SPI1, dat); //send a byte data
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);// wait for recieve data	
	data = SPI_I2S_ReceiveData(SPI1);
  return data;
}
//======================================================================
//Write a value to register of ADS1257
void SPI_Interface_WriteReg(unsigned char addr, unsigned char value)
{
	unsigned char inst;
			ADS1257_CS_EN;	//CS low for enable
			inst = ADS1257_CMD_WREG |(addr&15);   // register address
			SPI_Interface_Send_Byte(inst);//Send a byte data
			SPI_Interface_Send_Byte(0x00);//Number of registers - 1, write a register here
			SPI_Interface_Send_Byte(value);//Send a byte data
			ADS1257_CS_DI;   // CS high again
}
//=======================================================================
//Write a command for ADS1257
void ADS1257_WriteCmd(unsigned char cmd)
{
			ADS1257_CS_EN;	//CS low for enable                                   
			SPI_Interface_Send_Byte(cmd);//Send a byte data
			delay_us(1);
			ADS1257_CS_DI;   // CS high again
}

//=======================================================================
unsigned char SPI_Interface_ReadReg(unsigned char addr)
{
	unsigned char x, inst;
			inst= ADS1257_CMD_RREG | (addr&15); 	// register address
			ADS1257_CS_EN;	//CS low for enable
			SPI_Interface_Send_Byte(inst);//Send a byte data
//			delay_us(1);
			SPI_Interface_Send_Byte(0x00);//Number of registers - 1, write a register here
			delay_us(1);//Must be delayed to read the chip return data
			x=SPI_Interface_Send_Byte(0x00);//Send a byte data
			ADS1257_CS_DI;   // CS high again
			return x; // return register value
	}

//========================================================================
// Read ADS1257 ID stutus
unsigned char ADS1256_ReadChipID(void)
{
	uint8_t ID_Status;
	ADS1256_WaitDRDY();
	ID_Status = SPI_Interface_ReadReg(ADS1257_REG_STATUS);
	return (ID_Status >> 4);
}
//========================================================================
void ADS1256_SetChannal(uint8_t ADS1257_Channel)
{
	/*
	Bits 7-6 always write 0 
	Bits 5-4 
		00 = AIN0 (default)
		01 = AIN1
		10 = AIN2 
		11 = AIN3 (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)
	Bits 3-2 always write 0 
		00 = AIN0
		01 = AIN1 (default)
		10 = AIN2 
		11 = AIN3 
	*/
	if (ADS1257_Channel> 3) return;
	// Bit0 = 1 and Bit1=1, AINN (AIN3=AGND) Fixed access AINCOM
	SPI_Interface_WriteReg(ADS1257_REG_MUX, (ADS1257_Channel << 4) | (1 << 1)|(1 << 0));	 
}
//========================================================================
void ADS1256_SetDiffChannal(uint8_t ADS1257_Channel)
{
	/*
	Bits 7-6 always write 0 
	Bits 5-4 
		00 = AIN0 (default)
		01 = AIN1
		10 = AIN2 
		11 = AIN3 (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)
	Bits 3-2 always write 0 
		00 = AIN0
		01 = AIN1 (default)
		10 = AIN2 
		11 = AIN3 
	*/
	if (ADS1257_Channel== 0)
	{
		SPI_Interface_WriteReg(ADS1257_REG_MUX, (0 << 4) | (0 << 1)|(1 << 0));	/* Acess AIN0, AIN1 */
	}
	else if (ADS1257_Channel == 1)
	{
		SPI_Interface_WriteReg(ADS1257_REG_MUX, (1 << 4) | (1 << 1)|(1 << 0));	/* Access AIN1, AIN3 */
	}
	else if (ADS1257_Channel == 1)
	{
		SPI_Interface_WriteReg(ADS1257_REG_MUX, (2 << 4) | (1 << 1)|(1 << 0));	/* Access AIN2, AIN3 */
	}
}
//========================================================================
static void ADS1256_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 40000000; i++)
	{
		if (DRDY_IS_LOW())
		{
			break;
		}
	}
	if (i >= 40000000)
	{
		DBG_SER1_PutString("Time Out\r\n");	
	}
}


//========================================================================
signed int  ADS1256_ReadData(uint16_t time)
{
	while(time)
	if(data_ready_flag==1)
		{
			ADS1257_CS_EN;
			ADC_High_Byte = SPI_Interface_Send_Byte(0x00);
			ADC_Medium_Byte = SPI_Interface_Send_Byte(0x00);
			ADC_Low_Byte = SPI_Interface_Send_Byte(0x00);
			data_ready_flag=0;
			ADS1257_CS_DI;			
			DBG_SER1_PutChar(0x55);
			DBG_SER1_PutChar(0xaa);
			DBG_SER1_PutChar(ADC_Low_Byte);
			DBG_SER1_PutChar(ADC_Medium_Byte);
			DBG_SER1_PutChar(ADC_High_Byte);
			time--;
		}
		return 0;
}


signed int ADS1256_ReadADC_Channel(unsigned char ch)
{
	int32_t read;
	ADS1256_WaitDRDY();		
	ADS1256_SetChannal(ch);		
	delay_us(8);
	ADS1257_WriteCmd(ADS1257_CMD_SYNC);
	delay_us(8);
	ADS1257_WriteCmd(ADS1257_CMD_WAKEUP);
	delay_us(8);
	read =  ADS1256_ReadData(5000);
	return read ;
}

void ADS1256_Configation_Channel(void)
{
	ADS1257_WriteCmd(ADS1257_CMD_SDATAC);				//SDATAC Command
	delay_ms(10);						//Delay
	SPI_Interface_WriteReg(ADS1257_REG_STATUS,0x02);// 0x02);			//WREG - STATUS - Enable buffer
	delay_ms(10);				//Delay
	SPI_Interface_WriteReg(ADS1257_REG_MUX, 0x01);			//WREG - MUX - CHANNELS AIN0-AIN1
	delay_ms(10);
	SPI_Interface_WriteReg(ADS1257_REG_ADCON,0x22);//VIN = ± 2 · VREF / Gain==>>(WREG - ADCON - default PGA = 4)
	delay_ms(10);							//Delay
	SPI_Interface_WriteReg(ADS1257_REG_DRATE, 0xA1);//0x92);			//WREG - DRATE - 100SPS
	delay_ms(10);
//	REGValue[0]=SPI_Interface_ReadReg(ADS1257_REG_STATUS);
//	REGValue[1]=SPI_Interface_ReadReg(ADS1257_REG_MUX);
//	REGValue[2]=SPI_Interface_ReadReg(ADS1257_REG_ADCON);
//	REGValue[3]=SPI_Interface_ReadReg(ADS1257_REG_DRATE);
//	for(i=0;i<4;i++)
//	{
//	 DBG_SER1_PutChar(REGValue[i]);
//		delay_ms(100);
//	}
	ADS1257_WriteCmd(ADS1257_CMD_RDATAC);	
	delay_ms(10);	

}

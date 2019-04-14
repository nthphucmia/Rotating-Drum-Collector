/*****************************************************************************
  *Ten Tep          :     i2c.c
  *Ngay             :     19/03/2014
  *Tac Gia          :     MinhHa R&D Team
  *Cong Ty          :     MinhHaGroup
  *Webside          :     mcu.banlinhkien.vn
  *Phien Ban        :     V1.0
  *Tom Tat          :     Dinh nghia cac ham giao tiep I2C
  ******************************************************************************
  * Chu Y           :     Phai dinh nghia cac chan giao tiep chuan I2C vao main.h
      
        #define I2CSOFT_SDADDR  DDRC_4
        #define I2CSOFT_SDAOUT  PORTC_4
        #define I2CSOFT_SDAIN   PINC_4
        #define I2CSOFT_SCLDDR  DDRD_0
        #define I2CSOFT_SCL     PORTD_0                     
  ******************************************************************************
**/

  
#include "i2c.h"
#include "GPIO_Configuration.h"
#include "stdint.h"
#include "DEF_STM32.h"

#if (__I2C_H!=19032014)
    #error "Include Sai File i2c.h"
#endif

/*******************************************************************************
Noi Dung    :   Khoi tao giao thuc I2C
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/


void I2CSOFT_Init(void)
{
	GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_Out_OD);
	GPIO_SetState(GPIOB,GPIO_Pin_11,GPIO_Mode_Out_OD);
    I2CSOFT_SCL=1;
    delay_us(5);
    I2CSOFT_SDAOUT=1;
    delay_us(5);
}

/*******************************************************************************
Noi Dung    :   Gui lenh Start I2C (Keo chan SDA xuong 0 khi SCL o muc 1).
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/

void I2CSOFT_Start(void)
{
    GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_Out_OD);
    I2CSOFT_SDAOUT=1; 
    I2CSOFT_SCL=1; 
    delay_us(5);
    I2CSOFT_SDAOUT=0; 
    delay_us(5);
    I2CSOFT_SCL=0;
    delay_us(5);
}

/*******************************************************************************
Noi Dung    :   Gui lenh Stop I2C (Keo chan SDA len muc 1 khi SCL o muc 1).
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/

void I2CSOFT_Stop(void)
{
    GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_Out_OD);
    I2CSOFT_SDAOUT=0;
    I2CSOFT_SCL=1;
    delay_us(5);
    I2CSOFT_SDAOUT=1;
    delay_us(5);
    I2CSOFT_SCL=0;
    delay_us(5);
}

/*******************************************************************************
Noi Dung    :   Viet du lieu len Bus I2C.
Tham Bien   :   Data: Gia tri du lieu.
Tra Ve      :   Bit Ack.
********************************************************************************/ 

uint8_t I2CSOFT_WriteData(uint8_t Data)
{
    uint8_t i;
    GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_Out_OD);
    for(i=0;i<8;i++)
    {
        if(Data&0x80)
            I2CSOFT_SDAOUT=1;
        else
            I2CSOFT_SDAOUT=0;
        Data<<=1;   
        delay_us(5);
        I2CSOFT_SCL=1;
        delay_us(5);
        I2CSOFT_SCL=0;
        delay_us(5); 
    }
    GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);
    I2CSOFT_SCL=1;
    delay_us(5);
    i=I2CSOFT_SDAIN;
    delay_us(5);
    I2CSOFT_SCL=0;
    delay_us(5);
    return i;
}

/*******************************************************************************
Noi Dung    :   Lay du lieu tren Bus I2C.
Tham Bien   :   Ack: gia tri Bit Ack phan hoi khi doc het du lieu,
Tra Ve      :   Gia tri du lieu.
********************************************************************************/

uint8_t I2CSOFT_ReadData(uint8_t Ack)
{
    uint8_t I2C_Data=0,i;
    GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);
    for(i=0;i<8;i++)
    {         
        I2CSOFT_SCL=1; 
        delay_us(10);         
        I2C_Data<<=1;
        if(I2CSOFT_SDAIN)I2C_Data|=1;
        delay_us(10);
        I2CSOFT_SCL=0;
    }
    GPIO_SetState(GPIOB,GPIO_Pin_10,GPIO_Mode_Out_OD);
    I2CSOFT_SCL=1;
    delay_us(5);
    I2CSOFT_SDAOUT=Ack;
    delay_us(5);
    I2CSOFT_SCL=0;
    return I2C_Data;
}

/*------------------------------KET THUC FILE-------------------------------
 ______________________________MinhHa R&D Team______________________________*/
void GPIO_SetState(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;	         		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

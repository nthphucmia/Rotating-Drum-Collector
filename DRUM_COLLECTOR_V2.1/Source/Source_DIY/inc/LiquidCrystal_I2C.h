#ifndef __LIDQUIDCRYSTAL_I2C_H
#define __LIDQUIDCRYSTAL_I2C_H
	 
#include "stm32f10x.h"

//#define PCF8574A_Address      	0x7E
#define PCF8574A_Address      	0x4E
#define I2C_Chanel      				I2C1
void I2C_LCD_Configuration(void);
extern void Delay_msST(uint16_t nTime);
void lcd_Write_byte(char data);
void lcd_init (void);
void lcd_Data_Write(char data);
void lcd_Control_Write(char data);
void lcd_send_string (char *str);
void Delete_LCD(void)            ;
extern void Delay_msST(uint16_t nTime);
void lcd_goto_XY (int row, int col);
#endif

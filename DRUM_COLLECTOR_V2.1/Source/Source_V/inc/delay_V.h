/**
  ******************************************************************************
  * @file    	SPI/ADS8513_ADC/spi_ads8513.h
  * @author  Tran Ngoc Viet
  * @version 	V1.0.0
  * @date    	04/28/2012
  * @brief   	Header file for main.c module.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 ARMVietNam</center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DELAY_V_H
#define __DELAY_V_H

#include "delay_V.h"


/***************************************************************
*/
/*----- High layer function -----*/
//__asm void wait(void);
void  delay_us(__IO uint32_t  num);
void  delay_ms(__IO uint32_t  num);
/*------------------------------*/
#endif

/******************* (C) COPYRIGHT 2009 ARMVietNam *****END OF FILE****/


#ifndef __24CXX_H
#define __24CXX_H    20032014

#include "i2c.h"
#include "at24cxx.h"
#include "stdint.h"
#include "inttypes.h"

#define EEP24CXX_ADDR_WRITE	0xA0
#define EEP24CXX_ADDR_READ	0xA1

/*******************************************************************************
Noi Dung    :   Kiem tra 24CXX da san sang giao tiep hay chua.
Tham Bien   :   Khong.
Tra Ve      :   Bit Ack=0: 24CXX da san sang giao tiep.
                    Ack=1: 24CXX chua san sang gia tiep.
********************************************************************************/

uint8_t EEP24CXX_IsReady(void);

/*******************************************************************************
Noi Dung    :   Ghi du lieu vao 24CXX.
Tham Bien   :   Address : Dia chi vung nho can ghi du lieu.
                Data    : Du lieu ghi vao.
Tra Ve      :   Khong.
********************************************************************************/

void EEP24CXX_WriteData(uint16_t Address, uint8_t Data);

/*******************************************************************************
Noi Dung    :   Doc du lieu tu 1 vung nho cua 24CXX ra ngoai.
Tham Bien   :   Address : Dia chi vung nho can doc du lieu.
Tra Ve      :   Du lieu doc duoc.
********************************************************************************/

uint8_t EEP24CXX_ReadData(uint16_t Address);

/*******************************************************************************
Noi Dung    :   Ghi lien tiep du lieu vao 24CXX.
Tham Bien   :   Address    : Dia chi vung nho can ghi du lieu.
                Quantity   : So luong byte du lieu can ghi vao.
                *Buff_Data : bo dem luu du lieu ghi vao.
Tra Ve      :   Khong.
********************************************************************************/

void EEP24CXX_WriteMultiData(uint16_t Address, uint16_t Quantity, uint8_t *Buff_Data);

/*******************************************************************************
Noi Dung    :   Doc du lieu tu nhieu thanh ghi lien tiep cua 24CXX ra ngoai.
Tham Bien   :   Address    : Dia chi vung nho can doc du lieu.
                Quantity   : So luong byte vung nho can doc.
                *Buff_Data : Bo dem luu du lieu doc duoc.
Tra Ve      :   Khong.
********************************************************************************/

void EEP24CXX_ReadMultiData(uint16_t Address, uint16_t Quantity, uint8_t *Buff_Data);

void EEPROM_Write (int value);

uint16_t EEPROM_Read (void);
#endif
/*------------------------------KET THUC FILE-------------------------------
 ______________________________MinhHa R&D Team______________________________*/
 
 
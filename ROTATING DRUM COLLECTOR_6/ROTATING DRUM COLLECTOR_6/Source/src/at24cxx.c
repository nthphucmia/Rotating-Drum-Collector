/*****************************************************************************
  *Ten Tep          :     24cxx.c
  *Ngay             :     20/03/2014
  *Tac Gia          :     MinhHa R&D Team
  *Cong Ty          :     MinhHaGroup
  *Webside          :     mcu.banlinhkien.vn
  *Phien Ban        :     V1.0
  *Tom Tat          :     Dinh nghia cac ham giao tiep IC EEPROM 24Cxx
  ******************************************************************************
  * Dia chi vung nho cua cac dong 24CXX:
    24C02 : 0 - 255
    24C04 : 0 - 511  : Chia thanh 2 phan, moi phan co 256 byte
    24C08 : 0 - 1023 : Chia thanh 4 phan, moi phan co 256 byte
    24C16 : 0 - 2047 : Chia thanh 8 phan, moi phan co 256 byte
  ******************************************************************************
 **/

#include "at24cxx.h"

#if (__24CXX_H!=20032014)
    #error "Include Sai #File 24Cxx.h"
#endif

/*******************************************************************************
Noi Dung    :   Kiem tra 24CXX da san sang giao tiep hay chua.
Tham Bien   :   Khong.
Tra Ve      :   Bit Ack=0: 24CXX da san sang giao tiep.
                    Ack=1: 24CXX chua san sang gia tiep.
********************************************************************************/
extern uint16_t inputSpeed;
unsigned char High_byte, Low_byte;

uint8_t EEP24CXX_IsReady(void)
{
	uint8_t Ack;
	I2CSOFT_Start();
	Ack = I2CSOFT_WriteData((uint8_t)(EEP24CXX_ADDR_WRITE));
	I2CSOFT_Stop();
	return Ack;
}

/*******************************************************************************
Noi Dung    :   Ghi du lieu vao 24CXX.
Tham Bien   :   Address : Dia chi vung nho can ghi du lieu.
                Data    : Du lieu ghi vao.
Tra Ve      :   Khong.
********************************************************************************/

void EEP24CXX_WriteData(uint16_t Address, uint8_t Data)
{
	while(EEP24CXX_IsReady());
	I2CSOFT_Start();
	I2CSOFT_WriteData((EEP24CXX_ADDR_WRITE|(uint8_t)(Address>>7))&0xFE);
  I2CSOFT_WriteData(Address);
	I2CSOFT_WriteData(Data);
	I2CSOFT_Stop();
}

/*******************************************************************************
Noi Dung    :   Doc du lieu tu 1 vung nho cua 24CXX ra ngoai.
Tham Bien   :   Address : Dia chi vung nho can doc du lieu.
Tra Ve      :   Du lieu doc duoc.
********************************************************************************/

uint8_t EEP24CXX_ReadData(uint16_t Address)
{
	uint8_t Data;
	while(EEP24CXX_IsReady());
	I2CSOFT_Start();
	I2CSOFT_WriteData((EEP24CXX_ADDR_WRITE|(uint8_t)(Address>>7))&0xFE);
    I2CSOFT_WriteData(Address);
	I2CSOFT_Start();
	I2CSOFT_WriteData(EEP24CXX_ADDR_READ|(uint8_t)(Address>>7));
	Data=I2CSOFT_ReadData(0);
	I2CSOFT_Stop();
	return Data;
}

/*******************************************************************************
Noi Dung    :   Ghi lien tiep du lieu vao 24CXX.
Tham Bien   :   Address    : Dia chi vung nho can ghi du lieu.
                Quantity   : So luong byte du lieu can ghi vao.
                *Buff_Data : bo dem luu du lieu ghi vao.
Tra Ve      :   Khong.
********************************************************************************/

void EEP24CXX_WriteMultiData(uint16_t Address, uint16_t Quantity, uint8_t *Buff_Data)
{
    while(Quantity)
    {
        EEP24CXX_WriteData(Address, *Buff_Data);
        Buff_Data++;
        Address++;
        Quantity--;
    }
}

/*******************************************************************************
Noi Dung    :   Doc du lieu tu nhieu thanh ghi lien tiep cua 24CXX ra ngoai.
Tham Bien   :   Address    : Dia chi vung nho can doc du lieu.
                Quantity   : So luong byte vung nho can doc.
                *Buff_Data : Bo dem luu du lieu doc duoc.
Tra Ve      :   Khong.
********************************************************************************/

void EEP24CXX_ReadMultiData(uint16_t Address, uint16_t Quantity, uint8_t *Buff_Data)
{
    uint8_t i;
    for(i=0;i<Quantity;i++)
    {
        Buff_Data[i]=EEP24CXX_ReadData(Address);
        Address++;
    }
}

void EEPROM_Write (int value)
{
   High_byte=(inputSpeed&0xff00)>>8; 
   Low_byte=(inputSpeed&0x00ff);	
	 EEP24CXX_WriteData(0x00, High_byte);
	 EEP24CXX_WriteData(0x01, Low_byte);
}

uint16_t EEPROM_Read (void)
	{
		int bytehigh=EEP24CXX_ReadData(0);
		int bytelow=EEP24CXX_ReadData(1);
    uint16_t value=bytehigh*256+bytelow;
		return value;
	}
/*------------------------------KET THUC FILE-------------------------------
 ______________________________MinhHa R&D Team______________________________*/

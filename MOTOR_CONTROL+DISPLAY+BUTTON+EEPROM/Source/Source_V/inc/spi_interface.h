#ifndef __spi_ads1257_H
#define __spi_ads1257_H


/****************************************************************/
/* Define commands for the dc_control() function                */
/* The names of the commands are derived from the datasheet     */
/****************************************************************/
#define ADS1257_CMD_WAKEUP        (0x00)       			// Completes SYNC and Exits Standby Mode 0000  0000 (00h)
#define ADS1257_CMD_RDATA         (0x01)						// Read Data 0000  0001 (01h)
#define ADS1257_CMD_RDATAC        (0x03)						// Read Data Continuously 0000   0011 (03h)
#define ADS1257_CMD_SDATAC        (0x0F)						// Stop Read Data Continuously 0000   1111 (0Fh)
#define ADS1257_CMD_RREG          (0x10)						// Read from REG rrr 0001 rrrr (1xh)
#define ADS1257_CMD_WREG          (0x50)						// Write to REG rrr 0101 rrrr (5xh)
#define ADS1257_CMD_SELFCAL       (0xF0)            // Offset and Gain Self-Calibration 1111    0000 (F0h)
#define ADS1257_CMD_SELFOCAL      (0xF1)            // Offset Self-Calibration 1111    0001 (F1h)
#define ADS1257_CMD_SELFGCAL      (0xF2)						// Gain Self-Calibration 1111    0010 (F2h)
#define ADS1257_CMD_SYSOCAL       (0xF3)						// System Offset Calibration 1111   0011 (F3h)
#define ADS1257_CMD_SYSGCAL       (0xF4)						// System Gain Calibration 1111    0100 (F4h)
#define ADS1257_CMD_SYNC          (0xFC)						// Synchronize the A/D Conversion 1111   1100 (FCh)
#define ADS1257_CMD_STANDBY       (0xFD)						// Begin Standby Mode 1111   1101 (FDh)
#define ADS1257_CMD_RESET         (0xFE) 						// Reset to Power-Up Values 1111   1110 (FEh)

/****************************************************************/
/* Define names for the register addresses used in the RREG and */
/* WREG commands                                                */
/****************************************************************/
#define ADS1257_REG_STATUS        (0x00)
#define ADS1257_REG_MUX           (0x01)
#define ADS1257_REG_ADCON         (0x02)
#define ADS1257_REG_DRATE         (0x03)
#define ADS1257_REG_IO            (0x04)
#define ADS1257_REG_OFC0          (0x05)
#define ADS1257_REG_OFC1          (0x06)
#define ADS1257_REG_OFC2          (0x07)
#define ADS1257_REG_FSC0          (0x08)
#define ADS1257_REG_FSC1          (0x09)
#define ADS1257_REG_FSC2          (0x0A)

/****************************************************************/
/* ADS1256 defines for control registers bit set/clear          */
/****************************************************************/
/* register STATUS                                              */
#define ADS1256_BUFEN_ENABLE      (0x02)
#define ADS1256_BUFEN_DISABLE     (0x00)
#define ADS1256_ACAL_ENABLE       (0x04)
#define ADS1256_ACAL_DISABLE      (0x00)
#define ADS1256_ORDER_MSB         (0x08)
#define ADS1256_ORDER_LSB         (0x00)

/* register MUX                                                 */
#define ADS1256_NSEL_0            (0x00)
#define ADS1256_NSEL_1            (0x01)
#define ADS1256_NSEL_2            (0x02)
#define ADS1256_NSEL_3            (0x03)
#define ADS1256_NSEL_4            (0x04)
#define ADS1256_NSEL_5            (0x05)
#define ADS1256_NSEL_6            (0x06)
#define ADS1256_NSEL_7            (0x07)
#define ADS1256_NSEL_COMM         (0x08)



/* register ADCON                                               */
#define ADS1256_PGA_1             (0x00)
#define ADS1256_PGA_2             (0x01)
#define ADS1256_PGA_4             (0x02)
#define ADS1256_PGA_8             (0x03)
#define ADS1256_PGA_16            (0x04)
#define ADS1256_PGA_32            (0x05)
#define ADS1256_PGA_64            (0x06)


/* register DRATE                                               */
#define ADS1256_DRATE_2HALF       (0x03)
#define ADS1256_DRATE_5           (0x13)
#define ADS1256_DRATE_10          (0x23)
#define ADS1256_DRATE_15          (0x33)
#define ADS1256_DRATE_25          (0x43)
#define ADS1256_DRATE_30          (0x53)
#define ADS1256_DRATE_50          (0x63)
#define ADS1256_DRATE_60          (0x72)
#define ADS1256_DRATE_100         (0x82)
#define ADS1256_DRATE_500         (0x92)
#define ADS1256_DRATE_1000        (0xA1)
#define ADS1256_DRATE_2000        (0xB0)
#define ADS1256_DRATE_3750        (0xC0)
#define ADS1256_DRATE_7500        (0xD0)
#define ADS1256_DRATE_15000       (0xE0)
#define ADS1256_DRATE_30000       (0xF0)

/* other useful definitions                                     */
#define ADS1256_MAX_BUFFERS       (0x02)



static void busywait(int c);

void SPI_Interface_Configuration(void);
void ADS1257_Power_Up(void);
void SPI_Interface_Init(void);
unsigned char SPI_Interface_Send_Byte(unsigned char dat);
void SPI_Interface_WriteReg(unsigned char addr, unsigned char value);
void ADS1257_WriteCmd(unsigned char cmd);
unsigned char SPI_Interface_ReadReg(unsigned char addr);
unsigned char ADS1256_ReadChipID(void);
void ADS1256_SetChannal(unsigned char ADS1257_Channel);
//signed int ADS1256_ReadData(void);
signed int  ADS1256_ReadData(uint16_t time);
static void ADS1256_WaitDRDY(void);
signed int ADS1256_ReadADC_Channel(unsigned char ch);
void ADS1256_Configation_Channel(void);
#endif


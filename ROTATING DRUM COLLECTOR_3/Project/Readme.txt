+ Comprehensive code
+ Press 1: DOWN
+ Press 2: UP
+ Press 3: SET ( After running to the EEPROM, press SET to update the new set speed)
+ Press 4: Run directly to EEPROM
------------------------------------------* BUTTON *-------------------------------------------
													PA4PA5PB8PB9
PA4: UP_BUTTON 				(BUTTON1)	REAL			2 1 4 3
PA5: DOWN_BUTTON			(BUTTON2)					| | | |
PB8: SET_BUTTON (SAVE)      (BUTTON3)					| | | |
PB9: RUN FROM EEPROM		(BUTTON4)					| | | |
PB15: RUN SERVO MOTOR					ON KEYPAD		1 2 3 4
PB14: STOP SERVO MOTOR

-----------------------------------------* SERVO MOTOR *-----------------------------------
PA2: PWM1
BRA1: PC1
DIR1: PC0
PC4	: THR1
PC5: CUR1
PA6: chân encoder1
PA7: chân encoder2

------------------------------------------* LCD - I2C1 *-------------------------------------------
PB6: SCL_LCD
PB7: SDA_LCD

------------------------------------------* EEPROM - I2C2 *-------------------------------------------
PB10:
PB11:

------------------------------------------* UART *-------------------------------------------
PA9: TXD_UART1
PA10: RXD_UART1



#include "delay.h"

uint32_t Time_ST=0;							// bien delay bang system tick timer	

void  delay_ms(__IO uint32_t  num)		   // resolution la 1ms
{
	__IO uint32_t index = 0;

	/* default system clock is 72MHz */
	for(index = (10300 * num); index != 0; index--)	//delay_ms(100)-4.99Hz 
	{
	}
}


void  delay_us(__IO uint32_t  num)		   // resolution la 1us
{
	__IO uint32_t index = 0;

	/* default system clock is 72MHz */
	for(index = (11 * num); index != 0; index--)	//delay_us(100)-4.97Khz
	{
	}
}

void Delay_msST(uint16_t nTime)																			// ham delay tuyet doi
{
		Time_ST = nTime;
	while(Time_ST !=0)
			{
			}
}
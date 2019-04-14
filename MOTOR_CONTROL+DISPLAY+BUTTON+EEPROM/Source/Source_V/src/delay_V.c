#include "delay.h"

//__asm void wait()
//{
//    nop
//	  nop
//	  nop
//	
//    BX lr
//}

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

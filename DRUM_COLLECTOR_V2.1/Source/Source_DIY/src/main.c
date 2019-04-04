#include "stm32f10x.h"
 
GPIO_InitTypeDef  GPIO_InitStructure;
 
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
 
int main()
{
 GPIO_Configuration();
 while(1)        
 {
  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0))
  {
    GPIO_SetBits(GPIOB ,GPIO_Pin_0 | GPIO_Pin_1 );
  }
  else
  {
   GPIO_ResetBits(GPIOB ,GPIO_Pin_0 | GPIO_Pin_1 );
  }
 }
}
void GPIO_Configuration(void)
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
 
 /* Configure PB0 PB1 in output pushpull mode */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 /* Configure PA0 in input mode */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOA, &GPIO_InitStructure);                           
}
void Delay(__IO uint32_t nCount)
{
 while(nCount--)
 {
 }
}
  
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
 { 
 /* User can add his own implementation to report the file name and line number,
 ex: printf("Wrong parameters value: file %s on line %drn", file, line) */
 
 /* Infinite loop */
 while (1)
 {
 }
}
#endif
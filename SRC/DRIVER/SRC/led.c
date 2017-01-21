/**
  ************************************
  * @file    led.c
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.08.02
  * @brief   Led灯驱动(PB8)
	************************************
*/

#include "led.h"

/**
  ************************************
  * @brief  Led灯IO口的初始化
  * @param  None
  * @retval None
	************************************
*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;//定义结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);//GPIOB外部时钟使能，特别重要！！！
//	//用PB4时要禁用JTAG，禁用要先开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_15;//设置IO口
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//设置为普通推挽输出
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure1);//初始化
	
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_11;//设置IO口
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//设置为普通推挽输出
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//初始化
  
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;//设置IO口
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//设置为普通推挽输出
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure1);//初始化
}

/**
  **********************************
  * @brief  Led灯的开关
  * @param  status: 0 or 1
  * @retval None
  **********************************
*/
void LED_Open(uint8_t num, uint8_t status)
{
	if(status)
  {
    switch (num)
    {
      case 0: GPIO_SetBits(GPIOB, GPIO_Pin_3);break;//置1
      case 1: GPIO_SetBits(GPIOC, GPIO_Pin_10);break;//置1
      case 2: GPIO_SetBits(GPIOC, GPIO_Pin_11);break;//置1
      case 3: GPIO_SetBits(GPIOC, GPIO_Pin_12);break;//置1
    }
  }
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);//置1
	else
  {
    switch (num)
    {
      case 0: GPIO_ResetBits(GPIOB, GPIO_Pin_3);break;//置0
      case 1: GPIO_ResetBits(GPIOC, GPIO_Pin_10);break;//置0
      case 2: GPIO_ResetBits(GPIOC, GPIO_Pin_11);break;//置0
      case 3: GPIO_ResetBits(GPIOC, GPIO_Pin_12);break;//置0
    }
  }
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);//清0
}


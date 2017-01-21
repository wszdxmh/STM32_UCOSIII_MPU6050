
#ifndef __INFRARED_H_
#define __INFRARED_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "sys.h"

#define LIG_HIGHT()		TIM_Cmd(TIM3, ENABLE)
#define LIG_LOW()			TIM_Cmd(TIM3, DISABLE)

#define HIGHT()				{LIG_HIGHT();Delay_us(600);LIG_LOW();Delay_us(1600);}
#define LOW()					{LIG_HIGHT();Delay_us(600);LIG_LOW();Delay_us(600);}

#define HIGHT_TCL()		{LIG_HIGHT();Delay_us(500);LIG_LOW();Delay_us(1000);}
#define LOW_TCL()			{LIG_HIGHT();Delay_us(500);LIG_LOW();Delay_us(245);}

#define LEFT()				{if(ircode & 0x80)HIGHT_TCL()else LOW_TCL()ircode <<= 1;}

void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void Delay_us_TIM1(uint16_t us);
void Remote_Init(uint16_t arr, uint16_t psc);
void Test_Remoted_Zero(void);
void Remote_Send(void);

#endif

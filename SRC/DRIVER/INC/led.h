/**
  ************************************
  * @file    led.h
  * @author  wszdxmh
  * @version V4.5.0
  * @date    2015.08.02
  * @brief   PB8
	************************************
*/

#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void LED_Init(void);
void LED_Open(uint8_t num, uint8_t status);

#endif

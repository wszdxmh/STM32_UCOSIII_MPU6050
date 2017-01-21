/**
  ************************************
  * @file    spi.h
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.08.09
  * @brief   SPI2:  SCL(PB13) SDA|MOSI(PB15) MISO(PB14) RST(PD9) DC(PD8)
	************************************
*/

#ifndef __SPI_H
#define __SPI_H

//OLEDÒý½Å¶¨Òå
#define SCL_SPI2  PBout(13)
#define SDA_SPI2  PBout(15)
#define RST_SPI2  PDout(9)
#define DC_SPI2   PBout(8)

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "delay.h"
#include "sys.h"


uint8_t SPI2_ReadWriteByte(uint8_t TxData);
uint8_t LCD_SPI2_WrCmd(uint8_t cmd);
uint8_t LCD_SPI2_WrData(uint8_t data);
void LCD_SPI2_Set_Pos(uint8_t x, uint8_t y);
void LCD_SPI2_Fill(uint8_t bmp_dat);
void SPI2_Init(void);
void LCD_SPI2_Init(void);
void LCD_SPI2_P6x8Num(uint8_t x, uint8_t y, uint16_t data);
void LCD_SPI2_P8x16Str(uint8_t x,uint8_t y,char ch[]);

#endif

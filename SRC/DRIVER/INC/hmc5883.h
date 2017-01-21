
#ifndef __HMC5883_H_
#define __HMC5883_H_

#include "stm32f10x.h"

#define HMC5883_CON1     0X00
#define HMC5883_CON2     0X01
#define HMC5883_MOD      0X02
#define HMC5883_DOXM     0X03
#define HMC5883_DOXL     0X04
#define HMC5883_DOZM     0x05
#define HMC5883_DOZL     0X06
#define HMC5883_DOYM     0X07
#define HMC5883_DOYL     0X08
#define HMC5883_STA      0X09
#define HMC5883_IDEA     0X0A
#define HMC5883_IDEB     0X0B
#define HMC5883_IDEC     0X0C
#define HMC5883_SLAVE_ADDRESS  0x3C      //MPU6050Æ÷¼þ¶ÁµØÖ·

void HMC5883_WriteReg(u8 reg_add,u8 reg_dat);
void HMC5883_ReadData(u8 reg_add,unsigned char*Read,u8 num);
void HMC5883_Init(void);
void HMC5883ReadID(void);
void HMC5883ReadMag(short *MagData);

#endif

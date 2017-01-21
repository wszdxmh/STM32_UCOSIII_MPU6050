/**
  ************************************
  * @file    PC_Link.h
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.09.27
  * @brief   机械手串口调试函数
	************************************
*/
#ifndef __PC_LINK_H
#define __PC_LINK_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "usart.h"
#include "tim.h"
#include "sys.h"
#include "manipulator.h"

//接收数组处理宏
#define HEADER_JUDGE(header)	(buf[0] == header && buf[0] == (uint8_t)(~buf[3]))
#define UINT8_T_TO_INT(x)	((x) = (buf[1] << 8) | buf[2])
#define JUDGE(header,h_data,d_data) ((buf[0] == header && buf[1] == h_data && buf[2] == d_data && buf[3] == (~buf[0])))

void Send_PC(uint8_t header, uint16_t date);
void USART1_IRQHandler(void);
void usart1_niming_report(u8 fun, u8*data, u8 len);
void mpu6050_send_data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz);
void usart1_report_imu(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz, short roll, short pitch, short yaw);

#endif

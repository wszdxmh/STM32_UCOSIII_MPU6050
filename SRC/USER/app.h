
#ifndef __APP_H
#define __APP_H

#include "includes.h"
#include "led.h"
#include "delay.h"
#include "PC_Link.h"
#include "spi.h"
#include "sys.h"
#include "tim.h"
#include "usart.h"
#include "motor.h"
#include "manipulator.h"
#include "stm32f10x_it.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "Kalman_filter.h"
#include "ANO-Tech.h"
#include "includes.h"
#include "hmc5883.h"
#include "infrared.h"
#include "pid.h"

extern float pitch,roll,yaw; 		//欧拉角
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据

#define LED PCout(13)
#define LIG PAout(15)

/*开始任务*/
//任务优先级
#define START_TASK_PRIO	3
//任务堆栈大小
#define START_STK_SIZE	128
//任务控制块
extern OS_TCB	StartTackTCB;
//任务堆栈
extern CPU_STK	START_TASK_STK[START_STK_SIZE];
//开始任务控制函数
void start_task(volatile void *p_arg);

/*电机1*/
//任务MOTOR优先级
#define MOTOR1_TASK_PRIO	4
//任务MOTOR堆栈大小
#define MOTOR1_STK_SIZE	128
//任务MOTOR控制块
extern OS_TCB MOTOR1TCB;
//任务MOTOR堆栈
extern CPU_STK	MOTOR1_STK[MOTOR1_STK_SIZE];
//任务控制函数
void Motor1_task(volatile void *p_arg);

/*电机2*/
//任务MOTOR优先级
#define MOTOR2_TASK_PRIO	5
//任务MOTOR堆栈大小
#define MOTOR2_STK_SIZE	128
//任务MOTOR控制块
extern OS_TCB MOTOR2TCB;
//任务MOTOR堆栈
extern CPU_STK	MOTOR2_STK[MOTOR2_STK_SIZE];
//任务控制函数
void Motor2_task(volatile void *p_arg);

/*采集任务*/
//任务COLLECTION优先级
#define COLLECTION_TASK_PRIO	8
//任务COLLECTION堆栈大小
#define COLLECTION_STK_SIZE	128
//任务COLLECTION控制块
extern OS_TCB COLLECTIONTCB;
//任务COLLECTION堆栈
extern CPU_STK	COLLECTION_STK[COLLECTION_STK_SIZE];
//任务控制函数
void Collection_task(volatile void *p_arg);

/*滤波任务*/
//任务FILTER优先级
#define FILTER_TASK_PRIO	7
//任务FILTER堆栈大小
#define FILTER_STK_SIZE	128
//任务FILTER控制块
extern OS_TCB FILTERTCB;
//任务FILTER堆栈
extern CPU_STK	FILTER_STK[FILTER_STK_SIZE];
//任务控制函数
void filter_task(volatile void* p_arg);

/*串口显示任务*/
//任务DISPLAY优先级
#define DISPLAY_TASK_PRIO	6
//任务DISPLAY堆栈大小
#define DISPLAY_STK_SIZE	128
//任务DISPLAY控制块
extern OS_TCB DISPLAYTCB;
//任务DISPLAY堆栈
extern CPU_STK	DISPLAY_STK[DISPLAY_STK_SIZE];
//任务DISPLAY控制函数
void Display_task(volatile void *p_arg);

/*定时器1回调函数*/
extern OS_TMR tmr1;
void tmr1_callback_led(void *p_tmr, void *p_arg);

#endif

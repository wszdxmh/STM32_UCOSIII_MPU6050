
#ifndef __MANIPULATOR_H_
#define __MANIPULATOR_H_

#include "stm32f10x.h"
#include "led.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "stdio.h"
#include "PC_Link.h"
#include "math.h"
#include "string.h"

//延时宏定义
#define MANIPULATOR_DELAY_TIME  5
#define TURN_DELAY_TIME  5

//????PWM????
#define WAIST(x) TIM_SetCompare3(TIM3,x)//PB0
#define BIG_ARM(x)	TIM_SetCompare1(TIM3,x)//PB4
#define LITTLE_ARM(x)	TIM_SetCompare1(TIM4,x)//PB6
#define WRIST(x)	TIM_SetCompare2(TIM4,x)//PB7
#define WRIST_TURN(x)	TIM_SetCompare3(TIM4,x)//PB8
#define GRIPPER(x)	TIM_SetCompare4(TIM4,x)//PB9

//???????
#define INIT_Waist   910
#define INIT_Big_Arm  980
#define INIT_Little_Arm   1420
#define INIT_Wrist   700
#define INIT_Wrist_Turn 2000
#define INIT_Gripper   1400

//???????????
#define WRITE_LINE_INIT_Waist   700
#define WRITE_LINE_INIT_Big_Arm  980
#define WRITE_LINE_INIT_Little_Arm   1070
#define WRITE_LINE_INIT_Wrist   770
#define WRITE_LINE_INIT_Wrist_Turn 2000
#define WRITE_LINE_INIT_Gripper   2100

//?????
#define HIGHT_Waist   910
#define HIGHT_Big_Arm  980
#define HIGHT_Little_Arm   1420
#define HIGHT_Wrist   700
#define HIGHT_Wrist_Turn 2000
#define HIGHT_Gripper   1400

//��??1
#define Location_One_Waist   910
#define Location_One_Big_Arm  500
#define Location_One_Little_Arm   860
#define Location_One_Wrist   700
#define Location_One_Wrist_Turn 2000
#define Location_One_Gripper   1400

//��??2
#define Location_Two_Waist   1400
#define Location_Two_Big_Arm  500
#define Location_Two_Little_Arm   860
#define Location_Two_Wrist   700
#define Location_Two_Wrist_Turn 2000
#define Location_Two_Gripper   1400

//��??3
#define Location_Thr_Waist   1710
#define Location_Thr_Big_Arm  500
#define Location_Thr_Little_Arm   860
#define Location_Thr_Wrist   700
#define Location_Thr_Wrist_Turn 2000
#define Location_Thr_Gripper   1400

//��??4
#define Location_Fho_Waist   1960
#define Location_Fho_Big_Arm  500
#define Location_Fho_Little_Arm   860
#define Location_Fho_Wrist   700
#define Location_Fho_Wrist_Turn 2000
#define Location_Fho_Gripper   1400

extern struct MANIPULATOR_VALUE{
	int16_t Waist;
	int16_t Big_Arm;
	int16_t Little_Arm;
	int16_t Wrist;
	int16_t Wrist_Turn;
	int16_t Gripper;
}Manipulator_Value,Action_table;

//????????
void Test_Manipulator_Upper_Monitor(void);
void Catch_On(int16_t variable, int16_t *pVariable);
void Manipulator_Init(void);
void Manipulator_ALL_Positive(struct MANIPULATOR_VALUE *manipulator);
void Manipulator_ALL_Reverse(struct MANIPULATOR_VALUE *manipulator);

//???��??
void Manipulator_HIGHT_Start(char a[]);
void Manipulator_Init_Start(char a[], char b[]);
void Manipulator_Location_One_Start(void);
void Manipulator_Location_Two_Start(void);
void Manipulator_Location_Thr_Start(void);
void Manipulator_Location_Fho_Start(void);

//???????
void Hight_To_Location_To_Init(uint8_t num);
void Write_Line_Move(void);

#endif

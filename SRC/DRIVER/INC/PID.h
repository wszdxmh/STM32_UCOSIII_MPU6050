
#ifndef __PID_H_
#define __PID_H_

#include "Kalman_filter.h"
#include "motor.h"

#define DEAD_VOLTAGE	80u

struct PID
{
	float SetSpeed;			//设定值
	float ActualSpeed;	//实际值
	float err;					//偏差值
	float err_last;			//上上个偏差值
	float pwm;					//控制值
	float integral;			//定义积分值
	float Kp,Ki,Kd,Kpp;	//PID参数
};

struct D_PID
{
	float Last_Gro_y;			//上一次陀螺仪值
	float Gro_y;					//本次陀螺仪值
	float Errory_Out;			//外环误差
	float Out_Pi_Y;				//外环PI
	float Out_Yp;					//外环P
	float Out_Yi;					//外环I
	float Errory_In;			//内环误差
	float In_Yp;					//内环P
	float In_Yi;					//内环I
	float In_Yd;					//内环D
	float In_PID_Y;				//内环PID
};

extern struct PID pid;
extern struct D_PID d_pid;

#define Integral_Limiting(variable,amplitude)	do{\
																	if(variable > amplitude)\
																		variable = amplitude;\
																	else if(variable < -amplitude)\
																		variable = -amplitude;\
																	}while(0);

void PID_init(void);
void D_PID_Init(void);
void PWM_Control(int16_t Speed);
void PID_Colu(void);
void double_PID(void);

#endif

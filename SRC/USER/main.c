
#include "app.h"

//定义变量
float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据

#define TEST PBout(11)

int main(void)
{
	OS_ERR err;
	//硬件初始化
	SystemInit();
	delay_init();
	LED_Init();
	Usart_Init(500000);
	NVIC_Configuration();
	Motor_Init(999,7);//3KHz(23)
	D_PID_Init();
//	Remote_Init(1894,0);//38KHz
//	TIM1_Init(5, 5);
//	HMC5883_Init();
	while(MPU6050_Init());
	while(mpu_dmp_init());
	CPU_SR_ALLOC();
	OSInit(&err);
	OS_CRITICAL_ENTER();//进入临界区代码保护
	/*创建任务*/
	OSTaskCreate ((OS_TCB        *)&StartTackTCB,
								(CPU_CHAR      *)"start_task",
								(OS_TASK_PTR    )start_task,
								(void          *)0,
								(OS_PRIO        )START_TASK_PRIO,
								(CPU_STK       *)START_TASK_STK,
								(CPU_STK_SIZE   )START_STK_SIZE/10,
								(CPU_STK_SIZE   )START_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	OS_CRITICAL_EXIT();//退出临界区代码保护
	OSStart(&err);
	OSTaskDel((OS_TCB*)&StartTackTCB, (OS_ERR*)&err);
	while(1);
}


#include "app.h"

//开始任务控制块
OS_TCB	StartTackTCB;
//开始任务堆栈
CPU_STK	START_TASK_STK[START_STK_SIZE];

//任务MOTOR控制块
OS_TCB MOTOR1TCB;
//任务MOTOR堆栈
CPU_STK	MOTOR1_STK[MOTOR1_STK_SIZE];

//任务MOTOR控制块
OS_TCB MOTOR2TCB;
//任务MOTOR堆栈
CPU_STK	MOTOR2_STK[MOTOR2_STK_SIZE];

//任务COLLECTION控制块
OS_TCB COLLECTIONTCB;
//任务COLLECTION堆栈
CPU_STK	COLLECTION_STK[COLLECTION_STK_SIZE];

//任务FILTER控制块
OS_TCB FILTERTCB;
//任务FILTER堆栈
CPU_STK	FILTER_STK[FILTER_STK_SIZE];

//任务DISPLAY控制块
OS_TCB DISPLAYTCB;
//任务DISPLAY堆栈
CPU_STK	DISPLAY_STK[DISPLAY_STK_SIZE];

/*定时器1回调函数*/
OS_TMR tmr1;

//任务MOTOR1控制函数
void Motor1_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
//		OSSchedLock(&err);
//		Remote_Send();
//		OSSchedUnlock(&err);
//		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		MOTOR1_FORWARD(200);
		OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
		MOTOR1_REVERSE(200);
		OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

//任务MOTOR2控制函数
void Motor2_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		MOTOR2_FORWARD(100);
		OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
		MOTOR2_REVERSE(100);
		OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

//任务Collection控制函数
void Collection_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSSchedLock(&err);
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		}
		OSSchedUnlock(&err);
		OSTaskSemPost(&FILTERTCB, OS_OPT_POST_NONE, &err);//发送内嵌信号量
	}
}

//任务FILTER控制函数
void filter_task(volatile void* p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, NULL, &err);//等待内嵌信号量
		OSSchedLock(&err);
		Angle_Calcu();
		OSSchedUnlock(&err);
		OSTaskSemPost(&DISPLAYTCB, OS_OPT_POST_NONE, &err);//发送内嵌信号量
	}
}

//任务DISPLAY控制函数
void Display_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, NULL, &err);//等待内嵌信号量
		OSSchedLock(&err);
		double_PID();
		ANO_DT_Send_Senser(Angle_x_temp,Angle_X_Final,(Angle_y_temp),(Angle_Y_Final),(Gyro_y),(d_pid.In_PID_Y));
		OSSchedUnlock(&err);
//		OSTaskSemPost(&MOTOR2TCB, OS_OPT_POST_NONE, &err);//发送内嵌信号量
	}
}

//定时器1回调函数
void tmr1_callback_led(void *p_tmr, void *p_arg)
{
	LED = ~LED;
//	LIG = ~LIG;
}

//开始任务控制函数
void start_task(volatile void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

	OS_CRITICAL_ENTER();//进入临界区代码保护
	/*创建任务FILTER*/
	OSTaskCreate ((OS_TCB        *)&FILTERTCB,
								(CPU_CHAR      *)"FILTER",
								(OS_TASK_PTR    )filter_task,
								(void          *)0,
								(OS_PRIO        )FILTER_TASK_PRIO,
								(CPU_STK       *)FILTER_STK,
								(CPU_STK_SIZE   )FILTER_STK_SIZE/10,
								(CPU_STK_SIZE   )FILTER_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务DISPLAY*/
	OSTaskCreate ((OS_TCB        *)&DISPLAYTCB,
								(CPU_CHAR      *)"DISPLAY",
								(OS_TASK_PTR    )Display_task,
								(void          *)0,
								(OS_PRIO        )DISPLAY_TASK_PRIO,
								(CPU_STK       *)DISPLAY_STK,
								(CPU_STK_SIZE   )DISPLAY_STK_SIZE/10,
								(CPU_STK_SIZE   )DISPLAY_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务COLLECTION*/
	OSTaskCreate ((OS_TCB        *)&COLLECTIONTCB,
								(CPU_CHAR      *)"COLLECTION",
								(OS_TASK_PTR    )Collection_task,
								(void          *)0,
								(OS_PRIO        )COLLECTION_TASK_PRIO,
								(CPU_STK       *)COLLECTION_STK,
								(CPU_STK_SIZE   )COLLECTION_STK_SIZE/10,
								(CPU_STK_SIZE   )COLLECTION_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
//	/*创建任务MOTOR1*/
//	OSTaskCreate ((OS_TCB        *)&MOTOR1TCB,
//								(CPU_CHAR      *)"MOTOR1",
//								(OS_TASK_PTR    )Motor1_task,
//								(void          *)0,
//								(OS_PRIO        )MOTOR1_TASK_PRIO,
//								(CPU_STK       *)MOTOR1_STK,
//								(CPU_STK_SIZE   )MOTOR1_STK_SIZE/10,
//								(CPU_STK_SIZE   )MOTOR1_STK_SIZE,
//								(OS_MSG_QTY     )0,
//								(OS_TICK        )0,
//								(void          *)0,
//								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//								(OS_ERR        *)&err);
//	/*创建任务MOTOR2*/
//	OSTaskCreate ((OS_TCB        *)&MOTOR2TCB,
//								(CPU_CHAR      *)"MOTOR2",
//								(OS_TASK_PTR    )Motor2_task,
//								(void          *)0,
//								(OS_PRIO        )MOTOR2_TASK_PRIO,
//								(CPU_STK       *)MOTOR2_STK,
//								(CPU_STK_SIZE   )MOTOR2_STK_SIZE/10,
//								(CPU_STK_SIZE   )MOTOR2_STK_SIZE,
//								(OS_MSG_QTY     )0,
//								(OS_TICK        )0,
//								(void          *)0,
//								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//								(OS_ERR        *)&err);
	/*创建软件定时器1*/
	OSTmrCreate	((OS_TMR         			 *)&tmr1,
							 (CPU_CHAR             *)"tmr1_led",
							 (OS_TICK               )0,//初始延迟为0*10ms
							 (OS_TICK               )50,//定时周期为50*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//周期模式
							 (OS_TMR_CALLBACK_PTR   )tmr1_callback_led,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	OS_CRITICAL_EXIT();//退出临界区代码保护
	OSTmrStart(&tmr1,&err);
}

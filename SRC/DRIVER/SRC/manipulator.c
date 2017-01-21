
#include "manipulator.h"

struct MANIPULATOR_VALUE Manipulator_Value, Action_table;

extern uint16_t Flag;

/**
  ************************************
  * @brief  上位机调试机械手
  * @param  None
  * @retval None
	************************************
*/
void Test_Manipulator_Upper_Monitor(void)
{
	WAIST(Action_table.Waist);//PB0 Tracking_Positioning_Left
	BIG_ARM(Action_table.Big_Arm);//PB4 Tracking_Positioning_Right
	LITTLE_ARM(Action_table.Little_Arm);//PB6 Tracking_Lift
	WRIST(Action_table.Wrist);//PB7 Tracking_Flat
	WRIST_TURN(Action_table.Wrist_Turn);//PB8
	GRIPPER(Action_table.Gripper);//PB9
}

/**
  ************************************
  * @brief  手臂初始化
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_Init(void)
{
	WAIST(Action_table.Waist);//PB0 Tracking_Positioning_Left
	BIG_ARM(Action_table.Big_Arm);//PB4 Tracking_Positioning_Right
	LITTLE_ARM(Action_table.Little_Arm);//PB6 Tracking_Lift
	WRIST(Action_table.Wrist);//PB87 Tracking_Flat
	WRIST_TURN(Action_table.Wrist_Turn);//PB8
	GRIPPER(Action_table.Gripper);//PB9
  Manipulator_Value.Waist = WRITE_LINE_INIT_Waist;
  Manipulator_Value.Big_Arm = WRITE_LINE_INIT_Big_Arm;
  Manipulator_Value.Little_Arm = WRITE_LINE_INIT_Little_Arm;
  Manipulator_Value.Wrist = WRITE_LINE_INIT_Wrist;
	Manipulator_Value.Wrist_Turn = WRITE_LINE_INIT_Wrist_Turn;
  Manipulator_Value.Gripper = WRITE_LINE_INIT_Gripper;
}

/**
  ************************************
  * @brief  手臂单独运动子函数
  * @param  None
  * @retval None
	************************************
*/
static void Manipulator_Move_Part(int16_t variable, int16_t *pVariable, uint8_t ON)
{
	static uint8_t a;
  uint16_t max_variable = 0;
  max_variable = fabs((variable - (*pVariable)));
	if(max_variable != 0)
	{
		while(max_variable--)
		{
			if(variable < (*pVariable))
				(*pVariable) --;
			else if(variable > (*pVariable))
				(*pVariable) ++;
			if(ON == 1)
				switch(a)
				{
					case 0:GRIPPER(*pVariable);break;
					case 1:WRIST_TURN(*pVariable);break;
					case 2:WRIST(*pVariable);break;
					case 3:LITTLE_ARM(*pVariable);break;
					case 4:BIG_ARM(*pVariable);break;
					case 5:WAIST(*pVariable);break;
					default:break;
				}
			else
				switch(a)
				{
					case 5:GRIPPER(*pVariable);break;
					case 4:WRIST_TURN(*pVariable);break;
					case 3:WRIST(*pVariable);break;
					case 2:LITTLE_ARM(*pVariable);break;
					case 1:BIG_ARM(*pVariable);break;
					case 0:WAIST(*pVariable);break;
					default:break;
				}
			delay_ms(TURN_DELAY_TIME);//机械手运动延时
		}
	}
	a++;
	if(a == 6) a=0;
}

/**
  ************************************
  * @brief  夹
  * @param  None
  * @retval None
	************************************
*/
void Catch_On(int16_t variable, int16_t *pVariable)
{
  uint16_t max_variable = 0;
  max_variable = fabs((variable - (*pVariable)));
	if(max_variable != 0)
	{
		while(max_variable--)
		{
			if(variable < (*pVariable))
				(*pVariable) --;
			else if(variable > (*pVariable))
				(*pVariable) ++;
			GRIPPER(*pVariable);
			delay_ms(TURN_DELAY_TIME);//机械手运动延时
		}
	}
	*pVariable = variable;
}

/**
  ************************************
  * @brief  手臂联动
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_ALL_Positive(struct MANIPULATOR_VALUE *manipulator)
{
	//手
	Manipulator_Move_Part(manipulator->Gripper,&Manipulator_Value.Gripper,1);
	//腕转
	Manipulator_Move_Part(manipulator->Wrist_Turn,&Manipulator_Value.Wrist_Turn,1);
	//腕
	Manipulator_Move_Part(manipulator->Wrist,&Manipulator_Value.Wrist,1);	
	//小臂动
	Manipulator_Move_Part(manipulator->Little_Arm,&Manipulator_Value.Little_Arm,1);	
	//大臂动
	Manipulator_Move_Part(manipulator->Big_Arm,&Manipulator_Value.Big_Arm,1);	
	//云台
	Manipulator_Move_Part(manipulator->Waist,&Manipulator_Value.Waist,1);	
}

/**
  ************************************
  * @brief  手臂联动-反向
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_ALL_Reverse(struct MANIPULATOR_VALUE *manipulator)
{
	//云台
	Manipulator_Move_Part(manipulator->Waist,&Manipulator_Value.Waist,0);
	//大臂动
	Manipulator_Move_Part(manipulator->Big_Arm,&Manipulator_Value.Big_Arm,0);
	//小臂动
	Manipulator_Move_Part(manipulator->Little_Arm,&Manipulator_Value.Little_Arm,0);
	//腕
	Manipulator_Move_Part(manipulator->Wrist,&Manipulator_Value.Wrist,0);
	//腕转
	Manipulator_Move_Part(manipulator->Wrist_Turn,&Manipulator_Value.Wrist_Turn,0);
	//手
	Manipulator_Move_Part(manipulator->Gripper,&Manipulator_Value.Gripper,0);
}

/**
  ************************************
  * @brief  手臂初始状态
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_Init_Start(char a[], char b[])
{
	Action_table.Waist = INIT_Waist;
	Action_table.Big_Arm = INIT_Big_Arm;
	Action_table.Little_Arm = INIT_Little_Arm;
	Action_table.Wrist = INIT_Wrist;
	Action_table.Wrist_Turn = INIT_Wrist_Turn;
	if(!strcmp(b,"Catch"))
		Action_table.Gripper = 2000;
	else
		Action_table.Gripper = 1400;
	if(!strcmp(a,"Waist"))
		Manipulator_ALL_Reverse(&Action_table);
	else if(!strcmp(a,"Gripper"))
		Manipulator_ALL_Positive(&Action_table);
}

/**
  ************************************
  * @brief  手臂举高状态
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_HIGHT_Start(char a[])
{
	Action_table.Waist = HIGHT_Waist;
	Action_table.Big_Arm = HIGHT_Big_Arm;
	Action_table.Little_Arm = HIGHT_Little_Arm;
	Action_table.Wrist = HIGHT_Wrist;
	Action_table.Wrist_Turn = HIGHT_Wrist_Turn;
	if(!strcmp(a,"Catch"))
		Action_table.Gripper = 2000;
	else
		Action_table.Gripper = HIGHT_Gripper;
	Manipulator_ALL_Positive(&Action_table);
}

/**
  ************************************
  * @brief  手臂位置1状态
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_Location_One_Start(void)
{
	Action_table.Waist = Location_One_Waist;
	Action_table.Big_Arm = Location_One_Big_Arm;
	Action_table.Little_Arm = Location_One_Little_Arm;
	Action_table.Wrist = Location_One_Wrist;
	Action_table.Wrist_Turn = Location_One_Wrist_Turn;
	Action_table.Gripper = Location_One_Gripper;
	Manipulator_ALL_Positive(&Action_table);
}

/**
  ************************************
  * @brief  手臂位置2状态
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_Location_Two_Start(void)
{
  Action_table.Waist = Location_Two_Waist;
  Action_table.Big_Arm = Location_Two_Big_Arm;
  Action_table.Little_Arm = Location_Two_Little_Arm;
  Action_table.Wrist = Location_Two_Wrist;
	Action_table.Wrist_Turn = Location_Two_Wrist_Turn;
  Action_table.Gripper = Action_table.Gripper;
	Manipulator_ALL_Reverse(&Action_table);
}

/**
  ************************************
  * @brief  手臂位置3状态
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_Location_Thr_Start(void)
{
  Action_table.Waist = Location_Thr_Waist;
  Action_table.Big_Arm = Location_Thr_Big_Arm;
  Action_table.Little_Arm = Location_Thr_Little_Arm;
  Action_table.Wrist = Location_Thr_Wrist;
	Action_table.Wrist_Turn = Location_Thr_Wrist_Turn;
  Action_table.Gripper = Location_Thr_Gripper;
	Manipulator_ALL_Reverse(&Action_table);
}

/**
  ************************************
  * @brief  手臂位置4状态
  * @param  None
  * @retval None
	************************************
*/
void Manipulator_Location_Fho_Start(void)
{
  Action_table.Waist = Location_Fho_Waist;
  Action_table.Big_Arm = Location_Fho_Big_Arm;
  Action_table.Little_Arm = Location_Fho_Little_Arm;
  Action_table.Wrist = Location_Fho_Wrist;
	Action_table.Wrist_Turn = Location_Fho_Wrist_Turn;
  Action_table.Gripper = Location_Fho_Gripper;
	Manipulator_ALL_Reverse(&Action_table);
}

/**
  ************************************
  * @brief  举高->位置num->举高->初始
  * @param  None
  * @retval None
	************************************
*/
void Hight_To_Location_To_Init(uint8_t num)
{
	Catch_On(1550,&Manipulator_Value.Gripper);
	Manipulator_HIGHT_Start("Catch");
//	delay_ms(1500);
	if(num == 1)
		Manipulator_Location_One_Start();
	else if(num == 2)
		Manipulator_Location_Two_Start();
	else if(num == 3)
		Manipulator_Location_Thr_Start();
	else if(num == 4)
		Manipulator_Location_Fho_Start();
//	delay_ms(1500);
	Manipulator_HIGHT_Start("On");
//	delay_ms(1500);
//	Manipulator_Init_Start("Gripper");
}

/**
  ************************************
  * @brief  画直线舵机分动函数
  * @param  None
  * @retval None
	************************************
*/
static void Write_Line_Part_Move(int16_t variable, int16_t *pVariable, uint8_t ON)
{
	static uint8_t a;
  uint16_t max_variable = 0;
  max_variable = fabs((variable - (*pVariable)));
	if(max_variable != 0)
	{
		while(max_variable--)
		{
			if(variable < (*pVariable))
				(*pVariable) --;
			else if(variable > (*pVariable))
				(*pVariable) ++;
			if(ON == 1)
				switch(a)
				{
					case 0:GRIPPER(*pVariable);break;
					case 1:WRIST(*pVariable);break;
					case 2:LITTLE_ARM(*pVariable);break;
					case 3:WAIST(*pVariable);break;
					default:break;
				}
			else
				switch(a)
				{
					case 3:GRIPPER(*pVariable);break;
					case 2:WRIST(*pVariable);break;
					case 1:LITTLE_ARM(*pVariable);break;
					case 0:WAIST(*pVariable);break;
					default:break;
				}
			delay_ms(TURN_DELAY_TIME);//机械手运动延时
		}
	}
	a++;
	if(a == 4) a=0;
}

/**
  ************************************
  * @brief  画直线
  * @param  None
  * @retval None
	************************************
*/
void Write_Line_ALL_Move(struct MANIPULATOR_VALUE *manipulator)
{
	//云台
	Write_Line_Part_Move(manipulator->Waist,&Manipulator_Value.Waist,0);
	//腕
	Write_Line_Part_Move(manipulator->Wrist,&Manipulator_Value.Wrist,0);
	//小臂动
	Write_Line_Part_Move(manipulator->Little_Arm,&Manipulator_Value.Little_Arm,0);
}

/**
  ************************************
  * @brief  画直线
  * @param  None
  * @retval None
	************************************
*/
void Write_Line_Move(void)
{
	int a;
	for(a=0; a<200; a++)
	{
		Action_table.Waist++;
		if(Action_table.Waist > 800)
		{
			Action_table.Little_Arm++;
			Action_table.Wrist += 2;
		}
		else
		{
			Action_table.Little_Arm--;
			Action_table.Wrist -= 2;
		}
		Manipulator_ALL_Reverse(&Action_table);
//		Send_PC(0x00, Action_table.Waist);
//		Send_PC(0x02, Action_table.Little_Arm);
//		Send_PC(0x03, Action_table.Wrist);
	}
}

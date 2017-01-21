/**
  ************************************
  * @file    PC_Link.c
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.09.27
  * @brief   机械手串口调试函数
	************************************
*/

#include "PC_Link.h"
#include "ANO-Tech.h"
#include "includes.h"
#include "app.h"

uint8_t buf[4] = {0xff, 0xff, 0xff, 0xff};

extern struct MANIPULATOR_VALUE Action_table;

//定义全局变量
uint16_t Flag,Time;

/**
  ************************************
  * @brief  发送int型给PC
  * @param  header:头码   date:要发送的数据
  * @retval None
	************************************
*/
void Send_PC(uint8_t header, uint16_t date)
{
  uint8_t buf[4];
  buf[0] = header;
  buf[1] = date >> 8;
  buf[2] = date & 0xff;
  buf[3] = ~header;
  for(uint8_t i=0; i<4; i++)
  {
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
    USART_SendData(USART1, buf[i]);
  }
}

/**
  ************************************
  * @brief  判断函数
  * @param  None
  * @retval 对接收到的数据进行判断
	************************************
*/
//static void Judge(void)
//{
//	switch(buf[0])
//	{
//		case 0x00:
//		{
//			if(HEADER_JUDGE(0x00))
//				switch(buf[1])
//				{
//					case 0x01:Action_table.Waist += 10;Send_PC(0x00, Action_table.Waist);break;
//					case 0x02:Action_table.Waist -= 10;Send_PC(0x00, Action_table.Waist);break;
//					case 0x03:Action_table.Big_Arm += 10;Send_PC(0x01, Action_table.Big_Arm);break;
//					case 0x04:Action_table.Big_Arm -= 10;Send_PC(0x01, Action_table.Big_Arm);break;
//					case 0x05:Action_table.Little_Arm += 10;Send_PC(0x02, Action_table.Little_Arm);break;
//					case 0x06:Action_table.Little_Arm -= 10;Send_PC(0x02, Action_table.Little_Arm);break;
//					case 0x07:Action_table.Wrist += 10;Send_PC(0x03, Action_table.Wrist);break;
//					case 0x08:Action_table.Wrist -= 10;Send_PC(0x03, Action_table.Wrist);break;
//					case 0x0b:Action_table.Wrist_Turn += 10;Send_PC(0x05, Action_table.Wrist_Turn);break;
//					case 0x0c:Action_table.Wrist_Turn -= 10;Send_PC(0x05, Action_table.Wrist_Turn);break;
//					case 0x09:Action_table.Gripper += 10;Send_PC(0x04, Action_table.Gripper);break;
//					case 0x0a:Action_table.Gripper -= 10;Send_PC(0x04, Action_table.Gripper);break;
//					default:break;
//				}
//		}
//		case 0x01:if(HEADER_JUDGE(0x01)){UINT8_T_TO_INT(Action_table.Waist);Send_PC(0x00, Action_table.Waist);}break;
//		case 0x02:if(HEADER_JUDGE(0x02)){UINT8_T_TO_INT(Action_table.Big_Arm);Send_PC(0x01, Action_table.Big_Arm);}break;
//		case 0x03:if(HEADER_JUDGE(0x03)){UINT8_T_TO_INT(Action_table.Little_Arm);Send_PC(0x02, Action_table.Little_Arm);}break;
//		case 0x04:if(HEADER_JUDGE(0x04)){UINT8_T_TO_INT(Action_table.Wrist);Send_PC(0x03, Action_table.Wrist);}break;
//		case 0x06:if(HEADER_JUDGE(0x06)){UINT8_T_TO_INT(Action_table.Wrist_Turn);Send_PC(0x05, Action_table.Wrist_Turn);}break;
//		case 0x05:if(HEADER_JUDGE(0x05)){UINT8_T_TO_INT(Action_table.Gripper);Send_PC(0x04, Action_table.Gripper);}break;
//		case 0x07:if(HEADER_JUDGE(0x07)){Flag = 1;}break;
//		default:break;
//	}
//}

/**
  ************************************
  * @brief  串口1中断函数
  * @param  buf[0]为头码  buf[3]为尾码,即头码取反  buf[1]指令位
  * @retval None
	************************************
*/
//void USART1_IRQHandler(void)
//{
//	static uint8_t count = 0;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		buf[count] = USART_ReceiveData(USART1);
//    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		count++;
//	}
//  if(count == 4)
//  {
//    count = 0;
//		Judge();
//  }
//}

void USART1_IRQHandler(void)
{
	#if SYSTEM_SUPPORT_OS
		OSIntEnter();
	#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//ANO_DT_Data_Receive_Prepare(USART_ReceiveData(USART1));
		USART_SendData(USART1, USART_ReceiveData(USART1));
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	#if SYSTEM_SUPPORT_OS
		OSIntExit();
	#endif
}

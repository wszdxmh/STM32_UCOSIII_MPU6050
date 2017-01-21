
#include "mpuiic.h"

/**
  **********************************
  * @brief  MPU6050初始化
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

/**
  **********************************
  * @brief  产生IIC起始信号
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();//sda线输出
	MPU_IIC_SDA = 1;
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SDA = 0;//START:when CLK is high,DATA change form high to low
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 0;//钳住I2C总线，准备发送或接收数据
}

/**
  **********************************
  * @brief  产生IIC停止信号
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda线输出
	MPU_IIC_SCL = 0;
	MPU_IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_SDA = 1;//发送I2C总线结束信号
	MPU_IIC_DELAY();
}

/**
  **********************************
  * @brief  等待应答信号
  * @param  None
  * @retval 1：接收应答失败	0：接收应答成功
  **********************************
*/
unsigned char MPU_IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	MPU_SDA_IN();      //SDA设置为输入  
	MPU_IIC_SDA = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	while (MPU_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL = 0;//时钟输出0
	return 0;
} 

/**
  **********************************
  * @brief  产生应答信号
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 0;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  不产生应答信号
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 1;
	MPU_IIC_DELAY();
	MPU_IIC_SCL = 0;
}

/**
  **********************************
  * @brief  发送一个字节
  * @param  None
  * @retval None
  **********************************
*/
void MPU_IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;
	MPU_SDA_OUT();
	MPU_IIC_SCL = 0;//拉低时钟开始数据传输
	for (t = 0;t<8;t++)
	{
		MPU_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		MPU_IIC_SCL = 1;
		MPU_IIC_DELAY();
		MPU_IIC_SCL = 0;
		MPU_IIC_DELAY();
	}
}

/**
  **********************************
  * @brief  读一个字节
  * @param  None
  * @retval None
  **********************************
*/
unsigned char MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MPU_SDA_IN();//SDA设置为输入
	for (i = 0;i<8;i++)
	{
		MPU_IIC_SCL = 0;
		MPU_IIC_DELAY();
		MPU_IIC_SCL = 1;
		receive <<= 1;
		if (MPU_READ_SDA)receive++;
		MPU_IIC_DELAY();
	}
	if (!ack)
		MPU_IIC_NAck();//发送nACK
	else
		MPU_IIC_Ack(); //发送ACK
	return receive;
}


#include "hmciic.h"
#include "hmc5883.h"

void HMC5883_WriteReg(u8 reg_add,u8 reg_dat)
{
	HMC_IIC_Start();
	HMC_IIC_Send_Byte(HMC5883_SLAVE_ADDRESS);
	HMC_IIC_Wait_Ack();
	HMC_IIC_Send_Byte(reg_add);
	HMC_IIC_Wait_Ack();
	HMC_IIC_Send_Byte(reg_dat);
	HMC_IIC_Wait_Ack();
	HMC_IIC_Stop();
}

void HMC5883_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	HMC_IIC_Start();
	HMC_IIC_Send_Byte(HMC5883_SLAVE_ADDRESS);
	HMC_IIC_Wait_Ack();
	HMC_IIC_Send_Byte(reg_add);
	HMC_IIC_Wait_Ack();
	
	HMC_IIC_Start();
	HMC_IIC_Send_Byte(HMC5883_SLAVE_ADDRESS+1);
	HMC_IIC_Wait_Ack();
	
	for(i=0;i<(num-1);i++){
		*Read=HMC_IIC_Read_Byte(1);
		Read++;
	}
	*Read=HMC_IIC_Read_Byte(0);
	HMC_IIC_Stop();
}

void HMC5883_Init(void)
{
	HMC_IIC_Init();
	delay_ms(100);
	HMC5883_WriteReg(HMC5883_MOD, 0x00);
	HMC5883_WriteReg(HMC5883_CON1 , 0x18);
	HMC5883_WriteReg(HMC5883_CON2 , 0x00);
}

void HMC5883ReadID(void)
{
	unsigned char ReA = 0,ReB = 0,ReC = 0;
	HMC5883_ReadData(HMC5883_IDEA,&ReA,1);    //读器件地址
	HMC5883_ReadData(HMC5883_IDEB,&ReB,1);    //读器件地址
	HMC5883_ReadData(HMC5883_IDEC,&ReC,1);    //读器件地址
}

void HMC5883ReadMag(short *MagData)
{
	u8 buf[6];
	HMC5883_ReadData(HMC5883_DOXM, buf, 6);
	MagData[0] = (buf[0] << 8) | buf[1];
	MagData[1] = (buf[2] << 8) | buf[3];
	MagData[2] = (buf[4] << 8) | buf[5];
}

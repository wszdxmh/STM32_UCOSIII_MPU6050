
#include "motor.h"

/**
  ************************************
  * @brief  PWM的初始化函数
  * @param  arr: 自动重装周期值   psc:预分频值
  * @retval arr=19999  psc=71     f=72000000/((19999+1)*(71+1))=50Hz
	************************************
*/
void Motor_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//初始化I/O时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);//GPIOB外部时钟使能，特别重要！！！
	//用PB3时要禁用JTAG，禁用要先开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_14 | GPIO_Pin_15;//设置IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置为普通推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_15, Bit_SET);//接高使能引脚
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//设置IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置为普通推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);//接高使能引脚
	
	//初始化PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE); //使能GPIO外设和AFIO复用功能模块时钟
	
	//设置该引脚为复用输出功能,输出TIM4 的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //TIM4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel2 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	//初始化TIM4 PWM全部通道
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

/**
  ************************************
  * @brief  电机1正转
  * @param  pwm：占空比
  * @retval None
	************************************
*/
void Motor1_Foreward(uint16_t pwm)
{
	MOTOR1_DIRECRION(Bit_SET);
	MOTOR1_SPEED(pwm);
}

/**
  ************************************
  * @brief  电机1反转
  * @param  pwm：占空比
  * @retval None
	************************************
*/
void Motor1_Reversal(uint16_t pwm)
{
	MOTOR1_DIRECRION(Bit_RESET);
	MOTOR1_SPEED(pwm);
}

/**
  ************************************
  * @brief  电机2正转
  * @param  pwm：占空比
  * @retval None
	************************************
*/
void Motor2_Foreward(uint16_t pwm)
{
	MOTOR2_DIRECRION(Bit_SET);
	MOTOR2_SPEED(pwm);
}

/**
  ************************************
  * @brief  电机2反转
  * @param  pwm：占空比
  * @retval None
	************************************
*/
void Motor2_Reversal(uint16_t pwm)
{
	MOTOR2_DIRECRION(Bit_RESET);
	MOTOR2_SPEED(pwm);
}

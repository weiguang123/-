#include "pwm.h"
/*		
 *		 PWM频率：24KHZ
 *     TIM_SetCompare1(TIM1,100);	//占空比90
 *		 TIM_SetCompare1(TIM1,1500);			//占空比50
 *		 TIM_SetCompare1(TIM1,3000);			//0
 *		 TIM_SetCompare1(TIM1,2700);  	 //10
 *		 TIM_SetCompare1(TIM1,2400);     //20
 *		 TIM_SetCompare1(TIM1,2100);     //30
 *		 TIM_SetCompare1(TIM1,1800);     //40
 *		 TIM_SetCompare1(TIM1,1500);     //50
 *		 TIM_SetCompare1(TIM1,1200);     //60
 *		 TIM_SetCompare1(TIM1,900);     //70
 *		 TIM_SetCompare1(TIM1,600);     //80
 *		 TIM_SetCompare1(TIM1,300);     //90 
 *		 TIM_SetCompare1(TIM1,0);     //100  
 *		 PWM频率：60KHZ
 *     TIM_SetCompare1(TIM1,100);	//占空比50
 *		 TIM_SetCompare1(TIM1,200);			//0
 *		 TIM_SetCompare1(TIM1,180);  	 //10
 *		 TIM_SetCompare1(TIM1,160);     //20
 *		 TIM_SetCompare1(TIM1,140);     //30
 *		 TIM_SetCompare1(TIM1,120);     //40
 *		 TIM_SetCompare1(TIM1,100);     //50
 *		 TIM_SetCompare1(TIM1,80);     //60
 *		 TIM_SetCompare1(TIM1,60);     //70
 *		 TIM_SetCompare1(TIM1,40);     //80
 *		 TIM_SetCompare1(TIM1,20);     //90 
 *		 TIM_SetCompare1(TIM1,0);     //100  
 */

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 配置TIMx复用输出PWM时用到的I/O
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void GENERAL_TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能定时器始终：设置TIM3CLK 为 72MHZ */
	GENERAL_TIM_APBxClock_FUN (GENERAL_TIM_CLK, ENABLE);

  /* 使能定时器通道引脚GPIO时钟 */
  GENERAL_TIM_GPIO_APBxClock_FUN(GENERAL_TIM_GPIO_CLK, ENABLE); 

  /* 配置定时器通道1输出引脚模式：复用推挽输出模式 */
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
  #if 0
    /* 配置定时器通道2输出引脚模式 */
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH2_PIN;
  GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);

  /* 配置定时器通道3输出引脚模式 */
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN;
  GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
	
  /* 配置定时器通道4输出引脚模式 */
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
  GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
	
  #endif
	
	
  /* Disable the Serial Wire Jtag Debug Port SWJ-DP */
  /* 禁用JTAG功能 */
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  /* 定时器通道引脚重映射 */
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
}

/**
  * 函数功能: 配置TIM3输出的PWM信号的模式，如周期、极性、占空比
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
static void GENERAL_TIMx_Configuration(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
// 	uint16_t CCR1_Val = 150;       
//  uint16_t CCR2_Val = 150; 
//  uint16_t CCR3_Val = 150; 
//  uint16_t CCR4_Val = 150;   

/* ----------------------------------------------------------------------- 
    GENERAL_TIMx Channel1 duty cycle = (GENERAL_TIMx_CCR1/ GENERAL_TIMx_ARR+1)* 100% = 50%
    GENERAL_TIMx Channel2 duty cycle = (GENERAL_TIMx_CCR2/ GENERAL_TIMx_ARR+1)* 100% = 37.5%
    GENERAL_TIMx Channel3 duty cycle = (GENERAL_TIMx_CCR3/ GENERAL_TIMx_ARR+1)* 100% = 25%
    GENERAL_TIMx Channel4 duty cycle = (GENERAL_TIMx_CCR4/ GENERAL_TIMx_ARR+1)* 100% = 12.5%
  ----------------------------------------------------------------------- */

  /* 定时器基本参数始终 */		 
  /* 当定时器从0计数到999，即为1000次，为一个定时周期 */
  TIM_TimeBaseStructure.TIM_Period = arr; 
  /* 设置预分频：不预分频，即为72MHz,输出脉冲频率：72MHz/720 */
  /* 同一个定时器的不同通道PWM的频率只能是相同的，不同通道可以设置为不同的占空比 */
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  /* 设置时钟分频系数：不分频(这里用不到) */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
  /* 向上计数模式 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);

  /* 定时器输出通道1模式配置 */
  /* 模式配置：PWM模式1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  /* 输出状态设置：使能输出 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  /* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */
  TIM_OCInitStructure.TIM_Pulse = 0;
  /* 当定时器计数值小于CCR1_Val时为高电平 */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  /* 定时器输出通道2模式配置 */
  /* 设置通道2的电平跳变值，输出另外一个占空比的PWM */
  TIM_OCInitStructure.TIM_Pulse = 0;
  /* 初始化定时器通道2输出PWM */
  TIM_OC2Init(GENERAL_TIMx, &TIM_OCInitStructure);
  /* 定时器比较输出通道2预装载配置：使能预装载 */
  TIM_OC2PreloadConfig(GENERAL_TIMx, TIM_OCPreload_Enable);


  
  /* 使能定时器重载寄存器ARR */
  TIM_ARRPreloadConfig(GENERAL_TIMx, ENABLE);

  /* 使能定时器 */
  TIM_Cmd(GENERAL_TIMx, ENABLE);
}

/**
  * 函数功能: TIMx 输出PWM信号初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：只要调用这个函数TIMx的四个通道就会有PWM信号输出
  */
void GENERAL_TIMx_PWM_Init(u16 arr,u16 psc)
{
	GENERAL_TIMx_GPIO_Config();
	GENERAL_TIMx_Configuration(arr,psc);	
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
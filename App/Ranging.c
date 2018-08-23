#include "Ranging.h"
#include "stm32f1xx_hal.h" 
#include "sys.h"

TIM_HandleTypeDef htim3;
//#define GPIOC
//#define TRIG_PROE	GPIO_PIN_0//输出，用于发送开始信息
//#define ECHO_PROE	GPIO_PIN_1//输入,用于等到信号
//发送开始信号后等待回应，关键问题是如何统计时间

void StartModule()
{
//	TRIG_PROE;//
//	
//	TRIG_PROE;//
}















/* TIM3 init function */
void MX_TIM3_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 72000-1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 5000-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_Base_Start_IT(&htim3);
	
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if(tim_baseHandle->Instance==TIM3)
	{
		/* USER CODE BEGIN TIM3_MspInit 0 */

		/* USER CODE END TIM3_MspInit 0 */
		/* TIM3 clock enable */
		__HAL_RCC_TIM3_CLK_ENABLE();
		/* USER CODE BEGIN TIM3_MspInit 1 */
		/* TIM3 interrupt Init */
		HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		/* USER CODE END TIM3_MspInit 1 */
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if(tim_baseHandle->Instance==TIM3)
	{
		/* USER CODE BEGIN TIM3_MspDeInit 0 */

		/* USER CODE END TIM3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM3_CLK_DISABLE();
		/* USER CODE BEGIN TIM3_MspDeInit 1 */
		/* TIM3 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		/* USER CODE END TIM3_MspDeInit 1 */
	}
} 
void TIM3_IRQHandler(void)
{
	/* TIM Update event */
	if(__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_UPDATE) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) !=RESET)
		{
			__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5));
		}
	}
//	HAL_TIM_IRQHandler(&htim3);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM3)
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5));
//}






/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"  
#include "Sys.h"
#include "gpio.h"
#include "uart.h"

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	while(1)
	{
		printf("test");
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(500);
	}
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

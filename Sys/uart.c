#include "stm32f1xx_hal.h"
#include "uart.h"
#include "Sys.h"
#include <string.h>
#include "queue.h"
#include "flash.h"
#include "updata.h"
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
	USART1->DR = (uint8_t) ch;      
	return ch;
}
UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	init_uart1();
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(uartHandle->Instance==USART1)
	{
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		/**USART1 GPIO Configuration    
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

	if(uartHandle->Instance==USART1)
	{
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration    
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX 
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}
}

_Uart1 Uart1;

void USART1_IRQHandler(void)
{
	user_Uart1Handler();
}
void user_Uart1Handler()
{
	u8 temp;
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!=RESET))
	{
		temp = (uint8_t)(huart1.Instance->DR&(uint8_t)0x00FF);
		__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
		if(Uart1.status == 0x01)
		{
			__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//开启空闲中断
			Uart1.status = 0x02;
			Uart1.Rxlen = 0;
			Uart1.RxBuff[Uart1.Rxlen++] = temp;			
		}
		else if(Uart1.status == 0x02) 
		{
			Uart1.RxBuff[Uart1.Rxlen++] = temp;	
		}
	}
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!=RESET))//空闲中断函数
	{
		__HAL_UART_DISABLE_IT(&huart1,UART_IT_IDLE);//关闭空闲中断
		__HAL_UART_DISABLE_IT(&huart1,UART_IT_RXNE);//关闭接收中断
		if(Uart1.status == 0x02)
			Uart1.status = 0x03;//结束标志位
	}
}
void init_uart1()
{
	Uart1.status = 0x01;
	Uart1.Rxlen = 0x00;
	memset(Uart1.RxBuff,0,sizeof(Uart1.RxBuff));
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_PE);
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}
void re_uart1_rx()
{
	Uart1.status = 0x01;
	Uart1.Rxlen = 0x00;
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_PE);
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

_Update *Update;
_Update *pdata;
_UpdateHead *UpdateHead;
u16 soft_crc,soft_length,updata_pack;
void test_print()
{
	static u16 count=0;
	u32 updata_add;
	u8 *token,*segment_ptr;
	u8 *padd;
	if(Uart1.status == 0x03)
	{
		if(Uart1.RxBuff[0] == 'U')
		{
			pdata = (_Update *)Uart1.RxBuff;
			token = (u8 *)strtok((char *)(Uart1.RxBuff), ":");
			segment_ptr = token;//获取首字段
		}
		if(strcmp((char *)segment_ptr,"Updateinfo")==0)//更新信息
		{
			UpdateHead = (_UpdateHead *)pdata->data;
			if((local_type == UpdateHead->Type)&&(local_soft != UpdateHead->Soft))//进行升级动作
			{
				soft_crc = UpdateHead->Crc;
				soft_length = UpdateHead->Len;
				updata_pack = soft_length % 512 ? (soft_length>>9) + 1 : (soft_length>>9);//
				ProgramReady(soft_length);
			}
			else//进行重启动作
			{
				Set_Updata_flag(PROGRAM_APP_MODE);
				NVIC_SystemReset();
			}
		}
		if(strcmp((char *)segment_ptr,"Updatedata")==0)//开始写数据
		{
			if(count++ <= soft_length-1)//第一包到倒数第二包
			{
				updata_add = Flash_APP_ADDR + (count - 1) * 512;			  
				IAP_Write_App_Bin(updata_add,Update->data,512);
			}
			else if(count == soft_length)//last pack
			{
				updata_add = Flash_APP_ADDR + (count - 1) * 512;
				IAP_Write_App_Bin(updata_add,Update->data,512);
				padd = (uint8_t *)(Flash_APP_ADDR);
				if(soft_crc == ChkCrcValueEx(padd,soft_length,0xFFFF))
				{
					Set_Updata_flag(PROGRAM_APP_MODE);
					NVIC_SystemReset();
				}
				count = 0;
			}
		}
		re_uart1_rx();
		memset(Uart1.RxBuff,0,sizeof(Uart1.RxBuff));
	}

}

void init_queue()
{
	//InitQueue();
}





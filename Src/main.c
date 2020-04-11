/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include "usbh_def.h"
#include "usbh_hid_gamepad.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define GP_LEFT 	(1<<0)
#define GP_RIGHT 	(1<<1)
#define GP_UP 		(1<<2)
#define GP_DOWN		(1<<3)
#define GP_X		(1<<4)
#define GP_CIRC		(1<<5)
#define GP_SQ		(1<<6)
#define GP_TR		(1<<7)
#define GP_START	(1<<8)
#define GP_SEL		(1<<9)
#define GP_LT		(1<<10)
#define GP_LB		(1<<11)
#define GP_RT		(1<<12)
#define GP_RB		(1<<13)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint16_t buttons;
GPIO_InitTypeDef buttons_b, buttons_a;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int _write(int file, unsigned char *ptr, int len)
{
	if (file == 1) 
	{
		HAL_UART_Transmit(&huart2,ptr,len,1000000);
		return len;
	}
	return -1;
}


void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
	
	HID_PAD_Info_TypeDef *pi;
	pi=USBH_HID_GetPadInfo(phost);
	if (pi!=NULL)
	{
		if (pi->keys[0]<0x7f) buttons|=GP_LEFT;
			else buttons&=~GP_LEFT;
		if (pi->keys[0]>0x7f) buttons|=GP_RIGHT;
			else buttons&=~GP_RIGHT;
		if (pi->keys[1]<0x7f) buttons|=GP_UP;
			else buttons&=~GP_UP;
		if (pi->keys[1]>0x7f) buttons|=GP_DOWN;
			else buttons&=~GP_DOWN;
		if (pi->keys[5] & 0x40) buttons|=GP_X;
			else buttons&=~GP_X;
		if (pi->keys[5] & 0x80) buttons|=GP_SQ;
			else buttons&=~GP_SQ;
		if (pi->keys[5] & 0x20) buttons|=GP_CIRC;
			else buttons&=~GP_CIRC;
		if (pi->keys[5] & 0x10) buttons|=GP_TR;
			else buttons&=~GP_TR;
		if (pi->keys[6] & 0x20) buttons|=GP_START;
			else buttons&=~GP_START;
		if (pi->keys[6] & 0x10) buttons|=GP_SEL;
			else buttons&=~GP_SEL;
		if (pi->keys[6] & 0x01) buttons|=GP_LB;
			else buttons&=~GP_LB;
		if (pi->keys[6] & 0x04) buttons|=GP_LT;
			else buttons&=~GP_LT;
		if (pi->keys[6] & 0x02) buttons|=GP_RB;
			else buttons&=~GP_RB;
		if (pi->keys[6] & 0x08) buttons|=GP_RT;
			else buttons&=~GP_RT;
	}
  
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//int i;
	uint16_t old_buttons=0xffff;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_HOST_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	printf("\r\n\r\n");
	printf("USB Host HID\r\n");
	printf("---------------------\r\n");

	buttons_b.Pin=GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	buttons_b.Mode=GPIO_MODE_OUTPUT_PP;
	buttons_b.Pull=GPIO_NOPULL;
	buttons_b.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	buttons_b.Alternate=0;

	buttons_a.Pin=GPIO_PIN_15 ;
	buttons_a.Mode=GPIO_MODE_OUTPUT_PP;
	buttons_a.Pull=GPIO_NOPULL;
	buttons_a.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	buttons_a.Alternate=0;

	HAL_GPIO_Init(GPIOB,&buttons_b);
	HAL_GPIO_Init(GPIOA,&buttons_a);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();
    /* USER CODE BEGIN 3 */
    if(buttons !=old_buttons)
    {
		old_buttons=buttons;
//	    for(i=0;i<16;i++)
//	    {
//			if(buttons & (1<<i)) printf("1");
//			else printf("0");
//			
//		}
//		printf("\n");
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,buttons & GP_LEFT);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,buttons &  GP_RIGHT );
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,buttons &  GP_UP 	);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,buttons &  GP_DOWN);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,buttons &  GP_X	);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,buttons &  GP_CIRC);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,buttons &  GP_SQ	);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,buttons &  GP_TR);	
// GP_START
// GP_SEL	
// GP_LT	
// GP_LB	
// GP_RT	
// GP_RB	
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

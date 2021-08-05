/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

int fputc(int ch, FILE *f){
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart2, temp, 1, 2);//huart1 根据配置修改
    return ch;
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
	uint8_t u1,UART1_DATA[5];
	uint8_t UART2,UART2_DATA[4];
	uint8_t UART2_flag= 3;
	uint8_t tem= 0,flag= 0;
	uint8_t class_confiden[2][1]= {0};
	uint8_t end[3]= {0xff,0xff,0xff};
	uint32_t con;
	uint32_t k= 5000000;
	uint8_t p= 0;
//	uint8_t test[]= "test";
//	uint8_t t0[]="t0.txt=\"";
	uint8_t i= 5,j;
	float cof;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//	uint8_t j;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

	
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1220);   //舵机1     0度
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 1750);  //舵机2   180度
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 400);    //电机1    10/60
	
	
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);

	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	
	
	HAL_UART_Receive_IT(&huart1,&u1,1);
	HAL_UART_Receive_IT(&huart2,&UART2,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//			HAL_Delay(100);

		
		k++;
		if(k== 5000000)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 250);    //检测不到焊缝时速度加快
		}
		else if(k< 1000000)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 100);    //串口1检测到数据时速度变慢
		}
		HAL_UART_Receive_IT(&huart1,&u1,1);
		HAL_UART_Receive_IT(&huart2,&UART2,1);
		if(UART2_flag== 2)  //暂停
		{
			UART2_flag= 3;
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
		}
		else if(UART2_flag== 1)  //后退
		{
			UART2_flag= 3;
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
		}
		else if(UART2_flag== 0)   //前进
		{
			UART2_flag= 3;
			HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);


		}

		
		
//		HAL_UART_Receive_IT(&huart1,&UART1,1);
		if(flag== 1)
		{


			con= 0;
			

			if(1)
			{
//				con+= class_confiden[1][4];
//				con/= 5;
				con= class_confiden[1][0];
				if(class_confiden[0][0]== 0)
				{
					printf("t0.txt=\"焊缝质量：合格\"");
					HAL_UART_Transmit(&huart2,end,3,0xffff); 
					cof= con/ 100.0;
					printf("t1.txt=\"置信率：    %.2f\"",cof);
					HAL_UART_Transmit(&huart2,end,3,0xffff);
						
				}
				else if(class_confiden[0][0]== 1)
				{
					printf("t0.txt=\"焊缝质量：不合格\"");
					HAL_UART_Transmit(&huart2,end,3,0xffff); 
					cof= con/ 100.0;
					printf("t1.txt=\"置信率：    %.2f\"",cof);
					HAL_UART_Transmit(&huart2,end,3,0xffff); 
				}
				else
				{
					printf("t0.txt=\"%d\"",class_confiden[0][0]);
				}
				flag= 0;
			}
				
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	

	static uint8_t j= 4;
	if(huart -> Instance == huart1.Instance ) 
	{
		UART1_DATA[0]= u1;
//		HAL_UART_Transmit(&huart2,&u1,1,0xff);
		k= 0;
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 50);
		if(u1== 0xff)
		{
			i= 0;
//			HAL_UART_Transmit(&huart2,test,1,0xffff);
		}
		
		if(i< 5)
		{
//			 HAL_UART_Transmit(&huart2,test,1,0xffff);
			
			UART1_DATA[i]= u1;
			i++;
			if((i== 5) && (UART1_DATA[4]== 0xfe) && (UART1_DATA[3]== (UART1_DATA[1]+ UART1_DATA[2])) && UART1_DATA[0]== 0xff)
			{
//				printf("1\r\n");
				class_confiden[0][0]= UART1_DATA[1];
				class_confiden[1][0]= UART1_DATA[2];
				flag= 1;
//				HAL_UART_Transmit(&huart2,test,3,0xffff);
//				printf("t0.txt=\"%d  %d\"",class_confiden[0][0],class_confiden[1][0]);

//				tem++;
//				if(tem== 5)
//				{
//					tem= 0;
//					flag= 1;
//				}
			}
//		}
			HAL_UART_Receive_IT(&huart1,&u1,1);
	}
	
	if(huart -> Instance == huart2.Instance ) 
	{
		if(UART2== 0xfe)
		{
			j= 0;
		}
		if(j< 4)
		{
			UART2_DATA[j]= UART2;
			j++;
			if(j== 4 && UART2_DATA[0]== 0xfe && UART2_DATA[2]== 0xff && UART2_DATA[3]== 0xff)
			{
				UART2_flag= UART2_DATA[1];
			}
		}
	}
//		if(UART2[0]== 0xff && UART2[2]== 0xfe && UART2[3]== 0xff)
//		{
////			printf("t0.txt=\"Yes__uart2\"");
//			HAL_UART_Transmit(&huart2,end,3,0xffff); 
//			UART2_flag= UART2[1];
//		}
		HAL_UART_Receive_IT(&huart2,&UART2,1);
	}
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

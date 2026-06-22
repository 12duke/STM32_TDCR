/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "bdma.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "printf.h"
#include "ad.h"
#include "DcMotors.h"
#include "joy.h"
#include "vofa.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t htim6_i=0;
    static uint32_t htim7_i=0;

	if(htim==(&htim1))
	{
	 /* 判断当前计数器计数方向 */
		 __HAL_TIM_SET_COUNTER(&htim1,enc_default);
	 if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1)) M1.overflow--;
	 else M1.overflow++;

	}

	else if(htim==(&htim2))
	{
	 /* 判断当前计数器计数方向 */
		__HAL_TIM_SET_COUNTER(&htim2,enc_default);
	 if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2)) M2.overflow--;
	 else M2.overflow++;

	}

	else if(htim==(&htim3))
	{
	 /* 判断当前计数器计数方向 */
		__HAL_TIM_SET_COUNTER(&htim3,enc_default);
	 if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3)) M3.overflow--;
	 else M3.overflow++;

	}

	else if(htim==(&htim8))
	{
	 /* 判断当前计数器计数方向 */
		__HAL_TIM_SET_COUNTER(&htim8,enc_default);
	 if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim8)) M4.overflow--;
	 else M4.overflow++;

	}

	else if(htim->Instance==htim6.Instance)//0.1ms
	{

		M1.enc_now_total = (int64_t)M1.overflow * overflow_enc + M1_enc-enc_default;  //总编�?
		M2.enc_now_total = (int64_t)M2.overflow * overflow_enc + M2_enc-enc_default;
		M3.enc_now_total = (int64_t)M3.overflow * overflow_enc + M3_enc-enc_default;
		M4.enc_now_total = (int64_t)M4.overflow * overflow_enc + M4_enc-enc_default;


		if(htim6_i%500 == 0)                                 // 50ms
		{

//			if(abs((M1.enc_now_total - M1.enc_now_total_last))>1000)
//			{
//
//				if((M1.enc_now_total - M1.enc_now_total_last)>1000)M1.overflow--;
//				else M1.overflow++;
//
//				M1.enc_now_total = (int64_t)M1.overflow * overflow_enc + M1_enc;  //总编�?
//			}
//
//			if(abs((M2.enc_now_total - M2.enc_now_total_last))>1000)
//			{
//				if((M2.enc_now_total - M2.enc_now_total_last)>1000)M2.overflow--;
//				else M2.overflow++;
//
//				M2.enc_now_total = (int64_t)M2.overflow * overflow_enc + M2_enc;  //总编�?
//			}
//
//			if(abs((M3.enc_now_total - M3.enc_now_total_last))>1000)
//			{
//				if((M3.enc_now_total - M3.enc_now_total_last)>1000)M3.overflow--;
//				else M3.overflow++;
//
//				M3.enc_now_total = (int64_t)M3.overflow * overflow_enc + M3_enc;  //总编�?
//			}
//
//			if(abs((M4.enc_now_total - M4.enc_now_total_last))>1000)
//			{
//				if((M4.enc_now_total - M4.enc_now_total_last)>1000)M4.overflow--;
//				else M4.overflow++;
//
//				M4.enc_now_total = (int64_t)M4.overflow * overflow_enc + M4_enc;  //总编�?
//			}

			M1.enc_now_speed  = (M1.enc_now_total - M1.enc_now_total_last);   //转�??
			M2.enc_now_speed  = (M2.enc_now_total - M2.enc_now_total_last);
			M3.enc_now_speed  = (M3.enc_now_total - M3.enc_now_total_last);
			M4.enc_now_speed  = (M4.enc_now_total - M4.enc_now_total_last);

			M1.rpm = M1_rpm;
			M2.rpm = M2_rpm;
			M3.rpm = M3_rpm;
			M4.rpm = M4_rpm;

			M1.enc_now_total_last = M1.enc_now_total;   //上次编码记录
			M2.enc_now_total_last = M2.enc_now_total;
			M3.enc_now_total_last = M3.enc_now_total;
			M4.enc_now_total_last = M4.enc_now_total;


			motor_run_speed_50ms(&M1);       //速度�?
			motor_run_speed_50ms(&M2);
			motor_run_speed_50ms(&M3);
			motor_run_speed_50ms(&M4);

		}

		if(htim6_i %100 == 0)     // 100*0.1ms
		{
		}

		if(htim6_i++>=50000-1)htim6_i=0;
	}
	else if(htim->Instance==htim7.Instance)//1ms
	{


		M1.current = M1.current*0.9+M1_current*0.1;      //电流计算
		M2.current = M2.current*0.9+M2_current*0.1;
		M3.current = M3.current*0.9+M3_current*0.1;
		M4.current = M4.current*0.9+M4_current*0.1;


		joy_xy(JOY_X,JOY_Y);

		motor_run();

		if(!motor_homeing_n)
		{

//			current_errror();
//			enc_errror();
		}


		motor_run_position();                           //位置环电机控�?



		if(distal_rotating_en && M2.busy == 0)distal_rotating_en = 0;
		else if(distal_bending_en && M1.busy == 0)distal_bending_en = 0;
		if(labs(labs(M1.enc_now_total - M1.enc_Target) - labs(M2.enc_now_total - M2.enc_Target)/4)<10)distal_bending_en = 0;

		if(proximal_rotating_en && M3.busy == 0)proximal_rotating_en = 0;
		else if(proximal_bending_en&&M4.busy == 0)proximal_bending_en = 0;
		if(labs(labs(M4.enc_now_total - M4.enc_Target) - labs(M3.enc_now_total - M3.enc_Target)/4)<10)proximal_bending_en = 0;


		 if(M4.mode == 0)                                            //堵转�?�?
		 {
			if(M4.state == 1 && M4.rpm_Target>0)M4.state = 0;
			//	 else  if(M4.state == 2 && M4.rpm_Target<0)M4.state = 0;

			else if(M4.current>=150&&M4.rpm_Target<0)M4.state = 1;
			//else if(M4.current>=85&&M4.rpm_Target>0)M4.state = 2;

			else if(M4.state == 1 && M4.rpm_Target<0) M4.rpm_Target = 0;
			//	else if(M4.state == 2 && M4.rpm_Target>0) M4.rpm_Target = 0
		 }
		 if(M1.mode == 0)
		 {
			if(M1.state == 1 && M1.rpm_Target>0)M1.state = 0;
			//	 else  if(M1.state == 2 && M1.rpm_Target<0)M1.state = 0;

			else if(M1.current>150&&M1.rpm_Target>0)M1.state = 1;
			//else if(M1.current>=85&&M1.rpm_Target>0)M1.state = 2;

			else if(M1.state == 1 && M1.rpm_Target>0) M1.rpm_Target = 0;
			//	else if(M1.state == 2 && M1.rpm_Target>0) M1.rpm_Target = 0;

		 }

		/*if(htim7_i %25 == 0 && !motor_homeing_n)     // 500*1ms
		{

			if(Inspire_flag == 0)
			{
				whole_dis = interval_Inspire_Motor-interval_Inspire_Motor*Ins/AD_max;                     //因时电机

				if(whole_dis>interval_Inspire_Motor)whole_dis=interval_Inspire_Motor;
				else if(whole_dis<0)whole_dis=0;

				if(Inspire_buf!=whole_dis)
				{
					Inspire_buf = whole_dis;
				}

			}
			UART_DRV_DMA_SendCmd(0x01,CMD_SEEKPOS_SERVO,Min_Inspire_Motor+Inspire_buf);
		}暂时不用电推杠*/




		if(htim7_i %printf_cnt_time == 0)     // 500*1ms
		{
			if(printf_cnt<=10)printf_cnt++;                       //打印时间
		}
		if(htim7_i %1000 == 0)
		{
			printf_time++;
		}
		if(htim7_i%100 == 0)
		{

			if( M1.error || M2.error || M3.error ||M4.error) //错误   LED常量
			{
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_RESET);

			}
			else
			{
				HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			}
		}



		if(htim7_i++>=1000-1)htim7_i=0;
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
 uint8_t i;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_BDMA_Init();
  MX_ADC2_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_TIM15_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  printf_init();


  adc_init();
  motor_init();


	__HAL_TIM_CLEAR_IT(&htim6,TIM_IT_UPDATE);//清除中断标志�?
	__HAL_TIM_CLEAR_IT(&htim7,TIM_IT_UPDATE);//清除中断标志�?


	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim6);

	motor_homeing_n = 0;

	/*禁用归零系统*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(printf_cnt /*&& !motor_homeing_n*/)
	  {
		  printf_cnt--;
		  printf_mode =1;
		//  printf1("time    %d  ----------------\r\n",printf_time);
		  switch(printf_mode)
		  {
		  case 0:

	  		  printf1("JOY: %d, %d, %d, %f,%d\r\n",joy_x,joy_y,joy_r,joy_a,joy_en);
	  		  printf1("joy_key  %d   %d    \r\n",joy_key3_button,joy_button);
			  break;
		  case 1:
			  printf1("%f %f  \r\n",distal_bending_circle,distal_bending_circle_now);
			  printf1("%f %f  \r\n",proximal_bending_circle,proximal_bending_circle_now);
			  printf1("%d %d \r\n",distal_status,proximal_status);
			  break;
		  case 2:
				printf1("current:%f,%f,%f,%f \n",M1.current,M2.current,M3.current,M4.current);
//				printf1("error:%d,%d,%d,%d \n",M1.error,M2.error,M3.error,M4.error);
//				printf1("PWM:%f,%f,%f,%f \n",M1.PWM,M2.PWM,M3.PWM,M4.PWM);
//				printf1("rpm_Target:%f,%f,%f,%f \n",M1.rpm_Target,M2.rpm_Target,M3.rpm_Target,M4.rpm_Target);


			  break;
		  case 3:
			   printf1("distal_rotating_angle %f ",distal_rotating_angle);
			   printf1("proximal_rotating_angle %f ",proximal_rotating_angle);
			   printf1("joy_speed %f ", 1-0.7*JOY_S/AD_max);
			   printf1("rpm_max_Target %f ", M1.rpm_max_Target);
		  case 4:
	//		  printf1("%d %d %d %d \r\n",M1.enc_now_speed,M2.enc_now_speed,M3.enc_now_speed,M4.enc_now_speed);

//			  printf1("%8lld ,%d,%8lld ,%d\r\n",M1.enc_now_total,M1_enc,M2.enc_now_total,M2_enc);
//			  printf1("M:%8lld , %8lld , %8lld , %8lld , %8lld , %8lld , %8lld , %8lld\r\n",M1.enc_Target,M1.enc_now_total,M2.enc_Target,M2.enc_now_total,M3.enc_now_total_last,M3.enc_now_total,M4.enc_now_total_last,M4.enc_now_total);
//			  printf1("M1:%lld, M2:%lld, M3:%lld, M4:%lld\r\n",M1.enc_Target,M2.enc_Target,M3.enc_Target,M4.enc_Target);
//			  printf1("proximal bending_angle:%f, circle:%f  angle:%f\r\n",proximal_bending_angle_now,proximal_bending_circle_now,proximal_rotating_angle);
			  printf1("distal bending_angle:%f, circle:%f angle:%f \r\n",distal_bending_angle_now,distal_bending_circle_now,distal_rotating_angle);
//			  printf1("\r\n");
			  break;
		  case 5:
				//		  printf1("AD: %d %d %d %d",JOY_S,JOY_S2,button_home_time,button_home_flag);
					  printf1("AD: ");

//							  printf1("%d,",proximal_rotating_AD);
			  for(i=0;i<7*2;i++)
			  {
				  printf1("%d,",*ADC1_data[i]);;

			  }

//			  printf1("%d,",Ins);
//			  printf1("%d,",Min_Inspire_Motor+Inspire_buf);
//			  printf1("%d",whole_dis);

		//	  printf1("%f",distal_bending_circle);
			  printf1("\r\n");
			  break;
		  case 6:
			  printf1("%f,%f\r\n",-distal_bending_angle_now,-distal_rotating_angle);
			  break;
		  case 7:
			  printf1("distal_homeing_n = %d \r\n",distal_homeing_n);
			  break;

		  }
		//  printf1("  ----------------\r\n");
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 5;
  PeriphClkInitStruct.PLL2.PLL2N = 72;
  PeriphClkInitStruct.PLL2.PLL2P = 5;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
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



#include "../include/main.h"
#include "../include/comServo.hpp"
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/servo.h"
#include "../include/Servo/SCServo.h"
#include "stdio.h"


TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
UART_HandleTypeDef huart3;



void SystemClock_Config(void);

static void MX_USART6_UART_Init(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);


volatile uint16_t timerInt=0;
SMS_STS st;
//SCSCL st;

int main(void)
{
  HAL_Init();
  SystemClock_Config();


  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  //MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  
  /*
  ComServo *comServo = ComServo::getInstance();
  comServo->setHuart(&huart6);
  CServo servo1(1,40,3400);
  //servo1.servoEnableWheelMode();
  //servo1.servoSetServoPosition(200);*/
  
  
  st.pSerial = &huart6;

  LED led2((uint32_t *)(0x40020000UL),5);
  GPIO gpioC((uint32_t *)(0x40020800UL));
  gpioC.setPinMode(13,INPUT);
  HAL_TIM_Base_Start_IT(&htim2);


  bool test=0;
  int ID=1;
  uint8_t test1[6]={0xff,0xff,0x00,0x02,0x06,0xf7};
  //st.WritePosEx(1, 1000, 1500, 50);
  //HAL_UART_Transmit_IT(&huart6, test1, 6);
  /*
  st.unLockEprom(1); //Unlock EPROM-SAFE
  st.writeByte(1, SMS_STS_ID, 2);//Change ID
  st.LockEprom(2); // Lock EPROM-SAFE
  */
  while (1)
  {
  //HAL_UART_Transmit_IT(&huart6, (uint8_t *)"Hello World\n", 12);
    

    //Temps de 2,2 s 
    if(timerInt >= 22)
		{
      timerInt=0;
      //led2.ledToggle();
      //st.WheelMode(1);
      
      
      if(test==0)
      {
        if(st.WritePosEx(ID, 1000, 1500, 50)==1)
        {
          led2.ledOn();
          test=1;
        }
        
      }
      else{
        
        if(st.WritePosEx(ID, 20, 1500, 50)==1)
        {
          test=0;
          led2.ledOff(); 
        }
         
      }
      

      /*
      if(test==0)
      {
        ID = st.Ping(1);//If you ping the servo with that ID and receive no response, it will return -1.
        if(ID!=-1){
          led2.ledOn();
          test=1;
        }else{
          if(test==0)
            led2.ledOff();
        }
      }*/
      /*
      else
      {
        st.WritePosEx(1, 1000, 1500, 50);
      }*/
      
      //HAL_UART_Transmit_IT(&huart6, test1, 7);
      
      /*
      test=servo1.servoPing();
      if (test==1)
      {
        led2.ledOn();
      }
      else
      {
        led2.ledOff();
      }*/
    
    }

    /*
    if(gpioC.readPin(13)==LOW)
    {
      test=servo1.servoPing();
      led2.ledToggle();
    }*/
    
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

 
}

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
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 1000000;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 1000000;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 49999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
  
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  
    timerInt++;
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
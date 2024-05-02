#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>  ///< Standard library
#include <stdint.h>  ///< Std types
#include <stdbool.h> ///< _Bool to bool

#include "stm32f4xx_hal.h"


void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB



void Error_Handler(void);

#endif // MAIN_H
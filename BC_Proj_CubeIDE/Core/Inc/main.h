/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GLCD.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED8_Pin GPIO_PIN_13
#define LED8_GPIO_Port GPIOC
#define DS18B20_Pin GPIO_PIN_6
#define DS18B20_GPIO_Port GPIOA
#define TOUCHPWR_Pin GPIO_PIN_10
#define TOUCHPWR_GPIO_Port GPIOB
#define TOUCH8_Pin GPIO_PIN_8
#define TOUCH8_GPIO_Port GPIOD
#define TOUCH6_Pin GPIO_PIN_9
#define TOUCH6_GPIO_Port GPIOD
#define TOUCH4_Pin GPIO_PIN_10
#define TOUCH4_GPIO_Port GPIOD
#define TOUCH2_Pin GPIO_PIN_11
#define TOUCH2_GPIO_Port GPIOD
#define TOUCH1_Pin GPIO_PIN_12
#define TOUCH1_GPIO_Port GPIOD
#define TOUCH7_Pin GPIO_PIN_13
#define TOUCH7_GPIO_Port GPIOD
#define TOUCH5_Pin GPIO_PIN_14
#define TOUCH5_GPIO_Port GPIOD
#define TOUCH3_Pin GPIO_PIN_15
#define TOUCH3_GPIO_Port GPIOD
#define LED7_Pin GPIO_PIN_9
#define LED7_GPIO_Port GPIOC
#define LED5_Pin GPIO_PIN_8
#define LED5_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOD
#define LCD_BACKLIGHT_Pin GPIO_PIN_2
#define LCD_BACKLIGHT_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOD
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOD
#define LED6_Pin GPIO_PIN_7
#define LED6_GPIO_Port GPIOD
#define LEDENABLE_Pin GPIO_PIN_4
#define LEDENABLE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

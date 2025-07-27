/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define BUTTON3_Pin GPIO_PIN_13
#define BUTTON3_GPIO_Port GPIOC
#define BUTTON3_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON2_Pin GPIO_PIN_14
#define BUTTON2_GPIO_Port GPIOC
#define BUTTON2_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON1_Pin GPIO_PIN_15
#define BUTTON1_GPIO_Port GPIOC
#define BUTTON1_EXTI_IRQn EXTI4_15_IRQn
#define NTC_ADC1_IN1_Pin GPIO_PIN_1
#define NTC_ADC1_IN1_GPIO_Port GPIOA
#define BUZZER_PWM_Pin GPIO_PIN_0
#define BUZZER_PWM_GPIO_Port GPIOD
#define LED_HEATING_Pin GPIO_PIN_1
#define LED_HEATING_GPIO_Port GPIOD
#define LED_LIGHT_Pin GPIO_PIN_3
#define LED_LIGHT_GPIO_Port GPIOD
#define LED_ALIVE_Pin GPIO_PIN_4
#define LED_ALIVE_GPIO_Port GPIOB
#define HEATER_PWM_Pin GPIO_PIN_9
#define HEATER_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

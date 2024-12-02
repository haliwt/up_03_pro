/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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


//KEY_GPIO
//#define KEY_TIMER_Pin               GPIO_PIN_11
//#define KEY_TIMER_GPIO_Port         GPIOA

//ADC_FAN
#define ADC_FAN_Pin GPIO_PIN_0
#define ADC_FAN_GPIO_Port GPIOA

//CTL_GPIO
#define PLASMA_Pin GPIO_PIN_4
#define PLASMA_GPIO_Port GPIOA

#define ULTR_PWM_Pin GPIO_PIN_3
#define ULTR_PWM_GPIO_Port GPIOB

#define FAN_CTL_Pin GPIO_PIN_7
#define FAN_CTL_GPIO_Port GPIOB

//VOICE_GPIO
#define VOICE_MUTE_Pin GPIO_PIN_5
#define VOICE_MUTE_GPIO_Port GPIOA


//KEY GPIO
#define RF_KEY_Pin             GPIO_PIN_6
#define RF_KEY_GPIO_Port       GPIOA

#define KEY_POWER_Pin                       GPIO_PIN_12
#define KEY_POWER_GPIO_Port                 GPIOA


#define GPIO_2_Pin GPIO_PIN_7
#define GPIO_2_GPIO_Port GPIOA
#define GPIO_1_Pin GPIO_PIN_0
#define GPIO_1_GPIO_Port GPIOB



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

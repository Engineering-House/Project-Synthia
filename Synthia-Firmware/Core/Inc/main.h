/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define DAC_0_Pin GPIO_PIN_0
#define DAC_0_GPIO_Port GPIOF
#define DAC_1_Pin GPIO_PIN_1
#define DAC_1_GPIO_Port GPIOF
#define DAC_3_Pin GPIO_PIN_0
#define DAC_3_GPIO_Port GPIOA
#define DAC_4_Pin GPIO_PIN_1
#define DAC_4_GPIO_Port GPIOA
#define DAC_5_Pin GPIO_PIN_4
#define DAC_5_GPIO_Port GPIOA
#define DAC_6_Pin GPIO_PIN_5
#define DAC_6_GPIO_Port GPIOA
#define DAC_7_Pin GPIO_PIN_6
#define DAC_7_GPIO_Port GPIOA
#define majorChords_Pin GPIO_PIN_7
#define majorChords_GPIO_Port GPIOA
#define minorChords_Pin GPIO_PIN_0
#define minorChords_GPIO_Port GPIOB
#define DAC_2_Pin GPIO_PIN_8
#define DAC_2_GPIO_Port GPIOA
#define seventhChords_Pin GPIO_PIN_9
#define seventhChords_GPIO_Port GPIOA
#define rythymSwitch_Pin GPIO_PIN_10
#define rythymSwitch_GPIO_Port GPIOA
#define rythymMatrix4_Pin GPIO_PIN_11
#define rythymMatrix4_GPIO_Port GPIOA
#define fontSwitch_Pin GPIO_PIN_12
#define fontSwitch_GPIO_Port GPIOA
#define fontMatrix4_Pin GPIO_PIN_15
#define fontMatrix4_GPIO_Port GPIOA
#define chordMatrix_0_Pin GPIO_PIN_4
#define chordMatrix_0_GPIO_Port GPIOB
#define chordMatrix_1_Pin GPIO_PIN_5
#define chordMatrix_1_GPIO_Port GPIOB
#define chordMatrix_2_Pin GPIO_PIN_6
#define chordMatrix_2_GPIO_Port GPIOB
#define chordMatrix_3_Pin GPIO_PIN_7
#define chordMatrix_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

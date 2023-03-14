/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
void   MX_I2C1_Init(void);
void   MX_ADC1_Init(void);
void   MX_I2S3_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_I2C_SPI_Pin GPIO_PIN_3
#define CS_I2C_SPI_GPIO_Port GPIOE
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define OTG_FS_PowerSwitchOn_Pin GPIO_PIN_0
#define OTG_FS_PowerSwitchOn_GPIO_Port GPIOC
#define MIXER_FM_DF_Pin GPIO_PIN_1
#define MIXER_FM_DF_GPIO_Port GPIOC
#define MIXER_PWM_Pin GPIO_PIN_2
#define MIXER_PWM_GPIO_Port GPIOC
#define MIXER_OSC1_FM_Pin GPIO_PIN_3
#define MIXER_OSC1_FM_GPIO_Port GPIOC
#define MIXER_OSC_PMF_Pin GPIO_PIN_0
#define MIXER_OSC_PMF_GPIO_Port GPIOA
#define MIXER_FILTER_LOW_Pin GPIO_PIN_1
#define MIXER_FILTER_LOW_GPIO_Port GPIOA
#define MIXER_FILTER_HIGH_Pin GPIO_PIN_2
#define MIXER_FILTER_HIGH_GPIO_Port GPIOA
#define MIXER_GAIN_Pin GPIO_PIN_3
#define MIXER_GAIN_GPIO_Port GPIOA
#define I2S3_WS_Pin GPIO_PIN_4
#define I2S3_WS_GPIO_Port GPIOA
#define MIXER_OSC2_FM_Pin GPIO_PIN_5
#define MIXER_OSC2_FM_GPIO_Port GPIOA
#define MIXER_ATTACK_Pin GPIO_PIN_6
#define MIXER_ATTACK_GPIO_Port GPIOA
#define MIXER_DECAY_Pin GPIO_PIN_7
#define MIXER_DECAY_GPIO_Port GPIOA
#define MIXER_OSC1_PM_BETA_Pin GPIO_PIN_4
#define MIXER_OSC1_PM_BETA_GPIO_Port GPIOC
#define MIXER_OSC2_PM_BETA_Pin GPIO_PIN_5
#define MIXER_OSC2_PM_BETA_GPIO_Port GPIOC
#define MIXER_SUSTAIN_Pin GPIO_PIN_0
#define MIXER_SUSTAIN_GPIO_Port GPIOB
#define MIXER_RELEASE_Pin GPIO_PIN_1
#define MIXER_RELEASE_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define TEST_PIN_Pin GPIO_PIN_11
#define TEST_PIN_GPIO_Port GPIOE
#define CLK_IN_Pin GPIO_PIN_10
#define CLK_IN_GPIO_Port GPIOB
#define BUTTON_PHASE_MULT_Pin GPIO_PIN_10
#define BUTTON_PHASE_MULT_GPIO_Port GPIOD
#define BUTTON_PHASE_MULT_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_OSC3_CYCLE_Pin GPIO_PIN_11
#define BUTTON_OSC3_CYCLE_GPIO_Port GPIOD
#define BUTTON_OSC3_CYCLE_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_OSC2_CYCLE_Pin GPIO_PIN_12
#define BUTTON_OSC2_CYCLE_GPIO_Port GPIOD
#define BUTTON_OSC2_CYCLE_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_OSC1_CYCLE_Pin GPIO_PIN_13
#define BUTTON_OSC1_CYCLE_GPIO_Port GPIOD
#define BUTTON_OSC1_CYCLE_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_OUT_WAVE_CYCLE_Pin GPIO_PIN_14
#define BUTTON_OUT_WAVE_CYCLE_GPIO_Port GPIOD
#define BUTTON_OUT_WAVE_CYCLE_EXTI_IRQn EXTI15_10_IRQn
#define I2S3_MCK_Pin GPIO_PIN_7
#define I2S3_MCK_GPIO_Port GPIOC
#define VBUS_FS_Pin GPIO_PIN_9
#define VBUS_FS_GPIO_Port GPIOA
#define OTG_FS_ID_Pin GPIO_PIN_10
#define OTG_FS_ID_GPIO_Port GPIOA
#define OTG_FS_DM_Pin GPIO_PIN_11
#define OTG_FS_DM_GPIO_Port GPIOA
#define OTG_FS_DP_Pin GPIO_PIN_12
#define OTG_FS_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define I2S3_SCK_Pin GPIO_PIN_10
#define I2S3_SCK_GPIO_Port GPIOC
#define I2S3_SD_Pin GPIO_PIN_12
#define I2S3_SD_GPIO_Port GPIOC
#define Audio_RST_Pin GPIO_PIN_4
#define Audio_RST_GPIO_Port GPIOD
#define OTG_FS_OverCurrent_Pin GPIO_PIN_5
#define OTG_FS_OverCurrent_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Audio_SCL_Pin GPIO_PIN_6
#define Audio_SCL_GPIO_Port GPIOB
#define Audio_SDA_Pin GPIO_PIN_9
#define Audio_SDA_GPIO_Port GPIOB
#define MEMS_INT2_Pin GPIO_PIN_1
#define MEMS_INT2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

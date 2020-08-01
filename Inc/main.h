/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

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
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define ALRM_Pin GPIO_PIN_0
#define ALRM_GPIO_Port GPIOF
#define ALRM_EXTI_IRQn EXTI0_IRQn
#define BRKR_Pin GPIO_PIN_1
#define BRKR_GPIO_Port GPIOF
#define TSPD_Pin GPIO_PIN_2
#define TSPD_GPIO_Port GPIOF
#define SPD0_Pin GPIO_PIN_4
#define SPD0_GPIO_Port GPIOF
#define SRDY_Pin GPIO_PIN_5
#define SRDY_GPIO_Port GPIOF
#define ZSPD_Pin GPIO_PIN_10
#define ZSPD_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define SPDLM_Pin GPIO_PIN_2
#define SPDLM_GPIO_Port GPIOC
#define PL_Pin GPIO_PIN_0
#define PL_GPIO_Port GPIOA
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_6
#define A1_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define EMGS_Pin GPIO_PIN_0
#define EMGS_GPIO_Port GPIOB
#define SON_Pin GPIO_PIN_1
#define SON_GPIO_Port GPIOB
#define ARST_Pin GPIO_PIN_2
#define ARST_GPIO_Port GPIOB
#define CTHT1_Pin GPIO_PIN_9
#define CTHT1_GPIO_Port GPIOE
#define CTHT2_Pin GPIO_PIN_11
#define CTHT2_GPIO_Port GPIOE
#define STATUS_Pin GPIO_PIN_12
#define STATUS_GPIO_Port GPIOE
#define ENTER_Pin GPIO_PIN_14
#define ENTER_GPIO_Port GPIOE
#define ENTER_EXTI_IRQn EXTI15_10_IRQn
#define CLOSE_Pin GPIO_PIN_15
#define CLOSE_GPIO_Port GPIOE
#define CLOSE_EXTI_IRQn EXTI15_10_IRQn
#define DOWN_Pin GPIO_PIN_10
#define DOWN_GPIO_Port GPIOB
#define DOWN_EXTI_IRQn EXTI15_10_IRQn
#define UP_Pin GPIO_PIN_11
#define UP_GPIO_Port GPIOB
#define UP_EXTI_IRQn EXTI15_10_IRQn
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define NL_Pin GPIO_PIN_11
#define NL_GPIO_Port GPIOD
#define A2_Pin GPIO_PIN_12
#define A2_GPIO_Port GPIOD
#define B2_Pin GPIO_PIN_13
#define B2_GPIO_Port GPIOD
#define RW_Pin GPIO_PIN_2
#define RW_GPIO_Port GPIOG
#define EN_Pin GPIO_PIN_3
#define EN_GPIO_Port GPIOG
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define B1_Pin GPIO_PIN_7
#define B1_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define HOME_Pin GPIO_PIN_0
#define HOME_GPIO_Port GPIOD
#define RS_Pin GPIO_PIN_2
#define RS_GPIO_Port GPIOD
#define D4_Pin GPIO_PIN_3
#define D4_GPIO_Port GPIOD
#define D5_Pin GPIO_PIN_4
#define D5_GPIO_Port GPIOD
#define D6_Pin GPIO_PIN_5
#define D6_GPIO_Port GPIOD
#define D7_Pin GPIO_PIN_6
#define D7_GPIO_Port GPIOD
#define D8_Pin GPIO_PIN_7
#define D8_GPIO_Port GPIOD
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SPD1_Pin GPIO_PIN_6
#define SPD1_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

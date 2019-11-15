/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Platform config -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "Macro.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#include "Custom_USART_Device.h"

#include "Custom_Button.h"

#include "Custom_SPI_Device.h"

/* Define config -------------------------------------------------------------*/
/* Macro ---------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
extern uint8_t IsBTNPress;

void LED_Config(void);

void Button_Procedure(void);
void Polling_Procedure(void);

void TIM2_Config(void);
void TIM3_Config(void);

void USART_Test(void);
void USART_Config(void);

void SysTickConfig(void);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t uTime);
/* Exported constants --------------------------------------------------------*/

#endif  /* __HW_CONFIG_H */


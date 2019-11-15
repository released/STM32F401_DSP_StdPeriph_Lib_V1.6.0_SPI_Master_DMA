/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_USART_DEVICE_H
#define __CUSTOM_USART_DEVICE_H

/* Platform config -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include "string.h"

#include "Macro.h"
#include "Custom_SPI_Device.h"

/* Define config -------------------------------------------------------------*/
#define USART_DEVICE							USART2
#define USART_DEVICE_CLK						RCC_APB1Periph_USART2
#define USART_DEVICE_CLK_INIT             		RCC_APB1PeriphClockCmd 

#define USART_DEVICE_TX_PIN					GPIO_Pin_2
#define USART_DEVICE_TX_GPIO_PORT				GPIOA
#define USART_DEVICE_TX_GPIO_SOURCE			GPIO_PinSource2
#define USART_DEVICE_TX_GPIO_AF				GPIO_AF_USART2
#define USART_DEVICE_TX_GPIO_CLK				RCC_AHB1Periph_GPIOA

#define USART_DEVICE_RX_PIN 					GPIO_Pin_3
#define USART_DEVICE_RX_GPIO_PORT				GPIOA
#define USART_DEVICE_RX_GPIO_SOURCE			GPIO_PinSource3
#define USART_DEVICE_RX_GPIO_AF				GPIO_AF_USART2
#define USART_DEVICE_RX_GPIO_CLK				RCC_AHB1Periph_GPIOA

#define USART_DEVICE_IRQn						USART2_IRQn
#define USART_DEVICE_Handler					USART2_IRQHandler

/* Macro ---------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
void Custom_USART_DEVICE_TestCommand(void);
void Custom_USART_DEVICE_Config(void);

/* Exported constants --------------------------------------------------------*/

#endif  /* __CUSTOM_USART_DEVICE_H */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_SPI_DEVICE_H
#define __CUSTOM_SPI_DEVICE_H

/* Platform config -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include "string.h"

/* Define config -------------------------------------------------------------*/

#define MASTER_SPI_BAUDRATE				(SPI_BaudRatePrescaler_16)
#define DataSize								(1)
#define CheckSumSize							(0)

#define SPI_DEVICE							SPI1
#define SPI_DEVICE_CLK						RCC_APB2Periph_SPI1
#define SPI_DEVICE_CLK_INIT             		RCC_APB2PeriphClockCmd 

#define SPI_DEVICE_SCK_PIN					GPIO_Pin_3
#define SPI_DEVICE_SCK_GPIO_PORT				GPIOB
#define SPI_DEVICE_SCK_GPIO_SOURCE			GPIO_PinSource3
#define SPI_DEVICE_SCK_GPIO_AF				GPIO_AF_SPI1
#define SPI_DEVICE_SCK_GPIO_CLK				RCC_AHB1Periph_GPIOB

#define SPI_DEVICE_MISO_PIN 					GPIO_Pin_4
#define SPI_DEVICE_MISO_GPIO_PORT			GPIOB
#define SPI_DEVICE_MISO_GPIO_SOURCE			GPIO_PinSource4
#define SPI_DEVICE_MISO_GPIO_AF				GPIO_AF_SPI1
#define SPI_DEVICE_MISO_GPIO_CLK				RCC_AHB1Periph_GPIOB

#define SPI_DEVICE_MOSI_PIN					GPIO_Pin_5
#define SPI_DEVICE_MOSI_GPIO_PORT			GPIOB
#define SPI_DEVICE_MOSI_GPIO_SOURCE			GPIO_PinSource5
#define SPI_DEVICE_MOSI_GPIO_AF				GPIO_AF_SPI1
#define SPI_DEVICE_MOSI_GPIO_CLK				RCC_AHB1Periph_GPIOB

//GPIO
#define SPI_DEVICE_CS_PIN					GPIO_Pin_10
#define SPI_DEVICE_CS_GPIO_PORT				GPIOB
#define SPI_DEVICE_CS_GPIO_CLK				RCC_AHB1Periph_GPIOB

#define SPI_NCS_LOW(void)       				(GPIO_ResetBits(SPI_DEVICE_CS_GPIO_PORT, SPI_DEVICE_CS_PIN))
#define SPI_NCS_HIGH(void)      				(GPIO_SetBits(SPI_DEVICE_CS_GPIO_PORT, SPI_DEVICE_CS_PIN))  


#define Custom_SPI_DEVICE_Master_DMATx_IRQHandler    DMA2_Stream3_IRQHandler
#define Custom_SPI_DEVICE_Master_DMARx_IRQHandler    DMA2_Stream2_IRQHandler
/* Macro ---------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
extern uint8_t DataBuffer[8];


void Custom_SPI_DEVICE_TestCommand(void);
void Custom_SPI_DEVICE_Master_Config(void);


void SPI_WriteByte(uint8_t Data);
void SPI_DMABufferStart(uint8_t pBuffer, uint16_t NumByteToWrite);
void SPI_DMABufferWait(void);

/* Exported constants --------------------------------------------------------*/

#endif  /* __CUSTOM_SPI_DEVICE_H */


/* Includes ------------------------------------------------------------------*/
#include "Custom_USART_DEVICE.h"

/* Private typedef -----------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)  
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)  
#endif /* __GNUC__ */

// TODO: for printf function , need to confirm use USART2 or USART2
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART_DEVICE, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART_DEVICE, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


/* Private define ------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void Custom_USART_DEVICE_TestCommand(void)
{
	__IO uint8_t temp;
	
	if(USART_GetFlagStatus(USART_DEVICE, USART_FLAG_RXNE) == SET)
	{
			temp = USART_ReceiveData(USART_DEVICE);
			printf("temp = 0x%x \n\r",temp);

			switch (temp)
			{

				default : 
					printf("INPUT CMD not support !\r\n");
					break;
			}

	}
}

void Custom_USART_DEVICE_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(USART_DEVICE_TX_GPIO_CLK|USART_DEVICE_RX_GPIO_CLK, ENABLE);

	/* Enable UART clock */
	USART_DEVICE_CLK_INIT(USART_DEVICE_CLK, ENABLE);

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(USART_DEVICE_TX_GPIO_PORT,USART_DEVICE_TX_GPIO_SOURCE,USART_DEVICE_TX_GPIO_AF);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(USART_DEVICE_RX_GPIO_PORT,USART_DEVICE_RX_GPIO_SOURCE,USART_DEVICE_RX_GPIO_AF);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = USART_DEVICE_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_DEVICE_TX_GPIO_PORT, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = USART_DEVICE_RX_PIN;
	GPIO_Init(USART_DEVICE_TX_GPIO_PORT, &GPIO_InitStructure);

	/* USARTx configured as follows:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(USART_DEVICE, &USART_InitStructure);

	USART_ITConfig(USART_DEVICE, USART_IT_RXNE, ENABLE); 

	while (USART_GetFlagStatus(USART_DEVICE, USART_FLAG_TC) == RESET)
	{}
	
	/* NVIC configuration */
	/* Enable the USARRx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART_DEVICE_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART_DEVICE, ENABLE);


	printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");


}



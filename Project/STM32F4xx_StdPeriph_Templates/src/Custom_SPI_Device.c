/* Includes ------------------------------------------------------------------*/
#include "Custom_SPI_DEVICE.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*SPI variable*/
uint8_t Dummy=0x5A;

uint8_t DataTxBuffer[DataSize+CheckSumSize] = {0};	
uint8_t DataRxBuffer[DataSize+CheckSumSize] = {0};	

uint8_t Flag_DataReady = 1;
DMA_InitTypeDef 	DMA_InitStructure_IT;	
/* Private functions ---------------------------------------------------------*/
static void Delay(uint32_t u32Delay)
{
	//uint32_t i;
	//i = 0xffff;
	while(u32Delay--);
}

void Custom_SPI_DEVICE_TestCommand(void)
{
	#if 1
	static uint16_t data = 0;

	data = (data++ >= 0xFF) ? (0) : (data) ; 
	SPI_WriteByte(data);

	#else	
	SPI_WriteByte(0xFA);
	Delay(5);
	SPI_WriteByte(0xF1);
	Delay(5);
	SPI_WriteByte(0xF2);
	Delay(5);
	SPI_WriteByte(0xF3);
	Delay(5);
	SPI_WriteByte(0xF4);
	Delay(5);	
	#endif

}

void Custom_SPI_DEVICE_Master_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	SPI_InitTypeDef  	SPI_InitStructure;
  
	/* Enable the SPI periph */
	SPI_DEVICE_CLK_INIT(SPI_DEVICE_CLK, ENABLE);

	/* Enable SCK, MOSI, MISO and NSS GPIO clocks */
	RCC_AHB1PeriphClockCmd(SPI_DEVICE_CS_GPIO_CLK | SPI_DEVICE_MOSI_GPIO_CLK | SPI_DEVICE_MISO_GPIO_CLK |
	                     SPI_DEVICE_SCK_GPIO_CLK , ENABLE);

	GPIO_PinAFConfig(SPI_DEVICE_SCK_GPIO_PORT, SPI_DEVICE_SCK_GPIO_SOURCE, SPI_DEVICE_SCK_GPIO_AF);			//CLK
	GPIO_PinAFConfig(SPI_DEVICE_MISO_GPIO_PORT, SPI_DEVICE_MISO_GPIO_SOURCE, SPI_DEVICE_MISO_GPIO_AF);   	//MISO
	GPIO_PinAFConfig(SPI_DEVICE_MOSI_GPIO_PORT, SPI_DEVICE_MOSI_GPIO_SOURCE, SPI_DEVICE_MOSI_GPIO_AF);		//MOSI

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	/*!< Configure SPI_DEVICE_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = SPI_DEVICE_SCK_PIN;
	GPIO_Init(SPI_DEVICE_SCK_GPIO_PORT, &GPIO_InitStructure);
	/*!< Configure SPI_DEVICE_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin =  SPI_DEVICE_MISO_PIN;
	GPIO_Init(SPI_DEVICE_MISO_GPIO_PORT, &GPIO_InitStructure);  
	/*!< Configure SPI_DEVICE_SPI pins: MOSI */
	GPIO_InitStructure.GPIO_Pin =  SPI_DEVICE_MOSI_PIN;
	GPIO_Init(SPI_DEVICE_MOSI_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = SPI_DEVICE_CS_PIN;
	GPIO_Init(SPI_DEVICE_CS_GPIO_PORT, &GPIO_InitStructure);

	SPI_NCS_HIGH();
	
	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI_DEVICE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	//SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = MASTER_SPI_BAUDRATE;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_DEVICE, &SPI_InitStructure);

	SPI_Cmd(SPI_DEVICE, ENABLE); /*!< SPI_DEVICE_SPI enable */

}


void SPI_WriteByte(uint8_t Data)
{
#if 1	//use DMA to transfer data
	Delay(1);
	SPI_NCS_LOW();	
	
//	SPI_DMABufferStart(~(Data),1);
	SPI_DMABufferStart((Data),1);	//DEBUG
	
	SPI_DMABufferWait();	

	Delay(1);	
	SPI_NCS_HIGH();	
	
#else	//use regular SPI method to transfer data
//	Delay(1);
	SPI_NCS_LOW();	
	
	while (SPI_I2S_GetFlagStatus(SPI_DEVICE, SPI_I2S_FLAG_TXE) == RESET)
	{};
	SPI_I2S_SendData(SPI_DEVICE,Data);
//	SPI_I2S_SendData(SPI_DEVICE, ~(Data));
	
//	while (SPI_I2S_GetFlagStatus(SPI_DEVICE, SPI_I2S_FLAG_RXNE) == RESET)
//	{};
//	SPI_I2S_ReceiveData(SPI_DEVICE);

//	Delay(1);	
	SPI_NCS_HIGH();
#endif
}

void SPI_DMABufferStart(uint8_t pBuffer, uint16_t NumByteToWrite)
{                                                                                                      
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);	
	DMA_DeInit(DMA2_Stream3);//TX
	
	DMA_InitStructure.DMA_BufferSize = NumByteToWrite;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable ;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPI_DEVICE->DR)); 

	/* Configure TX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3 ;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;	
	DMA_InitStructure.DMA_Memory0BaseAddr =	(uint32_t)&pBuffer;
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);                                                       
	// ------------------------------------------------------------------------------------------------------------ //
	DMA_Cmd(DMA2_Stream3,ENABLE);                                                                      
	// ------------------------------------------------------------------------------------------------------------ //
	SPI_I2S_DMACmd(SPI_DEVICE, SPI_I2S_DMAReq_Tx, ENABLE);                                                   

}                                                                                                

void SPI_DMABufferWait(void)
{        
	while (DMA_GetFlagStatus(DMA2_Stream3,DMA_FLAG_TCIF3)==RESET);                                     

	DMA_ClearFlag(DMA2_Stream3,DMA_FLAG_TCIF3);                                                        
	SPI_I2S_ClearFlag(SPI_DEVICE,SPI_I2S_FLAG_TXE);                                                          

	DMA_Cmd(DMA2_Stream3,DISABLE);                                                                     

	SPI_I2S_DMACmd(SPI_DEVICE, SPI_I2S_DMAReq_Tx, DISABLE);                                                  
}                                                                                                      


uint8_t Get_CheckSum(uint8_t data[],uint32_t len)
{
	int i,res=0;
	for(i=0;i<len;i++)
	{
		res += data[i];
	}
	return (uint8_t)(0xFF - res);
}



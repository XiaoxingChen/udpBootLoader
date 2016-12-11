/********************************************************************************
* @file    CUartConsole.cpp
* @author  Chenxx
* @version V1.0
* @date    2016-04-21
* @brief   this file defines the Console function that can printf with uart.
*	   This is for STM32F4.
********************************************************************************/
#include "CUartConsole.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#if CONSOLE_TX_USE_DMA
char CUartConsole::TxDmaBuf_[TX_DMA_SIZE];	//for txDma
#endif //CONSOLE_TX_DMAST
char CUartConsole::TxBuf_[TXBUF_SIZE];		//buffer for None DMA Mode txQueue_
char CUartConsole::vsnprintfBuf_[TXBUF_SIZE];	//for sprintf

#ifdef CONSOLE_RX_DMAST
char CUartConsole::RxDmaBuf_[RX_DMA_SIZE];	//for rxDma
#endif //CONSOLE_RX_DMAST
char CUartConsole::RxBuf_[RXBUF_SIZE];			//for rxQueue_

/**
  * @brief  Constructor
	* @param  None
  * @retval None
  */
CUartConsole::CUartConsole()
	:txQueue_(TxBuf_, TXBUF_SIZE),
	rxQueue_(RxBuf_, RXBUF_SIZE),
	overflowCounter_(0)
{
#if CONSOLE_SILENT
	return ;
#endif
	
#ifdef CONSOLE_TX_DMAST
	DMA_TypeDef * txDMAx = (DMA_TypeDef *)((uint32_t)CONSOLE_TX_DMAST&0xFFFFFC00);
	uint32_t txStreamx = (((uint32_t)CONSOLE_TX_DMAST&0xFF) - 0x10)/0x18;	// 0-7
	
	if(txStreamx < 4) TXDMA_IFCR_ = &(txDMAx->LIFCR);
	else TXDMA_IFCR_ = &(txDMAx->HIFCR);
	switch(txStreamx % 4)
	{
		case 0:
			TXDMA_IFCR_MASK = 1<<5;
			break;
		case 1:
			TXDMA_IFCR_MASK = 1<<11;
			break;
		case 2:
			TXDMA_IFCR_MASK = 1<<21;
			break;
		case 3:
			TXDMA_IFCR_MASK = 1<<27;
			break;
	}
#endif
	
#ifdef CONSOLE_RX_DMAST
	DMA_TypeDef * rxDMAx = (DMA_TypeDef *)((uint32_t)CONSOLE_RX_DMAST&0xFFFFFC00);
	uint32_t rxStreamx = (((uint32_t)CONSOLE_RX_DMAST&0xFF) - 0x10)/0x18;	// 0-7
	
	if(rxStreamx < 4) RXDMA_IFCR_ = &(rxDMAx->LIFCR);
	else RXDMA_IFCR_ = &(rxDMAx->HIFCR);
	switch(rxStreamx % 4)
	{
		case 0:
			RXDMA_IFCR_MASK = 1<<5;
			break;
		case 1:
			RXDMA_IFCR_MASK = 1<<11;
			break;
		case 2:
			RXDMA_IFCR_MASK = 1<<21;
			break;
		case 3:
			RXDMA_IFCR_MASK = 1<<27;
			break;
	}
#endif
	InitSciGpio();
	InitSci();
	
}
/**
  * @brief  Initialize the UART GPIO
  * @param  None
  * @retval None
  */
void CUartConsole::InitSciGpio()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t RCC_AHB1Periph_GPIOx;
	uint8_t GPIO_PinSource_Tx;
	uint8_t GPIO_PinSource_Rx;
	uint8_t GPIO_AF_USARTx;
	GPIO_TypeDef *GPIOx_Tx;
	GPIO_TypeDef *GPIOx_Rx;
	
	#if CONSOLE_IOGROUP_A2
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA;
		GPIOx_Tx = GPIOx_Rx = GPIOA;
		GPIO_PinSource_Tx = GPIO_PinSource2;
		GPIO_PinSource_Rx = GPIO_PinSource3;
	
	#elif CONSOLE_IOGROUP_A9
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA;
		GPIOx_Tx = GPIOx_Rx = GPIOA;
		GPIO_PinSource_Tx = GPIO_PinSource9;
		GPIO_PinSource_Rx = GPIO_PinSource10;
	
	#elif CONSOLE_IOGROUP_B10
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOB;
		GPIOx_Tx = GPIOx_Rx = GPIOB;
		GPIO_PinSource_Tx = GPIO_PinSource10;
		GPIO_PinSource_Rx = GPIO_PinSource11;
		
	#elif CONSOLE_IOGROUP_C10
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC;
		GPIOx_Tx = GPIOx_Rx = GPIOC;
		GPIO_PinSource_Tx = GPIO_PinSource10;
		GPIO_PinSource_Rx = GPIO_PinSource11;
	
	#elif CONSOLE_IOGROUP_C12D2
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC;
		GPIOx_Tx = GPIOC;
		GPIOx_Rx = GPIOD;
		GPIO_PinSource_Tx = GPIO_PinSource12;
		GPIO_PinSource_Rx = GPIO_PinSource2;
		
	#elif CONSOLE_IOGROUP_C6
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC;
		GPIOx_Tx = GPIOx_Rx = GPIOC;
		GPIO_PinSource_Tx = GPIO_PinSource6;
		GPIO_PinSource_Rx = GPIO_PinSource7;
		
	#else 
		#error
	#endif
	
	#if  CONSOLE_USE_UART1
		GPIO_AF_USARTx = GPIO_AF_USART1;
	#elif CONSOLE_USE_UART2	
		GPIO_AF_USARTx = GPIO_AF_USART2;
	#elif CONSOLE_USE_UART3	
		GPIO_AF_USARTx = GPIO_AF_USART3;
	#elif CONSOLE_USE_UART4	
		GPIO_AF_USARTx = GPIO_AF_UART4;
	#elif CONSOLE_USE_UART5	
		GPIO_AF_USARTx = GPIO_AF_UART5;
	#elif CONSOLE_USE_UART6	
		GPIO_AF_USARTx = GPIO_AF_USART6;
	#else 
		#error
	#endif 

	/* open clock of UART */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
	
	/* Config Pin: RXD TXD */
	GPIO_PinAFConfig(GPIOx_Tx, GPIO_PinSource_Tx, GPIO_AF_USARTx);
	GPIO_PinAFConfig(GPIOx_Rx, GPIO_PinSource_Rx, GPIO_AF_USARTx);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0<< GPIO_PinSource_Tx;
	GPIO_Init(GPIOx_Tx, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0<< GPIO_PinSource_Rx;
	GPIO_Init(GPIOx_Rx, &GPIO_InitStructure);
}

/**
  * @brief  Initialize the UART
  * @param  None
  * @retval None
  */
void CUartConsole::InitSci()
{
	/* init clock of USART */
	#if  CONSOLE_USE_UART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	#elif  CONSOLE_USE_UART2 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	#elif  CONSOLE_USE_UART3  
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	#elif  CONSOLE_USE_UART4  
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	#elif  CONSOLE_USE_UART5  
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	#elif  CONSOLE_USE_UART6  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	#else 
		#error
	#endif
	
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_Init(CONSOLE_UART, &USART_InitStructure); 

	USART_Cmd(CONSOLE_UART, ENABLE); 
	
	USART_DMACmd(CONSOLE_UART, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(CONSOLE_UART, USART_DMAReq_Rx, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	
#if (defined CONSOLE_TX_DMAST) || (defined CONSOLE_RX_DMAST)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	if(CONSOLE_UART == USART6)
		DMA_InitStructure.DMA_Channel = DMA_Channel_5; //attention!
	else
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; //attention!
#endif

#ifdef CONSOLE_TX_DMAST
	/* DMA Clock Config */
	uint32_t txRCC_AHB1Periph;
	DMA_TypeDef * txDMAx = (DMA_TypeDef *)((uint32_t)CONSOLE_TX_DMAST&0xFFFFFC00);
	if(txDMAx == DMA1) txRCC_AHB1Periph = RCC_AHB1Periph_DMA1;
	else if(txDMAx == DMA2) txRCC_AHB1Periph = RCC_AHB1Periph_DMA2;
	else while(1); //error
	RCC_AHB1PeriphClockCmd(txRCC_AHB1Periph, ENABLE);
	
	/* TX DMA Config */	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(CONSOLE_UART->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TxDmaBuf_;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 0;
	
	DMA_Cmd(CONSOLE_TX_DMAST, DISABLE);
	DMA_Init(CONSOLE_TX_DMAST, &DMA_InitStructure);
#endif

#ifdef CONSOLE_RX_DMAST
	/* DMA Clock Config */
	DMA_TypeDef * rxDMAx = (DMA_TypeDef *)((uint32_t)CONSOLE_RX_DMAST&0xFFFFFC00);
	uint32_t rxRCC_AHB1Periph;
	if(rxDMAx == DMA1) rxRCC_AHB1Periph = RCC_AHB1Periph_DMA1;
	else if(rxDMAx == DMA2) rxRCC_AHB1Periph = RCC_AHB1Periph_DMA2;
	else while(1); //error
	RCC_AHB1PeriphClockCmd(rxRCC_AHB1Periph, ENABLE);
		
	/* RX DMA Config */	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(CONSOLE_UART->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RxDmaBuf_;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = RX_DMA_SIZE;
	
	DMA_Cmd(CONSOLE_RX_DMAST, DISABLE);
	*RXDMA_IFCR_ = RXDMA_IFCR_MASK;
	DMA_Init(CONSOLE_RX_DMAST, &DMA_InitStructure);
	DMA_Cmd(CONSOLE_RX_DMAST, ENABLE);
#endif
}


/**
  * @brief  do nothing to avoid the warning of compiler
  * @param  None
  * @retval always be 0
  */
#if CONSOLE_SILENT
int CUartConsole::printf(const char* fmt, ...)
{
	return 0;
}

#else // !CONSOLE_SILENT

/**
  * @brief  printf a string without DMA controller.
	*					User should call the CUartConsole::run()
  * @param  None
  * @retval number of bytes were sent
  */
int CUartConsole::printf(const char* fmt, ...)
{
	va_list args;
	int n;
	int ret;
	
	//TODO lock vsnprintf mutex
	va_start(args, fmt);
	n = vsnprintf(vsnprintfBuf_, TXBUF_SIZE, fmt, args);
	va_end(args);
	if(n > TXBUF_SIZE) n = TXBUF_SIZE;
	
	ret = send_array(vsnprintfBuf_, n);
	//TODO release vsnprintf mutex
	return ret;
}

/**
  * @brief  for None Dma Mode
  * @param  char to send
  * @retval None
  */
void CUartConsole::putc(const char c)
{
	//TODO add mutex lock here
	txQueue_.push(c);
	//TODO release mutex lock here
	transmitterRun();
}

/**
  * @brief  for None Dma Mode
  * @param  string to send
  * @retval None
  */
void CUartConsole::puts(const char* s)
{
	//TODO add mutex lock here
	txQueue_.push_array((char*)s, strlen(s));
	//TODO release mutex lock here
	transmitterRun();
}

/**
  * @brief  for None Dma Mode
  * @param  None
  * @retval char get
  */
int CUartConsole::getc(void)
{
	//TODO add mutex lock here
	int ret = rxQueue_.front();
	
	/* check if ret is valid */
	if(rxQueue_.pop())
		return ret;
	/* empty queue, ret is invalid */		
	else 
		return -1;
	//TODO release mutex lock here
}

/**
  * @brief  send array
  * @param  None
  * @retval char get
  */
uint16_t CUartConsole::send_array(char* buf, uint16_t len)
{
	uint16_t res;
	//TODO add mutex lock here
	res = txQueue_.push_array(buf, len);
	//TODO release mutex lock here
	transmitterRun();
	return res;
}
#endif	// CONSOLE_SILENT

/**
  * @brief  wait until a char was read from UART
  * @param  None
  * @retval the value read from UART
  */
int CUartConsole::getch()
{
	while(USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_RXNE) == RESET);
	return CONSOLE_UART->DR;
}

/**
  * @brief  postErr() to cheat the IDE to show the macro function
	* @param  None
  * @retval None
  */
#undef postErr
void CUartConsole::postErr()
{}
#define postErr(msg) printf("Error: %s(%d)-%s(): %s\r\n", __FILE__, __LINE__, __FUNCTION__, msg)

/**
* @brief  get empty bytes in txQueue_
* @param  None
* @retval Number of empty bytes
*/
uint16_t CUartConsole::get_emptyBytesInTxQueue()
{
	return txQueue_.emptyElemsInQue();
}

/**
  * @brief  run UART transmitter, in another word TXD
  * @param  None
  * @retval None
  */
void CUartConsole::transmitterRun()
{
#if CONSOLE_TX_USE_DMA
	uint16_t bytesToSend;
	if((txQueue_.elemsInQue() != 0) && 0==DMA_GetCurrDataCounter(CONSOLE_TX_DMAST))
	{
		DMA_Cmd(CONSOLE_TX_DMAST, DISABLE);
		bytesToSend = (txQueue_.elemsInQue() < TX_DMA_SIZE) ? txQueue_.elemsInQue(): TX_DMA_SIZE;
		txQueue_.pop_array(TxDmaBuf_, bytesToSend);
		CONSOLE_TX_DMAST->NDTR = bytesToSend;
		*TXDMA_IFCR_ = TXDMA_IFCR_MASK;
		DMA_Cmd(CONSOLE_TX_DMAST, ENABLE);
	}
	
#else // !CONSOLE_TX_USE_DMA
	if((txQueue_.elemsInQue() != 0) && USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_TXE))
	{
		CONSOLE_UART->DR = txQueue_.front();
		txQueue_.pop();
	}
#endif
}

/**
  * @brief  run UART receiver, in another word RXD
  * @param  None
  * @retval None
  */
void CUartConsole::receiverRun()
{
#if CONSOLE_RX_USE_DMA
	rxDMA_to_rxQueue();
#else // !CONSOLE_RX_USE_DMA
	
#endif
}

/**
  * @brief  run. Valid only in NoneDMA mode
  * @param  None
  * @retval None
  */
void CUartConsole::run()
{
	transmitterRun();
	receiverRun();
}

/**
  * @brief  move data from RX DMA buffer to rxQueue_
  * @param  None
  * @retval None
  */
#ifdef CONSOLE_RX_DMAST
void CUartConsole::rxDMA_to_rxQueue()
{
	uint8_t rxDataSize;	
	/* get rx data size without stop DMA */
	rxDataSize = RX_DMA_SIZE - CONSOLE_RX_DMAST->NDTR;
	if(rxDataSize == 0) return;
	
	/* stop DMA */
	DMA_Cmd(CONSOLE_RX_DMAST, DISABLE);
	*RXDMA_IFCR_ = RXDMA_IFCR_MASK;
	/* get rx data size again */
	rxDataSize = RX_DMA_SIZE - CONSOLE_RX_DMAST->NDTR;
	
		/* check for overflow */
	if(rxDataSize > 10) overflowCounter_++;
	
	/* push data into rxQueue */
	rxQueue_.push_array(RxDmaBuf_, rxDataSize);
	
	CONSOLE_RX_DMAST->NDTR = RX_DMA_SIZE;
	DMA_Cmd(CONSOLE_RX_DMAST, ENABLE);
	
	/* enable stream until it was enabled */
	while((CONSOLE_RX_DMAST->CR&0x01) != 1) 
	{
		*RXDMA_IFCR_ = RXDMA_IFCR_MASK;
		DMA_Cmd(CONSOLE_RX_DMAST, ENABLE);
	}
}
#endif

/**
* @brief  Is Transmitter idel or not
* @param  None
* @retval If is idel
*/
bool CUartConsole::isTransmitterIdel()
{
	transmitterRun();
	return (0 == txQueue_.elemsInQue() 
		&& 0 == DMA_GetCurrDataCounter(CONSOLE_TX_DMAST)
		&& 1 == USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_TXE));
}

//end of file

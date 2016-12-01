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
#include "string.h"

/* when ucos was used */
#if CONSOLE_WITH_UCOS 
#include "ucos_ii.h"
#endif

char CUartConsole::TxBuf_[TXBUF_SIZE];
#ifdef CONSOLE_NONEDMA_MODE
char CUartConsole::vsnpritfBuf_[TXBUF_SIZE];
#endif

/**
  * @brief  Constructor
	* @param  None
  * @retval None
  */
CUartConsole::CUartConsole()
	:overflowCounter_(0),
#ifdef CONSOLE_NONEDMA_MODE
	buffront_ptr_(TxBuf_),
#endif
	bufback_ptr_(TxBuf_)
{
	uint32_t streamx = (((uint32_t)CONSOLE_TX_DMAST&0xFF) - 0x10)/0x18;	// 0-7
	
	if(streamx < 4) TXDMA_IFCR_ = &(CONSOLE_DMA->LIFCR);
	else TXDMA_IFCR_ = &(CONSOLE_DMA->HIFCR);
	switch(streamx % 4)
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
	
	InitSciGpio();
	InitSci();
	
	/* when ucos was used */
#if CONSOLE_WITH_UCOS 
	uint8_t err;
	sendQueMutex_ = OSMutexCreate(1,&err);
	if(err != OS_ERR_NONE) while(1);
#endif
}

/**
  * @brief  send an array with a length
	* @param  buf: pointer of the array
	* @param  size: length of the array
  * @retval If there is data in Tx buf, do nothing and return 0.
	*					If there is nothing is Tx buf, send message and return 1.
  */
uint8_t CUartConsole::send_Array(uint8_t* buf, uint8_t size)
{
	
	if(CONSOLE_TX_DMAST->NDTR != 0) return 0;
	
	DMA_Cmd(CONSOLE_TX_DMAST, DISABLE);//data left in buff will be cut
	*TXDMA_IFCR_ = TXDMA_IFCR_MASK;
	CONSOLE_TX_DMAST->NDTR = size;
	CONSOLE_TX_DMAST->M0AR = (uint32_t)buf;
	
	DMA_Cmd(CONSOLE_TX_DMAST, ENABLE);
	return 1;
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
	uint8_t GPIO_PinSource_BASE;
	uint8_t GPIO_AF_USARTx;
	GPIO_TypeDef *GPIOx;
	
	#if CONSOLE_IOGROUP_A2
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA;
		GPIOx = GPIOA;
		GPIO_PinSource_BASE = GPIO_PinSource2;
	#elif CONSOLE_IOGROUP_A9
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA;
		GPIOx = GPIOA;
		GPIO_PinSource_BASE = GPIO_PinSource9;
	#elif CONSOLE_IOGROUP_B10
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOB;
		GPIOx = GPIOB;
		GPIO_PinSource_BASE = GPIO_PinSource10;
	#else 
		#error
	#endif
	
	#if  CONSOLE_USE_UART1
		GPIO_AF_USARTx = GPIO_AF_USART1;
	#elif CONSOLE_USE_UART2	
		GPIO_AF_USARTx = GPIO_AF_USART2;
	#elif CONSOLE_USE_UART3	
		GPIO_AF_USARTx = GPIO_AF_USART3;
	#else 
		#error
	#endif 

	/* open clock of UART */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
	
	/* Config Pin: RXD TXD */
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE, GPIO_AF_USARTx);
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE + 1, GPIO_AF_USARTx);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1)<< GPIO_PinSource_BASE;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
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
	
#ifdef CONSOLE_DMA_MODE
	/* DMA Clock Config */
	uint32_t RCC_AHB1Periph;
	if(CONSOLE_DMA == DMA1) RCC_AHB1Periph = RCC_AHB1Periph_DMA1;
	else if(CONSOLE_DMA == DMA2) RCC_AHB1Periph = RCC_AHB1Periph_DMA2;
	else while(1); //error
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	
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
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; //attention!
	
	/* TX DMA Config */	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(CONSOLE_UART->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TxBuf_;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 0;
	
	DMA_Cmd(CONSOLE_TX_DMAST, DISABLE);
	DMA_Init(CONSOLE_TX_DMAST, &DMA_InitStructure);
#endif
}

/**
  * @brief  printf a string
  * @param  None
  * @retval number of bytes were sent
  */
#ifdef CONSOLE_DMA_MODE
int CUartConsole::printf(const char* fmt, ...)
{
	DMA_Cmd(CONSOLE_TX_DMAST, DISABLE);
	uint8_t dma_ndtr_temp = CONSOLE_TX_DMAST->NDTR;
	if(dma_ndtr_temp == 0) bufback_ptr_ = TxBuf_;
	
	uint16_t emptyBytesInBuf = TXBUF_SIZE - (bufback_ptr_ - TxBuf_);
	if(0 == emptyBytesInBuf) 
	{
		overflowCounter_++;
		*TXDMA_IFCR_ = TXDMA_IFCR_MASK;
		DMA_Cmd(CONSOLE_TX_DMAST, ENABLE);
		return 0;
	}
	
	va_list args;
	int n;
	
	va_start(args, fmt);
	n = vsnprintf(bufback_ptr_, emptyBytesInBuf, fmt, args);
	va_end(args);
	
	/* check if TxBuf_ overflow */
	if(n > emptyBytesInBuf)
	{
		n = emptyBytesInBuf;
		overflowCounter_++;
	}
	
	CONSOLE_TX_DMAST->NDTR += n;
	CONSOLE_TX_DMAST->M0AR = (uint32_t)(bufback_ptr_ - dma_ndtr_temp);
	bufback_ptr_ += n;
	
	*TXDMA_IFCR_ = TXDMA_IFCR_MASK;
	DMA_Cmd(CONSOLE_TX_DMAST, ENABLE);
	return n;
}

/**
  * @brief  printf a string without DMA controller.
	*					User should call the CUartConsole::run()
  * @param  None
  * @retval number of bytes were sent
  */
#elif CONSOLE_NONEDMA_MODE
#include "CLed.h"
int CUartConsole::printf(const char* fmt, ...)
{
	int32_t bytesInBuf = int32_t((uint32_t)bufback_ptr_-(uint32_t)buffront_ptr_);
	if(bytesInBuf < 0) bytesInBuf += TXBUF_SIZE;
	uint16_t emptyBytesInBuf = TXBUF_SIZE - bytesInBuf;
	
	if(TXBUF_SIZE == bytesInBuf)
	{
		overflowCounter_++;
		return 0;
	}
	va_list args;
	int n;
	
	va_start(args, fmt);
	n = vsnprintf(vsnpritfBuf_, emptyBytesInBuf, fmt, args);
	va_end(args);
	
	/* check if TxBuf_ overflow */
	if(n > emptyBytesInBuf)
	{
		n = emptyBytesInBuf;
		overflowCounter_++;
	}
	
	/* The code blow access the bufback_ptr_ and buffront_ptr_.
		these two pointers will also be write by ::run(), when OS
		is runing. So Mutex is neccessary here	*/
#if CONSOLE_WITH_UCOS 
	uint8_t mutexErr;
	OSMutexPend(sendQueMutex_, 0, &mutexErr);//Infinite wait
#endif
	
	/* front pointer is before back pointer */
	if((uint32_t)buffront_ptr_ <= (uint32_t)bufback_ptr_)
	{
		/* pushback */
		uint16_t bytes_buffend_To_Queueback = TXBUF_SIZE - ((uint32_t)bufback_ptr_ - (uint32_t)TxBuf_);//[1, TXBUF_SIZE]
		if(n >= bytes_buffend_To_Queueback)
		{
			memcpy(bufback_ptr_, vsnpritfBuf_, bytes_buffend_To_Queueback);
			memcpy(TxBuf_, vsnpritfBuf_ + bytes_buffend_To_Queueback, n - bytes_buffend_To_Queueback);
			bufback_ptr_ = TxBuf_ + n - bytes_buffend_To_Queueback;
		}else
		{
			memcpy(bufback_ptr_, vsnpritfBuf_, n);
			bufback_ptr_ += n;
		}
	}
	/* back pointer is before front pointer */
	else
	{
		memcpy(bufback_ptr_, vsnpritfBuf_, n);
		bufback_ptr_ += n;
	}
	
	/* release mutex when ucos was used */
#if CONSOLE_WITH_UCOS 
	OSMutexPost(sendQueMutex_);
#endif
	return n;
}
#endif

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
  * @brief  run. Valid only in NoneDMA mode
  * @param  None
  * @retval None
  */
#ifdef CONSOLE_NONEDMA_MODE
void CUartConsole::run()
{
	int32_t bytesInBuf = int32_t((uint32_t)bufback_ptr_-(uint32_t)buffront_ptr_);
	
		/* The code blow access the bufback_ptr_ and buffront_ptr_.
		these two pointers will also be write by ::run(), when OS
		is runing. So Mutex is neccessary here	*/
#if CONSOLE_WITH_UCOS 
	uint8_t mutexErr;
	OSMutexPend(sendQueMutex_, 0,&mutexErr);
#endif
	
	if(bytesInBuf < 0) bytesInBuf += TXBUF_SIZE;
	if((bytesInBuf != 0) && USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_TXE))
	{
		CONSOLE_UART->DR = *buffront_ptr_;
		if(++buffront_ptr_ >= TxBuf_+TXBUF_SIZE) buffront_ptr_-=TXBUF_SIZE;
	}
	
	/* release mutex when ucos was used */
#if CONSOLE_WITH_UCOS 
	OSMutexPost(sendQueMutex_);
#endif
}
#endif
//end of file

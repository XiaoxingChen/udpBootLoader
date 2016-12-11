/********************************************************************************
* @file    CUartConsole.h
* @author  Chenxx
* @version V1.0
* @date    2016-04-21
* @brief   this file defines the Console function that can printf with uart.
*	   This is for STM32F4.
* release information: 
*	2016/07/01: chenxx add noneDMA mode.
* 2016/07/24: chenxx fixed the bug in printf: stop sending when overflow
* 2016/08/26: chenxx completed the TX DMA mode.
********************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
#ifndef _CUARTCONSOLE_H_
#define _CUARTCONSOLE_H_
#include "stm32f4xx.h"
#include "Singleton.h"
#include "ringque.h"


/****** UART IOGROUP DMAST reference ******/
//	UARTx		IOGROUP		TX_DMAST

//	UART1 	A9 				DMA2_Stream7
//	UART1 	B6 				DMA2_Stream7

//	UART2 	A2 				DMA1_Stream6
//	UART2 	D5 				DMA1_Stream6

//	UART3 	B10 			DMA1_Stream3
//	UART3 	C10 			DMA1_Stream3
//	UART3 	D8 				DMA1_Stream3

//	UART4 	A0 				DMA1_Stream4
//	UART4 	C10 			DMA1_Stream4

//BSP MACROS

/**
  * @brief  In silent mode, all functions of Console will be disabled. 
  */

#define CONSOLE_SILENT			0

// <o> Console on USART: <0=> silent mode <1=>USART 1 <2=>USART 2 <3=> USART 3 
// <4=> USART 4 <5=> USART 5 <6=> USART 6
// 	<i>Default: 1
#define STM32_CONSOLE_USART 0

// <e>Enable DMA for transmission
#define CONSOLE_TX_USE_DMA		1 
// </e>

// <e>Enable DMA for receive
#define CONSOLE_RX_USE_DMA		0
// </e>

/* Macros for Board Support */
#if STM32_CONSOLE_USART == 0
	#undef	CONSOLE_SILENT 
	#define CONSOLE_SILENT 				1
	#define CONSOLE_UART					USART1
	#define CONSOLE_IOGROUP_A9		1
	#define CONSOLE_USE_UART1 		1

#elif STM32_CONSOLE_USART == 1
	#define CONSOLE_UART	USART1
	#define CONSOLE_IOGROUP_A9		1
	#define CONSOLE_USE_UART1 		1
	
#elif STM32_CONSOLE_USART == 2
	#define CONSOLE_UART	USART2
	#define CONSOLE_IOGROUP_A2		1
	#define CONSOLE_USE_UART2 		1

#elif STM32_CONSOLE_USART == 3
	#define CONSOLE_UART	USART3
	#define CONSOLE_IOGROUP_B10		1
	#define CONSOLE_USE_UART3 		1
	
#elif STM32_CONSOLE_USART == 4
	#define CONSOLE_UART	UART4
	#define CONSOLE_IOGROUP_C10		1
	#define CONSOLE_USE_UART4 		1
	
#elif STM32_CONSOLE_USART == 5
	#define CONSOLE_UART	UART5
	#define CONSOLE_IOGROUP_C12D2	1
	#define CONSOLE_USE_UART5 		1
	
#elif STM32_CONSOLE_USART == 6
	#define CONSOLE_UART	USART6
	#define CONSOLE_IOGROUP_C6		1
	#define CONSOLE_USE_UART6 		1
#endif

/**
  * @brief  Macros for Console send 
  * @note  uncomment the CONSOLE_TX_DMAST macro can
	*		make the Console send port work at DMA mode
  */
#if CONSOLE_TX_USE_DMA
#ifdef 	CONSOLE_USE_UART1 
	#define 	CONSOLE_TX_DMAST	DMA2_Stream7	
#elif 	CONSOLE_USE_UART2
	#define 	CONSOLE_TX_DMAST	DMA1_Stream6	
#elif 	CONSOLE_USE_UART3
	#define 	CONSOLE_TX_DMAST	DMA1_Stream3
#elif 	CONSOLE_USE_UART4
	#define 	CONSOLE_TX_DMAST	DMA1_Stream4
#elif 	CONSOLE_USE_UART5
	#define 	CONSOLE_TX_DMAST	DMA1_Stream7
#elif		CONSOLE_USE_UART6
	#define 	CONSOLE_TX_DMAST	DMA2_Stream6
//	#define 	CONSOLE_TX_DMAST	DMA2_Stream7
#endif
#endif //CONSOLE_TX_USE_DMA

/**
  * @brief  Macros for Console receive 
  * @note  uncomment the CONSOLE_RX_DMAST macro can
	*		make the Console send port work at DMA mode
  */
#if CONSOLE_RX_USE_DMA
#ifdef 	CONSOLE_USE_UART1 
	#define 	CONSOLE_RX_DMAST	DMA2_Stream2	
#elif 	CONSOLE_USE_UART2
	#define 	CONSOLE_RX_DMAST	DMA1_Stream5	
#elif 	CONSOLE_USE_UART3
	#define 	CONSOLE_RX_DMAST	DMA1_Stream1
#elif 	CONSOLE_USE_UART4
	#define 	CONSOLE_RX_DMAST	DMA1_Stream2
#elif 	CONSOLE_USE_UART5
	#define 	CONSOLE_RX_DMAST	DMA1_Stream0
#elif		CONSOLE_USE_UART6
	#define 	CONSOLE_RX_DMAST	DMA2_Stream1
//	#define 	CONSOLE_RX_DMAST	DMA2_Stream2
#endif
#endif //CONSOLE_RX_USE_DMA

class CUartConsole
{
public:
	CUartConsole();
	~CUartConsole();
	uint16_t send_array(char*, uint16_t);
	int printf(const char* fmt, ...) __attribute__((format(printf,2,3)));
	void putc(const char c);
	void puts(const char* s);
	int getc(void);
	int getch(void);
	enum{
		//<o> Console TX queue size
		//<20-400>
		//<i> [20, 400]
		TXBUF_SIZE = 400,
		//<o> Console TX DMA buffer size
		//<10-50>
		//<i> [10, 50]
		TX_DMA_SIZE = 50,
		//<o> Console RX queue size
		//<10-50>
		//<i> [10, 50]
		RXBUF_SIZE = 15,
		//<o> Console RX DMA buffer size
		//<10-50>
		//<i> [10, 50]
		RX_DMA_SIZE = 15
	};
	void postErr();
	uint16_t get_emptyBytesInTxQueue();
	void transmitterRun();
	void receiverRun();
	bool isTransmitterIdel();

	void run();
	static char vsnprintfBuf_[TXBUF_SIZE]; //for sprintf

private:
	void InitSciGpio();
	void InitSci();

	static char TxBuf_[TXBUF_SIZE]; //for txQueue_.
	ringque<char> txQueue_;
#if CONSOLE_TX_USE_DMA
	static char TxDmaBuf_[TX_DMA_SIZE]; //for txDma
#endif

	static char RxBuf_[RXBUF_SIZE];	//for rxQueue_.
	ringque<char> rxQueue_;
#ifdef CONSOLE_RX_DMAST
	static char RxDmaBuf_[RX_DMA_SIZE]; //for rxDma
#endif
	
	/* interrupt flag clear register */
#ifdef CONSOLE_TX_DMAST
	volatile uint32_t* TXDMA_IFCR_;
	uint32_t TXDMA_IFCR_MASK;
#endif

#ifdef CONSOLE_RX_DMAST
	volatile uint32_t* RXDMA_IFCR_;
	uint32_t RXDMA_IFCR_MASK;
	void rxDMA_to_rxQueue();
#endif

	uint16_t overflowCounter_;
	

};

typedef NormalSingleton<CUartConsole> Console;
#define postErr(msg) printf("Error: %s(%d)-%s(): %s\r\n", __FILE__, __LINE__, __FUNCTION__, msg)

#endif
// <<< end of configuration section >>>

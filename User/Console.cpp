#include "Console.h"
#include "udpDevice.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


char CConsole::vsnprintfBuf_[TXBUF_SIZE];	//for sprintf
extern CUdpDevice udpConsole;


/**
  * @brief  Constructor
	* @param  None
  * @retval None
  */
CConsole::CConsole()
	:consoleDevice(udpConsole)
{
	consoleDevice.open();
}


/**
  * @brief  printf a string without DMA controller.
	*					User should call the CUartConsole::run()
  * @param  None
  * @retval number of bytes were sent
  */
int CConsole::printf(const char* fmt, ...)
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
  * @brief  send array
  * @param  None
  * @retval char get
  */
uint16_t CConsole::send_array(char* buf, uint16_t len)
{
	uint16_t res;
	
	res = consoleDevice.write((const uint8_t*)buf, len);
	
	consoleDevice.runTransmitter();
	return res;
}
//end of file

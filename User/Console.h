#ifndef CONSOLE_H
#define CONSOLE_H
#include "device.h"
#include "singleton.h"

class CConsole
{
	public:
	CConsole();
	~CConsole();
	uint16_t send_array(char*, uint16_t);
	int printf(const char* fmt, ...) __attribute__((format(printf,2,3)));
	void putc(const char c);
	void puts(const char* s);
	
	void transmitterRun() {};
	void receiverRun() {};
		
	bool isTransmitterIdel() {return true;}

	void run() {};
	enum{
		//<o> Console TX queue size
		//<20-300>
		//<i> [20, 300]
		TXBUF_SIZE = 300
	};
	static char vsnprintfBuf_[TXBUF_SIZE]; //for sprintf
	private:
		CDevice<uint8_t>& consoleDevice;
};

//typedef NormalSingleton<CConsole> Console;
#endif
//end of file

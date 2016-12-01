#ifndef UDPDEVICE_H
#define UDPDEVICE_H
#include "device.h"
#include "ringque.h"
class CUdpDevice
	:public CDevice<uint8_t>
{
	public:
		CUdpDevice();
	
		virtual uint32_t open();
		virtual uint32_t close();
	
		virtual uint32_t write(const uint8_t*, uint32_t);
		virtual uint32_t read(uint8_t*, uint32_t);
	
		virtual uint32_t data_in_write_buf();
		virtual uint32_t freesize_in_write_buf();
		virtual uint32_t data_in_read_buf();
	
		virtual void runTransmitter() {}
		virtual void runReceiver() {}
		void run();

	private:
		uint16_t native_port_;
		uint16_t iap_port_;
		uint8_t socket_n_;
		uint8_t iap_ip_[4];
		bool is_first_in_;
};

extern CUdpDevice iapUdpDevice;
#endif
//end of file

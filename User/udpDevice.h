#ifndef UDPDEVICE_H
#define UDPDEVICE_H
#include "device.h"
#include "ringque.h"
class CUdpDevice
	:public CDevice<uint8_t>
{
	public:
		CUdpDevice(uint16_t, uint16_t, uint8_t, uint8_t* = 0);
	
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
		void set_remote_ip(uint8_t, uint8_t, uint8_t, uint8_t);
		void set_remote_port(uint16_t);

	private:
		uint16_t native_port_;
		uint16_t remote_port_;
		uint8_t socket_n_;
		uint8_t remote_ip_[4];
		bool is_first_in_;
};

extern CUdpDevice iapUdpDevice;
#endif
//end of file

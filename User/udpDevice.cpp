#include "udpDevice.h"
#include "socket.h"
/**
  * @brief  Constructor
	* @param  None
  * @retval None
  */
CUdpDevice::CUdpDevice(uint16_t native_port, 
											uint16_t remote_port,
											uint8_t socket_n,
											uint8_t* remote_ip)
	:native_port_(native_port),
	remote_port_(remote_port),
	socket_n_(socket_n),
	is_first_in_(true)
{
	if(remote_ip != (uint8_t*)0)
	{
		for(int i = 0; i < 4; i++)
		{
			remote_ip_[i] = remote_ip[i];
		}
	}
	
}
	
/**
* @brief  open
* @param  None
* @retval state
*/
uint32_t CUdpDevice::open()
{
	return socket(socket_n_, Sn_MR_UDP, native_port_, 0);
	//TODO switch socket return type into CDevice return type
}

/**
* @brief  close
* @param  None
* @retval 
*/
uint32_t CUdpDevice::close()
{
	return ::close(socket_n_);
}

/**
  * @brief  write
	* @param  databuf: the data pointer wants to send
	* @param  len: data length
  * @retval actual send length
  */
uint32_t CUdpDevice::write(const uint8_t* databuf, uint32_t len)
{
	return sendto(socket_n_, (uint8_t *)databuf, len, (uint8_t*)remote_ip_, remote_port_);
}

/**
  * @brief  read
	* @param  databuf: the data pointer wants to read
	* @param  len: data length
  * @retval actual read length
  */
uint32_t CUdpDevice::read(uint8_t* databuf, uint32_t len)
{
	int32_t ret;
	ret = recvfrom(socket_n_, databuf, len, remote_ip_, &remote_port_);
	return ret;
}

/**
  * @brief  data in read buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUdpDevice::data_in_read_buf()
{
	return getSn_RX_RSR(socket_n_);
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUdpDevice::data_in_write_buf()
{
	while(1);
	return 0;
	//need to calculate
	//TXBUFSIZE - TX_FREESIZE
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUdpDevice::freesize_in_write_buf()
{
	return getSn_TX_FSR(socket_n_);
}

/**
  * @brief  run
	* @param  None
  * @retval None
  */
void CUdpDevice::run()
{
		switch(getSn_SR(socket_n_))
	{
																											 // -------------------------------
		case SOCK_UDP:                                     //
			 break;
																											 // -----------------
		case SOCK_CLOSED:                                  // CLOSED
			::close(socket_n_);                                       // close the SOCKET
			 socket(socket_n_, Sn_MR_UDP, native_port_, 0);                  // open the SOCKET with UDP mode
			 break;
		default:
			 break;
	}
}

/**
  * @brief  set remote ip address
	* @param  192 168 1 4
  * @retval None
  */
void CUdpDevice::set_remote_ip(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
	remote_ip_[0] = byte0;
	remote_ip_[1] = byte1;
	remote_ip_[2] = byte2;
	remote_ip_[3] = byte3;
}

/**
  * @brief  set remote port
	* @param  port num
  * @retval None
  */
void CUdpDevice::set_remote_port(uint16_t port)
{
	remote_port_ = port;
}

CUdpDevice iapUdpDevice(5003, 0, 2);
extern uint8_t w5500_ip[4];
uint8_t consoleRemoteIp[4] = {192, 168, 1, 255};
CUdpDevice udpConsole(19205, 5003, 3, consoleRemoteIp);
//end of file

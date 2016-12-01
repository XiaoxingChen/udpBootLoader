#include "udpDevice.h"
#include "socket.h"
/**
  * @brief  Constructor
	* @param  None
  * @retval None
  */
CUdpDevice::CUdpDevice()
	:native_port_(5004),
	iap_port_(0),
	socket_n_(2),
	is_first_in_(true)
{}
	
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
	return sendto(socket_n_, (uint8_t *)databuf, len, (uint8_t*)iap_ip_, iap_port_);
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
	ret = recvfrom(socket_n_, databuf, len, iap_ip_, &iap_port_);
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
CUdpDevice iapUdpDevice;
//end of file

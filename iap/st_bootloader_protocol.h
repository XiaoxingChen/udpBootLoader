/********************************************************************************
* @file    st_bootloader_protocol.cp
* @author  Chenxx
* @version V1.0
* @date    2016-11-30
* @brief   
*	(C) COPYRIGHT 2016 Seer Robotics
********************************************************************************/
#ifndef ST_BOOTLOADER_PROTOCOL_H
#define ST_BOOTLOADER_PROTOCOL_H
#include <stdint.h>
#include "device.h"

//enum BL_CMD_CODE
//{
//	GET 						= 0x00,
//	GET_VERSION_RPS = 0x01,
//	GET_ID 					= 0x02,
//	READ_MEM 				= 0x11,
//	GO 							= 0x21,
//	WRITE_MEM 			= 0x31,
//	ERASE 					= 0x43,
//	EXTENDED_ERASE 	= 0x44,
//	WRITE_PROTECT 	= 0x63,
//	WRITE_UNPROTECT = 0x73,
//	READ_PROTECT 		= 0x82,
//	READ_UNPROTECT 	= 0x92
//};

enum bl_err_t
{
	BL_OK = 0,
	BL_YIELD,
	BL_ERR
};

#define ACK 0x79
#define NACK 0x1F

bl_err_t bl_excute_cmd(uint8_t cmd);
bl_err_t iapdev_write_byte(uint8_t value);
int iapdev_read_byte();
extern CDevice<uint8_t>& iap_device;

#endif
//end of file

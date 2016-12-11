#include "stmflash.h"
#include "stm32f4xx.h"
#include <string.h>
#include "printf.h"
 



uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

uint8_t FLASH_Sector_to_index(uint16_t FLASH_Sector)
{
	return (FLASH_Sector>>3);
}

uint8_t is_sector_erased[12] = 
{
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};

#define RESET_SECTOR_ERASE_MARK() \
	memset(is_sector_erased, 0x00, 12)

/**
  * @brief  Check before erasing. If erased, do nothing
	* @param  FLASH_Sector
	* @retval None
  */
FLASH_Status erase_sector_once(uint32_t FLASH_Sector)
{
	FLASH_Status ret = FLASH_COMPLETE;
	uint8_t idx = FLASH_Sector_to_index(FLASH_Sector);
	if(!is_sector_erased[idx])
	{
		printf("Erasing sector %d\r\n", FLASH_Sector_to_index(FLASH_Sector));
		ret = FLASH_EraseSector(FLASH_Sector, VoltageRange_3);
		is_sector_erased[idx] = 0xFF;
	}
	return ret;
}

/**
  * @brief  write byte array to flash
	* @param  FLASH_Sector
	* @retval -1 means error
	* @Note   Also active for OTP. OTP address: 0X1FFF7800~0X1FFF7A0F
  */
int STMFLASH_write_bytes(uint32_t appxaddr,uint8_t *buf,uint16_t len)
{
	int i;
	int ret = 0;
	// pack_length must be a multiple of 4
	// pack_length is less than 256
	// that means it will never write cross sector
	if((len & 0x3) != 0 || len > 256 || appxaddr<STM32_FLASH_BASE)
	{
		printf("buffer length check failed\r\n");
		return -1;
	}	
	
	FLASH_Unlock();							
  FLASH_DataCacheCmd(DISABLE);
	
	if(appxaddr < 0X1FFF0000)			//do not erase OTP sector
	{
		//printf("Erasing sector %d\r\n", FLASH_Sector_to_index(STMFLASH_GetFlashSector(appxaddr)));
		if(erase_sector_once(STMFLASH_GetFlashSector(appxaddr)) != FLASH_COMPLETE)
		{
			RESET_SECTOR_ERASE_MARK();
			ret = -1;
		}
	}
//	printf("Erase finished...\r\nStart to write\r\n");
	if(ret == 0) //did not get any error
	{
		for(i = 0; i < len; i++)
		{
			if(FLASH_ProgramByte(appxaddr + i, *(buf + i)) != FLASH_COMPLETE)//д������
			{ 
				RESET_SECTOR_ERASE_MARK();
				ret = -1;
				break;
			}
		}
	}
//	printf("Flash programming finished...\r\n");
	FLASH_DataCacheCmd(ENABLE);
	FLASH_Lock();
		
	return ret;
}

#define BOOT_PARAM_ADDR 0x0800C000
/**
  * @brief  get boot parameter in special flash address
	* @param  None
	* @retval None
  */
uint32_t read_boot_parameter()
{
	return *((volatile uint32_t*)BOOT_PARAM_ADDR);
}

/**
  * @brief  write boot parameter in special flash address
	* @param  value to write
	* @retval None
  */
int write_boot_parameter(uint32_t value)
{
	return STMFLASH_write_bytes(BOOT_PARAM_ADDR, (uint8_t*)&value, sizeof(value));
}











#include "stmflash.h"
#include "iap.h" 
#include "st_bootloader_protocol.h"
#include "printf.h"

iapfun jump2app; 

//__asm void __set_MSP(uint32_t mainStackPointer)
//{
//  msr msp, r0
//  bx lr
//}
//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr)
{
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		__set_MSP(*(volatile uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
	printf("stack top address illegal...\r\n");
}		 

/**
  * @brief  run iap logic
	* @param  None
	* @retval None
  */
void iap_run()
{
	static uint8_t iap_tick = 0;
	static uint8_t bl_cmd = 0xFF;
	switch(iap_tick)
	{
		case 0:
		{
			uint8_t sum_check = 0;
			if(iap_device.data_in_read_buf() < 2)
				break;
			
			bl_cmd = iapdev_read_byte();
			sum_check = iapdev_read_byte();
			
			/* XOR != 0 */
			if((bl_cmd ^ sum_check) != 0xFF)
			{
				iapdev_write_byte(NACK);
				break;
			}
				
			iap_tick++;
			break;
		}
		
		case 1:
		{
			bl_err_t ret_code = bl_excute_cmd(bl_cmd);
			if(ret_code == BL_OK || ret_code == BL_ERR)
			{
				iap_tick = 0;
			}
			//else: ret_code == BL_YIELD
			//excute until BL_OK or BL_ERR
			break;
		}
		default:
			break;
	}
}











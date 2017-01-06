#include "stm32f4xx.h"
#include "CUartConsole.h"
#include "Console.h"
#include "CommonConfig.h"
#include "CSpi.h"
#include "CW5500.h"
#include "w5500.h"
#include "socket.h"
#include "udpDevice.h"
#include "iap.h"
#include "Timer.h"
#include "stmflash.h"

uint8_t tx_mem_conf[8] = {8,8,8,8,8,8,8,8};	// for setting TMSR regsiter
uint8_t rx_mem_conf[8] = {8,8,8,8,8,8,8,8};   // for setting RMSR regsiter
uint8_t data[5000];
uint8_t *data_buf = &data[0];    				// buffer for loopack data
uint8_t w5500_ip[4] = {192,168,1,4};            // for setting SIP register
uint8_t gw[4] = {192,168,1,1};              // for setting GAR register
uint8_t sn[4] = {255,255,255,0};              // for setting SUBR register
uint8_t mac[6] = {0x00,0x08,0xDC,0x01,0x02,0x03};   // for setting SHAR register
uint8_t test_buf[10] = {0,0,0,0,0,0,0,0,0,0};

extern CUdpDevice udpConsole;
int main(void)
{ 
	NVIC_CONFIG();
	BaseTimer::Instance()->initialize();
	
	//if(BOOT_PARAM_NORM == read_boot_parameter())
	if(BOOT_PARAM_IAP != read_boot_parameter())
	{
		iap_load_app(FLASH_APP1_ADDR);
	}
	Spi1.Init();
	w5500.Bsp_Init();
	
	setSHAR(&mac[0]);	/* set source hardware address */
	setGAR(&gw[0]);     /* set gateway IP address */
	setSUBR(&sn[0]);	/* set subnet mask address */		
	setSIPR(&w5500_ip[0]);    /* set source IP address */ 
	iapUdpDevice.open();

	BaseTimer::Instance()->delay_ms(2000);
	if(BOOT_PARAM_IAP == read_boot_parameter())
	{
		Console::Instance()->printf("\r\n Bootloader start!\r\n");
	}else
	{
		Console::Instance()->printf("\r\n Boot paramter error: 0x%X, keep at bootloader\r\n",
			read_boot_parameter());
	}

	while(1)
	{
		iap_run();
		Console::Instance()->transmitterRun();
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


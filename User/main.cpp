#include "stm32f4xx.h"
//#include "delay.h"
#include "CUartConsole.h"
#include "CommonConfig.h"
#include "CSysTick.h"
#include "CSpi.h"
#include "CW5500.h"
#include "w5500.h"
#include "socket.h"
#include "udpDevice.h"
#include "iap.h"

uint8_t tx_mem_conf[8] = {8,8,8,8,8,8,8,8};	// for setting TMSR regsiter
uint8_t rx_mem_conf[8] = {8,8,8,8,8,8,8,8};   // for setting RMSR regsiter
uint8_t data[5000];
uint8_t *data_buf = &data[0];    				// buffer for loopack data
uint8_t ip[4] = {192,168,1,4};            // for setting SIP register
uint8_t gw[4] = {192,168,1,1};              // for setting GAR register
uint8_t sn[4] = {255,255,255,0};              // for setting SUBR register
uint8_t mac[6] = {11,22,33,44,55,66};   // for setting SHAR register
uint8_t test_buf[10] = {0,0,0,0,0,0,0,0,0,0};
void loopback_udp(uint8_t s, uint16_t port, uint8_t* buf, uint16_t mode);
void loopback_tcps(uint8_t s, uint16_t port, uint8_t* buf, uint16_t mode);
int tt = 1;

int main(void)
{ 
	NVIC_CONFIG();
	CSysTick::instance()->start();
	Spi1.Init();
	w5500.Bsp_Init();
	w5500.RST_L();
	CSysTick::instance()->wait();
	CSysTick::instance()->wait();
	w5500.RST_H();
	CSysTick::instance()->wait();
	CSysTick::instance()->wait();
	
	setSHAR(&mac[0]);	/* set source hardware address */
	getSHAR(test_buf);
	setGAR(&gw[0]);     /* set gateway IP address */
	getGAR(test_buf);
	setSUBR(&sn[0]);	/* set subnet mask address */		
	getSUBR(test_buf);
	setSIPR(&ip[0]);    /* set source IP address */ 
	getSIPR(test_buf);
	setSn_KPALVTR(0,4);
	
	Console::Instance()->printf("\r\nstart!\r\n");
	iapUdpDevice.open();
	while(1)
	{
		iap_run();
	}
}

void loopback_udp(uint8_t s, uint16_t port, uint8_t* buf, uint16_t mode)
{
	uint32_t len;
	uint8_t destip[4];
	uint16_t destport;

	switch(getSn_SR(s))
	{
																											 // -------------------------------
		case SOCK_UDP:                                     //
			 if((len=getSn_RX_RSR(s)) > 0)                   // check the size of received data
			 {
					len = recvfrom(s,buf,len,destip,&destport);  // receive data from a destination
					if(len !=sendto(s,buf,len,destip,destport))  // send the data to the destination
					{
	//              printf("%d : Sendto Fail.len=%d,",s,len);
	//              printf("%d.%d.%d.%d(%d)\r\n",destip[0],destip[1],destip[2],destip[3],destport);
					}
			 }
			 break;
																											 // -----------------
		case SOCK_CLOSED:                                  // CLOSED
			 close(s);                                       // close the SOCKET
			 socket(s,Sn_MR_UDP,port,mode);                  // open the SOCKET with UDP mode
			 break;
		default:
			 break;
	}
}

void loopback_tcps(uint8_t s, uint16_t port, uint8_t* buf, uint16_t mode)
{
   uint32_t len;

   switch(getSn_SR(s))                // check SOCKET status
   {                                   // ------------
      case SOCK_ESTABLISHED:           // ESTABLISHED?
         if(getSn_IR(s) & Sn_IR_CON)   // check Sn_IR_CON bit
         {
//          printf("%d : Connect OK\r\n",s);
            setSn_IR(s,Sn_IR_CON);     // clear Sn_IR_CON
         }
         if((len=getSn_RX_RSR(s)) > 0) // check the size of received data
         {
            len = recv(s,buf,len);     // recv
            if(len !=send(s,buf,len))  // send
            {
 //              printf("%d : Send Fail.len=%d\r\n",s,len);
            }
						Console::Instance()->printf("get tcp message\r\n");
         }
         break;
                                       // ---------------
   case SOCK_CLOSE_WAIT:               // PASSIVE CLOSED
         disconnect(s);                // disconnect
         break;
                                       // --------------
   case SOCK_CLOSED:                   // CLOSED
      close(s);                        // close the SOCKET
      socket(s,Sn_MR_TCP,port,mode);   // open the SOCKET
      break;
                                       // ------------------------------
   case SOCK_INIT:                     // The SOCKET opened with TCP mode
      listen(s);                       // listen to any connection request from "TCP CLIENT"
 //     printf("%d : LOOPBACK_TCPS(%d) Started.\r\n",s,port);
      break;
   default:
      break;
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


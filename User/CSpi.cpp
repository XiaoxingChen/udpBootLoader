/**
  ******************************************************************************
  * @file    CSpi.cpp
  * @author  Chenxx
  * @version V1.1
  * @date    2015-10-17
  * @brief   This file provides all the Spi functions.
  ******************************************************************************/
/*******************************************************************************
* release information
*******************************************************************************/	
#include "stm32f4xx.h"
#include "CSpi.h"
#include <string.h>
/******************************************************************************
* @brief   This part for CInte_Spi
******************************************************************************/
CInte_Spi::CInte_Spi(const CSpi* pSpi)
{
	NVIC_IRQChannel_ = pSpi->Get_NVIC_IRQChannel_();
	Periphx_ = (void*)pSpi->Get_SPIx_();
}

/******************************************************************************
* @brief   This part for CDma_Spi
******************************************************************************/
void CDma_Spi::Init(uint32_t PeriphAddr,
							uint32_t MemAddr,
							uint32_t DMA_DIR,
							uint32_t BufferSize,
							uint32_t PDataSize,	
							uint32_t MDataSize,
							uint32_t DMA_Mode,
							CInte_Dma* new_Inte_Dma)
{
	INTE = new_Inte_Dma;
	ClkConfig();
	Config(PeriphAddr,
				MemAddr,
				DMA_DIR,
				BufferSize,
				PDataSize,	
				MDataSize,
				DMA_Mode);
	Enable();
}

/******************************************************************************
* @brief   This part for CSpi
******************************************************************************/
CSpi::CSpi(int IOGroup, void(*InitFunc)(void)):
	Init(InitFunc),DMA(NULL),INTE(NULL),IOGroup_(IOGroup)
{
	if(IOGroup_ == SPI1_B3 || IOGroup == SPI1_A5)
	{
		SPIx_ = SPI1;
		NVIC_IRQChannel_ = SPI1_IRQn;
	}
	else if(IOGroup_ == SPI2_B13)
	{
		SPIx_ = SPI2;
		NVIC_IRQChannel_ = SPI2_IRQn;
	}
	else if(IOGroup_ == SPI3_B3)
	{
		SPIx_ = SPI3;
		NVIC_IRQChannel_ = SPI3_IRQn;
	}
	else	while(1);//error
	
}
void CSpi::ClkConfig(void)
{
	//			SPI1	APB2;
	//			SPI2	APB1;
	//			SPI3	APB1;
	if (SPIx_ == SPI1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	}
	else if(SPIx_ == SPI2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	}
	else if(SPIx_ == SPI3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	}
}

//
//void CSpi::PinConfig(void)
// @brief: Config the CLK, MOSI, MISO but not nCS
//
void CSpi::PinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	uint32_t RCC_AHB1Periph_GPIOx;
	uint8_t GPIO_PinSource_BASE;
	uint8_t GPIO_AF_SPIx;
	GPIO_TypeDef *GPIOx;
	
	if(IOGroup_ == SPI1_B3)
	{
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOB;
		GPIOx = GPIOB;
		GPIO_PinSource_BASE = GPIO_PinSource3;
		GPIO_AF_SPIx = GPIO_AF_SPI1;
	}	
	else if(IOGroup_ == SPI1_A5)
	{
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA;
		GPIOx = GPIOA;
		GPIO_PinSource_BASE = GPIO_PinSource5;
		GPIO_AF_SPIx = GPIO_AF_SPI1;
	}
	else while(1); //undefined!

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
		
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE, GPIO_AF_SPIx);
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE + 1, GPIO_AF_SPIx);
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE + 2, GPIO_AF_SPIx);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2)<< GPIO_PinSource_BASE;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
		
}

//
//void CSpi::Config(u32 Baudrate)
//
void CSpi::Config(uint16_t SPI_DataSize,
									uint16_t SPI_CPOL,
									uint16_t SPI_CPHA,
									uint16_t Prescaler,
									uint16_t SPI_FirstBit,
									uint16_t SPI_Mode)
{
	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//Full Duplex
	SPI_InitStructure.SPI_DataSize = SPI_DataSize;											//
	SPI_InitStructure.SPI_CPOL = SPI_CPOL;															//keep SCK high when free
	SPI_InitStructure.SPI_CPHA = SPI_CPHA;			                       	//
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          	//NSS by software
	SPI_InitStructure.SPI_BaudRatePrescaler = Prescaler;     						//baud rate
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit;                     	//Endian
	SPI_InitStructure.SPI_CRCPolynomial = 7;                            //CRC polygon
	SPI_InitStructure.SPI_Mode = SPI_Mode;															//master/slave mode
	SPI_Init(SPIx_, &SPI_InitStructure);
	
	SPI_Cmd(SPIx_, ENABLE);
}
//
// void DmaSendArray(u8* array, u16 len)
//
void CSpi::DmaSendArray(u8* array, u16 len)
{
	DMA->handle(CSpi::DmaChTxd)->Reboot((uint32_t)array, len);
}

//
//uint8_t CSpi::SendByte(uint8_t byte)
//
uint8_t CSpi::SendByte(uint8_t byte) const
{
  while (SPI_I2S_GetFlagStatus(SPIx_, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx_, byte);

  while (SPI_I2S_GetFlagStatus(SPIx_, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPIx_);
}

//
//uint8_t CSpi::ReadByte()
//
uint8_t CSpi::ReadByte() const
{
	return SendByte(0xFF);
}

//
//uint32_t CSpi::DmaChCalc(DmaChType DmaCh) const
//
uint32_t CSpi::DmaChCalc(int DmaCh) const
{
	if(SPIx_ == SPI1)
	{
		return DMA_Channel_4;
	}
	else
		while(1);
}

//
//DMA_Stream_TypeDef* CSpi::DmaStmCalc(DmaChType DmaCh) const
//
DMA_Stream_TypeDef* CSpi::DmaStmCalc(int DmaCh) const
{
	if(SPIx_ == SPI1)
	{
		if(DmaCh == DmaChTxd)
			return DMA2_Stream7;
		else if(DmaCh == DmaChRxd)
			return DMA2_Stream2;
		else if(DmaCh == DmaChRxd_2)
			return DMA2_Stream5;
		else
			while(1);	//there is no DmaChTxd_2 for SPI1
	}
	else
		while(1);
}

//
//virtual
//
CDma_Base* CSpi::Creat_CDma(int DmaCh) const
{
	/* Periph Config */
	int SPI_DMAReq_Ch;
	if(DmaCh == DmaChTxd || DmaCh == DmaChTxd_2)
		SPI_DMAReq_Ch = SPI_DMAReq_Tx;
	else if(DmaCh == DmaChRxd || DmaCh == DmaChRxd_2)
		SPI_DMAReq_Ch = SPI_DMAReq_Rx;
	else
		while(1); //error!
	SPI_DMACmd(SPIx_, SPI_DMAReq_Ch, ENABLE);
	
	CDma_Base* temp = new CDma_Spi(DmaStmCalc(DmaCh));
	temp->Write_DMA_Channel(DmaChCalc(DmaCh));
	return temp;
}

//
//virtual 
//
void CSpi::Destroy_CDma(int DmaCh) const
{
	/* Periph Config */
	int SPI_DMAReq_Ch;
	if(DmaCh == DmaChTxd || DmaCh == DmaChTxd_2)
		SPI_DMAReq_Ch = SPI_DMAReq_Tx;
	else if(DmaCh == DmaChRxd || DmaCh == DmaChRxd_2)
		SPI_DMAReq_Ch = SPI_DMAReq_Rx;
	else
		while(1); //error!
	SPI_DMACmd(SPIx_, SPI_DMAReq_Ch, DISABLE);
	
	delete DMA->handle(DmaCh);
}
/******************************************************************************
* @brief   This part for Init Function
******************************************************************************/
void SPI1_Init(void)
{
	CSpi& Spix = Spi1;
	Spix.ClkConfig();
	Spix.PinConfig();
	Spix.Config(SPI_DataSize_8b,
							SPI_CPOL_High,
							SPI_CPHA_2Edge,
							SPI_BaudRatePrescaler_4,
							SPI_FirstBit_MSB,
							SPI_Mode_Master);
	
//	Spix.DMA = new CDmaKit_Spi(COM1);
	#if 0
	Spix.DMA = new CDmaKit(&Spix);
	Spix.DMA->add(CSpi::DmaChTxd);

	Spix.DMA->handle(CSpi::DmaChTxd)->Init((uint32_t)&(COM1.Get_SPIx_()->DR),
					(uint32_t)"a",
					DMA_DIR_MemoryToPeripheral,
					1,
					DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte,
					DMA_Mode_Normal,
					new CInte_Dma(Spix.DMA->handle(CSpi::DmaChTxd)));
					
	Spix.DMA->handle(CSpi::DmaChTxd)->INTE->Init(1,1,DMA_IT_TC);
	#endif
//	Spix.INTE = new CInte_Spi(COM1);
//	Spix.INTE->Init(1,1,SPI_IT_RXNE);
}
CSpi Spi1(CSpi::SPI1_A5, SPI1_Init);
/*End of File*/

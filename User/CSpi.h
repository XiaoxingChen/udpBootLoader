/**
  ******************************************************************************
  * @file    CSpi.h
  * @author  Chenxx
  * @version V1.2
  * @date    2015-09-05
  * @brief   This file defines the CSpi class.
  ******************************************************************************/
/*******************************************************************************
* release information
* 2016/01/02 chenxx add DmaSendArray();
*******************************************************************************/	
#ifndef __CSPI_H
#define __CSPI_H

#include "stm32f4xx.h"
#include "CPeriph.h"
class CSpi;
class CDmaKit_Spi;
/******************************************************************************
* @brief   This part for CInte_Spi
******************************************************************************/
class CInte_Spi
	:public CInte_Base
{
public:
	CInte_Spi()
	{}
	CInte_Spi(const CSpi* pSpi);
	virtual void EnableLine(uint32_t SPI_IT)
		{SPI_ITConfig((SPI_TypeDef*)Periphx_, SPI_IT, ENABLE);}
	virtual void DisableLine(uint32_t SPI_IT)
		{SPI_ITConfig((SPI_TypeDef*)Periphx_, SPI_IT, DISABLE);}
};

/******************************************************************************
* @brief   This part for CDma_Spi
******************************************************************************/
class CDma_Spi
	:public CDma_Base
{
public:
	/*default constructor*/
	CDma_Spi()
	{}
	/*mostly used constructor*/
	CDma_Spi(DMA_Stream_TypeDef* DMAy_Streamx)
		:CDma_Base(DMAy_Streamx)
	{}
	/*copy constructor*/
	CDma_Spi(const CDma_Spi& other)
		:CDma_Base(other.GetDMAy_Streamx_())		
	{}
	virtual ~CDma_Spi()
	{
		if (INTE != NULL) delete INTE;
	}
	virtual void Init(uint32_t PeriphAddr,
		uint32_t MemAddr,
		uint32_t DMA_DIR,
		uint32_t BufferSize,
		uint32_t PDataSize,	
		uint32_t MDataSize,
		uint32_t DMA_Mode,
		CInte_Dma* new_Inte_Dma);
};

/******************************************************************************
* @brief   This part for CSpi
******************************************************************************/
class CSpi
	:public CPeriph
{
public:
	friend void SPI1_Init(void);
	
	enum DmaChType
	{
		DmaChTxd = 0,
		DmaChRxd = 1,
		DmaChTxd_2 = 2,
		DmaChRxd_2 = 3
	};
	
	enum IOGroup_Type
	{
		SPI1_B3 = 0,
		SPI1_A5,
		SPI2_B13,
		SPI3_B3
	};
	
	CSpi(int IOGroup, void(*InitFunc)(void));
	virtual ~CSpi()
	{
		if(DMA != NULL) delete DMA;
		if(INTE != NULL) delete INTE;
	}
	void(* Init)();
	void Config(uint16_t SPI_DataSize,
							uint16_t SPI_CPOL,
							uint16_t SPI_CPHA,
							uint16_t Prescaler,
							uint16_t SPI_FirstBit,
							uint16_t SPI_Mode);
	
	void ClkConfig();
	void PinConfig();
	CDmaKit* DMA;
	CInte_Spi* INTE;
		
	void DmaSendArray(u8* array, u16 len);
	uint8_t SendByte(uint8_t byte) const;
	uint8_t ReadByte() const;
	
	SPI_TypeDef* Get_SPIx_(void) const
		{return SPIx_;}
	uint8_t Get_NVIC_IRQChannel_(void) const
		{return NVIC_IRQChannel_;}
		
	uint32_t DmaChCalc(int DmaCh) const;
	DMA_Stream_TypeDef* DmaStmCalc(int DmaCh) const;
		
	virtual CDma_Base* Creat_CDma(int DmaCh) const;
	virtual void Destroy_CDma(int DmaCh) const;

private:
	SPI_TypeDef* SPIx_;
	uint8_t IOGroup_;
	uint8_t NVIC_IRQChannel_;
};

/******************************************************************************
* @brief   This part for extern declaration
******************************************************************************/
extern CSpi Spi1;
#endif
/*End of File*/

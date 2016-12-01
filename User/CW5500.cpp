/**
******************************************************************************
* @file    CW5500.c
* @author  Chenxx
* @version V1.0
* @date    2016-03-17
* @brief   This file defines the CW5500 class.
******************************************************************************/
#include "CW5500.h"
#include "CSpi.h"
#include "stm32f4xx.h"
//
//constructor
//
CW5500::CW5500(void (*BspInitFunc)()):
	Bsp_Init(BspInitFunc),Spi_(NULL)
{

}

//
//void CW5500::attach_spi(CSpi* spi)
//
void CW5500::attach_spi(CSpi* spi)
{
	if (spi == NULL) while (1); //NULL pointer
	Spi_ = spi;
}

//
//
//
void CW5500::IO_Init(GPIO_TypeDef *GPIO_nCS, uint16_t Pin_nCS,
	GPIO_TypeDef *GPIO_RST, uint16_t Pin_RST,
	GPIO_TypeDef *GPIO_INT, uint16_t Pin_INT)
{
	Pin_nCS_.GPIOx = GPIO_nCS;
	Pin_nCS_.GPIO_Pin = Pin_nCS;

	Pin_RST_.GPIOx = GPIO_RST;
	Pin_RST_.GPIO_Pin = Pin_RST;

	Pin_INT_.GPIOx = GPIO_INT;
	Pin_INT_.GPIO_Pin = Pin_INT;

	PinConfig();
}

//
//void CW5500::PinConfig()
//
void CW5500::PinConfig()
{
	uint32_t RCC_AHB1Periph_GPIOx;

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	/* Pin nCS */
	RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA << (((uint32_t)Pin_nCS_.GPIOx - (uint32_t)GPIOA) / 0x400);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
	GPIO_InitStructure.GPIO_Pin = Pin_nCS_.GPIO_Pin;
	GPIO_Init(Pin_nCS_.GPIOx, &GPIO_InitStructure);

	/* Pin RST */
	RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA << (((uint32_t)Pin_RST_.GPIOx - (uint32_t)GPIOA) / 0x400);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
	GPIO_InitStructure.GPIO_Pin = Pin_RST_.GPIO_Pin;
	RST_H();
	GPIO_Init(Pin_RST_.GPIOx, &GPIO_InitStructure);

	/* Pin INT */
//	RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA << (((uint32_t)Pin_INT_.GPIOx - (uint32_t)GPIOA) / 0x400);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Pin = Pin_INT_.GPIO_Pin;
//	GPIO_Init(Pin_INT_.GPIOx, &GPIO_InitStructure);
}

//
//CSpi* CW5500::get_spi()
//
CSpi* CW5500::get_spi() const
{
	return Spi_;
}


//
//
//
void w5500_BspInit()
{
	CW5500& device = w5500;
	device.attach_spi(&Spi1);
	device.IO_Init(GPIOA, GPIO_Pin_4,
								GPIOA, GPIO_Pin_8,
								GPIOA, GPIO_Pin_1);
}

//
//
//
CW5500 w5500(w5500_BspInit);

//end of file

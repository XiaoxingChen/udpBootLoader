/**
******************************************************************************
* @file    CW5500.h
* @author  Chenxx
* @version V1.0
* @date    2016-03-17
* @brief   This file defines the CW5500 class.
******************************************************************************/
#ifndef __CW5500_H
#define __CW5500_H
#include <stdint.h>
#include "stm32f4xx.h"

class CSpi;

class CW5500
{
	struct PinType
	{
		GPIO_TypeDef *GPIOx;
		uint16_t GPIO_Pin;
	};

public:
	CW5500(void (*BspInitFunc)());
	void attach_spi(CSpi* spi);
	void IO_Init(GPIO_TypeDef *GPIO_nCS, uint16_t Pin_nCS,
						GPIO_TypeDef *GPIO_RST, uint16_t Pin_RST,
						GPIO_TypeDef *GPIO_INT, uint16_t Pin_INT);
	void (*Bsp_Init)();
	void PinConfig();
	
	CSpi* get_spi() const;
	
	inline void CS_L()
		{Pin_nCS_.GPIOx->ODR &= ~Pin_nCS_.GPIO_Pin;}
	inline void CS_H()
		{Pin_nCS_.GPIOx->ODR |= Pin_nCS_.GPIO_Pin;}
	inline void RST_L()
		{Pin_RST_.GPIOx->ODR &= ~Pin_RST_.GPIO_Pin;}
	inline void RST_H()
		{Pin_RST_.GPIOx->ODR |= Pin_RST_.GPIO_Pin;}

private:
	CSpi* Spi_;
	PinType Pin_nCS_;
	PinType Pin_RST_;
	PinType Pin_INT_;

};

extern CW5500 w5500;
#endif
//end of file

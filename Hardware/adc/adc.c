#include "adc.h"

#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)   //定义ADC1的数据地址，在stm32f4xx.h中可以找到地址
 uint16_t	ADC1ConvertedValue;                       //定义ADC1的转换值
 /***************************************************************************
*函数名：ADC_GPIO_Config()
*参数：void
*返回值:void
*功能：ADC的GPIO配置函数，内部调用
*****************************************************************************/ 
static void ADC_GPIO_Config(void)
	{
	GPIO_InitTypeDef    GPIO_InitStructure;
		/*使能GPIOB时钟*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*配置PB0为模拟输出，PB0->ADC12的通道8*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                         //初始化IO配置成PIN0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                      //GPIO模式配置成模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;                 //不带上拉下拉电阻
  GPIO_Init(GPIOB, &GPIO_InitStructure);                            //初始化GPIOB的PIN0
  }
/***************************************************************************
*函数名：ADC_CH8_Config()
*参数：void
*返回值:void
*功能：ADC1通道8的配置，内部调用
*****************************************************************************/ 
static	void ADC_CH8_Config(void)
	{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
		/*使能ADC1的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		  /*ADC公共初始化配置*/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //配置ADC模式为独立模式
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                  //配置ADC时钟分频为2分频
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      //失能DMA复用模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //配置两次采用的间隔时间
  ADC_CommonInit(&ADC_CommonInitStructure);                                     //初始化ADC公共配置

  /*ADC1初始化配置*/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                        //配置ADC分辨率为12位
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                 //失能多通道扫描转换模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //使能连续转换模式
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //配置数据对齐方式为右对齐
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    //指定ADC的转换次数为1
  ADC_Init(ADC1, &ADC_InitStructure);                                            //初始化配置ADC1
  /*ADC1通道8采样通道配置，采用频率为3个周期*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);
 /*使能ADC1的DMA传输请求在最后一次传输完成以后*/
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  /* 使能ADC1的DMA传输*/
  ADC_DMACmd(ADC1, ENABLE);
  /* 使能ADC1*/
  ADC_Cmd(ADC1, ENABLE);	
	}
/***************************************************************************
*函数名：ADC_DMA_Config()
*参数：void
*返回值:void
*功能：ADC的DMA传输配置，内部调用
*****************************************************************************/ 
	static void ADC_DMA_Config(void)
	{
		DMA_InitTypeDef DMA_InitStructure;
		/*使能DMA2时钟*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	  /* DMA2 Stream0 通道0配置*/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                                        //指定DMA通道为通道0
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;                 //指定DMA的外设基地址为ADC1的数据地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;                //指定DNA的内存基地址为ADC1转换值存储地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                               //配置DMA传输方向为外设到内存
  DMA_InitStructure.DMA_BufferSize = 1;                                                 //配置缓存大小为1
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                      //失能外设地址增长
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;                              //失能内存地址增长
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;           //配置外设数据大小为半个字长，即16bit
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;                   //配置内存数据大小为半个字长，即16bit
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                       //配置DMA模式为循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                    //配置优先权为高
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                                //失能FIFO模式
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;                     //配置FIFO的阀值为半满
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                           //设置内存突发传输为单一
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;                   //设置外设突发传输为单一
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);                                           //初始化DMA2_Stream0

  /* 使能DMA2_Stream0*/
  DMA_Cmd(DMA2_Stream0, ENABLE);
	}
/***************************************************************************
*函数名：ADC1_CH8_Init()
*参数：void
*返回值:void
*功能：ADC1通道8初始化，外部调用
*****************************************************************************/ 
void ADC1_CH8_Init(void)
	{ 
		/*配置ADC的GPIO*/
		ADC_GPIO_Config();  
    /*配置ADC的DMA传输*/		
		ADC_DMA_Config();
		/*配置ADC的通道8*/
		ADC_CH8_Config();
		/*软件启动ADC1转换*/
		ADC_SoftwareStartConv(ADC1);
	}

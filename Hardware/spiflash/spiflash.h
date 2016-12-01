#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#include "stm32f4xx.h"

#define SPI_FLASH_PageSize      256                                  //页大小
#define SPI_FLASH_PerWritePageSize      256                          //每页大小

/*W25X指令定义*/
#define W25X_WriteEnable		      0x06                            //写使能指令
#define W25X_WriteDisable		      0x04                            //写失能指令
#define W25X_ReadStatusReg		      0x05                            //读状态寄存器
#define W25X_WriteStatusReg		      0x01                            //写状态寄存器
#define W25X_ReadData			      0x03                            //读数据指令
#define W25X_FastReadData		      0x0B                            //快速读模式指令
#define W25X_FastReadDual		      0x3B                            //快读双输出指令    
#define W25X_PageProgram		      0x02                            //页写入指令
#define W25X_BlockErase			      0xD8                            //块擦除指令
#define W25X_SectorErase		      0x20                            //扇区擦除指令
#define W25X_ChipErase			      0xC7                            //片擦除指令
#define W25X_PowerDown			      0xB9                            //进入掉电模式指令
#define W25X_ReleasePowerDown	      0xAB                            //退出掉电模式
#define W25X_DeviceID			      0xAB                            //读取芯片ID
#define W25X_ManufactDeviceID   	  0x90                            //读取制造ID
#define W25X_JedecDeviceID		      0x9F 

#define WIP_Flag                      0x01                            //写入忙标志位
#define Dummy_Byte                    0xFF                            //空数据

#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)    //使能片选
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)      //失能片选

/******************************************************************************************
*函数名：SPI_FLASH_Init()
* 参数：void
* 返回值：void
* 功能：SPIFLASH初始化函数，外部调用
*********************************************************************************************/
void SPI_FLASH_Init(void);
/******************************************************************************************
*函数名：SPI_FLASH_SectorErase()
* 参数：u32 SectorAddr   块地址
* 返回值：void
* 功能：SPIFLASH扇区擦除函数，外部调用
*********************************************************************************************/
void SPI_FLASH_SectorErase(u32 SectorAddr);
/******************************************************************************************
*函数名：SPI_FLASH_BulkErase()
* 参数：void
* 返回值：void
* 功能：SPIFLASH整片擦除函数，外部调用
*********************************************************************************************/
void SPI_FLASH_BulkErase(void);
/******************************************************************************************
*函数名：SPI_FLASH_PageWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH页写入数据函数，外部调用
*********************************************************************************************/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
/******************************************************************************************
*函数名：SPI_FLASH_BufferWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH多个数据函数，外部调用
*********************************************************************************************/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
/******************************************************************************************
*函数名：SPI_FLASH_BufferRead()
* 参数：u8* pBuffer, u32 ReadAddr, u16 NumByteToRead 数据指针，读出的地址，读出的个数
* 返回值：void
* 功能：SPIFLASH多个数据函数，外部调用
*********************************************************************************************/
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
/******************************************************************************************
*函数名：SPI_FLASH_ReadID()
* 参数：void
* 返回值：u32 器件ID
* 功能：SPIFLASH读取ID函数，外部调用
*********************************************************************************************/
u32 SPI_FLASH_ReadID(void);
/******************************************************************************************
*函数名：SPI_FLASH_ReadDeviceID()
* 参数：void
* 返回值：u32 设备ID
* 功能：SPIFLASH读取设备ID函数，外部调用
*********************************************************************************************/
u32 SPI_FLASH_ReadDeviceID(void);
/******************************************************************************************
*函数名：SPI_FLASH_StartReadSequence()
* 参数：u32 ReadAddr 24位读地址
* 返回值：void
* 功能：SPIFLASH读开始函数，外部调用
*********************************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
/******************************************************************************************
*函数名：SPI_FLASH_ReadByte()
* 参数：void
* 返回值：u8 8位数据
* 功能：SPIFLASH读一个字节函数，外部调用
*********************************************************************************************/
u8 SPI_FLASH_ReadByte(void);
/******************************************************************************************
*函数名：SPI_FLASH_SendByte()
* 参数：u8 byte        写入的数据
* 返回值：u8 8位数据
* 功能：SPIFLASH读写一个字节函数，外部调用
*********************************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte);
/******************************************************************************************
*函数名：SPI_FLASH_SendHalfWord()
* 参数：u16 HalfWord        写入的16位数据
* 返回值：u16 16位数据
* 功能：SPIFLASH读写16位数据函数，外部调用
*********************************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
/******************************************************************************************
*函数名：SPI_FLASH_WriteEnable()
* 参数：void
* 返回值：void
* 功能：SPIFLASH写使能函数，外部调用
*********************************************************************************************/
void SPI_FLASH_WriteEnable(void);
/******************************************************************************************
*函数名：SPI_FLASH_WaitForWriteEnd()
* 参数：void
* 返回值：void
* 功能：SPIFLASH等待写完毕函数，外部调用
*********************************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void);
/******************************************************************************************
*函数名：SPI_Flash_PowerDown()
* 参数：void
* 返回值：void
* 功能：SPIFLASH进入掉电模式函数，外部调用
*********************************************************************************************/
void SPI_Flash_PowerDown(void);
/******************************************************************************************
*函数名：SPI_Flash_WAKEUP()
* 参数：void
* 返回值：void
* 功能：SPIFLASH唤醒掉电模式函数，外部调用
*********************************************************************************************/
void SPI_Flash_WAKEUP(void);
#endif

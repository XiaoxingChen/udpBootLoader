#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
#include "delay.h"
#include "fonts.h"
#define HORIZONTAL 0
#if HORIZONTAL
#define LCD_XMAX 320
#define LCD_YMAX 240
#else
#define LCD_XMAX 240
#define LCD_YMAX 320
#endif


#define RED	  0XF800
#define GREEN 0X07E0
#define BLUE  0X001F  
#define BRED  0XF81F
#define GRED  0XFFE0
#define GBLUE 0X07FF
#define BLACK 0X0000
#define WHITE 0XFFFF

#define LCD_PIXEL_WIDTH          0x0140
#define LCD_PIXEL_HEIGHT         0x00F0

#define ABS(X)  ((X) > 0 ? (X) : -(X))

#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)

/**************************************************************************************************************
 * 函数名：LCD_WriteReg()
 * 输入  ：uint8_t LCD_Reg 寄存器地址, uint16_t LCD_RegValue 寄存器的值
 * 输出  ：void
 * 描述  ：写LCD寄存器函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
/**************************************************************************************************************
 * 函数名：LCD_ReadReg()
 * 输入  ：uint8_t LCD_Reg 需要读取的寄存器地址
 * 输出  ：uint16_t 寄存器的值
 * 描述  ：读取LCD寄存器的值
 * 调用  ：外部调用        
 *****************************************************************************************************************/
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
/**************************************************************************************************************
 * 函数名：LCD_WriteRAM_Start()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：开始写LCD的RAM
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_WriteRAM_Start(void);
/**************************************************************************************************************
 * 函数名：LCD_WriteRAM()
 * 输入  ：uint16_t RGB_data  16bit颜色数据
 * 输出  ：void
 * 描述  ：向LCD的RAM写数据
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_WriteRAM(uint16_t RGB_data);
/**************************************************************************************************************
 * 函数名：LCD_ReadRAM()
 * 输入  ：void
 * 输出  ：uint16_t  16bit颜色值
 * 描述  ：读取LCD的RAM数据
 * 调用  ：外部调用        
 *****************************************************************************************************************/
uint16_t LCD_ReadRAM(void);

/**************************************************************************************************************
 * 函数名：LCD_DisplayOn()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD打开显示
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayOn(void);
/**************************************************************************************************************
 * 函数名：LCD_DisplayOff()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD关闭显示
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayOff(void);
/**************************************************************************************************************
 * 函数名：LCD_SetMode()
 * 输入  ：uint8_t mode 设置屏幕刷屏模式
 * 输出  ：void
 * 描述  ：设置LCD刷屏模式
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetMode(uint8_t mode);
/**************************************************************************************************************
 * 函数名：LCD_Clear()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD清屏函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_Clear(uint16_t Color);
/**************************************************************************************************************
 * 函数名：LCD_SetCursor()
 * 输入  ：uint16_t Xpos, uint16_t Ypos 设定的屏幕的X值和Y值
 * 输出  ：void
 * 描述  ：LCD设置光标位置函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
/**************************************************************************************************************
 * 函数名：LCD_SetColors()
 * 输入  ：_TextColor 前景色,_BackColor 背景色
 * 输出  ：void
 * 描述  ：设置LCD的前景色和背景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor);
/**************************************************************************************************************
 * 函数名：LCD_GetColors()
 * 输入  ：*_TextColor 前景色的指针,*_BackColor 背景色的指针
 * 输出  ：void
 * 描述  ：获取LCD的前景色和背景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor);
/**************************************************************************************************************
 * 函数名：LCD_SetFont()
 * 输入  ：sFONT *fonts 要设置的字体
 * 输出  ：void
 * 描述  ：设置LCD的字体
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetFont(sFONT *fonts);
/**************************************************************************************************************
 * 函数名：LCD_Init()
 * 输入  ：void
 * 输出  ：void
 * 描述  ：LCD初始化函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_Init(void);
/**************************************************************************************************************
 * 函数名：LCD_GetFont()
 * 输入  ：void
 * 输出  ：sFONT * 获取字体
 * 描述  ：设置LCD的字体
 * 调用  ：外部调用        
 *****************************************************************************************************************/
sFONT *LCD_GetFont(void);
/**************************************************************************************************************
 * 函数名：LCD_DrawChar()
 * 输入  ：const uint16_t *c   字符编码
 * 输出  ：void
 * 描述  ：LCD画一个字符
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
/**************************************************************************************************************
 * 函数名：LCD_DisplayChar()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint8_t Ascii 显示的位置和字符
 * 输出  ：void
 * 描述  ：LCD显示一个字符
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);
/**************************************************************************************************************
 * 函数名：LCD_DrawString()
 * 输入  ：u16 xpos, u16 ypos, u8 *ptr 显示的位置和字符串
 * 输出  ：void
 * 描述  ：LCD显示一串字符
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawString(u16 xpos, u16 ypos, u8 *ptr);
/**************************************************************************************************************
 * 函数名：LCD_DrawNum()
 * 输入  ：u16 x, u16 y, s32 num, u8 len   显示的位置，数字及长度
 * 输出  ：void
 * 描述  ：LCD显示一个数字
 * 调用  ：外部调用        
 *****************************************************************************************************************/	 
void LCD_DrawNum(u16 x, u16 y, s32 num, u8 len);
/**************************************************************************************************************
 * 函数名：LCD_SetWindow()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width 区域的起点，宽和高
 * 输出  ：void
 * 描述  ：设置某个特定的填充区域
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint8_t Height);
/**************************************************************************************************************
 * 函数名：LCD_DrawHLine()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Length 起点X和Y坐标及长度
 * 输出  ：void
 * 描述  ：画水平线
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
/**************************************************************************************************************
 * 函数名：LCD_DrawVLine()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Length 起点X和Y坐标及长度
 * 输出  ：void
 * 描述  ：画垂直线
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
/**************************************************************************************************************
 * 函数名：LCD_DrawVLine()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint8_t Height 矩形左上角点的坐标及宽和高
 * 输出  ：void
 * 描述  ：画矩形函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint8_t Height);
/**************************************************************************************************************
 * 函数名：LCD_DrawCircle()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Radius 圆心坐标点及半径
 * 输出  ：void
 * 描述  ：画圆函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
/**************************************************************************************************************
 * 函数名：LCD_DrawMonoPict()
 * 输入  ：const uint32_t *Pict 画一个全屏单色的取膜数据
 * 输出  ：void
 * 描述  ：画一个单色的全屏图片函数
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawMonoPict(const uint32_t *Pict);
/**************************************************************************************************************
 * 函数名：DrawPixel()
 * 输入  ：int16_t x, int16_t y  点的坐标
 * 输出  ：void
 * 描述  ：画一个象素点
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void DrawPixel(uint16_t x, uint16_t y);
/**************************************************************************************************************
 * 函数名：LCD_DrawUniLine()
 * 输入  ：uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 起始点坐标和终点坐标
 * 输出  ：void
 * 描述  ：画任意方向的直线
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
/**************************************************************************************************************
 * 函数名：LCD_FillCircle()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Radius 填充圆的圆心和半径
 * 输出  ：void
 * 描述  ：画一个填充圆
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
/**************************************************************************************************************
 * 函数名：LCD_FillRect()
 * 输入  ：uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height 填充矩形左上角点、宽和高
 * 输出  ：void
 * 描述  ：画一个填充的矩形
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**************************************************************************************************************
 * 函数名：LCD_SetTextColor()
 * 输入  ：uint16_t Color 前景色
 * 输出  ：void
 * 描述  ：设置LCD的前景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetTextColor(__IO uint16_t Color);
/**************************************************************************************************************
 * 函数名：LCD_SetBackColor()
 * 输入  ：uint16_t Color 背景色
 * 输出  ：void
 * 描述  ：设置LCD的背景色
 * 调用  ：外部调用        
 *****************************************************************************************************************/
void LCD_SetBackColor(__IO uint16_t Color);
/**************************************************************************************************************
 * 函数名：ClearAera()
 * 输入  ：uint16_t x,uint16_t y,uint16_t w,uint16_t h   清除的位置，长度和宽度
 * 输出  ：void
 * 描述  ：LCD清除某个区域
 * 调用  ：外部调用        
 *****************************************************************************************************************/	
void ClearAera(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
#endif

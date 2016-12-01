#ifndef __PRINTF_H__
#define __PRINTF_H__
#include "CUartConsole.h"

#define printf(fmt, args...) Console::Instance()->printf(fmt, ##args)

#endif
//end of file

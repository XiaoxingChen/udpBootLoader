#include "stm32f4xx.h"
#include "CommonConfig.h"

void NVIC_CONFIG(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
}
//void AFIO_CLK_En(void)
//{
//	
//}

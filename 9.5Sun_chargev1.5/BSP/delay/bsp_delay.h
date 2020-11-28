#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"

void NVIC_Configuration(void);
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif

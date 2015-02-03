#ifndef UC1701X_H
#define UC1701X_H

/*
 * A UC1701X LCD driver written around the STM32F0 Discovery board by STMicroelectronics
 *
 * Author: Harris Shallcross
 * Year: 2/2/2015
 *
 *
 *Code and example descriptions can be found on my blog at:
 *www.hsel.co.uk
 *
 *The MIT License (MIT)
 *Copyright (c) 2015 Harris Shallcross
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>

typedef enum WModesS{
	Dat,
	Reg,
} WModes;

//Main functions
void UC1701X_InitSetup();
void SB(uint8_t, uint8_t, uint8_t);
void PScrn(void);
void ClrBuf(void);
uint8_t BacklightIO(uint8_t);

void Delay(uint32_t);

#define Clk GPIO_Pin_5
#define DIn GPIO_Pin_7
#define DC GPIO_Pin_2
#define CE GPIO_Pin_1

#define ClkPS GPIO_PinSource5
#define DInPS GPIO_PinSource7

#define IOGPIO GPIOA

#define XPix 132
#define YPix 64
#define GBufS (XPix*YPix)/8

extern uint8_t Mode;
extern uint8_t GBuf[GBufS];

#endif

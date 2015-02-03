#ifndef GFX_H
#define GFX_H

#include <UC1701X.h>
#include <math.h>
#include <stdlib.h>


/*
 * My GFX Library!
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

#define LetterSpace 0

typedef enum {
	PixNorm,
	PixInv
} PixT;

void DispMode(uint8_t);

uint8_t WritePix(int16_t, int16_t, PixT);
uint8_t SetPix(uint8_t, uint8_t);
uint8_t ClrPix(uint8_t, uint8_t);

uint8_t Circle(uint8_t, uint8_t, uint8_t, PixT);
uint8_t Semicircle(uint8_t, uint8_t, uint8_t, uint8_t, PixT);

uint8_t Square(uint8_t, uint8_t, uint8_t, uint8_t, PixT);
uint8_t LineP(uint8_t, uint8_t, uint8_t, int16_t, PixT);
uint8_t LineL(uint8_t, uint8_t, uint8_t, uint8_t, PixT);

int PChar(uint16_t, int16_t, int16_t, uint8_t, PixT);
int PStr(const char*, int16_t, int16_t, uint8_t, PixT);
int PNum(int, int16_t, int16_t, uint8_t, uint8_t, PixT);
int PNumF(float, int16_t, int16_t, uint8_t, uint8_t, PixT);

int32_t FPow(int32_t, int32_t);

extern uint8_t GBuf[GBufS];

#endif

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_adc.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>
#include <GFX.h>

/*
 * A simple frequency counter for the STM32F0 Discovery board
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

#define Fs 40000

volatile float Freq = 0;
volatile uint32_t MSec = 0;

void SysTick_Handler(void){
	static uint8_t MCnt = 0, Edge = 0;
	uint16_t Sample;

	static int32_t LPF = 0, HPF = 0, HPFO = 0;
	static uint32_t SCnt = 0, SSamp = 0, MSamp = 0, ESamp = 0;

	//Grab current sample and set ADC to do another conversion
	Sample = ADC_GetConversionValue(ADC1);
	ADC_StartOfConversion(ADC1);

	//Low pass filter incoming data
	LPF += (Sample-LPF)>>8;

	//High pass filter the data and set the high pass filter old data to
	//current value of high pass filter (used for zero cross detection)
	HPFO = HPF;
	HPF = Sample-LPF;

	//Check if the previous HPF value was less than zero and that the
	//current value is more than zero. As data is HPF'd, data will be
	//both positive and negative.

	if(HPFO<0 && HPF>0){
		//Capture two "rising" edges (crosses from negative to positive)
		if(!Edge){
			//Capture the current sample count value
			SSamp = SCnt;
			Edge = 1;
		}
		else{
			//Capture the current sample count value
			ESamp = SCnt;
			//Subtract the two sample capture values and filter the
			//calculated frequency to produce a visible result on the
			//LCD
			Freq += ((float)Fs/(float)((ESamp-SSamp))-Freq)*0.002f;

			//Start again!
			Edge = 0;
		}
	}

	//Increment a millsecond counter - used for Delay
	MCnt++;
	if(MCnt == (Fs/1000)-1){
		MCnt = 0;
		MSec++;
	}

	//Increment sample counter
	SCnt++;
}

//Simple delay routine
void Delay(uint32_t D){
	uint32_t MSS = MSec;
	while((MSec-MSS)<D) asm volatile("nop");
}

ADC_InitTypeDef A;
GPIO_InitTypeDef G;

int main(void)
{
	//Enable GPIO and ADC clocks
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//Set PA0 as analog input
	G.GPIO_Pin = GPIO_Pin_0;
	G.GPIO_OType = GPIO_OType_PP;
	G.GPIO_PuPd = GPIO_PuPd_NOPULL;
	G.GPIO_Speed = GPIO_Speed_2MHz;
	G.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(GPIOA, &G);

	//Initialize the ADC for 12bit conversions
	A.ADC_ContinuousConvMode = DISABLE;
	A.ADC_DataAlign = ADC_DataAlign_Right;
	A.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	A.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &A);
	ADC_Cmd(ADC1, ENABLE);

	//Configure the ADC channel to PA0 (ADC Channel 0)
	ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_41_5Cycles);

	//Initialize systick timer
	SysTick_Config(SystemCoreClock/Fs);

	//Initialize LCD
	UC1701X_InitSetup();

	uint8_t X;

	while(1)
	{
		//Clear screen buffer and print frequency to screen
		ClrBuf();
		X = PStr("Frequency: ", 0, 0, 0, 0);
		X = PNumF(Freq, X, 0, 2, 0, 0);
		PStr("Hz", X, 0, 0, 0);
		PScrn();
	}
}

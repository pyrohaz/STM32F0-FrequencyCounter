#include <UC1701X.h>
uint8_t GBuf[GBufS];

GPIO_InitTypeDef G;
SPI_InitTypeDef S;

void UC1701X_InitSetup(){
	static uint8_t Init = 1;

	if(Init==1){
		Init = 0;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

		G.GPIO_Pin = DC | CE;
		G.GPIO_Mode = GPIO_Mode_OUT;
		G.GPIO_OType = GPIO_OType_PP;
		G.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(IOGPIO, &G);

		G.GPIO_Pin = Clk | DIn;
		G.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(IOGPIO, &G);

		GPIO_PinAFConfig(GPIOA, ClkPS, GPIO_AF_0);
		GPIO_PinAFConfig(GPIOA, DInPS, GPIO_AF_0);

		S.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
		S.SPI_CPHA = SPI_CPHA_1Edge;
		S.SPI_CPOL = SPI_CPOL_Low;
		S.SPI_DataSize = SPI_DataSize_8b;
		S.SPI_FirstBit = SPI_FirstBit_MSB;
		S.SPI_Mode = SPI_Mode_Master;
		S.SPI_NSS = SPI_NSS_Soft;
		SPI_Init(SPI1, &S);
		SPI_Cmd(SPI1, ENABLE);
	}

	Delay(10);
	GPIO_ResetBits(IOGPIO, DC|CE);
	Delay(100);

	//Reset
	SB(0xE2, 0, 1);
	Delay(100);

	SB(0xA0, 0, 1);			//Set segment direction
	SB(0xC8, 0, 1);			//Set common direction

	SB(0x23, 0, 1);			//VLCD Ratio
	SB(0x81, 0, 1);			//Contrast
	SB(0x20, 0, 1);			//Contrast

	SB(0xFA, 0, 1);			//Advanced program control
	SB(0x93, 0, 1);

	SB(0xA2, 0, 1);			//Bias ratio
	SB(0xEE, 0, 1);			//Reset cursor update mode
	SB(0x15, 0, 1);			//Contrast

	SB(0x2F, 0, 1);			//Power Control
	SB(0xAF, 0, 1);			//Enable Screen

	ClrBuf();
	PScrn();
}

void SB(uint8_t Dat, uint8_t CD, uint8_t SC){
	uint16_t Cnt;

	GPIO_WriteBit(IOGPIO, DC, CD);
	if(SC) GPIO_ResetBits(IOGPIO, CE);

	SPI_SendData8(SPI1, Dat);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	GPIO_ResetBits(IOGPIO, Clk|DIn);
	if(SC)GPIO_SetBits(IOGPIO, CE);
}

void SetCol(uint8_t Col){
	SB(Col&15, 0, 1);
	SB(0x10|((Col>>4)&15), 0, 1);
}

void SetPage(uint8_t Page){
	SB(0xB0|(Page&15), 0, 1);
}

void PScrn(void){
	uint16_t Cnt;

	SB(0xB0, 0, 1);
	SB(0x00, 0, 1);
	SB(0x10, 0, 1);
	GPIO_ResetBits(IOGPIO, CE);

	for(Cnt = 0; Cnt<((XPix*YPix)>>3); Cnt++){
		SB(GBuf[Cnt], 1, 0);
	}

	GPIO_SetBits(IOGPIO, CE);
}

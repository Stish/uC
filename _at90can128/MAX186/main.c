#include "main.h"

int main(void)
{
	unsigned int iResult_ADC = 0x0000;
	
	SW_SPI_MasterInit();
	
	while(1)
	{
		iResult_ADC = SW_SPI_ReadMAX186(0x00 | (0x01 << MAX186_START) | (0x01 << MAX186_SGL_DIF) | (0x01 << MAX186_PD1) | (0x01 << MAX186_PD0));
	}
}

void SW_SPI_Wait(void)
{
	for (int i = 1; i <= 5; i++)
	{
		asm("nop");
	}
}

void SW_SPI_SCK_Strope(void)
{
	SPI_PORT &= ~(0x01 << SPI_SCK);
	SW_SPI_Wait();
	SPI_PORT |= (0x01 << SPI_SCK);
	SW_SPI_Wait();
}

void SW_SPI_MasterInit(void)
{
	SPI_DDR = (0x01 << SPI_MOSI) | (0x01 << SPI_SCK);
	SPI_DDR &= ~(0x01 << SPI_MISO);
}

unsigned int SW_SPI_ReadMAX186(char cData)
{
	unsigned int iInputData = 0x0000;
	
	for (char i = 0; i <= 7; i++)
	{
		if (cData & (0x80 >> i))
		{
			SPI_PORT |= (0x01 << SPI_MOSI);
		}
		else
		{
			SPI_PORT &= ~(0x01 << SPI_MOSI);
		}
		
		SW_SPI_SCK_Strope();
	}
	
	SPI_PORT &= ~(0x01 << SPI_MOSI);
	SW_SPI_SCK_Strope();
	SW_SPI_SCK_Strope();
		
	for (char i = 0; i <= 11; i++)
	{
		if (SPI_PIN & (0x01 << SPI_MISO))
		{
			iInputData |= (0x800 >> i);
		}
		SW_SPI_SCK_Strope();
	}
	SW_SPI_SCK_Strope();
	SW_SPI_SCK_Strope();
	
	return iInputData;
}

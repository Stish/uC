//LEM with CAN and MAX186
#include "main.h"

SIGNAL (SIG_OVERFLOW0)								//a/d converter in interrupt mode
{
	TCNT0 = 0x05;
	timer0_cnt++;
	if (timer0_cnt == 25) //100ms
	{	
		msg.data[0] = val_ad_01_bcd[4];
		msg.data[1] = val_ad_01_bcd[0];
		msg.data[2] = val_ad_01_bcd[1];
		msg.data[3] = val_ad_01_bcd[2];
		msg.data[4] = val_ad_01_bcd[3];

		can_send_data(0, msg);

		PORTE ^= (0x01 << 4);

		timer0_cnt = 0;
	}
}

int main(void)								//main program
{
	signed int val_ad_01_buffer = 0;

	unsigned int iResult_ADC = 0x0000;
	
	DDRE |= (0x01 << 4);
	PORTE &= ~(0x01 << 4);
	
	can_init(125, NONE);
	
	msg.id_type = 0;
	msg.id = 0x060;
	msg.idm = 0x7ff;
	msg.length = 8;
	
	can_enable_mob(0, TRANSMIT_DATA, msg);

	sei();										//global interrupt enable on
	TIMER0_ini();
	SW_SPI_MasterInit();

	while(1)
	{
		iResult_ADC = SW_SPI_ReadMAX186(0x00 | (0x01 << MAX186_START) | (0x01 << MAX186_SGL_DIF) | (0x01 << MAX186_PD1) | (0x01 << MAX186_PD0));
		
		if (iResult_ADC & (0x01 << 11))	//negative: truth complement
		{
			iResult_ADC = ((~iResult_ADC) & 0x0fff) + 1;
			val_ad_storage[val_ad_storage_index] = (iResult_ADC * (-1)) + 0; //3
			//val_ad_01_bcd[4] = '-';
		}
		else
		{
			val_ad_storage[val_ad_storage_index] = iResult_ADC + 0; // 4
			//val_ad_01_bcd[4] = '+';
		}
		
		val_ad_storage_index++;
		// Mean out of 10 values
		if (val_ad_storage_index == 10)
		{
			val_ad_01_buffer = 0;
			
			for (unsigned char i = 0; i <= 9; i++)
			{
				val_ad_01_buffer += val_ad_storage[i];
			}
			
			val_ad_01_buffer /= (10*2.275);
			
			
			if (val_ad_01_buffer < 0)
			{
				val_ad_01_bcd[4] = '-';
				val_ad_01_buffer = ((~val_ad_01_buffer) & 0x0fff) + 1;
			}
			else
			{
				val_ad_01_bcd[4] = '+';
				val_ad_01_buffer = val_ad_01_buffer & 0x0fff;
			}
			
			val_ad_01_bcd[0] = (val_ad_01_buffer / 1000) + 0x30;
			val_ad_01_buffer = val_ad_01_buffer % 1000;
			val_ad_01_bcd[1] = (val_ad_01_buffer / 100) + 0x30;
			val_ad_01_buffer = val_ad_01_buffer % 100;
			val_ad_01_bcd[2] = (val_ad_01_buffer / 10) + 0x30;
			val_ad_01_buffer = val_ad_01_buffer % 10;
			val_ad_01_bcd[3] = val_ad_01_buffer + 0x30;
			
			val_ad_storage_index = 0;
		}
	}
}	

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x05;
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
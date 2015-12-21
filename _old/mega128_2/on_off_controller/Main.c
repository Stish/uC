#define AD_PIN 0	//connect to ad0

#include "lib/delay.c"
#include "lib/io_fct.c"
#include "lib/lcd.c"
#include <avr/interrupt.h>
#include <avr/signal.h>

void LCD_write_string(unsigned char pos, unsigned char length, unsigned char str[]);

unsigned char result_bcd[5] = {'0','0','0','0'};
unsigned int result = 0;

SIGNAL (SIG_ADC)
{
	result = ADCL;
	result |= (ADCH<<8);
}

int main (void)
{
	unsigned char i = 0;
	unsigned int buffer_result = 0;

	sei();
	ADMUX = 0x40|AD_PIN;	//connect to pin 0
	ADCSRA = 0x9e;	//enable ad converter
	ADCSRA |= 0x60;	//start converter
	
	LCD_init();
	LCD_write_string(0, 5, "A/D: ");
	LCD_write_string(9, 2, "mV");
	
	while(1)
	{
		buffer_result = result;
		buffer_result *= (1000*0.0048875855327468230694037145650049);
		result_bcd[0] = (buffer_result/1000)+48;
		buffer_result %= 1000;
		result_bcd[1] = (buffer_result/100)+48;
		buffer_result %= 100;
		result_bcd[2] = (buffer_result/10)+48;
		buffer_result %= 10;
		result_bcd[3] = buffer_result+48;
	
		for (i = 0; i <= 3; i++)
		{
			LCD_setCursor(i+5);
			LCD_write_data(result_bcd[i]);
		}
		delay_ms(500);
	}
}

void LCD_write_string(unsigned char pos, unsigned char length, unsigned char str[16])
{
	unsigned char i = 0;
	
	while(i <= length)
	{
		LCD_setCursor(pos + i);
		LCD_write_data(str[i]);
		i++;
	}
}

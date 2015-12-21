#define DEVICE_NUMBER 0x81

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

void disp_char(unsigned char colums[5]);

unsigned char colums[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char rec_char = 0;
unsigned char rec_char_buffer = 0;
unsigned char data_incoming = 0;
unsigned char i = 0;

ISR (USART_RX_vect)
{
	rec_char_buffer = UDR;
	//colums[0] = 0xff;
	//if (rec_char_buffer == DEVICE_NUMBER)
		//colums[0] += 0x01;
	
	if (data_incoming)
	{
		colums[i] = rec_char_buffer;
		i++;
		if (i == 5)
			data_incoming = 0;
	}
	
	if (rec_char_buffer == DEVICE_NUMBER)
	{
		data_incoming = 1;
		i = 0;
	}
} 

int main(void)
{
	//unsigned char colums_a[5] = {0x3f, 0x48, 0x48, 0x48, 0x3f};
	//unsigned char colums_b[5] = {0x7f, 0x49, 0x49, 0x49, 0x36};
	//unsigned char colums_c[5] = {0x3e, 0x41, 0x41, 0x41, 0x22};
	//unsigned char colums_d[5] = {0x7f, 0x41, 0x41, 0x41, 0x3e};
	//unsigned char colums_e[5] = {0x7f, 0x49, 0x49, 0x49, 0x41};
	//unsigned char colums_f[5] = {0x7f, 0x48, 0x48, 0x48, 0x40};
	//unsigned char colums_h[5] = {0x7f, 0x08, 0x08, 0x08, 0x7f};
	//unsigned char colums_l[5] = {0x7f, 0x01, 0x01, 0x01, 0x01};
	//unsigned char colums_o[5] = {0x3e, 0x41, 0x41, 0x41, 0x3e};
	//unsigned int i;
	
	DDRB |= 0x1f;
	DDRD |= 0x7c;
	DDRA |= 0x03;

	UBRRH = 0;		//UBRR to 15 for 115200bps with 14.7456Mhz
	UBRRL = 25;
	
	UCSRA = 0x62;	// 
	UCSRB = 0x18;	// interrupts disabled, transmitter and reciever enabled, 8bit, no 9th bit
	UCSRC = 0x86; 	// no parity, asynchronous, 1 stopbit, 8bit number
	UCSRB |= (1 << RXCIE);
	sei();
	
	PORTB &= ~0x1f;
	
	while(1)
	{
		disp_char(colums);
	}
}

void disp_char(unsigned char *colums)
{
	unsigned char i, j;
	
	for (j = 0; j <= 4; j++)
	{
		if (colums[j] & (0x01 << 0))
			PORTA &= ~(0x01 << 0);
		else
			PORTA |= (0x01 << 0);
		if (colums[j] & (0x01 << 1))
			PORTA &= ~(0x01 << 1);
		else
			PORTA |= (0x01 << 1);
		for (i = 2; i <= 6; i++)
		{
			if (colums[j] & (0x01 << i))
				PORTD &= ~(0x01 << i);
			else
				PORTD |= (0x01 << i);
		}
		PORTB |= (0x01 << j);
		for (i = 0; i <= 100; i++);
			//for (k = 0; k <= 100; k++);
		PORTB &= ~(0x01 << j);
	}
}
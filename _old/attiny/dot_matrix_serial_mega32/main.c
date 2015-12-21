#define BAUD 12

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

void write_data(unsigned char device, unsigned char colums[5]);

/*ISR (USART_TXC_vect)
{
	UDR = 0x81;
}*/

int main(void)
{
	unsigned long int i;
	unsigned char cursor = 0;
	//unsigned char colums_a[5] = {0x3f, 0x48, 0x48, 0x48, 0x3f};
	//unsigned char rec_char = 0;
	//unsigned char colums_b[5] = {0x7f, 0x49, 0x49, 0x49, 0x36};
	//unsigned char colums_c[5] = {0x3e, 0x41, 0x41, 0x41, 0x22};
	//unsigned char colums_d[5] = {0x7f, 0x41, 0x41, 0x41, 0x3e};
	unsigned char colums_e[5] = {0x7f, 0x49, 0x49, 0x49, 0x41};
	//unsigned char colums_f[5] = {0x7f, 0x48, 0x48, 0x48, 0x40};
	unsigned char colums_h[5] = {0x7f, 0x08, 0x08, 0x08, 0x7f};
	unsigned char colums_l[5] = {0x7f, 0x01, 0x01, 0x01, 0x01};
	unsigned char colums_o[5] = {0x3e, 0x41, 0x41, 0x41, 0x3e};
	unsigned char colums_empty[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char colums_exp_mark[5] = {0x00, 0x00, 0x7d, 0x00, 0x00};
	unsigned char colums_80[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char colums_81[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	//unsigned int i;
	
	//DDRB |= 0x1f;
	//DDRD |= 0x7c;
	//DDRA |= 0x03;

	UBRRH = 0;		//UBRR to 15 for 115200bps with 14.7456Mhz
	UBRRL = 25;
	
	UCSRA = 0x62;	// 
	UCSRB = 0x18;	// interrupts disabled, transmitter and reciever enabled, 8bit, no 9th bit
	UCSRC = 0x86; 	// no parity, asynchronous, 1 stopbit, 8bit number
	//UCSRB |= (1 << RXCIE);
	//sei();
	
	//PORTB &= ~0x1f;
	UDR = 0x01;
	
	while(1)
	{
		/*write_data(0x80, colums_empty);
		write_data(0x81, colums_h);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_h);
		write_data(0x81, colums_e);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_e);
		write_data(0x81, colums_l);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_l);
		write_data(0x81, colums_l);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_l);
		write_data(0x81, colums_o);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_o);
		write_data(0x81, colums_exp_mark);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_exp_mark);
		write_data(0x81, colums_empty);
		for (i = 0; i <= 100000; i++);
		//while (!( UCSRA & (1<<UDRE)));
		//UDR = 0x81;
		//for (i = 0; i <= 1000; i++);*/
		/*write_data(0x80, colums_80);
		write_data(0x81, colums_81);
		for (i = 0; i <= 100000; i++);*/
		for (cursor = 0; cursor <= 29; cursor++)
		{
			
			write_data(0x80, colums_80);
			write_data(0x81, colums_81);
			for (i = 0; i <= 100000; i++);
		}
	}
}

void write_data(unsigned char device, unsigned char colums[5])
{
	unsigned int i, j;
	
	while (!( UCSRA & (1<<UDRE)));
	UDR = device;
	//for (j = 0; j <= 10; j++);
	for (i = 0; i <= 4; i++)
	{
		while (!( UCSRA & (1<<UDRE)));
		UDR = colums[i];	
		//for (j = 0; j <= 10; j++);	
	}
}


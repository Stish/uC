#define DEVICE_NUMBER 0x81

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

void disp_char(unsigned char colums[5]);
void gnd_wait(unsigned char j);

unsigned char colums[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
//unsigned char colums[5] = {0x3f, 0x48, 0x48, 0x48, 0x3f};
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
	
	//PORTB &= ~0x1f;
	
	while(1)
	{
		disp_char(colums);
	}
}

void disp_char(unsigned char *colums)
{
	unsigned char j;
	
	for (j = 0; j <= 4; j++)
	{
//=== 1. row ===
		if (colums[j] & (0x01 << 0))
		{
			PORTB &= ~(0x01 << 0);
			gnd_wait(j);
			PORTB |= (0x01 << 0);
		}
		else
			gnd_wait(j);
//=== 2. row ===
		if (colums[j] & (0x01 << 1))
		{
			PORTB &= ~(0x01 << 4);
			gnd_wait(j);
			PORTB |= (0x01 << 4);
		}
		else
			gnd_wait(j);
//=== 3. row ===
		if (colums[j] & (0x01 << 2))
		{
			PORTD &= ~(0x01 << 6);
			gnd_wait(j);
			PORTD |= (0x01 << 6);
		}
		else
			gnd_wait(j);
//=== 4. row ===
		if (colums[j] & (0x01 << 3))
		{
			PORTB &= ~(0x01 << 2);
			gnd_wait(j);
			PORTB |= (0x01 << 2);
		}
		else
			gnd_wait(j);
//=== 5. row ===
		if (colums[j] & (0x01 << 4))
		{
			PORTA &= ~(0x01 << 1);
			gnd_wait(j);
			PORTA |= (0x01 << 1);
		}
		else
			gnd_wait(j);
//=== 6. row ===
		if (colums[j] & (0x01 << 5))
		{
			PORTD &= ~(0x01 << 5);
			gnd_wait(j);
			PORTD |= (0x01 << 5);
		}
		else
			gnd_wait(j);
//=== 7. row ===
		if (colums[j] & (0x01 << 6))
		{
			PORTA &= ~(0x01 << 0);
			gnd_wait(j);
			PORTA |= (0x01 << 0);
		}
		else
			gnd_wait(j);
		
	}
}

void gnd_wait(unsigned char j)
{
	unsigned char cnt = 0;
	switch (j)
	{
		case 0:
			PORTD |= (0x01 << 4);
			for (cnt = 0; cnt <= 100; cnt++);
			PORTD &= ~(0x01 << 4);
		break;
		case 1:
			PORTB |= (0x01 << 1);
			for (cnt = 0; cnt <= 100; cnt++);
			PORTB &= ~(0x01 << 1);
		break;
		case 2:
			PORTD |= (0x01 << 3);
			for (cnt = 0; cnt <= 100; cnt++);
			PORTD &= ~(0x01 << 3);
		break;
		case 3:
			PORTD |= (0x01 << 2);
			for (cnt = 0; cnt <= 100; cnt++);
			PORTD &= ~(0x01 << 2);
		break;
		case 4:
			PORTB |= (0x01 << 3);
			for (cnt = 0; cnt <= 100; cnt++);
			PORTB &= ~(0x01 << 3);
		break;
	}
}

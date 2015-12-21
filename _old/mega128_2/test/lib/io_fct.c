#define ADC_VREF_TYPE 0xC0				// ADC Used Internal Reference

#include <avr/io.h>

void set_pin(char port, char pin)
{
	switch(port)
	{
		case ('a'):
			PORTA |= (0x01 << pin);
			break;
		case ('b'):
			PORTB |= (0x01 << pin);
			break;
		case ('c'):
			PORTC |= (0x01 << pin);
			break;
		case ('d'):
			PORTD |= (0x01 << pin);
			break;
		case ('e'):
			PORTE |= (0x01 << pin);
			break;
		case ('f'):
			PORTF |= (0x01 << pin);
			break;
		default:
			break;
	}
}

void reset_pin(char port, char pin)
{
	switch(port)
	{
		case ('a'):
			PORTA &= ~(0x01 << pin);
			break;
		case ('b'):
			PORTB &= ~(0x01 << pin);
			break;
		case ('c'):
			PORTC &= ~(0x01 << pin);
			break;
		case ('d'):
			PORTD &= ~(0x01 << pin);
			break;
		case ('e'):
			PORTE &= ~(0x01 << pin);
			break;
		case ('f'):
			PORTF &= ~(0x01 << pin);
			break;
		default:
			break;
	}
}

void toggle_pin(char port, char pin)
{
	switch(port)
	{
		case ('a'):
			PORTA ^= (0x01 << pin);
			break;
		case ('b'):
			PORTB ^= (0x01 << pin);
			break;
		case ('c'):
			PORTC ^= (0x01 << pin);
			break;
		case ('d'):
			PORTD ^= (0x01 << pin);
			break;
		case ('e'):
			PORTE ^= (0x01 << pin);
			break;
		case ('f'):
			PORTF ^= (0x01 << pin);
			break;
		default:
			break;
	}
}

void init_out_pin(char port, char pin)
{
	switch(port)
	{
		case ('a'):
			DDRA |= (0x01 << pin);
			break;
		case ('b'):
			DDRB |= (0x01 << pin);
			break;
		case ('c'):
			DDRC |= (0x01 << pin);
			break;
		case ('d'):
			DDRD |= (0x01 << pin);
			break;
		case ('e'):
			DDRE |= (0x01 << pin);
			break;
		case ('f'):
			DDRF |= (0x01 << pin);
			break;
		default:
			break;
	}
}

void init_in_pin(char port, char pin)
{
	switch(port)
	{
		case ('a'):
			DDRA &= ~(0x01 << pin);
			break;
		case ('b'):
			DDRB &= ~(0x01 << pin);
			break;
		case ('c'):
			DDRC &= ~(0x01 << pin);
			break;
		case ('d'):
			DDRD &= ~(0x01 << pin);
			break;
		case ('e'):
			DDRE &= ~(0x01 << pin);
			break;
		case ('f'):
			DDRF &= ~(0x01 << pin);
			break;
		default:
			break;
	}
}

void init_out_port(char port)
{
	switch(port)
	{
		case ('a'):
			DDRA |= 0xff;
			break;
		case ('b'):
			DDRB |= 0xff;
			break;
		case ('c'):
			DDRC |= 0xff;
			break;
		case ('d'):
			DDRD |= 0xff;
			break;
		case ('e'):
			DDRE |= 0xff;
			break;
		case ('f'):
			DDRF |= 0xff;
			break;
		default:
			break;
	}
}

void init_in_port(char port)
{
	switch(port)
	{
		case ('a'):
			DDRA &= 0x00;
			break;
		case ('b'):
			DDRB &= 0x00;
			break;
		case ('c'):
			DDRC &= 0x00;
			break;
		case ('d'):
			DDRD &= 0x00;
			break;
		case ('e'):
			DDRE &= 0x00;
			break;
		case ('f'):
			DDRF &= 0x00;
			break;
		default:
			break;
	}
}

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS)|(3<<UCSZ0);
}

void init_serial(void)
{
//	UBRR0H = 0;		//UBRR to 15 for 115200bps with 20Mhz
//	UBRR0L = 21;

	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x86;
	UBRR0H=0x00;
	UBRR0L=0x08;

/*	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x06;
	UBRR0H=0x00;
	UBRR0L=0x08;
//	UBRR0L=0x08;
	
/*	UCSR0A = 0x62;	// 
	UCSR0B = 0x18;	// interrupts disabled, transmitter and reciever enabled, 8bit, no 9th bit
	UCSR0C = 0x86; 	// no parity, asynchronous, 1 stopbit, 8bit number*/
}

void init_in_ana(void)
{
	ADMUX = 0x40;	//connect to pin 0
	ADCSRA = 0x9e;	//enable ad converter
	ADCSRA |= 0x60;	//start converter
}

unsigned char get_in_pin(unsigned char port, unsigned char pin)
{
	unsigned char c = 0;

	switch(port)
	{
		case ('a'):
			c = PINA;
			break;
		case ('b'):
			c = PINB;
			break;
		case ('c'):
			c = PINC;
			break;
		case ('d'):
			c = PIND;
			break;
		case ('e'):
			c = PINE;
			break;
		case ('f'):
			c = PINF;
			break;
		default:
			break;
	}	
	c &= (0x01 << pin);
	
	return c;
}

unsigned int read_adc(unsigned char adc_input)                  // Read Result ADC
{
	ADMUX=adc_input|ADC_VREF_TYPE;
	
	ADCSRA|=0x40;                                                 // Start the AD conversion
	
	while ((ADCSRA & 0x10)==0);                                   // Wait for the AD conversion to complete
	ADCSRA|=0x10;
	
	return ADCW;
}


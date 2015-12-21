#include <avr/io.h>

void init_serial(void)
{
	UBRRH = 0;		//UBRR to 15 for 115200bps with 14.7456Mhz
	UBRRL = 15;
	
	UCSRA = 0x62;	// 
	UCSRB = 0x18;	// interrupts disabled, transmitter and reciever enabled, 8bit, no 9th bit
	UCSRC = 0x86; 	// no parity, asynchronous, 1 stopbit, 8bit number
}

void init_in_ana(void)
{
	ADMUX = 0x40;	//connect to pin 0
	ADCSRA = 0x9e;	//enable ad converter
	ADCSRA |= 0x60;	//start converter
}

void init_in(void)
{
	DDRC = 0x00;
}

unsigned char get_in_pin(unsigned char p)
{
	unsigned char c = 0;
	unsigned char shift = 0x01;
	
	shift <<= (p-1);
	c = PINC;
	c &= shift;
	
	return c;
}

unsigned char get_in_port(void)
{
	unsigned char c = 0;
	
	c = PINC;
	
	return c;
}

void init_out(void)
{
	DDRB = 0x0f;
	DDRD = 0xf0;
	PORTB = 0xff;
	PORTD = 0xff;	
}


void set_out_pin(unsigned char p)
{
	switch(p)
	{
		case 4:
			PORTD = PORTD & 0x7f;
			break;
		case 3:
			PORTD = PORTD & 0xBf;
			break;
		case 2:
			PORTD = PORTD & 0xDf;
			break;
		case 1:
			PORTD = PORTD & 0xEf;
			break;
		case 8:
			PORTB = PORTB & 0xf7;
			break;
		case 7:
			PORTB = PORTB & 0xfb;
			break;
		case 6:
			PORTB = PORTB & 0xfd;
			break;
		case 5:
			PORTB = PORTB & 0xfe;
			break;
	}		
}

void reset_out_pin(unsigned char p)
{
	switch(p)
	{
		case 4:
			PORTD = PORTD | 0x80;
			break;
		case 3:
			PORTD = PORTD | 0x40;
			break;
		case 2:
			PORTD = PORTD | 0x20;
			break;
		case 1:
			PORTD = PORTD | 0x10;
			break;
		case 8:
			PORTB = PORTB | 0x08;
			break;
		case 7:
			PORTB = PORTB | 0x04;
			break;
		case 6:
			PORTB = PORTB | 0x02;
			break;
		case 5:
			PORTB = PORTB | 0x01;
			break;
	}
}

void toggle_out_pin(unsigned char p)
{
	switch(p)
	{
		case 4:
			PORTD = PORTD ^ 0x80;
			break;
		case 3:
			PORTD = PORTD ^ 0x40;
			break;
		case 2:
			PORTD = PORTD ^ 0x20;
			break;
		case 1:
			PORTD = PORTD ^ 0x10;
			break;
		case 8:
			PORTB = PORTB ^ 0x08;
			break;
		case 7:
			PORTB = PORTB ^ 0x04;
			break;
		case 6:
			PORTB = PORTB ^ 0x02;
			break;
		case 5:
			PORTB = PORTB ^ 0x01;
			break;
	}
}

void set_out_ports(unsigned char p)
{
	if (p & 0x80)
		set_out_pin(8);
	else
		reset_out_pin(8);
	if (p & 0x40)
		set_out_pin(7);
	else
		reset_out_pin(7);
	if (p & 0x20)
		set_out_pin(6);
	else
		reset_out_pin(6);
	if (p & 0x10)
		set_out_pin(5);
	else
		reset_out_pin(5);
	if (p & 0x08)
		set_out_pin(4);
	else
		reset_out_pin(4);
	if (p & 0x04)
		set_out_pin(3);
	else
		reset_out_pin(3);
	if (p & 0x02)
		set_out_pin(2);
	else
		reset_out_pin(2);
	if (p & 0x01)
		set_out_pin(1);
	else
		reset_out_pin(1);
}

void lcd(char p)
{
	switch(p)
	{
		case 0:
			set_out_ports(0xee);
		break;
		case 1:
			set_out_ports(0x60);
		break;
		case 2:
			set_out_ports(0xcd);
		break;
		case 3:
			set_out_ports(0xe9);
		break;
		case 4:
			set_out_ports(0x63);
		break;
		case 5:
			set_out_ports(0xab);
		break;
		case 6:
			set_out_ports(0xaf);
		break;
		case 7:
			set_out_ports(0xe0);
		break;
		case 8:
			set_out_ports(0xef);
		break;
		case 9:
			set_out_ports(0xeb);
		break;
	}
}

#include "main.h"

int main(void)
{
	sei();
	TIMER0_ini();
	USART1_ini();

	while(1)
	{
	}
}

SIGNAL (SIG_UART0_TRANS)								//a/d converter in interrupt mode
{
}

SIGNAL (SIG_OVERFLOW0)								//a/d converter in interrupt mode
{
	TCNT0 = 0x06;
	timer0_cnt++;
	
	if (timer0_cnt == 250) //100ms
	{
		timer0_cnt = 0;
		UDR1 = 'A';
	}
}

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x06;
}

void USART1_ini (void)
{
	/* Set baud rate */
	UBRR1H = (unsigned char) (BAUD>>8);
	UBRR1L = (unsigned char) BAUD;
	UCSR1C = (0 << UMSEL1) | //asynchronous
			(0 << UPM10) | (0<<UPM11) |	//Mode 00: no parity
			(0 << USBS1) | 	//1 stop bit
			(1 << UCSZ11) | (1 << UCSZ10);	//Mode 011: 8 bit data
	/* Enable transmitter, disable receiver */
	UCSR1B = (0 << RXCIE1) | (0 << TXCIE1) | (0 << RXEN1) | (1 << TXEN1) | (0 << UCSZ12);
	UCSR1A = (0 << U2X1);
}

void USART0_ini (void)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (BAUD>>8);
	UBRR0L = (unsigned char) BAUD;
	UCSR0C = (0 << UMSEL0) | //asynchronous
			(0 << UPM00) | (0<<UPM01) |	//Mode 00: no parity
			(0 << USBS0) | 	//1 stop bit
			(1 << UCSZ01) | (1 << UCSZ00);	//Mode 011: 8 bit data
	/* Enable transmitter, disable receiver */
	UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
	UCSR0A = (0 << U2X0);
}

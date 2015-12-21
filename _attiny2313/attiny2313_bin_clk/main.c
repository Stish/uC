#include <avr/io.h>			
#include <avr/interrupt.h>	

unsigned int timer0_cnt = 0;
unsigned char t_sec = 0;
unsigned char t_min = 0;
unsigned char t_hou = 0;
unsigned char tx_frame[] = "00:00:00\n\0";
unsigned char rx_frame[] = "00:00:00\n\0";
unsigned char send_data = 0;
unsigned char receive_data = 0;
unsigned char tx_frame_index = 0;
unsigned char rx_frame_index = 0;

void vTIMER0_ini (void);

ISR (USART_TX_vect)
{
	if (send_data == 1)
	{
		UDR = tx_frame[tx_frame_index];
		tx_frame_index++;
		if (tx_frame_index == 9)
		{
			tx_frame_index = 1;
			send_data = 0;
		}
	}
} 

ISR (USART_RX_vect)
{
	unsigned char rx_buffer = 0;
	
	rx_buffer = UDR;
	
	if (receive_data == 1)
	{
		rx_frame[rx_frame_index] = rx_buffer;
		rx_frame_index++;
		
		if (rx_frame_index == 8)
		{
			t_sec = ((rx_frame[6] - 48) * 10) + (rx_frame[7] - 48);
			t_min = ((rx_frame[3] - 48) * 10) + (rx_frame[4] - 48);
			t_hou = ((rx_frame[0] - 48) * 10) + (rx_frame[1] - 48);
			
			tx_frame[0] = rx_frame[0];
			tx_frame[1] = rx_frame[1];
			tx_frame[3] = rx_frame[3];
			tx_frame[4] = rx_frame[4];
			tx_frame[6] = rx_frame[6];
			tx_frame[7] = rx_frame[7];
			
			send_data = 1;
			rx_frame_index = 0;
			receive_data = 0;
			UDR = tx_frame[0];	
		}
	}
	
	if (rx_buffer == 't')
	{
		send_data = 1;
		UDR = tx_frame[0];	
	}
	
	if (rx_buffer == 's')
	{	
		receive_data = 1;
	}
}

SIGNAL (SIG_TIMER0_OVF)								//a/d converter in interrupt mode
{
	//TCNT0 = 0x01;
	PORTB ^= (0x01 << PB0);
	timer0_cnt++;
	if (timer0_cnt == 4020)
	{
		timer0_cnt = 0;
		t_sec++;
		if (t_sec == 60)
		{
			t_sec = 0;
			t_min++;
			if (t_min == 60)
			{
				t_min = 0;
				t_hou++;
				if (t_hou == 24)
				{
					t_hou = 0;
				}
				tx_frame[0] = ((t_hou / 10) + 48);
				tx_frame[1] = ((t_hou - ((t_hou / 10)*10)) + 48);
			}
			tx_frame[3] = ((t_min / 10) + 48);
			tx_frame[4] = ((t_min - ((t_min / 10)*10)) + 48);
		}
		tx_frame[6] = ((t_sec / 10) + 48);
		tx_frame[7] = ((t_sec - ((t_sec / 10)*10)) + 48);
	}
}

int main(void)
{
	DDRB |= (0x01 << PB0);
	PORTB &= ~(0x01 << PB0);
	sei();										//global interrupt enable on
	vTIMER0_ini();

	UBRRH = 0;		//UBRR to 12 for 38400 baud @ 8MHz
	UBRRL = 12;
	UCSRA |= (1 << UDRE);	//Buffer is empty
	UCSRB |= (1 << TXCIE); 	//Interrupt enable for RX
	UCSRB |= (1 << TXEN);	//RX enable, pin is used as RX signal
	UCSRB |= (1 << RXCIE); 	//Interrupt enable for RX
	UCSRB |= (1 << RXEN);	//RX enable, pin is used as RX signal
	UCSRC |= (1 << UCSZ1);	//UCSZ2 = 0, UCSZ1 = 1, UCSZ0 = 1 : 8 data bits
	UCSRC |= (1 << UCSZ0);	

	while(1)
	{

	}
}

void vTIMER0_ini (void)
{
	TCCR0A = 0x00 | (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	// TCCR0A = 0x00
	TCCR0B = 0x00 | (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (1 << CS01) | (1 << CS00); //011 - 64 prescaler
	// TCCR0B = 0x01
	//TCNT0 = 0x05;
	// TCNT0 = 0
	TIMSK |= (0 << OCIE0B) | (1 << TOIE0) | (0 << OCIE0A);
	// TIMSK = 0x02
}


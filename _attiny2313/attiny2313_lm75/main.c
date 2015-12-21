#define F_CPU 8000000UL

#include <avr/io.h>			
#include <avr/interrupt.h>	
#include <util/delay.h>

#define SDA PB0
#define SCL PB1

#define DDR_SDA DDRB
#define DDR_SCL DDRB

#define PORT_SDA PORTB
#define PORT_SCL PORTB

#define PIN_SDA PINB

/* Macros */
#define SCL_HIGH; 	PORT_SCL|=(0x01<<SCL);
#define SCL_LOW; 	PORT_SCL&=~(0x01<<SCL);

#define SDA_HIGH; 	PORT_SDA|=(0x01<<SDA);
#define SDA_LOW;	PORT_SDA&=~(0x01<<SDA);

unsigned int timer0_cnt = 0;

unsigned char send_data = 0;
unsigned char tx_frame_index = 0;
unsigned char temp_h = 0x00;
unsigned char temp_l = 0x00;
unsigned char tx_frame[3] = {0x12, 0x34, 0x0d};

void vTIMER0_ini (void);
void lm75_read_temp(void);
void scl_clk(void);
unsigned char get_data(void);

ISR (USART_TX_vect)
{
	if (send_data == 1)
	{
		UDR = tx_frame[tx_frame_index];
		tx_frame_index++;
		if (tx_frame_index == 3)
		{
			tx_frame_index = 1;
			send_data = 0;
		}
	}
} 

SIGNAL (SIG_TIMER0_OVF)								//a/d converter in interrupt mode
{
	//TCNT0 = 0x01;
	
	timer0_cnt++;
	if (timer0_cnt == 61)
	{
		PORTB ^= (0x01 << PB2);
		timer0_cnt = 0;
		send_data = 1;
		UDR = tx_frame[0];
		tx_frame_index = 1;
	}
}

int main(void)
{
	DDRB |= (0x01 << PB2);
	PORTB &= ~(0x01 << PB2);
	DDRD |= (0x01 << PD5);
	PORTD &= ~(0x01 << PD5);
	
	DDR_SDA |= (0x01 << SDA);
	DDR_SCL |= (0x01 << SCL);
	
	/*SDA_LOW;
	
	for (int i = 0; i <= 40; i++)
	{
		_delay_ms(10);
	}*/
	
	SDA_HIGH;
	SCL_HIGH;
	
	
	sei();										//global interrupt enable on
	vTIMER0_ini();

	UBRRH = 0;		//UBRR to 51 for 9600 baud @ 8MHz
	UBRRL = 51;
	UCSRA |= (1 << UDRE);	//Buffer is empty
	UCSRB |= (1 << TXCIE); 	//Interrupt enable for RX
	UCSRB |= (1 << TXEN);	//RX enable, pin is used as RX signal
	//UCSRB |= (1 << RXCIE); 	//Interrupt enable for RX
	//UCSRB |= (1 << RXEN);	//RX enable, pin is used as RX signal
	UCSRC |= (1 << UCSZ1);	//UCSZ2 = 0, UCSZ1 = 1, UCSZ0 = 1 : 8 data bits
	UCSRC |= (1 << UCSZ0);	

	//lm75_read_temp();

	while(1)
	{
		SDA_LOW;
		
		for (int i = 0; i <= 200; i++)
		{
			_delay_ms(10);
		}
		
		SDA_HIGH;
		SCL_HIGH;
		lm75_read_temp();
	}
}

void vTIMER0_ini (void)
{
	TCCR0A = 0x00 | (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	// TCCR0A = 0x00
	TCCR0B = 0x00 | (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (1 << CS02) | (0 << CS01) | (0 << CS00); //100 - 256 prescaler
	// TCCR0B = 0x01
	//TCNT0 = 0x05;
	// TCNT0 = 0
	TIMSK |= (0 << OCIE0B) | (1 << TOIE0) | (0 << OCIE0A);
	// TIMSK = 0x02
}

void lm75_read_temp(void)
{
	unsigned char i;
	
// ================================
	/* Start condition */
	SDA_HIGH;	
	_delay_us(4);
	SCL_HIGH;
	//_delay_us(10);

	PORTD |= (0x01 << PD5);
	_delay_us(1);
	PORTD &= ~(0x01 << PD5);
	
	SDA_LOW;
	_delay_us(4);
	SCL_LOW;
	_delay_us(4);
	/* Address bit 6 */
	SDA_HIGH;
	scl_clk();	
	/* Address bit 5 */
	SDA_LOW;
	scl_clk();
	/* Address bit 4 */
	SDA_LOW;
	scl_clk();
	/* Address bit 3 */
	SDA_HIGH;
	scl_clk();	
	/* Address bit 2 */
	SDA_LOW;
	scl_clk();	
	/* Address bit 1 */
	SDA_LOW;
	scl_clk();	
	/* Address bit 0 */
	SDA_LOW;
	scl_clk();
	/* R/W */
	SDA_HIGH;
	_delay_us(4);
	SCL_HIGH;
	_delay_us(4);
	SCL_LOW;
	/* Ack from LM75 */
	DDR_SDA &= ~(0x01 << SDA); // Input
	scl_clk();
	/* Data bits 7-0 high byte */
	temp_h = 0;
	for (i = 0; i <= 7; i++)
	{
		temp_h |= get_data() << (7 - i);
	}
	tx_frame[0] = temp_h;
	/* Ack from master */
	DDR_SDA |= (0x01 << SDA); // Output
	//_delay_us(1);
	SDA_LOW;
	scl_clk();
	DDR_SDA &= ~(0x01 << SDA); // Input
	/* Data bits 7-0 low byte */
	temp_l = 0;
	for (i = 0; i <= 7; i++)
	{
		temp_l |= get_data() << (7 - i);
	}
	tx_frame[1] = temp_l;
	/* No Ack from master */
	DDR_SDA |= (0x01 << SDA); // Output
	SDA_HIGH;
	scl_clk();
	/* Stop condition */
	SDA_LOW;
	//_delay_us(4);
	SCL_HIGH;
	//_delay_us(4);
	SDA_HIGH;
	//_delay_ms(10);
}

void scl_clk(void)
{
	_delay_us(4);
	SCL_HIGH;
	_delay_us(4);
	SCL_LOW;
	//_delay_us(10);
}

unsigned char get_data(void)
{
	unsigned char buffer = 0;
	
	_delay_us(4);
	SCL_HIGH;
	_delay_us(4);
	if (PIN_SDA & (0x01 << SDA))
	{
		buffer = 1;
	}
	else
	{
		buffer = 0;
	}
	SCL_LOW;

	return buffer;
}
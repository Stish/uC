void vPORTS_ini (void);
void vTIMER0_ini (void);
void vINT0_ini (void);
//void vLED_on (char LED_number);
//void vLED_off (char LED_number);
//void vLED_off_all (void);
//void vUSART1_ini (void);

void vTIMER0_ini (void)
{
	TCCR0A = 0x00 | (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	// TCCR0A = 0x00
	TCCR0B = 0x00 | (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (1 << CS01) | (0 << CS00); //010 - 8 prescaler
	// TCCR0B = 0x01
	TCNT0 = 0x00;
	// TCNT0 = 0
	TIMSK |= (0 << OCIE0B) | (1 << TOIE0) | (0 << OCIE0A);
	// TIMSK = 0x02
}

void vINT0_ini (void)
{
	GIMSK |= (0x01 << 6);
	//Interrupt enable INT0
	MCUCR |= (0x01 << ISC01) | (0x00 << ISC00);
	//ISC01 = 1, ISC00 = 0: Iterrupt flag on rising edge
}

void vPORTS_ini (void)
{
	PORT_INI_LED1 |= (0x01 << PIN_LED1);
	PORT_INI_LED2 |= (0x01 << PIN_LED2);
	PORT_INI_LED3 |= (0x01 << PIN_LED3);
	PORT_INI_LED4 |= (0x01 << PIN_LED4);
	PORT_INI_LED5 |= (0x01 << PIN_LED5);
	PORT_INI_LED6 |= (0x01 << PIN_LED6);
	PORT_INI_LED7 |= (0x01 << PIN_LED7);
	PORT_INI_LED8 |= (0x01 << PIN_LED8);
	PORT_INI_LED9 |= (0x01 << PIN_LED9);
	PORT_INI_LED10 |= (0x01 << PIN_LED10);
	PORT_INI_LED11 |= (0x01 << PIN_LED11);
	PORT_INI_LED12 |= (0x01 << PIN_LED12);
}
/*
void USART1_ini (void)
{
	UBRR1H = (unsigned char) (BAUD>>8);
	UBRR1L = (unsigned char) BAUD;
	UCSR1C = (0 << UMSEL1) | //asynchronous
			(0 << UPM10) | (0<<UPM01) |	//Mode 00: no parity
			(0 << USBS1) | 	//1 stop bit
			(1 << UCSZ11) | (1 << UCSZ10);	//Mode 011: 8 bit data
	UCSR1B = (0 << RXCIE1) | (1 << TXCIE1) | (0 << RXEN1) | (1 << TXEN1) | (0 << UCSZ12);
	UCSR1A = (0 << U2X1);
}
*/

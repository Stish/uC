//	RS		PD4
//	EN		PD5
//	D0-7	PB0-7

/*#ifndef CLCK
#define CLCK 7		//pin for clock signal
#endif
#ifndef DATA
#define DATA 8		//pin for data signal
#endif*/

/*void lcd_clck_cycle(void)
{
	set_out_pin(CLCK);		//clck high
	lcd_wait(0);
	reset_out_pin(CLCK);	//clck low
	lcd_wait(0);
}*/

void lcd_send(unsigned char mode, unsigned char cmd)
{
	if (mode == 'd')
		PORTD |= (1 << 4);
	else
		PORTD &= ~(1 << 4);
	PORTB = cmd;
	PORTD |= (1 << 5);
	PORTD &= ~(1 << 5);
}

/*void lcd_send_nibble(unsigned char mode, unsigned char cmd)
{
	char i = 0;
//---	Cleare the SR with feeding 0---//	
	reset_out_pin(DATA);
	for (i = 1; i <= 9; i++)
		lcd_clck_cycle();
//---	Feeding Enable pin			---//	
	set_out_pin(DATA);
	lcd_clck_cycle();
//---	Feeding Data pins (d4-d7)	---//
	for (i = 0; i <= 3; i++)
	{
		if (cmd & (1<<i))
			set_out_pin(DATA);
		else
			reset_out_pin(DATA);
		lcd_clck_cycle();
	}
//---	Feeding R/s pin 			---//	
	if (mode == 'd')
		set_out_pin(DATA);
	else
		reset_out_pin(DATA);
	lcd_clck_cycle();
//---	Feeding two zeros  			---//		
	reset_out_pin(DATA);
	lcd_clck_cycle();
	lcd_clck_cycle();
//---	Generating Enable Strobe	---//	
	set_out_pin(DATA);
	lcd_clck_cycle();
	reset_out_pin(DATA);
}*/

void lcd_wait(unsigned char time)
{
	unsigned char i,j = 0;
	
	for (i = 1; i <= time; i++)
	{
		j = 1;
		while (j >= 1)
			j++;
	}
	for (i = 1; i <= 3; i++)
		j++;
}

void lcd_reset(void)
{
	lcd_wait(30);
	lcd_send('c', 0x3);
	lcd_wait(15);
	lcd_send('c', 0x3);
	lcd_send('c', 0x3);
	lcd_send('c', 0x2);
}

void lcd_clr(void)
{
	lcd_send('c', 0x01);
	lcd_send('c', 0x14);
}

void lcd_init (void)
{
	DDRD |= (1 << 4) | (1 << 5);
	DDRB |= 0xff;
	lcd_reset();         		// Call LCD reset
	lcd_send('c', 0x34);       	// 4-bit mode
	lcd_send('c', 0x09);
	lcd_send('c', 0x20);
	lcd_send('c', 0x0c);       	// Display no cursor - no blink.
	lcd_send('c', 0x06);       	// Automatic Increment - No Display shift.
	lcd_send('c', 0x80);      	// Address DDRAM with 0 offset 80h.
	lcd_clr();
}

void lcd_string(unsigned char message[20])
{
	unsigned char i;
	
	for (i = 0; i < 20; i++)
	{
		if ((!message[i]))
			break;
		lcd_send('d', message[i]);
	}
}

void lcd_setpos(unsigned char row, unsigned char col)
{
	char i,j = 0;
	
	lcd_send('c', 0x02);
	for (i = 1; i < row; i++)
	{
		for (j = 1; j <= 20; j++)
			lcd_send('c', 0x14);
	}
	for (i = 1; i < col; i++)
		lcd_send('c', 0x14);
}

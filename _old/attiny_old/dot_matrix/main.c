#include <avr/io.h>

void disp_char(unsigned char colums[5]);

int main(void)
{
	unsigned char colums_a[5] = {0x3f, 0x48, 0x48, 0x48, 0x3f};
	unsigned char colums_b[5] = {0x7f, 0x49, 0x49, 0x49, 0x36};
	unsigned char colums_c[5] = {0x3e, 0x41, 0x41, 0x41, 0x22};
	unsigned char colums_d[5] = {0x7f, 0x41, 0x41, 0x41, 0x3e};
	unsigned char colums_e[5] = {0x7f, 0x49, 0x49, 0x49, 0x41};
	unsigned char colums_f[5] = {0x7f, 0x48, 0x48, 0x48, 0x40};
	unsigned char colums_h[5] = {0x7f, 0x08, 0x08, 0x08, 0x7f};
	unsigned char colums_l[5] = {0x7f, 0x01, 0x01, 0x01, 0x01};
	unsigned char colums_o[5] = {0x3e, 0x41, 0x41, 0x41, 0x3e};
	unsigned int i;
	
	DDRB |= 0x1f;
	DDRD |= 0x7c;
	DDRA |= 0x03;
	
	PORTB &= ~0x1f;
	
	while(1)
	{
		for (i = 0; i <= 1000; i++)
			disp_char(colums_h);
		for (i = 0; i <= 1000; i++)
			disp_char(colums_a);
		for (i = 0; i <= 2000; i++)
			disp_char(colums_l);
		for (i = 0; i <= 1000; i++)
			disp_char(colums_o);			
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
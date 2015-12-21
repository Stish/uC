//LED Blinking
#include <avr/io.h>

int main(void)
{
	unsigned char i, j;
	
	DDRB |= (0x01 << 0);	//PB.0 output
	DDRB &= ~(0x01 << 1);	//PB.1 input
	
	while(PINB & (0x01 << 1))
	{
		PORTB ^= (0x01 << 0);
		
		/*for(i = 0; i <= 255; i++)
		{
			for(j = 0; j <= 255; j++)
			{ 
			}
		}*/
	}
	
}
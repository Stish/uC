#define F_CPU 8000000

#include <avr/io.h>
#include <avr/delay.h>

void write_cmd(unsigned char *cmd);

int main(void)
{
	unsigned int wait_s = 0;
	unsigned char dial_num[] = "ATD00420607227360\n";
	unsigned char sht_dwn[] = "AT^SMSO\n";

	UBRRH = 0;		
	UBRRL = 25;
	UCSRA = 0x60;	// 
	UCSRB = 0x18;	// interrupts disabled, transmitter and reciever enabled, 8bit, no 9th bit
	UCSRC = 0x06; 	// no parity, asynchronous, 1 stopbit, 8bit number

	for (wait_s = 0; wait_s <= 10; wait_s++)
		_delay_ms(250);
	
	while(1)
	{
		write_cmd(&dial_num[0]);
		for (wait_s = 0; wait_s <= 50; wait_s++)
			_delay_ms(250);
		write_cmd(&sht_dwn[0]);
		while(1);
	}
}

void write_cmd(unsigned char *cmd)
{
	unsigned int i = 0;
	
	while (!( UCSRA & (1<<UDRE)));
	UDR = 0x0D;	
	_delay_ms(50);

	while(cmd[i] != 0)
    {
		while (!( UCSRA & (1<<UDRE)));
		UDR = cmd[i];	
		_delay_ms(50);
		i++;
	}
	
	while (!( UCSRA & (1<<UDRE)));
	UDR = 0x0D;	
	_delay_ms(50);
}


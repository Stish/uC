//====================================//
//==			Transmitter			==//
//==								==//
//==		C Code by Alex.			==//
//====================================//

#include "lib/mct_fct.c"
#include <avr/interrupt.h>
#include <avr/signal.h>

void init_timer(void);
void wait(long int time);

char rc_seq = 0;
unsigned int time_global = 0;

SIGNAL (SIG_OVERFLOW2)
{
	time_global++;
}

int main(void)
{
	char i = 0;

	init_out();
	init_in();
    init_timer();

	sei();
	
	while(1)
	{
		wait(6);
		reset_out_pin(8);
		wait(36);
		set_out_pin(8);
		wait(6);		
		for (i = 0; i <= 4; i++)
		{
			wait(6);
			if (rc_seq & (1<<i))
				set_out_pin(8);
			else
				reset_out_pin(8);
		}
		
		rc_seq = 0;
		
		if (get_in_pin(8))
		{
			rc_seq |= 1;
		}
		if (get_in_pin(7))
		{
			rc_seq |= (1<<1);
		}
		if (get_in_pin(6))
		{
			rc_seq |= (1<<2);
		}
		if (get_in_pin(5))
		{
			rc_seq |= (1<<3);
		}
		
		switch(rc_seq)
		{
			case 0x01:	//forward
				rc_seq = 0x03;
				break;
			case 0x04:	//backward
				rc_seq = 0x0c;
				break;
			case 0x02:	//turn left
				rc_seq = 0x06;
				break;
			case 0x08:	//turn right
				rc_seq = 0x09;
				break;
			case 0x03:	//forward left
				rc_seq = 0x02;
				break;
			case 0x09:	//forward right
				rc_seq = 0x01;
				break;	
			case 0x06:	//backward left
				rc_seq = 0x08;
				break;
			case 0x0c:	//backward right
				rc_seq = 0x04;
				break;
			case 0x05:	//backward forward
				rc_seq = 0x00;
				break;
			case 0x0a:	//left right
				rc_seq = 0x00;
				break;
			default:
				break;
		}
		
		if (get_in_pin(4))
		{
			rc_seq |= (1<<4);
		}
	}
}

void wait(long int time)
{
	time_global = 1;
	while(time_global<=time);
}

void init_timer(void)
{
	TCNT2 = 0x00;
	TCCR2 = 0x04;
	TIMSK |= 0x40;
}
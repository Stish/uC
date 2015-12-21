#include "main.h"

int main(void)
{
	can_init(125, NONE);
	
	msg.id = 0x00000002;
	msg.idm = 0x1fffffff;
	msg.length = 5;
	msg.data[0] = 'T';
	msg.data[1] = 'e';
	msg.data[2] = 's';
	msg.data[3] = 't';
	msg.data[4] = 0;
	msg.data[5] = 0;
	msg.data[6] = 0;
	msg.data[7] = 0;
	
	can_enable_mob (10, TRANSMIT_DATA, msg);
	
	TIMER0_ini();
	sei();

	while(1)
	{
	}
}

SIGNAL (SIG_OVERFLOW0)								//a/d converter in interrupt mode
{
	TCNT0 = 0x06;
	timer0_cnt++;
	
	if (timer0_cnt == 25) //100ms
	{
		can_send_data (10, msg.data, msg.length);
		timer0_cnt = 0;
	}
}

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x06;
}
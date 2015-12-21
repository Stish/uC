/*
                         _____   _____
             _____      |     \_/     |
             Reset O--1-|PA2      VCC |-20-O
                        |             |
               RXD O--2-|PD0       PB7|-19-O UCSK/SCL/PCINT7
                        |             |
               TXD O--3-|PD1       PB6|-18-O MISO/DO/PCINT6
                        |             |
             XTAL2 O--4-|PA1       PB5|-17-O MOSI/DI/PCINT5
                        |             |
             XTAL1 O--5-|PA0       PB4|-16-O OC1B/PCINT4
                        |             |
    CKOUT/XCK/INT0 O--6-|PD2       PB3|-15-O OC1A/PCINT3
                        |             |
              INT1 O--7-|PD3       PB2|-14-O OC0A/PCINT2
                        |             |
                T0 O--8-|PD4       PB1|-13-O AIN1/PCINT1
                        |             |
           OC0B/T1 O--9-|PD5       PB0|-12-O AIN0/PCINT0
                        |             |
               GND O-10-|GND       PD6|-11-O ICP
                        |_____________|
                          ATTiny 2313
						  
*/
#define R_START	255	//pin b.0
#define G_START	255	//pin b.1
#define B_START	255	//pin b.2
#define FADE_ON

#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#ifdef FADE_ON
	#include <stdlib.h>
#endif

unsigned int ovrflw_cnt = 1;
unsigned char rgb[3] = {R_START, G_START, B_START};
#ifdef FADE_ON
	unsigned char rgb_fade[3] = {R_START, G_START, B_START};
#endif

SIGNAL (SIG_TIMER0_OVF)
{
	if (ovrflw_cnt == 256)
		ovrflw_cnt = 0;
	
//=== Red LED ===
	if ((ovrflw_cnt >= 0) && (ovrflw_cnt <= rgb[0]))
	{
		PORTB |= (1 << 0);
		//PORTB &= ~(1 << 1) & ~(1 << 2);
	}
	else
	{
		PORTB &= ~(1 << 0);
	}
//=== Green LED ===
	if ((ovrflw_cnt >= 0) && (ovrflw_cnt <= rgb[1]))
	{
		PORTB |= (1 << 1);
		//PORTB &= ~(1 << 0) & ~(1 << 2);
	}
	else
	{
		PORTB &= ~(1 << 1);
	}
//=== Blue LED ===
	if ((ovrflw_cnt >= 0) && (ovrflw_cnt <= rgb[2]))
	{
		PORTB |= (1 << 2);
		//PORTB &= ~(1 << 0) & ~(1 << 1);
	}
	else
	{
		PORTB &= ~(1 << 2);
	}
	ovrflw_cnt++;
}

int main(void)
{
#ifdef FADE_ON
	unsigned long int i = 0;
#endif
	
	DDRB |= (1 << 0) | (1 << 1) | (1 << 2);
	PORTB &= ~(1 << 0) & ~(1 << 1) & ~(1 << 2);
	
	TCNT0 = 0x00;
	TCCR0B = 0x01;
	TIMSK  |= (1 << TOIE0);
	sei();

	while(1)
	{
	#ifdef FADE_ON
		if ((rgb[0] == rgb_fade[0]) && (rgb[1] == rgb_fade[1]) && (rgb[2] == rgb_fade[2]))
		{
			rgb_fade[0] = (rand()%255);
			rgb_fade[1] = (rand()%255);
			rgb_fade[2] = (rand()%255);
		}
		else
		{
			for (i = 0; i <= 2; i++)
			{
				if (rgb_fade[i] > rgb[i])
					rgb[i]++;
				if (rgb_fade[i] < rgb[i])
					rgb[i]--;
			}
			for (i = 0; i <= 2000; i++);
		}
	#endif
	}
}
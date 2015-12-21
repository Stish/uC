#define RATIO_RD	33	//pin b.0
#define RATIO_GRN	33	//pin b.1
#define RATIO_BL	33	//pin b.2

#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

unsigned int ovrflw_cnt = 1;
unsigned int rat_rd = RATIO_RD;
unsigned int rat_grn = RATIO_GRN;
unsigned int rat_bl = RATIO_BL;
unsigned int rat_rd_target = RATIO_RD;
unsigned int rat_grn_target = RATIO_GRN;
unsigned int rat_bl_target = RATIO_BL;

SIGNAL (SIG_TIMER0_OVF)
{
	if (ovrflw_cnt == 101)
		ovrflw_cnt = 1;
	
//=== Red LED ===
	if ((ovrflw_cnt >= 1) && (ovrflw_cnt <= rat_rd))
	{
		PORTB |= (1 << 0);
		PORTB &= ~(1 << 1) & ~(1 << 2);
	}
//=== Green LED ===
	if ((ovrflw_cnt >= rat_rd) && (ovrflw_cnt <= (rat_rd + rat_grn)))
	{
		PORTB |= (1 << 1);
		PORTB &= ~(1 << 0) & ~(1 << 2);
	}
//=== Blue LED ===
	if ((ovrflw_cnt >= (rat_rd + rat_grn)) && (ovrflw_cnt <= (rat_rd + rat_grn + rat_bl)))
	{
		PORTB |= (1 << 2);
		PORTB &= ~(1 << 0) & ~(1 << 1);
	}
	ovrflw_cnt++;
}

int main(void)
{
	//unsigned int rnd_nmbr = 0;
	unsigned long int i,j = 0;
	//unsigned int mode = 0;
	
	DDRB |= (1 << 0) | (1 << 1) | (1 << 2);
	PORTB &= ~(1 << 0) & ~(1 << 1) & ~(1 << 2);
	
	TCNT0 = 0x00;
	TCCR0B = 0x01;
	TIMSK  |= (1 << TOIE0);
	sei();

	while(1)
	{
		if ((rat_rd_target == rat_rd) && (rat_bl_target == rat_bl) && (rat_grn_target == rat_grn))
		{
			rat_rd_target = (rand()%100);
			rat_grn_target = (rand()%(100 - rat_rd_target));
			rat_bl_target = (100 - rat_rd_target - rat_grn_target);
/*			switch (mode)
			{
				case 0:
					rat_rd_target = (rand()%100);
					rat_grn_target = (rand()%(100 - rat_rd_target));
					rat_bl_target = (rand()%(100 - rat_rd_target - rat_grn_target));
					mode = 1;
					break;
				case 1:
					rat_bl_target = (rand()%100);
					rat_rd_target = (rand()%(100 - rat_bl_target));
					rat_grn_target = (rand()%(100 - rat_rd_target - rat_bl_target));
					mode = 2;
					break;
				case 2:
					rat_grn_target = (rand()%100);
					rat_bl_target = (rand()%(100 - rat_grn_target));
					rat_rd_target = (rand()%(100 - rat_bl_target - rat_grn_target));
					mode = 0;
					break;
			}*/
		}
		if (rat_rd_target > rat_rd)
			rat_rd++;
		if (rat_rd_target < rat_rd)
			rat_rd--;	
	
		if (rat_bl_target > rat_bl)
			rat_bl++;
		if (rat_bl_target < rat_bl)
			rat_bl--;	

		if (rat_grn_target > rat_grn)
			rat_grn++;
		if (rat_grn_target < rat_grn)
			rat_grn--;	
			
		for (i = 0; i <= 2000; i++);
	}
}
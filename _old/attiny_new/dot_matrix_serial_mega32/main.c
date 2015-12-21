#define BAUD 12

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void write_data(unsigned char device, unsigned char colums[5]);

/*ISR (USART_TXC_vect)
{
	UDR = 0x81;
}*/

int main(void)
{
	unsigned long int i;
	//unsigned char cursor = 0;
	//unsigned char colums_a[5] = {0x3f, 0x48, 0x48, 0x48, 0x3f};
	//unsigned char rec_char = 0;
	//unsigned char colums_b[5] = {0x7f, 0x49, 0x49, 0x49, 0x36};
	//unsigned char colums_c[5] = {0x3e, 0x41, 0x41, 0x41, 0x22};
	//unsigned char colums_d[5] = {0x7f, 0x41, 0x41, 0x41, 0x3e};
	//unsigned char colums_e[5] = {0x7f, 0x49, 0x49, 0x49, 0x41};
	//unsigned char colums_f[5] = {0x7f, 0x48, 0x48, 0x48, 0x40};
	/*unsigned char colums_h[5] = {0x7f, 0x08, 0x08, 0x08, 0x7f};
	unsigned char colums_l[5] = {0x7f, 0x01, 0x01, 0x01, 0x01};
	unsigned char colums_o[5] = {0x3e, 0x41, 0x41, 0x41, 0x3e};
	unsigned char colums_empty[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char colums_exp_mark[5] = {0x00, 0x00, 0x7d, 0x00, 0x00};
	unsigned char colums_80[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char colums_81[5] = {0x00, 0x00, 0x00, 0x00, 0x00};*/
	unsigned char mtrx[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char mtrx_on[5] = {0x7f, 0x7f, 0x7f, 0x7f, 0x7f};
	unsigned char mtrx_off[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char mtrx_itm_cnt = 0;
	unsigned char mtrx_itm_type = 1;
	unsigned char mtrx_itm_x = 0;
	unsigned char mtrx_itm_y = 0;
	unsigned char mtrx_new_itm = 1;
	//unsigned int i;
	
	//DDRB |= 0x1f;
	//DDRD |= 0x7c;
	//DDRA |= 0x03;

	UBRRH = 0;		//UBRR to 15 for 115200bps with 14.7456Mhz
	UBRRL = 25;
	
	UCSRA = 0x62;	// 
	UCSRB = 0x18;	// interrupts disabled, transmitter and reciever enabled, 8bit, no 9th bit
	UCSRC = 0x86; 	// no parity, asynchronous, 1 stopbit, 8bit number
	//UCSRB |= (1 << RXCIE);
	//sei();
	
	//PORTB &= ~0x1f;
	UDR = 0x01;
	
	while(1)
	{
		if (mtrx_new_itm)
		{
			if (mtrx_itm_type)
			{
				do
				{
					mtrx_itm_x = (rand()%5);
				}
				while (mtrx[mtrx_itm_x] & (1 << 6));
				mtrx[mtrx_itm_x] |= (1 << 6);
				mtrx_new_itm = 0;
				mtrx_itm_y = 6;
				mtrx_itm_cnt++;
			}
			else
			{
				do
				{
					mtrx_itm_x = (rand()%5);
				}
				while (!(mtrx[mtrx_itm_x] & (1 << 6)));
				mtrx[mtrx_itm_x] &= ~(1 << 6);
				mtrx_new_itm = 0;
				mtrx_itm_y = 6;
				mtrx_itm_cnt--;
			}
		}
		else
		{
			if (mtrx_itm_type)
			{
				if ((!(mtrx[mtrx_itm_x] & (1 << (mtrx_itm_y - 1)))) && (mtrx_itm_y > 0))
				{
					mtrx[mtrx_itm_x] &= ~(1 << mtrx_itm_y);
					mtrx[mtrx_itm_x] |= (1 << mtrx_itm_y - 1);
					mtrx_itm_y--;
				}
				else
					mtrx_new_itm = 1;
			}
			else
			{
				if (((mtrx[mtrx_itm_x] & (1 << (mtrx_itm_y - 1)))) && (mtrx_itm_y > 0))
				{
					mtrx[mtrx_itm_x] |= (1 << mtrx_itm_y);
					mtrx[mtrx_itm_x] &= ~(1 << mtrx_itm_y - 1);
					mtrx_itm_y--;
				}
				else
					mtrx_new_itm = 1;
			}
		}
		for (i = 0; i <= 10000; i++);
		write_data(0x81, mtrx);
		write_data(0x82, mtrx);
		if ((mtrx_itm_cnt == 0) || (mtrx_itm_cnt == 35))
		{
			mtrx_new_itm = 1;
			mtrx_itm_type ^= 0x01;
			//for (i = 0; i <= 100000; i++);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_off);
			write_data(0x82, mtrx_off);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_on);
			write_data(0x82, mtrx_on);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_off);
			write_data(0x82, mtrx_off);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_on);
			write_data(0x82, mtrx_on);
			for (i = 0; i <= 50000; i++);
		}
		/*if (mtrx_itm_cnt == 35) || 
		{
			mtrx_itm_type = 0;
			//mtrx_itm_cnt--;
			for (i = 0; i <= 100000; i++);
		}
		/*mtrx_itm_x = (rand()%5);
		mtrx_itm_y = (rand()%7);
		if ((!(mtrx[mtrx_itm_x] & (1 << mtrx_itm_y))) && (mtrx_itm_type))
		{
			mtrx[mtrx_itm_x] |= (1 << mtrx_itm_y);
			for (i = 0; i <= 50000; i++);
			mtrx_itm_cnt++;
		}
		if (((mtrx[mtrx_itm_x] & (1 << mtrx_itm_y))) && (!(mtrx_itm_type)))
		{
			mtrx[mtrx_itm_x] &= ~(1 << mtrx_itm_y);
			for (i = 0; i <= 50000; i++);
			mtrx_itm_cnt--;
		}
		if (mtrx_itm_cnt == 0)
			mtrx_itm_type = 1;
		if (mtrx_itm_cnt == 35)
			mtrx_itm_type = 0;
		write_data(0x81, mtrx);*/
		
		/*if (mtrx_itm_cnt == 35)
		{
			write_data(0x81, mtrx_off);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_on);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_off);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_on);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_off);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_on);
			for (i = 0; i <= 50000; i++);
			write_data(0x81, mtrx_off);
			for (i = 0; i <= 50000; i++);
			mtrx[0] = 0x00;
			mtrx[1] = 0x00;
			mtrx[2] = 0x00;
			mtrx[3] = 0x00;
			mtrx[4] = 0x00;
			mtrx_itm_cnt = 0;
		}
		/*write_data(0x80, colums_empty);
		write_data(0x81, colums_h);
		/*for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_h);
		write_data(0x81, colums_e);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_e);
		write_data(0x81, colums_l);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_l);
		write_data(0x81, colums_l);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_l);
		write_data(0x81, colums_o);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_o);
		write_data(0x81, colums_exp_mark);
		for (i = 0; i <= 100000; i++);
		write_data(0x80, colums_exp_mark);
		write_data(0x81, colums_empty);
		for (i = 0; i <= 100000; i++);
		//while (!( UCSRA & (1<<UDRE)));
		//UDR = 0x81;
		//for (i = 0; i <= 1000; i++);*/
		/*write_data(0x80, colums_80);
		write_data(0x81, colums_81);
		for (i = 0; i <= 100000; i++);*/
		/*for (cursor = 0; cursor <= 29; cursor++)
		{
			
			write_data(0x80, colums_80);
			write_data(0x81, colums_81);
			for (i = 0; i <= 100000; i++);
		}*/
	}
}

void write_data(unsigned char device, unsigned char colums[5])
{
	unsigned int i;
	
	while (!( UCSRA & (1<<UDRE)));
	UDR = device;
	//for (j = 0; j <= 10; j++);
	for (i = 0; i <= 4; i++)
	{
		while (!( UCSRA & (1<<UDRE)));
		UDR = colums[i];	
		//for (j = 0; j <= 10; j++);	
	}
}

